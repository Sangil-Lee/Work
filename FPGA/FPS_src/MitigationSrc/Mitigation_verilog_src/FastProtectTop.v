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

module FastProtectTop #(
    parameter TIMESTAMP_WIDTH     = 64,
    parameter INTERLOCKS_PER_NODE = 64,
    parameter NODES_PER_LINK      = 5,
    parameter LINK_COUNT          = 8,  // 8 for ZC706, 7 for BMB7
    parameter MAX_LINK_COUNT      = 8,
    parameter MITIGATION_COUNT    = 16
    ) (
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

    // FPGA fan
    input        SM_FAN_TACH,
    output       SM_FAN_PWM,

    // Transceiver reference
    input        SI5324_REFCLK_P, SI5324_REFCLK_N,
    input        SI5324_INT_ALM,
    output       SI5324_RST,
    input        FMC_HPC_GBTCLK0_M2C_P, FMC_HPC_GBTCLK0_M2C_N,
    input        FMC_HPC_GBTCLK1_M2C_P, FMC_HPC_GBTCLK1_M2C_N,

    // ZC706 SFP (Event Receiver)
    input        SFP0_RX_P, SFP0_RX_N,
    output       SFP0_TX_P, SFP0_TX_N,
    output       SFP0_TX_DISABLE,

    // HPC FMC SFPs
    input        FMC_HPC_SFP1_RX_P, FMC_HPC_SFP1_RX_N,
    input        FMC_HPC_SFP2_RX_P, FMC_HPC_SFP2_RX_N,
    input        FMC_HPC_SFP3_RX_P, FMC_HPC_SFP3_RX_N,
    input        FMC_HPC_SFP4_RX_P, FMC_HPC_SFP4_RX_N,
    input        FMC_HPC_SFP5_RX_P, FMC_HPC_SFP5_RX_N,
    input        FMC_HPC_SFP6_RX_P, FMC_HPC_SFP6_RX_N,
    input        FMC_HPC_SFP7_RX_P, FMC_HPC_SFP7_RX_N,
    input        FMC_HPC_SFP8_RX_P, FMC_HPC_SFP8_RX_N,
    output       FMC_HPC_SFP1_TX_P, FMC_HPC_SFP1_TX_N,
    output       FMC_HPC_SFP2_TX_P, FMC_HPC_SFP2_TX_N,
    output       FMC_HPC_SFP3_TX_P, FMC_HPC_SFP3_TX_N,
    output       FMC_HPC_SFP4_TX_P, FMC_HPC_SFP4_TX_N,
    output       FMC_HPC_SFP5_TX_P, FMC_HPC_SFP5_TX_N,
    output       FMC_HPC_SFP6_TX_P, FMC_HPC_SFP6_TX_N,
    output       FMC_HPC_SFP7_TX_P, FMC_HPC_SFP7_TX_N,
    output       FMC_HPC_SFP8_TX_P, FMC_HPC_SFP8_TX_N,
    input        FMC_HPC_SFP1_MODABS,
    input        FMC_HPC_SFP2_MODABS,
    input        FMC_HPC_SFP3_MODABS,
    input        FMC_HPC_SFP4_MODABS,
    input        FMC_HPC_SFP1_RXLOS,
    input        FMC_HPC_SFP2_RXLOS,
    input        FMC_HPC_SFP3_RXLOS,
    input        FMC_HPC_SFP4_RXLOS,
    input        FMC_HPC_SFP1_TXFAULT,
    input        FMC_HPC_SFP2_TXFAULT,
    input        FMC_HPC_SFP3_TXFAULT,
    input        FMC_HPC_SFP4_TXFAULT,
    output       FMC_HPC_SFP1_TXDISABLE,
    output       FMC_HPC_SFP2_TXDISABLE,
    output       FMC_HPC_SFP3_TXDISABLE,
    output       FMC_HPC_SFP4_TXDISABLE,
    output       FMC_HPC_SFP1_RS0,
    output       FMC_HPC_SFP2_RS0,
    output       FMC_HPC_SFP3_RS0,
    output       FMC_HPC_SFP4_RS0,
    output       FMC_HPC_SFP1_RS1,
    output       FMC_HPC_SFP2_RS1,
    output       FMC_HPC_SFP3_RS1,
    output       FMC_HPC_SFP4_RS1,
    inout        FMC_HPC_SFP1_SCL,
    inout        FMC_HPC_SFP2_SCL,
    inout        FMC_HPC_SFP3_SCL,
    inout        FMC_HPC_SFP4_SCL,
    inout        FMC_HPC_SFP1_SDA,
    inout        FMC_HPC_SFP2_SDA,
    inout        FMC_HPC_SFP3_SDA,
    inout        FMC_HPC_SFP4_SDA,
    inout        FMC_HPC_EEPROM_SCL,
    inout        FMC_HPC_EEPROM_SDA,

    // Mitigation outputs
    output [MITIGATION_COUNT-1:0] MITIGATION_OUTPUTS,

    // Logic analyzer connecions
    output [7:0] PMOD1_DIAG,

    // GPIO LEDS
    output       GPIO_LED_0,
    output       GPIO_LED_CENTER,
    output       GPIO_LED_LEFT,
    output       GPIO_LED_RIGHT
    );

parameter NODE_INDEX_WIDTH  = $clog2(NODES_PER_LINK);
parameter SYSMON_ADDR_WIDTH = 7;
parameter LINK_INDEX_WIDTH  = $clog2(MAX_LINK_COUNT);
parameter ACQ_INDEX_WIDTH   = NODE_INDEX_WIDTH + LINK_INDEX_WIDTH;

//////////////////////////////////////////////////////////////////////////////
// Static outputs
assign SI5324_RST = 1'b1;

//////////////////////////////////////////////////////////////////////////////
// The clock domains
// Net names starting with 'aurora' are in the Aurora user clock domain.
// Net names starting with 'evr' are in the event receiver clock domain.
// Other net names are in the system clock domain.
parameter SYSCLK_RATE = 100000000;
wire sysClk, evrClk, auroraUserClk;

//////////////////////////////////////////////////////////////////////////////
// Event receiver
localparam EVR_DEBUG = "false";

wire  [8:0] DRP0_daddr;
wire [15:0] DRP0_di, DRP0_do;
wire        DRP0_den, DRP0_drdy, DRP0_dwe;
wire [15:0] gt0_rxdata_out;
wire  [1:0] gt0_rxcharisk_out, gt0_rxdisperr_out, gt0_rxnotintable_out;
wire        gt0_rxoutclk_out;

wire [4:0] GPIO_EVR_GTX_control;
wire [2:0] GPIO_EVR_GTX_status;

wire [TIMESTAMP_WIDTH-1:0] evrTimestamp;
wire                       evrSecondsMarker;

// Reference clock
wire evrRefClk;
IBUFDS_GTE2 evrRefClkBuf (
    .O(evrRefClk),
    .CEB(1'b0),
    .I(SI5324_REFCLK_P),
    .IB(SI5324_REFCLK_N));

// Buffer recovered clock
BUFG evrGTX_BUFG (.I(gt0_rxoutclk_out), .O(evrClk));

// Instantiate the transceiver
evrGTX evrGTX_i (
     .sysclk_in(sysClk),
     .soft_reset_rx_in(GPIO_EVR_GTX_control[1]),
     .dont_reset_on_data_error_in(1'b0),
     .gt0_tx_fsm_reset_done_out(),
     .gt0_rx_fsm_reset_done_out(GPIO_EVR_GTX_status[0]),
     .gt0_data_valid_in(1'b1),

    //_________________________________________________________________________
    //GT0  (X0Y10)
    //____________________________CHANNEL PORTS________________________________
    //------------------------------- CPLL Ports -------------------------------
        .gt0_cpllfbclklost_out(),
        .gt0_cplllock_out(),
        .gt0_cplllockdetclk_in(sysClk),
        .gt0_cpllreset_in(GPIO_EVR_GTX_control[0]),
    //------------------------ Channel - Clocking Ports ------------------------
        .gt0_gtrefclk0_in(evrRefClk),
        .gt0_gtrefclk1_in(1'b0),
    //-------------------------- Channel - DRP Ports  --------------------------
        .gt0_drpaddr_in(DRP0_daddr),
        .gt0_drpclk_in(sysClk),
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
    //---------------- Receive Ports - FPGA RX Interface Ports -----------------
        .gt0_rxusrclk_in(evrClk),
        .gt0_rxusrclk2_in(evrClk),
    //---------------- Receive Ports - FPGA RX interface Ports -----------------
        .gt0_rxdata_out(gt0_rxdata_out),
    //---------------- Receive Ports - RX 8B/10B Decoder Ports -----------------
        .gt0_rxdisperr_out(gt0_rxdisperr_out),
        .gt0_rxnotintable_out(gt0_rxnotintable_out),
    //------------------------- Receive Ports - RX AFE -------------------------
        .gt0_gtxrxp_in(SFP0_RX_P),
    //---------------------- Receive Ports - RX AFE Ports ----------------------
        .gt0_gtxrxn_in(SFP0_RX_N),
    //----------------- Receive Ports - RX Buffer Bypass Ports -----------------
        .gt0_rxphmonitor_out(),
        .gt0_rxphslipmonitor_out(),
    //------------------- Receive Ports - RX Equalizer Ports -------------------
        .gt0_rxdfelpmreset_in(1'b0),
        .gt0_rxmonitorout_out(),
        .gt0_rxmonitorsel_in(2'b0),
    //------------- Receive Ports - RX Fabric Output Control Ports -------------
        .gt0_rxoutclk_out(gt0_rxoutclk_out),
        .gt0_rxoutclkfabric_out(),
    //----------- Receive Ports - RX Initialization and Reset Ports ------------
        .gt0_gtrxreset_in(GPIO_EVR_GTX_control[2]),
        .gt0_rxpmareset_in(GPIO_EVR_GTX_control[3]),
    //----------------- Receive Ports - RX8B/10B Decoder Ports -----------------
        .gt0_rxcharisk_out(gt0_rxcharisk_out),
    //------------ Receive Ports -RX Initialization and Reset Ports ------------
        .gt0_rxresetdone_out(GPIO_EVR_GTX_status[1]),
    //------------------- TX Initialization and Reset Ports --------------------
        .gt0_gttxreset_in(1'b0),
    //____________________________COMMON PORTS________________________________
     .gt0_qplloutclk_in(1'b0),
     .gt0_qplloutrefclk_in(1'b0));

// Announce that receiver clock is locked only when all
// characters have been valid 8b/10b codes for a while.
(* mark_debug = EVR_DEBUG *) reg       evrRxLocked = 0;
                             reg [3:0] evrGoodCount = 0;
always @(posedge evrClk) begin
    if (|gt0_rxnotintable_out) begin
        evrRxLocked <= 0;
        evrGoodCount <= 0;
    end
    else if (evrGoodCount == 15) begin
        evrRxLocked <= 1;
    end
    else begin
        evrGoodCount <= evrGoodCount + 1;
    end
end

// Instantiate the event receiver
tinyEVR #(.DEBUG(EVR_DEBUG)) tinyEVR (
      .evrRxClk(evrClk),
      .evrRxWord(gt0_rxdata_out & {16{evrRxLocked}}),
      .evrCharIsK(gt0_rxcharisk_out),
      .ppsMarker(evrSecondsMarker),
      .timestampValid(GPIO_EVR_GTX_status[2]),
      .timestamp(evrTimestamp),
      .evStrobe(),
      .distributedDataBus());

// Forward time to other clock domains
wire [TIMESTAMP_WIDTH-1:0] sysTimestamp, auroraTimestamp;
forwardData #(.WIDTH(TIMESTAMP_WIDTH))
  forwardTimeToSys(.srcClk(evrClk),
                   .srcData(evrTimestamp),
                   .dstClk(sysClk),
                   .dstData(sysTimestamp));
forwardData #(.WIDTH(TIMESTAMP_WIDTH))
  forwardTimeToAurora(.srcClk(evrClk),
                      .srcData(evrTimestamp),
                      .dstClk(auroraUserClk),
                      .dstData(auroraTimestamp));

//////////////////////////////////////////////////////////////////////////////
// Aurora links

parameter AURORA_WIDTH = 16;

// The following are multiplexed into the downstream logic using the
// link number as the multiplex selector.
wire                   [(8*MAX_LINK_COUNT)-1:0] AuroraStatus;
(* mark_debug = "false" *)
wire                       [MAX_LINK_COUNT-1:0] auroraINPtoggles;
wire      [(NODES_PER_LINK*MAX_LINK_COUNT)-1:0] auroraStaleNodes;
wire [(INTERLOCKS_PER_NODE*MAX_LINK_COUNT)-1:0] auroraInputStates;
wire [(INTERLOCKS_PER_NODE*MAX_LINK_COUNT)-1:0] auroraInputTransitions;
wire                       [MAX_LINK_COUNT-1:0] auroraTransitionFlags;
wire    [(NODE_INDEX_WIDTH*MAX_LINK_COUNT)-1:0] auroraNodeIndices;
wire   [(SYSMON_ADDR_WIDTH*MAX_LINK_COUNT)-1:0] auroraSysmonAddresses;
wire        [(AURORA_WIDTH*MAX_LINK_COUNT)-1:0] auroraSysmonDataWords;

// Daisy chains
genvar i, n;
generate
for (i = 0 ; i < LINK_COUNT ; i = i + 1) begin : link
wire auroraCORE_STATUS_channel_up;
wire auroraCORE_STATUS_frame_err;
wire auroraCORE_STATUS_hard_err;
wire auroraCORE_STATUS_rx_resetdone_out;
wire auroraCORE_STATUS_soft_err;
wire auroraCORE_STATUS_tx_lock;
wire auroraCORE_STATUS_tx_resetdone_out;
wire [AURORA_WIDTH-1:0] auroraUSER_DATA_M_AXI_RX_tdata;
wire                    auroraUSER_DATA_M_AXI_RX_tlast;
wire                    auroraUSER_DATA_M_AXI_RX_tvalid;

assign AuroraStatus[i*8+0] = auroraCORE_STATUS_rx_resetdone_out;
assign AuroraStatus[i*8+1] = auroraCORE_STATUS_tx_resetdone_out;
assign AuroraStatus[i*8+2] = auroraCORE_STATUS_tx_lock;
assign AuroraStatus[i*8+3] = auroraCORE_STATUS_soft_err;
assign AuroraStatus[i*8+4] = auroraCORE_STATUS_hard_err;
assign AuroraStatus[i*8+5] = auroraCORE_STATUS_frame_err;
assign AuroraStatus[i*8+6] = 1'b0;
assign AuroraStatus[i*8+7] = auroraCORE_STATUS_channel_up;

// Transpose stale nodes
wire [NODES_PER_LINK-1:0] auroraStale;
for (n = 0 ; n < NODES_PER_LINK ; n = n + 1) begin : node
assign auroraStaleNodes[n*MAX_LINK_COUNT+i] = auroraStale[n];
end

receiveINPpacket #(.INTERLOCKS_PER_NODE(INTERLOCKS_PER_NODE),
                   .NODES_PER_LINK(NODES_PER_LINK),
                   .MAX_LINK_COUNT(MAX_LINK_COUNT),
                   .DEBUG(i==1 ? "true" : "false"))
  receiveINPpacket_i (
    .clk(auroraUserClk),
    .incomingTDATA(auroraUSER_DATA_M_AXI_RX_tdata),
    .incomingTVALID(auroraUSER_DATA_M_AXI_RX_tvalid),
    .incomingTLAST(auroraUSER_DATA_M_AXI_RX_tlast),
    .staleNodes(auroraStale),
    .newDataToggle(auroraINPtoggles[i]),
    .inputState(auroraInputStates[i*INTERLOCKS_PER_NODE+:INTERLOCKS_PER_NODE]),
    .inputTransitions (auroraInputTransitions[i*INTERLOCKS_PER_NODE+:
                                                        INTERLOCKS_PER_NODE]),
    .transitionFlag(auroraTransitionFlags[i]),
    .nodeIndex(auroraNodeIndices[i*NODE_INDEX_WIDTH+:NODE_INDEX_WIDTH]),
    .sysmonIndex(auroraSysmonAddresses[i*SYSMON_ADDR_WIDTH+:SYSMON_ADDR_WIDTH]),
    .sysmonTDATA(auroraSysmonDataWords[i*AURORA_WIDTH+:AURORA_WIDTH]));
end /* for */

// Null out remainder of values
for (i = LINK_COUNT ; i < MAX_LINK_COUNT ; i = i + 1) begin
  assign AuroraStatus[i*8+:8] = 8'h00;
  for (n = 0 ; n < NODES_PER_LINK ; n = n + 1) begin : node
    assign auroraStaleNodes[n*MAX_LINK_COUNT+i] = 1'b1;
  end
end

endgenerate

//////////////////////////////////////////////////////////////////////////////
// Multiplex values into downstream logic
(* mark_debug = "false" *)
reg    [LINK_INDEX_WIDTH-1:0] auroraLinkMuxSel = 0, auroraLinkIndex = 0;
(* mark_debug = "false" *)
reg [INTERLOCKS_PER_NODE-1:0] auroraInputStateMux, auroraInputTransitionsMux;
(* mark_debug = "false" *)
reg    [NODE_INDEX_WIDTH-1:0] auroraNodeIndexMux;
(* mark_debug = "false" *)
reg   [SYSMON_ADDR_WIDTH-1:0] auroraSysmonAddrMux;
(* mark_debug = "false" *)
reg        [AURORA_WIDTH-1:0] auroraSysmonDataMux;
(* mark_debug = "false" *)
reg                           auroraTransitionFlagMux;
(* mark_debug = "false" *)
reg      [MAX_LINK_COUNT-1:0] auroraINPmatches;
(* mark_debug = "false" *)
reg                           auroraMuxValNew = 0;

always @(posedge auroraUserClk) begin
    auroraLinkIndex <= auroraLinkMuxSel;
    auroraLinkMuxSel <= (auroraLinkMuxSel == (LINK_COUNT-1)) ? 0 :
                                                           auroraLinkMuxSel + 1;
    auroraInputStateMux <=
                        auroraInputStates[auroraLinkMuxSel*INTERLOCKS_PER_NODE+:
                                                           INTERLOCKS_PER_NODE];
    auroraInputTransitionsMux <=
                   auroraInputTransitions[auroraLinkMuxSel*INTERLOCKS_PER_NODE+:
                                                           INTERLOCKS_PER_NODE];
    auroraTransitionFlagMux <= auroraTransitionFlags[auroraLinkMuxSel];
    auroraNodeIndexMux <= auroraNodeIndices[auroraLinkMuxSel*NODE_INDEX_WIDTH+:
                                                             NODE_INDEX_WIDTH];
    auroraSysmonAddrMux <=
                      auroraSysmonAddresses[auroraLinkMuxSel*SYSMON_ADDR_WIDTH+:
                                                             SYSMON_ADDR_WIDTH];
    auroraSysmonDataMux <= auroraSysmonDataWords[auroraLinkMuxSel*AURORA_WIDTH+:
                                                                  AURORA_WIDTH];
    if (auroraINPtoggles[auroraLinkMuxSel] !=
                                       auroraINPmatches[auroraLinkMuxSel]) begin
        auroraINPmatches[auroraLinkMuxSel] <=
                                            !auroraINPmatches[auroraLinkMuxSel];
        auroraMuxValNew <= 1;
    end
    else begin
        auroraMuxValNew <= 0;
    end
end
wire [ACQ_INDEX_WIDTH-1:0] auroraAcqIndex = { auroraNodeIndexMux,
                                              auroraLinkIndex };

//////////////////////////////////////////////////////////////////////////////
// System monitoring
wire [31:0] GPIO_SYSMON_READOUT_tri_i, GPIO_SYSMON_READOUT_tri_o;

sysmonDPRAM #(.ADDR_WIDTH(ACQ_INDEX_WIDTH+SYSMON_ADDR_WIDTH),
              .DATA_WIDTH(AURORA_WIDTH),
              .DEBUG("false"))
  sysmonDPRAM(
    .wclk(auroraUserClk),
    .wen(auroraMuxValNew),
    .waddr({auroraAcqIndex, auroraSysmonAddrMux}),
    .wdata(auroraSysmonDataMux),
    .clk(sysClk),
    .addr(GPIO_SYSMON_READOUT_tri_o[
                      NODE_INDEX_WIDTH+LINK_INDEX_WIDTH+SYSMON_ADDR_WIDTH-1:0]),
    .data(GPIO_SYSMON_READOUT_tri_i[AURORA_WIDTH-1:0]));

//////////////////////////////////////////////////////////////////////////////
// Logging
parameter LOG_AXI_WIDTH = 32;
wire [LOG_AXI_WIDTH-1:0] logTDATA;
wire                     logTVALID, logTLAST, logTREADY;
wire [MITIGATION_COUNT-1:0] auroraMitigationOutputs;
wire sysLogEnable = sysControl[2];
wire sysLogForce  = sysControl[3];

createLOGstream #(
          .MITIGATION_COUNT(MITIGATION_COUNT),
          .TIMESTAMP_WIDTH(TIMESTAMP_WIDTH),
          .INTERLOCKS_PER_NODE(INTERLOCKS_PER_NODE),
          .AXI_WIDTH(LOG_AXI_WIDTH),
          .ACQ_INDEX_WIDTH(ACQ_INDEX_WIDTH),
          .DEBUG("false"))
  createLOGstream (
    .sysClk(sysClk),
    .sysLogEnable(sysLogEnable),
    .sysLogForce(sysLogForce),
    .clk(auroraUserClk),
    .timestamp(auroraTimestamp),
    .outputState(auroraMitigationOutputs),
    .newDataStrobe(auroraMuxValNew),
    .transitionFlag(auroraTransitionFlagMux),
    .acqIndex(auroraAcqIndex),
    .inputState(auroraInputStateMux),
    .inputTransitions(auroraInputTransitionsMux),
    .sysLogTDATA(logTDATA),
    .sysLogTREADY(logTREADY),
    .sysLogTVALID(logTVALID),
    .sysLogTLAST(logTLAST));

//////////////////////////////////////////////////////////////////////////////
// Log Marker
wire [LOG_AXI_WIDTH-1:0] markerTDATA;
wire                     markerTVALID, markerTLAST, markerTREADY;

createMarkerStream #(
    .SYSCLK_RATE(SYSCLK_RATE),
    .TIMESTAMP_WIDTH(TIMESTAMP_WIDTH),
    .AXI_WIDTH(LOG_AXI_WIDTH),
    .DMA_COUNT(1+((TIMESTAMP_WIDTH+2*INTERLOCKS_PER_NODE)/LOG_AXI_WIDTH)),
    .DEBUG("false"))
  createMarkerStream (
    .sysClk(sysClk),
    .sysLogEnable(sysLogEnable),
    .sysTimestamp(sysTimestamp),
    .sysMarkerTDATA(markerTDATA),
    .sysMarkerTREADY(markerTREADY),
    .sysMarkerTVALID(markerTVALID),
    .sysMarkerTLAST(markerTLAST));

//////////////////////////////////////////////////////////////////////////////
// Fast protection
wire [15:0] mitigateMemAddress;
wire [31:0] mitigateMemWrData;
wire        mitigateMemWrStrobe;
wire [31:0] mitigationControl;

mitigate #(.INTERLOCKS_PER_NODE(INTERLOCKS_PER_NODE),
           .MITIGATION_COUNT(MITIGATION_COUNT),
           .MAX_LINK_COUNT(MAX_LINK_COUNT),
           .NODES_PER_LINK(NODES_PER_LINK),
           .ACQ_INDEX_WIDTH(ACQ_INDEX_WIDTH),
           .DEBUG("false"))
  mitigate (
          .sysClk(sysClk),
          .sysMemAddress(mitigateMemAddress[13:0]),
          .sysMemWrData(mitigateMemWrData),
          .sysMemWrStrobe(mitigateMemWrStrobe),
          .sysSetToggle(mitigationControl[MITIGATION_COUNT-1:0]),
          .clk(auroraUserClk),
          .newDataStrobe(auroraMuxValNew),
          .acqIndex(auroraAcqIndex),
          .inputState(auroraInputStateMux),
          .inputTransitions(auroraInputTransitionsMux),
          .staleNodes(auroraStaleNodes),
          .mitigationOutputs(auroraMitigationOutputs));

// Show some outputs on LEDs
assign MITIGATION_OUTPUTS = auroraMitigationOutputs;
assign GPIO_LED_LEFT   = auroraMitigationOutputs[0];
assign GPIO_LED_CENTER = auroraMitigationOutputs[1];
assign GPIO_LED_RIGHT  = auroraMitigationOutputs[2];
assign GPIO_LED_0      = auroraMitigationOutputs[3];

// Copy outputs to header for possible connection to external logic analyzer.
genvar o;
generate
for (o = 0 ; o < 8 ; o = o + 1) begin : pmod1
  if (o < MITIGATION_COUNT)
    assign PMOD1_DIAG[o] = auroraMitigationOutputs[o];
  else
    assign PMOD1_DIAG[o] = 1'b0;
end /* for */
endgenerate

//////////////////////////////////////////////////////////////////////////////
// Use a single I2c controller for all FMC SFP module operations.
// Drive all outputs in common and multiplex incoming streams.
parameter SFP_DEBUG = "false";
wire [31:0] GPIO_SFP_STATUS = {8'b0,
                               4'b0,
                               FMC_HPC_SFP4_TXFAULT, FMC_HPC_SFP3_TXFAULT,
                               FMC_HPC_SFP2_TXFAULT, FMC_HPC_SFP1_TXFAULT,
                               4'b0,
                               FMC_HPC_SFP4_RXLOS, FMC_HPC_SFP3_RXLOS,
                               FMC_HPC_SFP2_RXLOS, FMC_HPC_SFP1_RXLOS,
                               4'b0,
                               FMC_HPC_SFP4_MODABS, FMC_HPC_SFP3_MODABS,
                               FMC_HPC_SFP2_MODABS, FMC_HPC_SFP1_MODABS};

parameter SFP_COUNT = 5;
parameter SFP_ADDR_WIDTH = $clog2(SFP_COUNT);

(*mark_debug=SFP_DEBUG*) wire [SFP_ADDR_WIDTH-1:0] SFP_IIC_SELECT;
(*mark_debug=SFP_DEBUG*) wire [SFP_COUNT-1:0] SFP_IIC_SCL_I, SFP_IIC_SDA_I;
(*mark_debug=SFP_DEBUG*) wire SFPS_IIC_SCL_I = SFP_IIC_SCL_I[SFP_IIC_SELECT+:1];
(*mark_debug=SFP_DEBUG*) wire SFPS_IIC_SDA_I = SFP_IIC_SDA_I[SFP_IIC_SELECT+:1];
(*mark_debug=SFP_DEBUG*) wire SFPS_IIC_SCL_O, SFPS_IIC_SDA_O, SFPS_IIC_SCL_T, SFPS_IIC_SDA_T;

// I/O pins
IOBUF SFP1_SCL(.I(SFPS_IIC_SCL_O),
               .O(SFP_IIC_SCL_I[0]),
               .T(SFPS_IIC_SCL_T),
               .IO(FMC_HPC_SFP1_SCL));
IOBUF SFP1_SDA(.I(SFPS_IIC_SDA_O),
               .O(SFP_IIC_SDA_I[0]),
               .T(SFPS_IIC_SDA_T),
               .IO(FMC_HPC_SFP1_SDA));
IOBUF SFP2_SCL(.I(SFPS_IIC_SCL_O),
               .O(SFP_IIC_SCL_I[1]),
               .T(SFPS_IIC_SCL_T),
               .IO(FMC_HPC_SFP2_SCL));
IOBUF SFP2_SDA(.I(SFPS_IIC_SDA_O),
               .O(SFP_IIC_SDA_I[1]),
               .T(SFPS_IIC_SDA_T),
               .IO(FMC_HPC_SFP2_SDA));
IOBUF SFP3_SCL(.I(SFPS_IIC_SCL_O),
               .O(SFP_IIC_SCL_I[2]),
               .T(SFPS_IIC_SCL_T),
               .IO(FMC_HPC_SFP3_SCL));
IOBUF SFP3_SDA(.I(SFPS_IIC_SDA_O),
               .O(SFP_IIC_SDA_I[2]),
               .T(SFPS_IIC_SDA_T),
               .IO(FMC_HPC_SFP3_SDA));
IOBUF SFP4_SCL(.I(SFPS_IIC_SCL_O),
               .O(SFP_IIC_SCL_I[3]),
               .T(SFPS_IIC_SCL_T),
               .IO(FMC_HPC_SFP4_SCL));
IOBUF SFP4_SDA(.I(SFPS_IIC_SDA_O),
               .O(SFP_IIC_SDA_I[3]),
               .T(SFPS_IIC_SDA_T),
               .IO(FMC_HPC_SFP4_SDA));

// Static outputs
assign FMC_HPC_SFP1_TXDISABLE = 1'b0;
assign FMC_HPC_SFP2_TXDISABLE = 1'b0;
assign FMC_HPC_SFP3_TXDISABLE = 1'b0;
assign FMC_HPC_SFP4_TXDISABLE = 1'b0;
assign FMC_HPC_SFP1_RS0 = 1'b1;
assign FMC_HPC_SFP2_RS0 = 1'b1;
assign FMC_HPC_SFP3_RS0 = 1'b1;
assign FMC_HPC_SFP4_RS0 = 1'b1;
assign FMC_HPC_SFP1_RS1 = 1'b1;
assign FMC_HPC_SFP2_RS1 = 1'b1;
assign FMC_HPC_SFP3_RS1 = 1'b1;
assign FMC_HPC_SFP4_RS1 = 1'b1;

//////////////////////////////////////////////////////////////////////////////
// Fan tachometer
wire [15:0] fanRPM;
fanTach #(.CLOCK_RATE(SYSCLK_RATE),
          .PULSES_PER_REVOLUTION(2))
  fanTach (.clk(sysClk),
           .fanTach(SM_FAN_TACH),
           .fanRPM(fanRPM));

//////////////////////////////////////////////////////////////////////////////
// Block design

`include "firmwareBuildDate.v"

wire [31:0] sysControl;

base_zynq_design base_zynq_design_i (
    .sysClk(sysClk),
    .sysReset_n(sysReset_n),
    .auroraUserClk(auroraUserClk),

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

    .GTXreset(sysControl[0]),

    .DRP0_daddr(DRP0_daddr),
    .DRP0_den(DRP0_den),
    .DRP0_di(DRP0_di),
    .DRP0_do(DRP0_do),
    .DRP0_drdy(DRP0_drdy),
    .DRP0_dwe(DRP0_dwe),

    .AuroraReset(sysControl[1]),
    .AuroraArefclk_clk_n(FMC_HPC_GBTCLK0_M2C_N),
    .AuroraArefclk_clk_p(FMC_HPC_GBTCLK0_M2C_P),
    .AuroraBrefclk_clk_n(FMC_HPC_GBTCLK1_M2C_N),
    .AuroraBrefclk_clk_p(FMC_HPC_GBTCLK1_M2C_P),

    // First SFP on CAENels FMC-4SFP+ on HPC
    .Aurora0_CORE_STATUS_channel_up(link[0].auroraCORE_STATUS_channel_up),
    .Aurora0_CORE_STATUS_frame_err(link[0].auroraCORE_STATUS_frame_err),
    .Aurora0_CORE_STATUS_hard_err(link[0].auroraCORE_STATUS_hard_err),
    .Aurora0_CORE_STATUS_rx_resetdone_out(link[0].auroraCORE_STATUS_rx_resetdone_out),
    .Aurora0_CORE_STATUS_soft_err(link[0].auroraCORE_STATUS_soft_err),
    .Aurora0_CORE_STATUS_tx_lock(link[0].auroraCORE_STATUS_tx_lock),
    .Aurora0_CORE_STATUS_tx_resetdone_out(link[0].auroraCORE_STATUS_tx_resetdone_out),
    .Aurora0_RX_rxn(FMC_HPC_SFP1_RX_N),
    .Aurora0_RX_rxp(FMC_HPC_SFP1_RX_P),
    .Aurora0_TX_txn(FMC_HPC_SFP1_TX_N),
    .Aurora0_TX_txp(FMC_HPC_SFP1_TX_P),
    .Aurora0_USER_DATA_M_AXI_RX_tdata(link[0].auroraUSER_DATA_M_AXI_RX_tdata),
    .Aurora0_USER_DATA_M_AXI_RX_tlast(link[0].auroraUSER_DATA_M_AXI_RX_tlast),
    .Aurora0_USER_DATA_M_AXI_RX_tvalid(link[0].auroraUSER_DATA_M_AXI_RX_tvalid),
    .Aurora0_USER_DATA_S_AXI_TX_tdata(16'h0000),
    .Aurora0_USER_DATA_S_AXI_TX_tkeep(2'b11),
    .Aurora0_USER_DATA_S_AXI_TX_tlast(1'b0),
    .Aurora0_USER_DATA_S_AXI_TX_tready(),
    .Aurora0_USER_DATA_S_AXI_TX_tvalid(1'b0),

    // Second SFP on CAENels FMC-4SFP+ on HPC
    .Aurora1_CORE_STATUS_channel_up(link[1].auroraCORE_STATUS_channel_up),
    .Aurora1_CORE_STATUS_frame_err(link[1].auroraCORE_STATUS_frame_err),
    .Aurora1_CORE_STATUS_hard_err(link[1].auroraCORE_STATUS_hard_err),
    .Aurora1_CORE_STATUS_rx_resetdone_out(link[1].auroraCORE_STATUS_rx_resetdone_out),
    .Aurora1_CORE_STATUS_soft_err(link[1].auroraCORE_STATUS_soft_err),
    .Aurora1_CORE_STATUS_tx_lock(link[1].auroraCORE_STATUS_tx_lock),
    .Aurora1_CORE_STATUS_tx_resetdone_out(link[1].auroraCORE_STATUS_tx_resetdone_out),
    .Aurora1_RX_rxn(FMC_HPC_SFP2_RX_N),
    .Aurora1_RX_rxp(FMC_HPC_SFP2_RX_P),
    .Aurora1_TX_txn(FMC_HPC_SFP2_TX_N),
    .Aurora1_TX_txp(FMC_HPC_SFP2_TX_P),
    .Aurora1_USER_DATA_M_AXI_RX_tdata(link[1].auroraUSER_DATA_M_AXI_RX_tdata),
    .Aurora1_USER_DATA_M_AXI_RX_tlast(link[1].auroraUSER_DATA_M_AXI_RX_tlast),
    .Aurora1_USER_DATA_M_AXI_RX_tvalid(link[1].auroraUSER_DATA_M_AXI_RX_tvalid),
    .Aurora1_USER_DATA_S_AXI_TX_tdata(16'h0000),
    .Aurora1_USER_DATA_S_AXI_TX_tkeep(2'b11),
    .Aurora1_USER_DATA_S_AXI_TX_tlast(1'b0),
    .Aurora1_USER_DATA_S_AXI_TX_tready(),
    .Aurora1_USER_DATA_S_AXI_TX_tvalid(1'b0),

    // Third SFP on CAENels FMC-4SFP+ on HPC
    .Aurora2_CORE_STATUS_channel_up(link[2].auroraCORE_STATUS_channel_up),
    .Aurora2_CORE_STATUS_frame_err(link[2].auroraCORE_STATUS_frame_err),
    .Aurora2_CORE_STATUS_hard_err(link[2].auroraCORE_STATUS_hard_err),
    .Aurora2_CORE_STATUS_rx_resetdone_out(link[2].auroraCORE_STATUS_rx_resetdone_out),
    .Aurora2_CORE_STATUS_soft_err(link[2].auroraCORE_STATUS_soft_err),
    .Aurora2_CORE_STATUS_tx_lock(link[2].auroraCORE_STATUS_tx_lock),
    .Aurora2_CORE_STATUS_tx_resetdone_out(link[2].auroraCORE_STATUS_tx_resetdone_out),
    .Aurora2_RX_rxn(FMC_HPC_SFP3_RX_N),
    .Aurora2_RX_rxp(FMC_HPC_SFP3_RX_P),
    .Aurora2_TX_txn(FMC_HPC_SFP3_TX_N),
    .Aurora2_TX_txp(FMC_HPC_SFP3_TX_P),
    .Aurora2_USER_DATA_M_AXI_RX_tdata(link[2].auroraUSER_DATA_M_AXI_RX_tdata),
    .Aurora2_USER_DATA_M_AXI_RX_tlast(link[2].auroraUSER_DATA_M_AXI_RX_tlast),
    .Aurora2_USER_DATA_M_AXI_RX_tvalid(link[2].auroraUSER_DATA_M_AXI_RX_tvalid),
    .Aurora2_USER_DATA_S_AXI_TX_tdata(16'h0000),
    .Aurora2_USER_DATA_S_AXI_TX_tkeep(2'b11),
    .Aurora2_USER_DATA_S_AXI_TX_tlast(1'b0),
    .Aurora2_USER_DATA_S_AXI_TX_tready(),
    .Aurora2_USER_DATA_S_AXI_TX_tvalid(1'b0),

    // Fourth SFP on CAENels FMC-4SFP+ on HPC
    .Aurora3_CORE_STATUS_channel_up(link[3].auroraCORE_STATUS_channel_up),
    .Aurora3_CORE_STATUS_frame_err(link[3].auroraCORE_STATUS_frame_err),
    .Aurora3_CORE_STATUS_hard_err(link[3].auroraCORE_STATUS_hard_err),
    .Aurora3_CORE_STATUS_rx_resetdone_out(link[3].auroraCORE_STATUS_rx_resetdone_out),
    .Aurora3_CORE_STATUS_soft_err(link[3].auroraCORE_STATUS_soft_err),
    .Aurora3_CORE_STATUS_tx_lock(link[3].auroraCORE_STATUS_tx_lock),
    .Aurora3_CORE_STATUS_tx_resetdone_out(link[3].auroraCORE_STATUS_tx_resetdone_out),
    .Aurora3_RX_rxn(FMC_HPC_SFP4_RX_N),
    .Aurora3_RX_rxp(FMC_HPC_SFP4_RX_P),
    .Aurora3_TX_txn(FMC_HPC_SFP4_TX_N),
    .Aurora3_TX_txp(FMC_HPC_SFP4_TX_P),
    .Aurora3_USER_DATA_M_AXI_RX_tdata(link[3].auroraUSER_DATA_M_AXI_RX_tdata),
    .Aurora3_USER_DATA_M_AXI_RX_tlast(link[3].auroraUSER_DATA_M_AXI_RX_tlast),
    .Aurora3_USER_DATA_M_AXI_RX_tvalid(link[3].auroraUSER_DATA_M_AXI_RX_tvalid),
    .Aurora3_USER_DATA_S_AXI_TX_tdata(16'h0000),
    .Aurora3_USER_DATA_S_AXI_TX_tkeep(2'b11),
    .Aurora3_USER_DATA_S_AXI_TX_tlast(1'b0),
    .Aurora3_USER_DATA_S_AXI_TX_tready(),
    .Aurora3_USER_DATA_S_AXI_TX_tvalid(1'b0),

    // Fifth SFP on FM-S18 on HPC
    .Aurora4_CORE_STATUS_channel_up(link[4].auroraCORE_STATUS_channel_up),
    .Aurora4_CORE_STATUS_frame_err(link[4].auroraCORE_STATUS_frame_err),
    .Aurora4_CORE_STATUS_hard_err(link[4].auroraCORE_STATUS_hard_err),
    .Aurora4_CORE_STATUS_rx_resetdone_out(link[4].auroraCORE_STATUS_rx_resetdone_out),
    .Aurora4_CORE_STATUS_soft_err(link[4].auroraCORE_STATUS_soft_err),
    .Aurora4_CORE_STATUS_tx_lock(link[4].auroraCORE_STATUS_tx_lock),
    .Aurora4_CORE_STATUS_tx_resetdone_out(link[4].auroraCORE_STATUS_tx_resetdone_out),
    .Aurora4_RX_rxn(FMC_HPC_SFP5_RX_N),
    .Aurora4_RX_rxp(FMC_HPC_SFP5_RX_P),
    .Aurora4_TX_txn(FMC_HPC_SFP5_TX_N),
    .Aurora4_TX_txp(FMC_HPC_SFP5_TX_P),
    .Aurora4_USER_DATA_M_AXI_RX_tdata(link[4].auroraUSER_DATA_M_AXI_RX_tdata),
    .Aurora4_USER_DATA_M_AXI_RX_tlast(link[4].auroraUSER_DATA_M_AXI_RX_tlast),
    .Aurora4_USER_DATA_M_AXI_RX_tvalid(link[4].auroraUSER_DATA_M_AXI_RX_tvalid),
    .Aurora4_USER_DATA_S_AXI_TX_tdata(16'h0000),
    .Aurora4_USER_DATA_S_AXI_TX_tkeep(2'b11),
    .Aurora4_USER_DATA_S_AXI_TX_tlast(1'b0),
    .Aurora4_USER_DATA_S_AXI_TX_tready(),
    .Aurora4_USER_DATA_S_AXI_TX_tvalid(1'b0),

    // Sixth SFP on FM-S18 on HPC
    .Aurora5_CORE_STATUS_channel_up(link[5].auroraCORE_STATUS_channel_up),
    .Aurora5_CORE_STATUS_frame_err(link[5].auroraCORE_STATUS_frame_err),
    .Aurora5_CORE_STATUS_hard_err(link[5].auroraCORE_STATUS_hard_err),
    .Aurora5_CORE_STATUS_rx_resetdone_out(link[5].auroraCORE_STATUS_rx_resetdone_out),
    .Aurora5_CORE_STATUS_soft_err(link[5].auroraCORE_STATUS_soft_err),
    .Aurora5_CORE_STATUS_tx_lock(link[5].auroraCORE_STATUS_tx_lock),
    .Aurora5_CORE_STATUS_tx_resetdone_out(link[5].auroraCORE_STATUS_tx_resetdone_out),
    .Aurora5_RX_rxn(FMC_HPC_SFP6_RX_N),
    .Aurora5_RX_rxp(FMC_HPC_SFP6_RX_P),
    .Aurora5_TX_txn(FMC_HPC_SFP6_TX_N),
    .Aurora5_TX_txp(FMC_HPC_SFP6_TX_P),
    .Aurora5_USER_DATA_M_AXI_RX_tdata(link[5].auroraUSER_DATA_M_AXI_RX_tdata),
    .Aurora5_USER_DATA_M_AXI_RX_tlast(link[5].auroraUSER_DATA_M_AXI_RX_tlast),
    .Aurora5_USER_DATA_M_AXI_RX_tvalid(link[5].auroraUSER_DATA_M_AXI_RX_tvalid),
    .Aurora5_USER_DATA_S_AXI_TX_tdata(16'h0000),
    .Aurora5_USER_DATA_S_AXI_TX_tkeep(2'b11),
    .Aurora5_USER_DATA_S_AXI_TX_tlast(1'b0),
    .Aurora5_USER_DATA_S_AXI_TX_tready(),
    .Aurora5_USER_DATA_S_AXI_TX_tvalid(1'b0),

    // Seventh SFP on FM-S18 on HPC
    .Aurora6_CORE_STATUS_channel_up(link[6].auroraCORE_STATUS_channel_up),
    .Aurora6_CORE_STATUS_frame_err(link[6].auroraCORE_STATUS_frame_err),
    .Aurora6_CORE_STATUS_hard_err(link[6].auroraCORE_STATUS_hard_err),
    .Aurora6_CORE_STATUS_rx_resetdone_out(link[6].auroraCORE_STATUS_rx_resetdone_out),
    .Aurora6_CORE_STATUS_soft_err(link[6].auroraCORE_STATUS_soft_err),
    .Aurora6_CORE_STATUS_tx_lock(link[6].auroraCORE_STATUS_tx_lock),
    .Aurora6_CORE_STATUS_tx_resetdone_out(link[6].auroraCORE_STATUS_tx_resetdone_out),
    .Aurora6_RX_rxn(FMC_HPC_SFP7_RX_N),
    .Aurora6_RX_rxp(FMC_HPC_SFP7_RX_P),
    .Aurora6_TX_txn(FMC_HPC_SFP7_TX_N),
    .Aurora6_TX_txp(FMC_HPC_SFP7_TX_P),
    .Aurora6_USER_DATA_M_AXI_RX_tdata(link[6].auroraUSER_DATA_M_AXI_RX_tdata),
    .Aurora6_USER_DATA_M_AXI_RX_tlast(link[6].auroraUSER_DATA_M_AXI_RX_tlast),
    .Aurora6_USER_DATA_M_AXI_RX_tvalid(link[6].auroraUSER_DATA_M_AXI_RX_tvalid),
    .Aurora6_USER_DATA_S_AXI_TX_tdata(16'h0000),
    .Aurora6_USER_DATA_S_AXI_TX_tkeep(2'b11),
    .Aurora6_USER_DATA_S_AXI_TX_tlast(1'b0),
    .Aurora6_USER_DATA_S_AXI_TX_tready(),
    .Aurora6_USER_DATA_S_AXI_TX_tvalid(1'b0),

    // Eighth SFP on FM-S18 on HPC
    .Aurora7_CORE_STATUS_channel_up(link[7].auroraCORE_STATUS_channel_up),
    .Aurora7_CORE_STATUS_frame_err(link[7].auroraCORE_STATUS_frame_err),
    .Aurora7_CORE_STATUS_hard_err(link[7].auroraCORE_STATUS_hard_err),
    .Aurora7_CORE_STATUS_rx_resetdone_out(link[7].auroraCORE_STATUS_rx_resetdone_out),
    .Aurora7_CORE_STATUS_soft_err(link[7].auroraCORE_STATUS_soft_err),
    .Aurora7_CORE_STATUS_tx_lock(link[7].auroraCORE_STATUS_tx_lock),
    .Aurora7_CORE_STATUS_tx_resetdone_out(link[7].auroraCORE_STATUS_tx_resetdone_out),
    .Aurora7_RX_rxn(FMC_HPC_SFP8_RX_N),
    .Aurora7_RX_rxp(FMC_HPC_SFP8_RX_P),
    .Aurora7_TX_txn(FMC_HPC_SFP8_TX_N),
    .Aurora7_TX_txp(FMC_HPC_SFP8_TX_P),
    .Aurora7_USER_DATA_M_AXI_RX_tdata(link[7].auroraUSER_DATA_M_AXI_RX_tdata),
    .Aurora7_USER_DATA_M_AXI_RX_tlast(link[7].auroraUSER_DATA_M_AXI_RX_tlast),
    .Aurora7_USER_DATA_M_AXI_RX_tvalid(link[7].auroraUSER_DATA_M_AXI_RX_tvalid),
    .Aurora7_USER_DATA_S_AXI_TX_tdata(16'h0000),
    .Aurora7_USER_DATA_S_AXI_TX_tkeep(2'b11),
    .Aurora7_USER_DATA_S_AXI_TX_tlast(1'b0),
    .Aurora7_USER_DATA_S_AXI_TX_tready(),
    .Aurora7_USER_DATA_S_AXI_TX_tvalid(1'b0),

    .Log_AXIS_tdata(logTDATA),
    .Log_AXIS_tlast(logTLAST),
    .Log_AXIS_tvalid(logTVALID),
    .Log_AXIS_tready(logTREADY),
    .Marker_AXIS_tdata(markerTDATA),
    .Marker_AXIS_tlast(markerTLAST),
    .Marker_AXIS_tvalid(markerTVALID),
    .Marker_AXIS_tready(markerTREADY),

    .GPIO_SYSMON_READOUT_tri_i(GPIO_SYSMON_READOUT_tri_i),
    .GPIO_SYSMON_READOUT_tri_o(GPIO_SYSMON_READOUT_tri_o),

    .GPIO_FIRMWARE_DATE_tri_i(FIRMWARE_BUILD_DATE),
    .GPIO_FAN_SPEED_tri_i({16'h0000, fanRPM}),

    .GPIO_EVR_GTX_tri_o(GPIO_EVR_GTX_control),
    .GPIO_EVR_GTX_tri_i({2'b0, GPIO_EVR_GTX_status}),
    .GPIO_TIMESTAMP_SECONDS_tri_i(sysTimestamp[63:32]),
    .GPIO_TIMESTAMP_TICKS_tri_i(sysTimestamp[31:0]),

    .GPIO_AURORA_A_tri_o(sysControl),
    .GPIO_AURORA_A_tri_i(AuroraStatus[0+:32]),
    .GPIO_AURORA_B_tri_i(AuroraStatus[32+:32]),

    .GPIO_STALE_NODES_LO_tri_i(auroraStaleNodes[0+:32]),
    .GPIO_STALE_NODES_HI_tri_i({
                      {32-(NODES_PER_LINK*MAX_LINK_COUNT){1'b0}},
                      auroraStaleNodes[32+:NODES_PER_LINK*MAX_LINK_COUNT-32] }),

    .mitigationBRAMctrl_addr(mitigateMemAddress),
    .mitigationBRAMctrl_din(mitigateMemWrData),
    .mitigationBRAMctrl_we(mitigateMemWrStrobe),
    .mitigationBRAMctrl_dout(32'h0),

    .GPIO_MITIGATION_CONTROL_tri_o(mitigationControl),

    .SFP_IIC_scl_i(SFPS_IIC_SCL_I),
    .SFP_IIC_scl_o(SFPS_IIC_SCL_O),
    .SFP_IIC_scl_t(SFPS_IIC_SCL_T),
    .SFP_IIC_sda_i(SFPS_IIC_SDA_I),
    .SFP_IIC_sda_o(SFPS_IIC_SDA_O),
    .SFP_IIC_sda_t(SFPS_IIC_SDA_T),
    .SFP_IIC_SELECT(SFP_IIC_SELECT)
    );

endmodule
