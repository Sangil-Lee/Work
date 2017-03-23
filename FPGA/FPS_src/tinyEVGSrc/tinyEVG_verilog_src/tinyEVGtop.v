// MIT License
//  
// Copyright (c) 2106 Osprey DCS
//  
// Permission is hereby granted, free of charge, to any person obtaining a copy
// of this software and associated documentation files (the "Software"), to deal
// in the Software without restriction, including without limitation the rights
// to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
// copies of the Software, and to permit persons to whom the Software is
// furnished to do so, subject to the following conditions:
//  
// The above copyright notice and this permission notice shall be included in all   
// copies or substantial portions of the Software.
//  
// THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
// IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
// FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
// AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
// LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
// OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
// SOFTWARE.     

module tinyEVGtop (
    inout [14:0] DDR_addr,
    inout  [2:0] DDR_ba,
    inout        DDR_cas_n,
    inout        DDR_ck_n,
    inout        DDR_ck_p,
    inout        DDR_cke,
    inout        DDR_cs_n,
    inout  [3:0] DDR_dm,
    inout [31:0] DDR_dq,
    inout  [3:0] DDR_dqs_n,
    inout  [3:0] DDR_dqs_p,
    inout        DDR_odt,
    inout        DDR_ras_n,
    inout        DDR_reset_n,
    inout        DDR_we_n,
    inout        FIXED_IO_ddr_vrn,
    inout        FIXED_IO_ddr_vrp,
    inout [53:0] FIXED_IO_mio,
    inout        FIXED_IO_ps_clk,
    inout        FIXED_IO_ps_porb,
    inout        FIXED_IO_ps_srstb,
    
    // GPS Receiver
    input        PMOD1_0, // 3DF
    output       PMOD1_1, // RxD (direction from point of view of GPS module)
    input        PMOD1_2, // TxD (direction from point of view of GPS module)
    input        PMOD1_3, // PPS

    // Logic analyzer (for tracking fast protection timing)
    (*mark_debug="true"*) input PMOD1_4,
    (*mark_debug="true"*) input PMOD1_5,
    (*mark_debug="true"*) input PMOD1_6,
    (*mark_debug="True"*) input PMOD1_7,

    // GPIO LEDs
    output       GPIO_LED_0,
    output       GPIO_LED_CENTER,

    // FPGA fan
    input        SM_FAN_TACH,
    output       SM_FAN_PWM,

    // Transceiver reference
    input        SI5324_REFCLK_P, SI5324_REFCLK_N,
    input        SI5324_INT_ALM,
    output       SI5324_RST,

    // SFP
    input        SFP_TX_DISABLE,
    output       SFP_TX_P, SFP_TX_N,
    input        SFP_RX_P, SFP_RX_N
    );

localparam SYSCLK_RATE = 100000000;
localparam EVG_CLOCK_RATE = 125000000;

//////////////////////////////////////////////////////////////////////////////
// Clocks
wire sysClk, evgTxClk, evgRxClk;

//////////////////////////////////////////////////////////////////////////////
// Frequency counters -- check rate of transceiver clocks
reg txRefPPS_m, txRefPPS, txRefPPS_d;
reg txRefToggle = 0;
reg [31:0] txRefCount = 0, txRefRate = 0;
always @(posedge evgTxClk) begin
    txRefPPS_m <= PMOD1_3;
    txRefPPS   <= txRefPPS_m;
    txRefPPS_d <= txRefPPS;
    if (txRefPPS && !txRefPPS_d) begin
        txRefRate <= txRefCount;
        txRefCount <= 0;
    end
    else begin
        txRefCount <= txRefCount + 1;
    end
end
reg rxRefPPS_m, rxRefPPS, rxRefPPS_d;
reg rxRefToggle = 0;
reg [31:0] rxRefCount = 0, rxRefRate = 0;
always @(posedge evgRxClk) begin
    rxRefPPS_m <= PMOD1_3;
    rxRefPPS   <= rxRefPPS_m;
    rxRefPPS_d <= rxRefPPS;
    if (rxRefPPS && !rxRefPPS_d) begin
        rxRefRate <= rxRefCount;
        rxRefCount <= 0;
    end
    else begin
        rxRefCount <= rxRefCount + 1;
    end
end

//////////////////////////////////////////////////////////////////////////////
// Static outputs
assign SFP_TX_DISABLE = 1'b0;
assign SI5324_RST = 1'b1;
assign SM_FAN_PWM = 1'b1;

//////////////////////////////////////////////////////////////////////////////
// PMOD GPS
parameter GPS_DEBUG = "false";
(* mark_debug = GPS_DEBUG *) wire gpsPPS, gpsTxD, gpsRxD, gps3DF;
assign gpsPPS  = PMOD1_3;
assign gpsTxD  = PMOD1_2;
assign PMOD1_1 = gpsRxD;
assign gps3DF  = PMOD1_0;

//////////////////////////////////////////////////////////////////////////////
// Diagnostics
assign GPIO_LED_0 = gpsPPS;
assign GPIO_LED_CENTER = gps3DF;

//////////////////////////////////////////////////////////////////////////////
// Event generator SFP transceiver
localparam EVG_TX_DBG = "true";
localparam EVG_RX_DBG = "true";

// Buffer RX clock too -- handy for ChipScope diagnosing
wire gt0_txusrclk2_out, gt0_rxusrclk2_out;
BUFG evgTxBufg (.I(gt0_txusrclk2_out), .O(evgTxClk));
BUFG evgRxBufg (.I(gt0_rxusrclk2_out), .O(evgRxClk));

(* mark_debug = EVG_TX_DBG *) wire [15:0] evgTxWord;
(* mark_debug = EVG_TX_DBG *) wire  [1:0] evgTxIsK;

wire [8:0]DRP0_daddr;
wire [15:0]DRP0_di, DRP0_do;
wire DRP0_den, DRP0_drdy, DRP0_dwe;

// Not really needed for event generator, but handy as a loopback test
(* mark_debug = EVG_RX_DBG *) wire [15:0] gt0_rxdata_out;
(* mark_debug = EVG_RX_DBG *) wire  [1:0] gt0_rxcharisk_out;
(* mark_debug = EVG_RX_DBG *) wire  [1:0] gt0_rxdisperr_out;
(* mark_debug = EVG_RX_DBG *) wire  [1:0] gt0_rxnotintable_out;

wire [5:0] GTXcontrol, GTXstatus;
wire gt0_rx_fsm_reset_done_out, gt0_rxresetdone_out;
wire gt0_tx_fsm_reset_done_out, gt0_txresetdone_out;
wire soft_reset_tx_in, gt0_gttxreset_in;
assign gt0_gttxreset_in = GTXcontrol[0];
assign soft_reset_tx_in = GTXcontrol[1];
assign GTXstatus = { gt0_rx_fsm_reset_done_out, gt0_rxresetdone_out,
                     gt0_tx_fsm_reset_done_out, gt0_txresetdone_out,
                     soft_reset_tx_in, gt0_gttxreset_in };

evgTransceiver evgTransceiver_i (
    .soft_reset_tx_in(soft_reset_tx_in),
    .soft_reset_rx_in(soft_reset_tx_in),
    .dont_reset_on_data_error_in(1'b0),
    .q1_clk1_gtrefclk_pad_n_in(SI5324_REFCLK_N),
    .q1_clk1_gtrefclk_pad_p_in(SI5324_REFCLK_P),
    .gt0_tx_fsm_reset_done_out(gt0_tx_fsm_reset_done_out),
    .gt0_rx_fsm_reset_done_out(gt0_rx_fsm_reset_done_out),
    .gt0_data_valid_in(1'b1),
    
    .gt0_txusrclk_out(),
    .gt0_txusrclk2_out(gt0_txusrclk2_out),
    .gt0_rxusrclk_out(),
    .gt0_rxusrclk2_out(gt0_rxusrclk2_out),
    //_________________________________________________________________________
    //GT0  (X0Y10)
    //____________________________CHANNEL PORTS________________________________
    //------------------------------- CPLL Ports -------------------------------
    .gt0_cpllfbclklost_out(),
    .gt0_cplllock_out(),
    .gt0_cpllreset_in(1'b0),
    //-------------------------- Channel - DRP Ports  --------------------------
    .gt0_drpaddr_in(DRP0_daddr),
    .gt0_drpdi_in(DRP0_di),
    .gt0_drpdo_out(DRP0_do),
    .gt0_drpen_in(DRP0_den),
    .gt0_drprdy_out(DRP0_drdy),
    .gt0_drpwe_in(DRP0_dwe),
    //------------------------- Digital Monitor Ports --------------------------
    .gt0_dmonitorout_out(),
    //------------------- RX Initialization and Reset Ports --------------------
    .gt0_eyescanreset_in(1'b0),
    .gt0_rxuserrdy_in(1'b1),
    //------------------------ RX Margin Analysis Ports ------------------------
    .gt0_eyescandataerror_out(),
    .gt0_eyescantrigger_in(1'b0),
    //---------------- Receive Ports - FPGA RX interface Ports -----------------
    .gt0_rxdata_out(gt0_rxdata_out),
    //---------------- Receive Ports - RX 8B/10B Decoder Ports -----------------
    .gt0_rxdisperr_out(gt0_rxdisperr_out),
    .gt0_rxnotintable_out(gt0_rxnotintable_out),
    //------------------------- Receive Ports - RX AFE -------------------------
    .gt0_gtxrxp_in(SFP_RX_P),
    //---------------------- Receive Ports - RX AFE Ports ----------------------
    .gt0_gtxrxn_in(SFP_RX_N),
    //----------------- Receive Ports - RX Buffer Bypass Ports -----------------
    .gt0_rxphmonitor_out(),
    .gt0_rxphslipmonitor_out(),
    //------------------- Receive Ports - RX Equalizer Ports -------------------
    .gt0_rxdfelpmreset_in('b0),
    .gt0_rxmonitorout_out(),
    .gt0_rxmonitorsel_in(1'b0),
    //------------- Receive Ports - RX Fabric Output Control Ports -------------
    .gt0_rxoutclkfabric_out(),
    //----------- Receive Ports - RX Initialization and Reset Ports ------------
    .gt0_gtrxreset_in(gt0_gttxreset_in),
    .gt0_rxpmareset_in(1'b0),
    //----------------- Receive Ports - RX8B/10B Decoder Ports -----------------
    .gt0_rxcharisk_out(gt0_rxcharisk_out),
    //------------ Receive Ports -RX Initialization and Reset Ports ------------
    .gt0_rxresetdone_out(gt0_rxresetdone_out),
    //------------------- TX Initialization and Reset Ports --------------------
    .gt0_gttxreset_in(gt0_gttxreset_in),
    .gt0_txuserrdy_in(1'b1),
    //---------------- Transmit Ports - TX Data Path interface -----------------
    .gt0_txdata_in(evgTxWord),
    //-------------- Transmit Ports - TX Driver and OOB signaling --------------
    .gt0_gtxtxn_out(SFP_TX_N),
    .gt0_gtxtxp_out(SFP_TX_P),
    //--------- Transmit Ports - TX Fabric Clock Output Control Ports ----------
    .gt0_txoutclkfabric_out(),
    .gt0_txoutclkpcs_out(),
    //------------------- Transmit Ports - TX Gearbox Ports --------------------
    .gt0_txcharisk_in(evgTxIsK),
    //----------- Transmit Ports - TX Initialization and Reset Ports -----------
    .gt0_txresetdone_out(gt0_txresetdone_out),

    //____________________________COMMON PORTS________________________________
    .gt0_qplloutclk_out(),
    .gt0_qplloutrefclk_out(),
    .sysclk_in(sysClk) // input wire sysclk_in
);

//////////////////////////////////////////////////////////////////////////////
// Event generator
wire [31:0] heartbeatInterval, evgHeartbeatInterval;
wire        evgPPStoggle;
wire [31:0] seconds;

forwardData #(.DATA_WIDTH(32))
  evgHeartbeat (
    .inClk(sysClk),
    .inData(heartbeatInterval),
    .outClk(evgTxClk),
    .outData(evgHeartbeatInterval));
    
tinyEVG #(.EVG_CLOCK_RATE(EVG_CLOCK_RATE), .DEBUG("true"))
  tinyEVG (
    .evgTxClk(evgTxClk),
    .evgTxWord(evgTxWord),
    .evgTxIsK(evgTxIsK),
    .heartbeatInterval(evgHeartbeatInterval),
    .distributedBus(8'b0),
    .eventCode(8'b0),
    .eventStrobe(1'b0),
    .ppsMarker_a(gpsPPS),
    .ppsToggle(evgPPStoggle),
    .seconds_a(seconds));

//////////////////////////////////////////////////////////////////////////////
// Time to Digital converter
wire [31:0] tdcResult;
simpleTDC #(.DEBUG("false"))
  simpleTDC(
    .clk(sysClk),
    .start_n(PMOD1_4),
    .stop_n(PMOD1_7),
    .dout(tdcResult));

//////////////////////////////////////////////////////////////////////////////
// Block design
`include "firmwareBuildDate.v"
base_zynq_design base_zynq_design_i (
    .sysClk(sysClk),

    .DDR_addr(DDR_addr),
    .DDR_ba(DDR_ba),
    .DDR_cas_n(DDR_cas_n),
    .DDR_ck_n(DDR_ck_n),
    .DDR_ck_p(DDR_ck_p),
    .DDR_cke(DDR_cke),
    .DDR_cs_n(DDR_cs_n),
    .DDR_dm(DDR_dm),
    .DDR_dq(DDR_dq),
    .DDR_dqs_n(DDR_dqs_n),
    .DDR_dqs_p(DDR_dqs_p),
    .DDR_odt(DDR_odt),
    .DDR_ras_n(DDR_ras_n),
    .DDR_reset_n(DDR_reset_n),
    .DDR_we_n(DDR_we_n),
    .FIXED_IO_ddr_vrn(FIXED_IO_ddr_vrn),
    .FIXED_IO_ddr_vrp(FIXED_IO_ddr_vrp),
    .FIXED_IO_mio(FIXED_IO_mio),
    .FIXED_IO_ps_clk(FIXED_IO_ps_clk),
    .FIXED_IO_ps_porb(FIXED_IO_ps_porb),
    .FIXED_IO_ps_srstb(FIXED_IO_ps_srstb),

    .GPIO_DATE_tri_i(FIRMWARE_BUILD_DATE),

    .DRP0_daddr(DRP0_daddr),
    .DRP0_den(DRP0_den),
    .DRP0_di(DRP0_di),
    .DRP0_do(DRP0_do),
    .DRP0_drdy(DRP0_drdy),
    .DRP0_dwe(DRP0_dwe),

    .GPIO_DIAG_0_tri_i(txRefRate),
    .GPIO_DIAG_1_tri_i(rxRefRate),

    .GPIO_GTX_CONTROL_tri_o(GTXcontrol),
    .GPIO_GTX_CONTROL_tri_i(GTXstatus),

    .UART_0_rxd(gpsTxD),
    .UART_0_txd(gpsRxD),

    .GPIO_EVG_CSR_tri_i({31'b0, evgPPStoggle}),
    .GPIO_EVG_CSR_tri_o(seconds),
    .GPIO_EVG_HEARTBEAT_INTERVAL_tri_i(heartbeatInterval),
    .GPIO_EVG_HEARTBEAT_INTERVAL_tri_o(heartbeatInterval),

    .GPIO_GPS_STATUS_tri_i({ gpsPPS, 2'b0, gps3DF }),
    
    .GPIO_TDC_tri_i(tdcResult));

endmodule
