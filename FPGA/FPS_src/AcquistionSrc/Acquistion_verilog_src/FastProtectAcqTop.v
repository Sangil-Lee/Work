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

module FastProtectAcqTop #(
    parameter INTERLOCK_COUNT = 64
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

    // GPIO LEDs
    output       GPIO_LED_0,
    output       GPIO_LED_CENTER,

    // Logic analzer connection
    output [7:0] PMOD1_DIAG,

    // GPIO Buttons
    input        GPIO_SW_LEFT,
    input        GPIO_SW_RIGHT,

    // FPGA fan
    input        SM_FAN_TACH,
    output       SM_FAN_PWM,

    // Transceiver reference
    input        SI5324_REFCLK_P, SI5324_REFCLK_N,
    input        SI5324_INT_ALM,
    output       SI5324_RST,

    // ZC706 SFP
    output       SFP0_TX_DISABLE,
    output       SFP0_TX_P, SFP0_TX_N,
    input        SFP0_RX_P, SFP0_RX_N,

    // Mezzanine SFP
    inout        SFP1_SCL,
    inout        SFP1_SDA,
    input        SFP1_MODABS,
    output       SFP1_TX_DISABLE,
    output       SFP1_TX_P, SFP1_TX_N,
    input        SFP1_RX_P, SFP1_RX_N,

    // Interlocks
    input [INTERLOCK_COUNT-1:0] interlockPins
    );

//////////////////////////////////////////////////////////////////////////////
// The clock domains
// Net names starting with 'aurora' are in the Aurora user clock domain.
// Other net names are in the system clock domain.
parameter SYSCLK_RATE         = 100000000;
parameter AURORACLK_RATE      = 312500000;
parameter AURORACLK_PERIOD_PS = 3200;

wire sysClk, auroraUserClk;

//////////////////////////////////////////////////////////////////////////////
// Static values
assign SFP1_TX_DISABLE = 1'b0;
`include "firmwareBuildDate.v"

//////////////////////////////////////////////////////////////////////////////
// SFP on LPC FMC
wire LPC_SFP_IIC_scl_i, LPC_SFP_IIC_scl_o, LPC_SFP_IIC_scl_t;
wire LPC_SFP_IIC_sda_i, LPC_SFP_IIC_sda_o, LPC_SFP_IIC_sda_t;
IOBUF LPC_SFP_IIC_SCL(.I(LPC_SFP_IIC_scl_o),
                      .O(LPC_SFP_IIC_scl_i),
                      .T(LPC_SFP_IIC_scl_t),
                      .IO(SFP1_SCL));
IOBUF LPC_SFP_IIC_SDA(.I(LPC_SFP_IIC_sda_o),
                      .O(LPC_SFP_IIC_sda_i),
                      .T(LPC_SFP_IIC_sda_t),
                      .IO(SFP1_SDA));

//////////////////////////////////////////////////////////////////////////////
// System monitoring dual-port RAM
// Values read are used only in the Aurora clock domain, but to ensure that
// values don't get garbled on a write/read conflict all access is from the
// system clock domain.
parameter SYSMON_ADDR_WIDTH = 7; // 128 16-bit monitors

wire [12:0]SYSMON_BRAM_addr;
wire [31:0]SYSMON_BRAM_din;
wire [31:0]SYSMON_BRAM_dout;
wire SYSMON_BRAM_en;
wire SYSMON_BRAM_rst;
wire [3:0]SYSMON_BRAM_we;

reg  [15:0] sysmonDPRAM [0:(1<<SYSMON_ADDR_WIDTH)-1];
reg  [15:0] sysmonQA, sysmonQB;
wire [15:0] auroraSysmonData;

wire [SYSMON_ADDR_WIDTH-1:0] sysmonADDRA;
assign sysmonADDRA = SYSMON_BRAM_addr[2+:SYSMON_ADDR_WIDTH];
assign SYSMON_BRAM_dout = { 16'h0000, sysmonQA };
wire [SYSMON_ADDR_WIDTH-1:0] auroraSysmonAddr;
wire [SYSMON_ADDR_WIDTH-1:0] sysmonADDRB;
always @(posedge sysClk) begin
    sysmonQA <= sysmonDPRAM[sysmonADDRA];
    sysmonQB <= sysmonDPRAM[sysmonADDRB];
    if (|SYSMON_BRAM_we[1:0]) sysmonDPRAM[sysmonADDRA] <= SYSMON_BRAM_din[15:0];
end
forwardData #(.DATA_WIDTH(SYSMON_ADDR_WIDTH)) forwardSysmonAddr (
    .inClk(auroraUserClk),
    .inData(auroraSysmonAddr),
    .outClk(sysClk),
    .outData(sysmonADDRB));
forwardData #(.DATA_WIDTH(16)) forwardSysmonData (
    .inClk(sysClk),
    .inData(sysmonQB),
    .outClk(auroraUserClk),
    .outData(auroraSysmonData));

//////////////////////////////////////////////////////////////////////////////
// Fan tachometer
wire [15:0] fanRPM;
fanTach #(.CLOCK_RATE(SYSCLK_RATE),
          .PULSES_PER_REVOLUTION(2))
  fanTach (.clk(sysClk),
           .fanTach(SM_FAN_TACH),
           .fanRPM(fanRPM));

//////////////////////////////////////////////////////////////////////////////
// Aurora links
// CW and CCW are in terms of data flow, not fiber links.
//
// The CW AXI streams carry data towards the mitigation node.
//
// The CW outgoing AXI stream connects to Auora block A and then to the
// fiber pair leading towards the mitigation node through the SFP module
// on the main board.
//
// The CW incoming AXI stream connects to Auora block B and then to the
// fiber pair leading away from the mitigation node through the first SFP
// module on the LPC FMC slot (J5).
//
parameter  CW_DEBUG = "false";
parameter CCW_DEBUG = "false";
(* mark_debug = CW_DEBUG *) wire [15:0] auroraCWincomingStream_tdata;
(* mark_debug = CW_DEBUG *) wire        auroraCWincomingStream_tlast;
(* mark_debug = CW_DEBUG *) wire        auroraCWincomingStream_tvalid;
(* mark_debug = CW_DEBUG *) wire [15:0] auroraCWforwardedStream_tdata;
(* mark_debug = CW_DEBUG *) wire        auroraCWforwardedStream_tlast;
(* mark_debug = CW_DEBUG *) wire        auroraCWforwardedStream_tvalid;
(* mark_debug = CW_DEBUG *) wire        auroraAcoreStatus_channel_up;
wire                                    auroraAcoreStatus_frame_err;
wire                                    auroraAcoreStatus_hard_err;
wire                                    auroraAcoreStatus_rx_resetdone_out;
wire                                    auroraAcoreStatus_soft_err;
wire                                    auroraAcoreStatus_tx_lock;
wire                                    auroraAcoreStatus_tx_resetdone_out;
(*mark_debug = CCW_DEBUG *) wire        auroraBcoreStatus_channel_up;
wire                                    auroraBcoreStatus_frame_err;
wire                                    auroraBcoreStatus_hard_err;
wire                                    auroraBcoreStatus_rx_resetdone_out;
wire                                    auroraBcoreStatus_soft_err;
wire                                    auroraBcoreStatus_tx_lock;
wire                                    auroraBcoreStatus_tx_resetdone_out;
wire                                    auroraBcoreStatus_pll_not_locked_out;

//////////////////////////////////////////////////////////////////////////////
// Input signal conditioning
parameter LOCAL_DATA_WIDTH = 2 * INTERLOCK_COUNT + 32;
wire [LOCAL_DATA_WIDTH-1:0] auroraLocalData;
wire                        auroraTxToggle;
inputHander #(.DATA_WIDTH(LOCAL_DATA_WIDTH),
              .INTERLOCK_COUNT(INTERLOCK_COUNT),
              .SYSMON_ADDR_WIDTH(SYSMON_ADDR_WIDTH),
              .CLK_PERIOD_PS(AURORACLK_PERIOD_PS),
              .DEBUG("false"))
  inputHandler (
    .clk(auroraUserClk),
    .interlockPins(interlockPins),
    .switches({GPIO_SW_LEFT, GPIO_SW_RIGHT}),
    .sysmonAddr(auroraSysmonAddr),
    .sysmonData(auroraSysmonData),
    .txToggle(auroraTxToggle),
    .localData(auroraLocalData));

// Copy some inputs for possible connection to external logic analyzer.
genvar i;
generate
for (i = 0 ; i < 8 ; i = i + 1) begin : pmod1
  assign PMOD1_DIAG[i] = interlockPins[i];
end /* for */
endgenerate

//////////////////////////////////////////////////////////////////////////////
// Local packet generation
wire [15:0] auroraLocalTDATA;
wire        auroraLocalTLAST, auroraLocalTVALID;

createLocalPacket #(.DATA_WIDTH(LOCAL_DATA_WIDTH),
                    .DEBUG("false"))
  createLocalPacket (
   .clk(auroraUserClk),
   .localData(auroraLocalData),
   .localDataToggle(auroraTxToggle),
   .localTDATA(auroraLocalTDATA),
   .localTLAST(auroraLocalTLAST),
   .localTVALID(auroraLocalTVALID));

//////////////////////////////////////////////////////////////////////////////
// Packet forwarding
forwardPacket #(.DEBUG("false"))
  forwardPacket (
   .clk(auroraUserClk),
   .incomingTDATA(auroraCWincomingStream_tdata),
   .incomingTVALID(auroraCWincomingStream_tvalid),
   .incomingTLAST(auroraCWincomingStream_tlast),
   .outgoingTDATA(auroraCWforwardedStream_tdata),
   .outgoingTVALID(auroraCWforwardedStream_tvalid),
   .outgoingTLAST(auroraCWforwardedStream_tlast));

//////////////////////////////////////////////////////////////////////////////
// Useful reset signals in different clock domains
wire [31:0] linkStatus, linkControl;
wire sysGTreset = linkControl[0];
wire sysAuroraReset = linkControl[1];
reg auroraARESETN_m = 0, auroraARESETN = 0;
always @(posedge auroraUserClk) begin
    auroraARESETN_m <= !sysAuroraReset;
    auroraARESETN <= auroraARESETN_m;
end

//////////////////////////////////////////////////////////////////////////////
// Block design

localparam CWM_DEBUG = "false";
(* mark_debug = CWM_DEBUG *) wire [15:0] auroraCWmergedStream_tdata;
(* mark_debug = CWM_DEBUG *) wire        auroraCWmergedStream_tlast;
(* mark_debug = CWM_DEBUG *) wire        auroraCWmergedStream_tvalid;
(* mark_debug = CWM_DEBUG *) wire        auroraCWmergedStream_tready;

assign linkStatus = {
    auroraBcoreStatus_pll_not_locked_out,
    auroraBcoreStatus_channel_up,
    auroraBcoreStatus_frame_err,
    auroraBcoreStatus_hard_err,
    auroraBcoreStatus_rx_resetdone_out,
    auroraBcoreStatus_soft_err,
    auroraBcoreStatus_tx_lock,
    auroraBcoreStatus_tx_resetdone_out,
    1'b0,
    auroraAcoreStatus_channel_up,
    auroraAcoreStatus_frame_err,
    auroraAcoreStatus_hard_err,
    auroraAcoreStatus_rx_resetdone_out,
    auroraAcoreStatus_soft_err,
    auroraAcoreStatus_tx_lock,
    auroraAcoreStatus_tx_resetdone_out,
    6'b0,
    SFP1_MODABS,
    1'b0,
    6'b0,
    sysAuroraReset,
    sysGTreset };

base_zynq_design base_zynq_design_i (
    .sysClk(sysClk),
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

    .firmwareBuildDate_tri_i(FIRMWARE_BUILD_DATE),

    .GT_REFCLK_clk_n(SI5324_REFCLK_N),
    .GT_REFCLK_clk_p(SI5324_REFCLK_P),

    .LPC_SFP_IIC_scl_i(LPC_SFP_IIC_scl_i),
    .LPC_SFP_IIC_scl_o(LPC_SFP_IIC_scl_o),
    .LPC_SFP_IIC_scl_t(LPC_SFP_IIC_scl_t),
    .LPC_SFP_IIC_sda_i(LPC_SFP_IIC_sda_i),
    .LPC_SFP_IIC_sda_o(LPC_SFP_IIC_sda_o),
    .LPC_SFP_IIC_sda_t(LPC_SFP_IIC_sda_t),
    .CW_Tx_txn(SFP0_TX_N),
    .CW_Tx_txp(SFP0_TX_P),
    .CCW_Rx_rxn(SFP0_RX_N),
    .CCW_Rx_rxp(SFP0_RX_P),
    .CCW_Tx_txn(SFP1_TX_N),
    .CCW_Tx_txp(SFP1_TX_P),
    .CW_Rx_rxn(SFP1_RX_N),
    .CW_Rx_rxp(SFP1_RX_P),

    .gtReset(sysGTreset),
    .auroraReset(sysAuroraReset),
    .auroraARESETN(auroraARESETN),

    .auroraCWincomingStream_tdata(auroraCWincomingStream_tdata),
    .auroraCWincomingStream_tlast(auroraCWincomingStream_tlast),
    .auroraCWincomingStream_tvalid(auroraCWincomingStream_tvalid),
    .auroraCWoutgoingStream_tdata(auroraCWmergedStream_tdata),
    .auroraCWoutgoingStream_tkeep(2'b11),
    .auroraCWoutgoingStream_tlast(auroraCWmergedStream_tlast),
    .auroraCWoutgoingStream_tvalid(auroraCWmergedStream_tvalid),
    .auroraCWoutgoingStream_tready(auroraCWmergedStream_tready),

    // The AXI stream interconnect
    // Can be set up only as a block design component
    .auroraCWforwardedStream_tdata(auroraCWforwardedStream_tdata),
    .auroraCWforwardedStream_tlast(auroraCWforwardedStream_tlast),
    .auroraCWforwardedStream_tvalid(auroraCWforwardedStream_tvalid),
    .auroraCWlocalStream_tdata(auroraLocalTDATA),
    .auroraCWlocalStream_tlast(auroraLocalTLAST),
    .auroraCWlocalStream_tvalid(auroraLocalTVALID),
    .auroraCWmergedStream_tdata(auroraCWmergedStream_tdata),
    .auroraCWmergedStream_tlast(auroraCWmergedStream_tlast),
    .auroraCWmergedStream_tready(auroraCWmergedStream_tready),
    .auroraCWmergedStream_tvalid(auroraCWmergedStream_tvalid),

    // CCW streams unused for now
    // Enhancement possibility -- use for remote access to console
    .auroraCCWincomingStream_tdata(),
    .auroraCCWincomingStream_tlast(),
    .auroraCCWincomingStream_tvalid(),
    .auroraCCWoutgoingStream_tdata(),
    .auroraCCWoutgoingStream_tlast(),
    .auroraCCWoutgoingStream_tvalid(),

    .auroraAcoreControl_loopback(3'b000),
    .auroraAcoreControl_pll_not_locked(auroraBcoreStatus_pll_not_locked_out),
    .auroraAcoreControl_power_down(1'b0),
    .auroraAcoreStatus_channel_up(auroraAcoreStatus_channel_up),
    .auroraAcoreStatus_frame_err(auroraAcoreStatus_frame_err),
    .auroraAcoreStatus_hard_err(auroraAcoreStatus_hard_err),
    .auroraAcoreStatus_rx_resetdone_out(auroraAcoreStatus_rx_resetdone_out),
    .auroraAcoreStatus_soft_err(auroraAcoreStatus_soft_err),
    .auroraAcoreStatus_tx_lock(auroraAcoreStatus_tx_lock),
    .auroraAcoreStatus_tx_resetdone_out(auroraAcoreStatus_tx_resetdone_out),

    .auroraBcoreControl_loopback(3'b000),
    .auroraBcoreControl_power_down(1'b0),
    .auroraBcoreStatus_channel_up(auroraBcoreStatus_channel_up),
    .auroraBcoreStatus_frame_err(auroraBcoreStatus_frame_err),
    .auroraBcoreStatus_hard_err(auroraBcoreStatus_hard_err),
    .auroraBcoreStatus_rx_resetdone_out(auroraBcoreStatus_rx_resetdone_out),
    .auroraBcoreStatus_soft_err(auroraBcoreStatus_soft_err),
    .auroraBcoreStatus_tx_lock(auroraBcoreStatus_tx_lock),
    .auroraBcoreStatus_tx_resetdone_out(auroraBcoreStatus_tx_resetdone_out),
    .auroraBcoreStatus_pll_not_locked_out(auroraBcoreStatus_pll_not_locked_out),

    .linkGPIO_tri_i(linkStatus),
    .linkGPIO_tri_o(linkControl),

    .fanSpeed_tri_i({16'h0000, fanRPM}),
    .SYSMON_BRAM_addr(SYSMON_BRAM_addr),
    .SYSMON_BRAM_din(SYSMON_BRAM_din),
    .SYSMON_BRAM_dout(SYSMON_BRAM_dout),
    .SYSMON_BRAM_en(SYSMON_BRAM_en),
    .SYSMON_BRAM_rst(SYSMON_BRAM_rst),
    .SYSMON_BRAM_we(SYSMON_BRAM_we)
    );

endmodule
