//Copyright 1986-2016 Xilinx, Inc. All Rights Reserved.
//--------------------------------------------------------------------------------
//Tool Version: Vivado v.2016.1 (lin64) Build 1538259 Fri Apr  8 15:45:23 MDT 2016
//Date        : Mon Oct 31 08:38:58 2016
//Host        : localhost.localdomain running 64-bit CentOS Linux release 7.2.1511 (Core)
//Command     : generate_target base_zynq_design_wrapper.bd
//Design      : base_zynq_design_wrapper
//Purpose     : IP block netlist
//--------------------------------------------------------------------------------
`timescale 1 ps / 1 ps

module base_zynq_design_wrapper
   (CCW_Rx_rxn,
    CCW_Rx_rxp,
    CCW_Tx_txn,
    CCW_Tx_txp,
    CW_Rx_rxn,
    CW_Rx_rxp,
    CW_Tx_txn,
    CW_Tx_txp,
    DDR_addr,
    DDR_ba,
    DDR_cas_n,
    DDR_ck_n,
    DDR_ck_p,
    DDR_cke,
    DDR_cs_n,
    DDR_dm,
    DDR_dq,
    DDR_dqs_n,
    DDR_dqs_p,
    DDR_odt,
    DDR_ras_n,
    DDR_reset_n,
    DDR_we_n,
    FIXED_IO_ddr_vrn,
    FIXED_IO_ddr_vrp,
    FIXED_IO_mio,
    FIXED_IO_ps_clk,
    FIXED_IO_ps_porb,
    FIXED_IO_ps_srstb,
    GT_REFCLK_clk_n,
    GT_REFCLK_clk_p,
    SYSMON_BRAM_addr,
    SYSMON_BRAM_clk,
    SYSMON_BRAM_din,
    SYSMON_BRAM_dout,
    SYSMON_BRAM_en,
    SYSMON_BRAM_rst,
    SYSMON_BRAM_we,
    auroraARESETN,
    auroraAcoreControl_loopback,
    auroraAcoreControl_pll_not_locked,
    auroraAcoreControl_power_down,
    auroraAcoreStatus_channel_up,
    auroraAcoreStatus_frame_err,
    auroraAcoreStatus_hard_err,
    auroraAcoreStatus_lane_up,
    auroraAcoreStatus_rx_resetdone_out,
    auroraAcoreStatus_soft_err,
    auroraAcoreStatus_tx_lock,
    auroraAcoreStatus_tx_resetdone_out,
    auroraBcoreControl_loopback,
    auroraBcoreControl_power_down,
    auroraBcoreStatus_channel_up,
    auroraBcoreStatus_frame_err,
    auroraBcoreStatus_hard_err,
    auroraBcoreStatus_lane_up,
    auroraBcoreStatus_pll_not_locked_out,
    auroraBcoreStatus_rx_resetdone_out,
    auroraBcoreStatus_soft_err,
    auroraBcoreStatus_tx_lock,
    auroraBcoreStatus_tx_resetdone_out,
    auroraCCWincomingStream_tdata,
    auroraCCWincomingStream_tkeep,
    auroraCCWincomingStream_tlast,
    auroraCCWincomingStream_tvalid,
    auroraCCWoutgoingStream_tdata,
    auroraCCWoutgoingStream_tkeep,
    auroraCCWoutgoingStream_tlast,
    auroraCCWoutgoingStream_tready,
    auroraCCWoutgoingStream_tvalid,
    auroraCWforwardedStream_tdata,
    auroraCWforwardedStream_tlast,
    auroraCWforwardedStream_tvalid,
    auroraCWincomingStream_tdata,
    auroraCWincomingStream_tkeep,
    auroraCWincomingStream_tlast,
    auroraCWincomingStream_tvalid,
    auroraCWlocalStream_tdata,
    auroraCWlocalStream_tlast,
    auroraCWlocalStream_tvalid,
    auroraCWmergedStream_tdata,
    auroraCWmergedStream_tlast,
    auroraCWmergedStream_tready,
    auroraCWmergedStream_tvalid,
    auroraCWoutgoingStream_tdata,
    auroraCWoutgoingStream_tkeep,
    auroraCWoutgoingStream_tlast,
    auroraCWoutgoingStream_tready,
    auroraCWoutgoingStream_tvalid,
    auroraReset,
    auroraUserClk,
    fanspeed_tri_i,
    firmwarebuilddate_tri_i,
    gtReset,
    linkgpio_tri_io,
    lpc_sfp_iic_scl_io,
    lpc_sfp_iic_sda_io,
    sfp_gpio_tri_io,
    sysClk);
  input [0:0]CCW_Rx_rxn;
  input [0:0]CCW_Rx_rxp;
  output [0:0]CCW_Tx_txn;
  output [0:0]CCW_Tx_txp;
  input [0:0]CW_Rx_rxn;
  input [0:0]CW_Rx_rxp;
  output [0:0]CW_Tx_txn;
  output [0:0]CW_Tx_txp;
  inout [14:0]DDR_addr;
  inout [2:0]DDR_ba;
  inout DDR_cas_n;
  inout DDR_ck_n;
  inout DDR_ck_p;
  inout DDR_cke;
  inout DDR_cs_n;
  inout [3:0]DDR_dm;
  inout [31:0]DDR_dq;
  inout [3:0]DDR_dqs_n;
  inout [3:0]DDR_dqs_p;
  inout DDR_odt;
  inout DDR_ras_n;
  inout DDR_reset_n;
  inout DDR_we_n;
  inout FIXED_IO_ddr_vrn;
  inout FIXED_IO_ddr_vrp;
  inout [53:0]FIXED_IO_mio;
  inout FIXED_IO_ps_clk;
  inout FIXED_IO_ps_porb;
  inout FIXED_IO_ps_srstb;
  input GT_REFCLK_clk_n;
  input GT_REFCLK_clk_p;
  output [12:0]SYSMON_BRAM_addr;
  output SYSMON_BRAM_clk;
  output [31:0]SYSMON_BRAM_din;
  input [31:0]SYSMON_BRAM_dout;
  output SYSMON_BRAM_en;
  output SYSMON_BRAM_rst;
  output [3:0]SYSMON_BRAM_we;
  input auroraARESETN;
  input [2:0]auroraAcoreControl_loopback;
  input auroraAcoreControl_pll_not_locked;
  input auroraAcoreControl_power_down;
  output auroraAcoreStatus_channel_up;
  output auroraAcoreStatus_frame_err;
  output auroraAcoreStatus_hard_err;
  output [0:0]auroraAcoreStatus_lane_up;
  output auroraAcoreStatus_rx_resetdone_out;
  output auroraAcoreStatus_soft_err;
  output auroraAcoreStatus_tx_lock;
  output auroraAcoreStatus_tx_resetdone_out;
  input [2:0]auroraBcoreControl_loopback;
  input auroraBcoreControl_power_down;
  output auroraBcoreStatus_channel_up;
  output auroraBcoreStatus_frame_err;
  output auroraBcoreStatus_hard_err;
  output [0:0]auroraBcoreStatus_lane_up;
  output auroraBcoreStatus_pll_not_locked_out;
  output auroraBcoreStatus_rx_resetdone_out;
  output auroraBcoreStatus_soft_err;
  output auroraBcoreStatus_tx_lock;
  output auroraBcoreStatus_tx_resetdone_out;
  output [0:15]auroraCCWincomingStream_tdata;
  output [0:1]auroraCCWincomingStream_tkeep;
  output auroraCCWincomingStream_tlast;
  output auroraCCWincomingStream_tvalid;
  input [0:15]auroraCCWoutgoingStream_tdata;
  input [0:1]auroraCCWoutgoingStream_tkeep;
  input auroraCCWoutgoingStream_tlast;
  output auroraCCWoutgoingStream_tready;
  input auroraCCWoutgoingStream_tvalid;
  input [15:0]auroraCWforwardedStream_tdata;
  input auroraCWforwardedStream_tlast;
  input auroraCWforwardedStream_tvalid;
  output [0:15]auroraCWincomingStream_tdata;
  output [0:1]auroraCWincomingStream_tkeep;
  output auroraCWincomingStream_tlast;
  output auroraCWincomingStream_tvalid;
  input [15:0]auroraCWlocalStream_tdata;
  input auroraCWlocalStream_tlast;
  input auroraCWlocalStream_tvalid;
  output [15:0]auroraCWmergedStream_tdata;
  output [0:0]auroraCWmergedStream_tlast;
  input [0:0]auroraCWmergedStream_tready;
  output [0:0]auroraCWmergedStream_tvalid;
  input [0:15]auroraCWoutgoingStream_tdata;
  input [0:1]auroraCWoutgoingStream_tkeep;
  input auroraCWoutgoingStream_tlast;
  output auroraCWoutgoingStream_tready;
  input auroraCWoutgoingStream_tvalid;
  input auroraReset;
  output auroraUserClk;
  input [31:0]fanspeed_tri_i;
  input [31:0]firmwarebuilddate_tri_i;
  input gtReset;
  inout [31:0]linkgpio_tri_io;
  inout lpc_sfp_iic_scl_io;
  inout lpc_sfp_iic_sda_io;
  inout [31:0]sfp_gpio_tri_io;
  output sysClk;

  wire [0:0]CCW_Rx_rxn;
  wire [0:0]CCW_Rx_rxp;
  wire [0:0]CCW_Tx_txn;
  wire [0:0]CCW_Tx_txp;
  wire [0:0]CW_Rx_rxn;
  wire [0:0]CW_Rx_rxp;
  wire [0:0]CW_Tx_txn;
  wire [0:0]CW_Tx_txp;
  wire [14:0]DDR_addr;
  wire [2:0]DDR_ba;
  wire DDR_cas_n;
  wire DDR_ck_n;
  wire DDR_ck_p;
  wire DDR_cke;
  wire DDR_cs_n;
  wire [3:0]DDR_dm;
  wire [31:0]DDR_dq;
  wire [3:0]DDR_dqs_n;
  wire [3:0]DDR_dqs_p;
  wire DDR_odt;
  wire DDR_ras_n;
  wire DDR_reset_n;
  wire DDR_we_n;
  wire FIXED_IO_ddr_vrn;
  wire FIXED_IO_ddr_vrp;
  wire [53:0]FIXED_IO_mio;
  wire FIXED_IO_ps_clk;
  wire FIXED_IO_ps_porb;
  wire FIXED_IO_ps_srstb;
  wire GT_REFCLK_clk_n;
  wire GT_REFCLK_clk_p;
  wire [12:0]SYSMON_BRAM_addr;
  wire SYSMON_BRAM_clk;
  wire [31:0]SYSMON_BRAM_din;
  wire [31:0]SYSMON_BRAM_dout;
  wire SYSMON_BRAM_en;
  wire SYSMON_BRAM_rst;
  wire [3:0]SYSMON_BRAM_we;
  wire auroraARESETN;
  wire [2:0]auroraAcoreControl_loopback;
  wire auroraAcoreControl_pll_not_locked;
  wire auroraAcoreControl_power_down;
  wire auroraAcoreStatus_channel_up;
  wire auroraAcoreStatus_frame_err;
  wire auroraAcoreStatus_hard_err;
  wire [0:0]auroraAcoreStatus_lane_up;
  wire auroraAcoreStatus_rx_resetdone_out;
  wire auroraAcoreStatus_soft_err;
  wire auroraAcoreStatus_tx_lock;
  wire auroraAcoreStatus_tx_resetdone_out;
  wire [2:0]auroraBcoreControl_loopback;
  wire auroraBcoreControl_power_down;
  wire auroraBcoreStatus_channel_up;
  wire auroraBcoreStatus_frame_err;
  wire auroraBcoreStatus_hard_err;
  wire [0:0]auroraBcoreStatus_lane_up;
  wire auroraBcoreStatus_pll_not_locked_out;
  wire auroraBcoreStatus_rx_resetdone_out;
  wire auroraBcoreStatus_soft_err;
  wire auroraBcoreStatus_tx_lock;
  wire auroraBcoreStatus_tx_resetdone_out;
  wire [0:15]auroraCCWincomingStream_tdata;
  wire [0:1]auroraCCWincomingStream_tkeep;
  wire auroraCCWincomingStream_tlast;
  wire auroraCCWincomingStream_tvalid;
  wire [0:15]auroraCCWoutgoingStream_tdata;
  wire [0:1]auroraCCWoutgoingStream_tkeep;
  wire auroraCCWoutgoingStream_tlast;
  wire auroraCCWoutgoingStream_tready;
  wire auroraCCWoutgoingStream_tvalid;
  wire [15:0]auroraCWforwardedStream_tdata;
  wire auroraCWforwardedStream_tlast;
  wire auroraCWforwardedStream_tvalid;
  wire [0:15]auroraCWincomingStream_tdata;
  wire [0:1]auroraCWincomingStream_tkeep;
  wire auroraCWincomingStream_tlast;
  wire auroraCWincomingStream_tvalid;
  wire [15:0]auroraCWlocalStream_tdata;
  wire auroraCWlocalStream_tlast;
  wire auroraCWlocalStream_tvalid;
  wire [15:0]auroraCWmergedStream_tdata;
  wire [0:0]auroraCWmergedStream_tlast;
  wire [0:0]auroraCWmergedStream_tready;
  wire [0:0]auroraCWmergedStream_tvalid;
  wire [0:15]auroraCWoutgoingStream_tdata;
  wire [0:1]auroraCWoutgoingStream_tkeep;
  wire auroraCWoutgoingStream_tlast;
  wire auroraCWoutgoingStream_tready;
  wire auroraCWoutgoingStream_tvalid;
  wire auroraReset;
  wire auroraUserClk;
  wire [31:0]fanspeed_tri_i;
  wire [31:0]firmwarebuilddate_tri_i;
  wire gtReset;
  wire [0:0]linkgpio_tri_i_0;
  wire [1:1]linkgpio_tri_i_1;
  wire [10:10]linkgpio_tri_i_10;
  wire [11:11]linkgpio_tri_i_11;
  wire [12:12]linkgpio_tri_i_12;
  wire [13:13]linkgpio_tri_i_13;
  wire [14:14]linkgpio_tri_i_14;
  wire [15:15]linkgpio_tri_i_15;
  wire [16:16]linkgpio_tri_i_16;
  wire [17:17]linkgpio_tri_i_17;
  wire [18:18]linkgpio_tri_i_18;
  wire [19:19]linkgpio_tri_i_19;
  wire [2:2]linkgpio_tri_i_2;
  wire [20:20]linkgpio_tri_i_20;
  wire [21:21]linkgpio_tri_i_21;
  wire [22:22]linkgpio_tri_i_22;
  wire [23:23]linkgpio_tri_i_23;
  wire [24:24]linkgpio_tri_i_24;
  wire [25:25]linkgpio_tri_i_25;
  wire [26:26]linkgpio_tri_i_26;
  wire [27:27]linkgpio_tri_i_27;
  wire [28:28]linkgpio_tri_i_28;
  wire [29:29]linkgpio_tri_i_29;
  wire [3:3]linkgpio_tri_i_3;
  wire [30:30]linkgpio_tri_i_30;
  wire [31:31]linkgpio_tri_i_31;
  wire [4:4]linkgpio_tri_i_4;
  wire [5:5]linkgpio_tri_i_5;
  wire [6:6]linkgpio_tri_i_6;
  wire [7:7]linkgpio_tri_i_7;
  wire [8:8]linkgpio_tri_i_8;
  wire [9:9]linkgpio_tri_i_9;
  wire [0:0]linkgpio_tri_io_0;
  wire [1:1]linkgpio_tri_io_1;
  wire [10:10]linkgpio_tri_io_10;
  wire [11:11]linkgpio_tri_io_11;
  wire [12:12]linkgpio_tri_io_12;
  wire [13:13]linkgpio_tri_io_13;
  wire [14:14]linkgpio_tri_io_14;
  wire [15:15]linkgpio_tri_io_15;
  wire [16:16]linkgpio_tri_io_16;
  wire [17:17]linkgpio_tri_io_17;
  wire [18:18]linkgpio_tri_io_18;
  wire [19:19]linkgpio_tri_io_19;
  wire [2:2]linkgpio_tri_io_2;
  wire [20:20]linkgpio_tri_io_20;
  wire [21:21]linkgpio_tri_io_21;
  wire [22:22]linkgpio_tri_io_22;
  wire [23:23]linkgpio_tri_io_23;
  wire [24:24]linkgpio_tri_io_24;
  wire [25:25]linkgpio_tri_io_25;
  wire [26:26]linkgpio_tri_io_26;
  wire [27:27]linkgpio_tri_io_27;
  wire [28:28]linkgpio_tri_io_28;
  wire [29:29]linkgpio_tri_io_29;
  wire [3:3]linkgpio_tri_io_3;
  wire [30:30]linkgpio_tri_io_30;
  wire [31:31]linkgpio_tri_io_31;
  wire [4:4]linkgpio_tri_io_4;
  wire [5:5]linkgpio_tri_io_5;
  wire [6:6]linkgpio_tri_io_6;
  wire [7:7]linkgpio_tri_io_7;
  wire [8:8]linkgpio_tri_io_8;
  wire [9:9]linkgpio_tri_io_9;
  wire [0:0]linkgpio_tri_o_0;
  wire [1:1]linkgpio_tri_o_1;
  wire [10:10]linkgpio_tri_o_10;
  wire [11:11]linkgpio_tri_o_11;
  wire [12:12]linkgpio_tri_o_12;
  wire [13:13]linkgpio_tri_o_13;
  wire [14:14]linkgpio_tri_o_14;
  wire [15:15]linkgpio_tri_o_15;
  wire [16:16]linkgpio_tri_o_16;
  wire [17:17]linkgpio_tri_o_17;
  wire [18:18]linkgpio_tri_o_18;
  wire [19:19]linkgpio_tri_o_19;
  wire [2:2]linkgpio_tri_o_2;
  wire [20:20]linkgpio_tri_o_20;
  wire [21:21]linkgpio_tri_o_21;
  wire [22:22]linkgpio_tri_o_22;
  wire [23:23]linkgpio_tri_o_23;
  wire [24:24]linkgpio_tri_o_24;
  wire [25:25]linkgpio_tri_o_25;
  wire [26:26]linkgpio_tri_o_26;
  wire [27:27]linkgpio_tri_o_27;
  wire [28:28]linkgpio_tri_o_28;
  wire [29:29]linkgpio_tri_o_29;
  wire [3:3]linkgpio_tri_o_3;
  wire [30:30]linkgpio_tri_o_30;
  wire [31:31]linkgpio_tri_o_31;
  wire [4:4]linkgpio_tri_o_4;
  wire [5:5]linkgpio_tri_o_5;
  wire [6:6]linkgpio_tri_o_6;
  wire [7:7]linkgpio_tri_o_7;
  wire [8:8]linkgpio_tri_o_8;
  wire [9:9]linkgpio_tri_o_9;
  wire [0:0]linkgpio_tri_t_0;
  wire [1:1]linkgpio_tri_t_1;
  wire [10:10]linkgpio_tri_t_10;
  wire [11:11]linkgpio_tri_t_11;
  wire [12:12]linkgpio_tri_t_12;
  wire [13:13]linkgpio_tri_t_13;
  wire [14:14]linkgpio_tri_t_14;
  wire [15:15]linkgpio_tri_t_15;
  wire [16:16]linkgpio_tri_t_16;
  wire [17:17]linkgpio_tri_t_17;
  wire [18:18]linkgpio_tri_t_18;
  wire [19:19]linkgpio_tri_t_19;
  wire [2:2]linkgpio_tri_t_2;
  wire [20:20]linkgpio_tri_t_20;
  wire [21:21]linkgpio_tri_t_21;
  wire [22:22]linkgpio_tri_t_22;
  wire [23:23]linkgpio_tri_t_23;
  wire [24:24]linkgpio_tri_t_24;
  wire [25:25]linkgpio_tri_t_25;
  wire [26:26]linkgpio_tri_t_26;
  wire [27:27]linkgpio_tri_t_27;
  wire [28:28]linkgpio_tri_t_28;
  wire [29:29]linkgpio_tri_t_29;
  wire [3:3]linkgpio_tri_t_3;
  wire [30:30]linkgpio_tri_t_30;
  wire [31:31]linkgpio_tri_t_31;
  wire [4:4]linkgpio_tri_t_4;
  wire [5:5]linkgpio_tri_t_5;
  wire [6:6]linkgpio_tri_t_6;
  wire [7:7]linkgpio_tri_t_7;
  wire [8:8]linkgpio_tri_t_8;
  wire [9:9]linkgpio_tri_t_9;
  wire lpc_sfp_iic_scl_i;
  wire lpc_sfp_iic_scl_io;
  wire lpc_sfp_iic_scl_o;
  wire lpc_sfp_iic_scl_t;
  wire lpc_sfp_iic_sda_i;
  wire lpc_sfp_iic_sda_io;
  wire lpc_sfp_iic_sda_o;
  wire lpc_sfp_iic_sda_t;
  wire [0:0]sfp_gpio_tri_i_0;
  wire [1:1]sfp_gpio_tri_i_1;
  wire [10:10]sfp_gpio_tri_i_10;
  wire [11:11]sfp_gpio_tri_i_11;
  wire [12:12]sfp_gpio_tri_i_12;
  wire [13:13]sfp_gpio_tri_i_13;
  wire [14:14]sfp_gpio_tri_i_14;
  wire [15:15]sfp_gpio_tri_i_15;
  wire [16:16]sfp_gpio_tri_i_16;
  wire [17:17]sfp_gpio_tri_i_17;
  wire [18:18]sfp_gpio_tri_i_18;
  wire [19:19]sfp_gpio_tri_i_19;
  wire [2:2]sfp_gpio_tri_i_2;
  wire [20:20]sfp_gpio_tri_i_20;
  wire [21:21]sfp_gpio_tri_i_21;
  wire [22:22]sfp_gpio_tri_i_22;
  wire [23:23]sfp_gpio_tri_i_23;
  wire [24:24]sfp_gpio_tri_i_24;
  wire [25:25]sfp_gpio_tri_i_25;
  wire [26:26]sfp_gpio_tri_i_26;
  wire [27:27]sfp_gpio_tri_i_27;
  wire [28:28]sfp_gpio_tri_i_28;
  wire [29:29]sfp_gpio_tri_i_29;
  wire [3:3]sfp_gpio_tri_i_3;
  wire [30:30]sfp_gpio_tri_i_30;
  wire [31:31]sfp_gpio_tri_i_31;
  wire [4:4]sfp_gpio_tri_i_4;
  wire [5:5]sfp_gpio_tri_i_5;
  wire [6:6]sfp_gpio_tri_i_6;
  wire [7:7]sfp_gpio_tri_i_7;
  wire [8:8]sfp_gpio_tri_i_8;
  wire [9:9]sfp_gpio_tri_i_9;
  wire [0:0]sfp_gpio_tri_io_0;
  wire [1:1]sfp_gpio_tri_io_1;
  wire [10:10]sfp_gpio_tri_io_10;
  wire [11:11]sfp_gpio_tri_io_11;
  wire [12:12]sfp_gpio_tri_io_12;
  wire [13:13]sfp_gpio_tri_io_13;
  wire [14:14]sfp_gpio_tri_io_14;
  wire [15:15]sfp_gpio_tri_io_15;
  wire [16:16]sfp_gpio_tri_io_16;
  wire [17:17]sfp_gpio_tri_io_17;
  wire [18:18]sfp_gpio_tri_io_18;
  wire [19:19]sfp_gpio_tri_io_19;
  wire [2:2]sfp_gpio_tri_io_2;
  wire [20:20]sfp_gpio_tri_io_20;
  wire [21:21]sfp_gpio_tri_io_21;
  wire [22:22]sfp_gpio_tri_io_22;
  wire [23:23]sfp_gpio_tri_io_23;
  wire [24:24]sfp_gpio_tri_io_24;
  wire [25:25]sfp_gpio_tri_io_25;
  wire [26:26]sfp_gpio_tri_io_26;
  wire [27:27]sfp_gpio_tri_io_27;
  wire [28:28]sfp_gpio_tri_io_28;
  wire [29:29]sfp_gpio_tri_io_29;
  wire [3:3]sfp_gpio_tri_io_3;
  wire [30:30]sfp_gpio_tri_io_30;
  wire [31:31]sfp_gpio_tri_io_31;
  wire [4:4]sfp_gpio_tri_io_4;
  wire [5:5]sfp_gpio_tri_io_5;
  wire [6:6]sfp_gpio_tri_io_6;
  wire [7:7]sfp_gpio_tri_io_7;
  wire [8:8]sfp_gpio_tri_io_8;
  wire [9:9]sfp_gpio_tri_io_9;
  wire [0:0]sfp_gpio_tri_o_0;
  wire [1:1]sfp_gpio_tri_o_1;
  wire [10:10]sfp_gpio_tri_o_10;
  wire [11:11]sfp_gpio_tri_o_11;
  wire [12:12]sfp_gpio_tri_o_12;
  wire [13:13]sfp_gpio_tri_o_13;
  wire [14:14]sfp_gpio_tri_o_14;
  wire [15:15]sfp_gpio_tri_o_15;
  wire [16:16]sfp_gpio_tri_o_16;
  wire [17:17]sfp_gpio_tri_o_17;
  wire [18:18]sfp_gpio_tri_o_18;
  wire [19:19]sfp_gpio_tri_o_19;
  wire [2:2]sfp_gpio_tri_o_2;
  wire [20:20]sfp_gpio_tri_o_20;
  wire [21:21]sfp_gpio_tri_o_21;
  wire [22:22]sfp_gpio_tri_o_22;
  wire [23:23]sfp_gpio_tri_o_23;
  wire [24:24]sfp_gpio_tri_o_24;
  wire [25:25]sfp_gpio_tri_o_25;
  wire [26:26]sfp_gpio_tri_o_26;
  wire [27:27]sfp_gpio_tri_o_27;
  wire [28:28]sfp_gpio_tri_o_28;
  wire [29:29]sfp_gpio_tri_o_29;
  wire [3:3]sfp_gpio_tri_o_3;
  wire [30:30]sfp_gpio_tri_o_30;
  wire [31:31]sfp_gpio_tri_o_31;
  wire [4:4]sfp_gpio_tri_o_4;
  wire [5:5]sfp_gpio_tri_o_5;
  wire [6:6]sfp_gpio_tri_o_6;
  wire [7:7]sfp_gpio_tri_o_7;
  wire [8:8]sfp_gpio_tri_o_8;
  wire [9:9]sfp_gpio_tri_o_9;
  wire [0:0]sfp_gpio_tri_t_0;
  wire [1:1]sfp_gpio_tri_t_1;
  wire [10:10]sfp_gpio_tri_t_10;
  wire [11:11]sfp_gpio_tri_t_11;
  wire [12:12]sfp_gpio_tri_t_12;
  wire [13:13]sfp_gpio_tri_t_13;
  wire [14:14]sfp_gpio_tri_t_14;
  wire [15:15]sfp_gpio_tri_t_15;
  wire [16:16]sfp_gpio_tri_t_16;
  wire [17:17]sfp_gpio_tri_t_17;
  wire [18:18]sfp_gpio_tri_t_18;
  wire [19:19]sfp_gpio_tri_t_19;
  wire [2:2]sfp_gpio_tri_t_2;
  wire [20:20]sfp_gpio_tri_t_20;
  wire [21:21]sfp_gpio_tri_t_21;
  wire [22:22]sfp_gpio_tri_t_22;
  wire [23:23]sfp_gpio_tri_t_23;
  wire [24:24]sfp_gpio_tri_t_24;
  wire [25:25]sfp_gpio_tri_t_25;
  wire [26:26]sfp_gpio_tri_t_26;
  wire [27:27]sfp_gpio_tri_t_27;
  wire [28:28]sfp_gpio_tri_t_28;
  wire [29:29]sfp_gpio_tri_t_29;
  wire [3:3]sfp_gpio_tri_t_3;
  wire [30:30]sfp_gpio_tri_t_30;
  wire [31:31]sfp_gpio_tri_t_31;
  wire [4:4]sfp_gpio_tri_t_4;
  wire [5:5]sfp_gpio_tri_t_5;
  wire [6:6]sfp_gpio_tri_t_6;
  wire [7:7]sfp_gpio_tri_t_7;
  wire [8:8]sfp_gpio_tri_t_8;
  wire [9:9]sfp_gpio_tri_t_9;
  wire sysClk;

  base_zynq_design base_zynq_design_i
       (.CCW_Rx_rxn(CCW_Rx_rxn),
        .CCW_Rx_rxp(CCW_Rx_rxp),
        .CCW_Tx_txn(CCW_Tx_txn),
        .CCW_Tx_txp(CCW_Tx_txp),
        .CW_Rx_rxn(CW_Rx_rxn),
        .CW_Rx_rxp(CW_Rx_rxp),
        .CW_Tx_txn(CW_Tx_txn),
        .CW_Tx_txp(CW_Tx_txp),
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
        .GT_REFCLK_clk_n(GT_REFCLK_clk_n),
        .GT_REFCLK_clk_p(GT_REFCLK_clk_p),
        .LPC_SFP_IIC_scl_i(lpc_sfp_iic_scl_i),
        .LPC_SFP_IIC_scl_o(lpc_sfp_iic_scl_o),
        .LPC_SFP_IIC_scl_t(lpc_sfp_iic_scl_t),
        .LPC_SFP_IIC_sda_i(lpc_sfp_iic_sda_i),
        .LPC_SFP_IIC_sda_o(lpc_sfp_iic_sda_o),
        .LPC_SFP_IIC_sda_t(lpc_sfp_iic_sda_t),
        .SFP_GPIO_tri_i({sfp_gpio_tri_i_31,sfp_gpio_tri_i_30,sfp_gpio_tri_i_29,sfp_gpio_tri_i_28,sfp_gpio_tri_i_27,sfp_gpio_tri_i_26,sfp_gpio_tri_i_25,sfp_gpio_tri_i_24,sfp_gpio_tri_i_23,sfp_gpio_tri_i_22,sfp_gpio_tri_i_21,sfp_gpio_tri_i_20,sfp_gpio_tri_i_19,sfp_gpio_tri_i_18,sfp_gpio_tri_i_17,sfp_gpio_tri_i_16,sfp_gpio_tri_i_15,sfp_gpio_tri_i_14,sfp_gpio_tri_i_13,sfp_gpio_tri_i_12,sfp_gpio_tri_i_11,sfp_gpio_tri_i_10,sfp_gpio_tri_i_9,sfp_gpio_tri_i_8,sfp_gpio_tri_i_7,sfp_gpio_tri_i_6,sfp_gpio_tri_i_5,sfp_gpio_tri_i_4,sfp_gpio_tri_i_3,sfp_gpio_tri_i_2,sfp_gpio_tri_i_1,sfp_gpio_tri_i_0}),
        .SFP_GPIO_tri_o({sfp_gpio_tri_o_31,sfp_gpio_tri_o_30,sfp_gpio_tri_o_29,sfp_gpio_tri_o_28,sfp_gpio_tri_o_27,sfp_gpio_tri_o_26,sfp_gpio_tri_o_25,sfp_gpio_tri_o_24,sfp_gpio_tri_o_23,sfp_gpio_tri_o_22,sfp_gpio_tri_o_21,sfp_gpio_tri_o_20,sfp_gpio_tri_o_19,sfp_gpio_tri_o_18,sfp_gpio_tri_o_17,sfp_gpio_tri_o_16,sfp_gpio_tri_o_15,sfp_gpio_tri_o_14,sfp_gpio_tri_o_13,sfp_gpio_tri_o_12,sfp_gpio_tri_o_11,sfp_gpio_tri_o_10,sfp_gpio_tri_o_9,sfp_gpio_tri_o_8,sfp_gpio_tri_o_7,sfp_gpio_tri_o_6,sfp_gpio_tri_o_5,sfp_gpio_tri_o_4,sfp_gpio_tri_o_3,sfp_gpio_tri_o_2,sfp_gpio_tri_o_1,sfp_gpio_tri_o_0}),
        .SFP_GPIO_tri_t({sfp_gpio_tri_t_31,sfp_gpio_tri_t_30,sfp_gpio_tri_t_29,sfp_gpio_tri_t_28,sfp_gpio_tri_t_27,sfp_gpio_tri_t_26,sfp_gpio_tri_t_25,sfp_gpio_tri_t_24,sfp_gpio_tri_t_23,sfp_gpio_tri_t_22,sfp_gpio_tri_t_21,sfp_gpio_tri_t_20,sfp_gpio_tri_t_19,sfp_gpio_tri_t_18,sfp_gpio_tri_t_17,sfp_gpio_tri_t_16,sfp_gpio_tri_t_15,sfp_gpio_tri_t_14,sfp_gpio_tri_t_13,sfp_gpio_tri_t_12,sfp_gpio_tri_t_11,sfp_gpio_tri_t_10,sfp_gpio_tri_t_9,sfp_gpio_tri_t_8,sfp_gpio_tri_t_7,sfp_gpio_tri_t_6,sfp_gpio_tri_t_5,sfp_gpio_tri_t_4,sfp_gpio_tri_t_3,sfp_gpio_tri_t_2,sfp_gpio_tri_t_1,sfp_gpio_tri_t_0}),
        .SYSMON_BRAM_addr(SYSMON_BRAM_addr),
        .SYSMON_BRAM_clk(SYSMON_BRAM_clk),
        .SYSMON_BRAM_din(SYSMON_BRAM_din),
        .SYSMON_BRAM_dout(SYSMON_BRAM_dout),
        .SYSMON_BRAM_en(SYSMON_BRAM_en),
        .SYSMON_BRAM_rst(SYSMON_BRAM_rst),
        .SYSMON_BRAM_we(SYSMON_BRAM_we),
        .auroraARESETN(auroraARESETN),
        .auroraAcoreControl_loopback(auroraAcoreControl_loopback),
        .auroraAcoreControl_pll_not_locked(auroraAcoreControl_pll_not_locked),
        .auroraAcoreControl_power_down(auroraAcoreControl_power_down),
        .auroraAcoreStatus_channel_up(auroraAcoreStatus_channel_up),
        .auroraAcoreStatus_frame_err(auroraAcoreStatus_frame_err),
        .auroraAcoreStatus_hard_err(auroraAcoreStatus_hard_err),
        .auroraAcoreStatus_lane_up(auroraAcoreStatus_lane_up),
        .auroraAcoreStatus_rx_resetdone_out(auroraAcoreStatus_rx_resetdone_out),
        .auroraAcoreStatus_soft_err(auroraAcoreStatus_soft_err),
        .auroraAcoreStatus_tx_lock(auroraAcoreStatus_tx_lock),
        .auroraAcoreStatus_tx_resetdone_out(auroraAcoreStatus_tx_resetdone_out),
        .auroraBcoreControl_loopback(auroraBcoreControl_loopback),
        .auroraBcoreControl_power_down(auroraBcoreControl_power_down),
        .auroraBcoreStatus_channel_up(auroraBcoreStatus_channel_up),
        .auroraBcoreStatus_frame_err(auroraBcoreStatus_frame_err),
        .auroraBcoreStatus_hard_err(auroraBcoreStatus_hard_err),
        .auroraBcoreStatus_lane_up(auroraBcoreStatus_lane_up),
        .auroraBcoreStatus_pll_not_locked_out(auroraBcoreStatus_pll_not_locked_out),
        .auroraBcoreStatus_rx_resetdone_out(auroraBcoreStatus_rx_resetdone_out),
        .auroraBcoreStatus_soft_err(auroraBcoreStatus_soft_err),
        .auroraBcoreStatus_tx_lock(auroraBcoreStatus_tx_lock),
        .auroraBcoreStatus_tx_resetdone_out(auroraBcoreStatus_tx_resetdone_out),
        .auroraCCWincomingStream_tdata(auroraCCWincomingStream_tdata),
        .auroraCCWincomingStream_tkeep(auroraCCWincomingStream_tkeep),
        .auroraCCWincomingStream_tlast(auroraCCWincomingStream_tlast),
        .auroraCCWincomingStream_tvalid(auroraCCWincomingStream_tvalid),
        .auroraCCWoutgoingStream_tdata(auroraCCWoutgoingStream_tdata),
        .auroraCCWoutgoingStream_tkeep(auroraCCWoutgoingStream_tkeep),
        .auroraCCWoutgoingStream_tlast(auroraCCWoutgoingStream_tlast),
        .auroraCCWoutgoingStream_tready(auroraCCWoutgoingStream_tready),
        .auroraCCWoutgoingStream_tvalid(auroraCCWoutgoingStream_tvalid),
        .auroraCWforwardedStream_tdata(auroraCWforwardedStream_tdata),
        .auroraCWforwardedStream_tlast(auroraCWforwardedStream_tlast),
        .auroraCWforwardedStream_tvalid(auroraCWforwardedStream_tvalid),
        .auroraCWincomingStream_tdata(auroraCWincomingStream_tdata),
        .auroraCWincomingStream_tkeep(auroraCWincomingStream_tkeep),
        .auroraCWincomingStream_tlast(auroraCWincomingStream_tlast),
        .auroraCWincomingStream_tvalid(auroraCWincomingStream_tvalid),
        .auroraCWlocalStream_tdata(auroraCWlocalStream_tdata),
        .auroraCWlocalStream_tlast(auroraCWlocalStream_tlast),
        .auroraCWlocalStream_tvalid(auroraCWlocalStream_tvalid),
        .auroraCWmergedStream_tdata(auroraCWmergedStream_tdata),
        .auroraCWmergedStream_tlast(auroraCWmergedStream_tlast),
        .auroraCWmergedStream_tready(auroraCWmergedStream_tready),
        .auroraCWmergedStream_tvalid(auroraCWmergedStream_tvalid),
        .auroraCWoutgoingStream_tdata(auroraCWoutgoingStream_tdata),
        .auroraCWoutgoingStream_tkeep(auroraCWoutgoingStream_tkeep),
        .auroraCWoutgoingStream_tlast(auroraCWoutgoingStream_tlast),
        .auroraCWoutgoingStream_tready(auroraCWoutgoingStream_tready),
        .auroraCWoutgoingStream_tvalid(auroraCWoutgoingStream_tvalid),
        .auroraReset(auroraReset),
        .auroraUserClk(auroraUserClk),
        .fanSpeed_tri_i(fanspeed_tri_i),
        .firmwareBuildDate_tri_i(firmwarebuilddate_tri_i),
        .gtReset(gtReset),
        .linkGPIO_tri_i({linkgpio_tri_i_31,linkgpio_tri_i_30,linkgpio_tri_i_29,linkgpio_tri_i_28,linkgpio_tri_i_27,linkgpio_tri_i_26,linkgpio_tri_i_25,linkgpio_tri_i_24,linkgpio_tri_i_23,linkgpio_tri_i_22,linkgpio_tri_i_21,linkgpio_tri_i_20,linkgpio_tri_i_19,linkgpio_tri_i_18,linkgpio_tri_i_17,linkgpio_tri_i_16,linkgpio_tri_i_15,linkgpio_tri_i_14,linkgpio_tri_i_13,linkgpio_tri_i_12,linkgpio_tri_i_11,linkgpio_tri_i_10,linkgpio_tri_i_9,linkgpio_tri_i_8,linkgpio_tri_i_7,linkgpio_tri_i_6,linkgpio_tri_i_5,linkgpio_tri_i_4,linkgpio_tri_i_3,linkgpio_tri_i_2,linkgpio_tri_i_1,linkgpio_tri_i_0}),
        .linkGPIO_tri_o({linkgpio_tri_o_31,linkgpio_tri_o_30,linkgpio_tri_o_29,linkgpio_tri_o_28,linkgpio_tri_o_27,linkgpio_tri_o_26,linkgpio_tri_o_25,linkgpio_tri_o_24,linkgpio_tri_o_23,linkgpio_tri_o_22,linkgpio_tri_o_21,linkgpio_tri_o_20,linkgpio_tri_o_19,linkgpio_tri_o_18,linkgpio_tri_o_17,linkgpio_tri_o_16,linkgpio_tri_o_15,linkgpio_tri_o_14,linkgpio_tri_o_13,linkgpio_tri_o_12,linkgpio_tri_o_11,linkgpio_tri_o_10,linkgpio_tri_o_9,linkgpio_tri_o_8,linkgpio_tri_o_7,linkgpio_tri_o_6,linkgpio_tri_o_5,linkgpio_tri_o_4,linkgpio_tri_o_3,linkgpio_tri_o_2,linkgpio_tri_o_1,linkgpio_tri_o_0}),
        .linkGPIO_tri_t({linkgpio_tri_t_31,linkgpio_tri_t_30,linkgpio_tri_t_29,linkgpio_tri_t_28,linkgpio_tri_t_27,linkgpio_tri_t_26,linkgpio_tri_t_25,linkgpio_tri_t_24,linkgpio_tri_t_23,linkgpio_tri_t_22,linkgpio_tri_t_21,linkgpio_tri_t_20,linkgpio_tri_t_19,linkgpio_tri_t_18,linkgpio_tri_t_17,linkgpio_tri_t_16,linkgpio_tri_t_15,linkgpio_tri_t_14,linkgpio_tri_t_13,linkgpio_tri_t_12,linkgpio_tri_t_11,linkgpio_tri_t_10,linkgpio_tri_t_9,linkgpio_tri_t_8,linkgpio_tri_t_7,linkgpio_tri_t_6,linkgpio_tri_t_5,linkgpio_tri_t_4,linkgpio_tri_t_3,linkgpio_tri_t_2,linkgpio_tri_t_1,linkgpio_tri_t_0}),
        .sysClk(sysClk));
  IOBUF linkgpio_tri_iobuf_0
       (.I(linkgpio_tri_o_0),
        .IO(linkgpio_tri_io[0]),
        .O(linkgpio_tri_i_0),
        .T(linkgpio_tri_t_0));
  IOBUF linkgpio_tri_iobuf_1
       (.I(linkgpio_tri_o_1),
        .IO(linkgpio_tri_io[1]),
        .O(linkgpio_tri_i_1),
        .T(linkgpio_tri_t_1));
  IOBUF linkgpio_tri_iobuf_10
       (.I(linkgpio_tri_o_10),
        .IO(linkgpio_tri_io[10]),
        .O(linkgpio_tri_i_10),
        .T(linkgpio_tri_t_10));
  IOBUF linkgpio_tri_iobuf_11
       (.I(linkgpio_tri_o_11),
        .IO(linkgpio_tri_io[11]),
        .O(linkgpio_tri_i_11),
        .T(linkgpio_tri_t_11));
  IOBUF linkgpio_tri_iobuf_12
       (.I(linkgpio_tri_o_12),
        .IO(linkgpio_tri_io[12]),
        .O(linkgpio_tri_i_12),
        .T(linkgpio_tri_t_12));
  IOBUF linkgpio_tri_iobuf_13
       (.I(linkgpio_tri_o_13),
        .IO(linkgpio_tri_io[13]),
        .O(linkgpio_tri_i_13),
        .T(linkgpio_tri_t_13));
  IOBUF linkgpio_tri_iobuf_14
       (.I(linkgpio_tri_o_14),
        .IO(linkgpio_tri_io[14]),
        .O(linkgpio_tri_i_14),
        .T(linkgpio_tri_t_14));
  IOBUF linkgpio_tri_iobuf_15
       (.I(linkgpio_tri_o_15),
        .IO(linkgpio_tri_io[15]),
        .O(linkgpio_tri_i_15),
        .T(linkgpio_tri_t_15));
  IOBUF linkgpio_tri_iobuf_16
       (.I(linkgpio_tri_o_16),
        .IO(linkgpio_tri_io[16]),
        .O(linkgpio_tri_i_16),
        .T(linkgpio_tri_t_16));
  IOBUF linkgpio_tri_iobuf_17
       (.I(linkgpio_tri_o_17),
        .IO(linkgpio_tri_io[17]),
        .O(linkgpio_tri_i_17),
        .T(linkgpio_tri_t_17));
  IOBUF linkgpio_tri_iobuf_18
       (.I(linkgpio_tri_o_18),
        .IO(linkgpio_tri_io[18]),
        .O(linkgpio_tri_i_18),
        .T(linkgpio_tri_t_18));
  IOBUF linkgpio_tri_iobuf_19
       (.I(linkgpio_tri_o_19),
        .IO(linkgpio_tri_io[19]),
        .O(linkgpio_tri_i_19),
        .T(linkgpio_tri_t_19));
  IOBUF linkgpio_tri_iobuf_2
       (.I(linkgpio_tri_o_2),
        .IO(linkgpio_tri_io[2]),
        .O(linkgpio_tri_i_2),
        .T(linkgpio_tri_t_2));
  IOBUF linkgpio_tri_iobuf_20
       (.I(linkgpio_tri_o_20),
        .IO(linkgpio_tri_io[20]),
        .O(linkgpio_tri_i_20),
        .T(linkgpio_tri_t_20));
  IOBUF linkgpio_tri_iobuf_21
       (.I(linkgpio_tri_o_21),
        .IO(linkgpio_tri_io[21]),
        .O(linkgpio_tri_i_21),
        .T(linkgpio_tri_t_21));
  IOBUF linkgpio_tri_iobuf_22
       (.I(linkgpio_tri_o_22),
        .IO(linkgpio_tri_io[22]),
        .O(linkgpio_tri_i_22),
        .T(linkgpio_tri_t_22));
  IOBUF linkgpio_tri_iobuf_23
       (.I(linkgpio_tri_o_23),
        .IO(linkgpio_tri_io[23]),
        .O(linkgpio_tri_i_23),
        .T(linkgpio_tri_t_23));
  IOBUF linkgpio_tri_iobuf_24
       (.I(linkgpio_tri_o_24),
        .IO(linkgpio_tri_io[24]),
        .O(linkgpio_tri_i_24),
        .T(linkgpio_tri_t_24));
  IOBUF linkgpio_tri_iobuf_25
       (.I(linkgpio_tri_o_25),
        .IO(linkgpio_tri_io[25]),
        .O(linkgpio_tri_i_25),
        .T(linkgpio_tri_t_25));
  IOBUF linkgpio_tri_iobuf_26
       (.I(linkgpio_tri_o_26),
        .IO(linkgpio_tri_io[26]),
        .O(linkgpio_tri_i_26),
        .T(linkgpio_tri_t_26));
  IOBUF linkgpio_tri_iobuf_27
       (.I(linkgpio_tri_o_27),
        .IO(linkgpio_tri_io[27]),
        .O(linkgpio_tri_i_27),
        .T(linkgpio_tri_t_27));
  IOBUF linkgpio_tri_iobuf_28
       (.I(linkgpio_tri_o_28),
        .IO(linkgpio_tri_io[28]),
        .O(linkgpio_tri_i_28),
        .T(linkgpio_tri_t_28));
  IOBUF linkgpio_tri_iobuf_29
       (.I(linkgpio_tri_o_29),
        .IO(linkgpio_tri_io[29]),
        .O(linkgpio_tri_i_29),
        .T(linkgpio_tri_t_29));
  IOBUF linkgpio_tri_iobuf_3
       (.I(linkgpio_tri_o_3),
        .IO(linkgpio_tri_io[3]),
        .O(linkgpio_tri_i_3),
        .T(linkgpio_tri_t_3));
  IOBUF linkgpio_tri_iobuf_30
       (.I(linkgpio_tri_o_30),
        .IO(linkgpio_tri_io[30]),
        .O(linkgpio_tri_i_30),
        .T(linkgpio_tri_t_30));
  IOBUF linkgpio_tri_iobuf_31
       (.I(linkgpio_tri_o_31),
        .IO(linkgpio_tri_io[31]),
        .O(linkgpio_tri_i_31),
        .T(linkgpio_tri_t_31));
  IOBUF linkgpio_tri_iobuf_4
       (.I(linkgpio_tri_o_4),
        .IO(linkgpio_tri_io[4]),
        .O(linkgpio_tri_i_4),
        .T(linkgpio_tri_t_4));
  IOBUF linkgpio_tri_iobuf_5
       (.I(linkgpio_tri_o_5),
        .IO(linkgpio_tri_io[5]),
        .O(linkgpio_tri_i_5),
        .T(linkgpio_tri_t_5));
  IOBUF linkgpio_tri_iobuf_6
       (.I(linkgpio_tri_o_6),
        .IO(linkgpio_tri_io[6]),
        .O(linkgpio_tri_i_6),
        .T(linkgpio_tri_t_6));
  IOBUF linkgpio_tri_iobuf_7
       (.I(linkgpio_tri_o_7),
        .IO(linkgpio_tri_io[7]),
        .O(linkgpio_tri_i_7),
        .T(linkgpio_tri_t_7));
  IOBUF linkgpio_tri_iobuf_8
       (.I(linkgpio_tri_o_8),
        .IO(linkgpio_tri_io[8]),
        .O(linkgpio_tri_i_8),
        .T(linkgpio_tri_t_8));
  IOBUF linkgpio_tri_iobuf_9
       (.I(linkgpio_tri_o_9),
        .IO(linkgpio_tri_io[9]),
        .O(linkgpio_tri_i_9),
        .T(linkgpio_tri_t_9));
  IOBUF lpc_sfp_iic_scl_iobuf
       (.I(lpc_sfp_iic_scl_o),
        .IO(lpc_sfp_iic_scl_io),
        .O(lpc_sfp_iic_scl_i),
        .T(lpc_sfp_iic_scl_t));
  IOBUF lpc_sfp_iic_sda_iobuf
       (.I(lpc_sfp_iic_sda_o),
        .IO(lpc_sfp_iic_sda_io),
        .O(lpc_sfp_iic_sda_i),
        .T(lpc_sfp_iic_sda_t));
  IOBUF sfp_gpio_tri_iobuf_0
       (.I(sfp_gpio_tri_o_0),
        .IO(sfp_gpio_tri_io[0]),
        .O(sfp_gpio_tri_i_0),
        .T(sfp_gpio_tri_t_0));
  IOBUF sfp_gpio_tri_iobuf_1
       (.I(sfp_gpio_tri_o_1),
        .IO(sfp_gpio_tri_io[1]),
        .O(sfp_gpio_tri_i_1),
        .T(sfp_gpio_tri_t_1));
  IOBUF sfp_gpio_tri_iobuf_10
       (.I(sfp_gpio_tri_o_10),
        .IO(sfp_gpio_tri_io[10]),
        .O(sfp_gpio_tri_i_10),
        .T(sfp_gpio_tri_t_10));
  IOBUF sfp_gpio_tri_iobuf_11
       (.I(sfp_gpio_tri_o_11),
        .IO(sfp_gpio_tri_io[11]),
        .O(sfp_gpio_tri_i_11),
        .T(sfp_gpio_tri_t_11));
  IOBUF sfp_gpio_tri_iobuf_12
       (.I(sfp_gpio_tri_o_12),
        .IO(sfp_gpio_tri_io[12]),
        .O(sfp_gpio_tri_i_12),
        .T(sfp_gpio_tri_t_12));
  IOBUF sfp_gpio_tri_iobuf_13
       (.I(sfp_gpio_tri_o_13),
        .IO(sfp_gpio_tri_io[13]),
        .O(sfp_gpio_tri_i_13),
        .T(sfp_gpio_tri_t_13));
  IOBUF sfp_gpio_tri_iobuf_14
       (.I(sfp_gpio_tri_o_14),
        .IO(sfp_gpio_tri_io[14]),
        .O(sfp_gpio_tri_i_14),
        .T(sfp_gpio_tri_t_14));
  IOBUF sfp_gpio_tri_iobuf_15
       (.I(sfp_gpio_tri_o_15),
        .IO(sfp_gpio_tri_io[15]),
        .O(sfp_gpio_tri_i_15),
        .T(sfp_gpio_tri_t_15));
  IOBUF sfp_gpio_tri_iobuf_16
       (.I(sfp_gpio_tri_o_16),
        .IO(sfp_gpio_tri_io[16]),
        .O(sfp_gpio_tri_i_16),
        .T(sfp_gpio_tri_t_16));
  IOBUF sfp_gpio_tri_iobuf_17
       (.I(sfp_gpio_tri_o_17),
        .IO(sfp_gpio_tri_io[17]),
        .O(sfp_gpio_tri_i_17),
        .T(sfp_gpio_tri_t_17));
  IOBUF sfp_gpio_tri_iobuf_18
       (.I(sfp_gpio_tri_o_18),
        .IO(sfp_gpio_tri_io[18]),
        .O(sfp_gpio_tri_i_18),
        .T(sfp_gpio_tri_t_18));
  IOBUF sfp_gpio_tri_iobuf_19
       (.I(sfp_gpio_tri_o_19),
        .IO(sfp_gpio_tri_io[19]),
        .O(sfp_gpio_tri_i_19),
        .T(sfp_gpio_tri_t_19));
  IOBUF sfp_gpio_tri_iobuf_2
       (.I(sfp_gpio_tri_o_2),
        .IO(sfp_gpio_tri_io[2]),
        .O(sfp_gpio_tri_i_2),
        .T(sfp_gpio_tri_t_2));
  IOBUF sfp_gpio_tri_iobuf_20
       (.I(sfp_gpio_tri_o_20),
        .IO(sfp_gpio_tri_io[20]),
        .O(sfp_gpio_tri_i_20),
        .T(sfp_gpio_tri_t_20));
  IOBUF sfp_gpio_tri_iobuf_21
       (.I(sfp_gpio_tri_o_21),
        .IO(sfp_gpio_tri_io[21]),
        .O(sfp_gpio_tri_i_21),
        .T(sfp_gpio_tri_t_21));
  IOBUF sfp_gpio_tri_iobuf_22
       (.I(sfp_gpio_tri_o_22),
        .IO(sfp_gpio_tri_io[22]),
        .O(sfp_gpio_tri_i_22),
        .T(sfp_gpio_tri_t_22));
  IOBUF sfp_gpio_tri_iobuf_23
       (.I(sfp_gpio_tri_o_23),
        .IO(sfp_gpio_tri_io[23]),
        .O(sfp_gpio_tri_i_23),
        .T(sfp_gpio_tri_t_23));
  IOBUF sfp_gpio_tri_iobuf_24
       (.I(sfp_gpio_tri_o_24),
        .IO(sfp_gpio_tri_io[24]),
        .O(sfp_gpio_tri_i_24),
        .T(sfp_gpio_tri_t_24));
  IOBUF sfp_gpio_tri_iobuf_25
       (.I(sfp_gpio_tri_o_25),
        .IO(sfp_gpio_tri_io[25]),
        .O(sfp_gpio_tri_i_25),
        .T(sfp_gpio_tri_t_25));
  IOBUF sfp_gpio_tri_iobuf_26
       (.I(sfp_gpio_tri_o_26),
        .IO(sfp_gpio_tri_io[26]),
        .O(sfp_gpio_tri_i_26),
        .T(sfp_gpio_tri_t_26));
  IOBUF sfp_gpio_tri_iobuf_27
       (.I(sfp_gpio_tri_o_27),
        .IO(sfp_gpio_tri_io[27]),
        .O(sfp_gpio_tri_i_27),
        .T(sfp_gpio_tri_t_27));
  IOBUF sfp_gpio_tri_iobuf_28
       (.I(sfp_gpio_tri_o_28),
        .IO(sfp_gpio_tri_io[28]),
        .O(sfp_gpio_tri_i_28),
        .T(sfp_gpio_tri_t_28));
  IOBUF sfp_gpio_tri_iobuf_29
       (.I(sfp_gpio_tri_o_29),
        .IO(sfp_gpio_tri_io[29]),
        .O(sfp_gpio_tri_i_29),
        .T(sfp_gpio_tri_t_29));
  IOBUF sfp_gpio_tri_iobuf_3
       (.I(sfp_gpio_tri_o_3),
        .IO(sfp_gpio_tri_io[3]),
        .O(sfp_gpio_tri_i_3),
        .T(sfp_gpio_tri_t_3));
  IOBUF sfp_gpio_tri_iobuf_30
       (.I(sfp_gpio_tri_o_30),
        .IO(sfp_gpio_tri_io[30]),
        .O(sfp_gpio_tri_i_30),
        .T(sfp_gpio_tri_t_30));
  IOBUF sfp_gpio_tri_iobuf_31
       (.I(sfp_gpio_tri_o_31),
        .IO(sfp_gpio_tri_io[31]),
        .O(sfp_gpio_tri_i_31),
        .T(sfp_gpio_tri_t_31));
  IOBUF sfp_gpio_tri_iobuf_4
       (.I(sfp_gpio_tri_o_4),
        .IO(sfp_gpio_tri_io[4]),
        .O(sfp_gpio_tri_i_4),
        .T(sfp_gpio_tri_t_4));
  IOBUF sfp_gpio_tri_iobuf_5
       (.I(sfp_gpio_tri_o_5),
        .IO(sfp_gpio_tri_io[5]),
        .O(sfp_gpio_tri_i_5),
        .T(sfp_gpio_tri_t_5));
  IOBUF sfp_gpio_tri_iobuf_6
       (.I(sfp_gpio_tri_o_6),
        .IO(sfp_gpio_tri_io[6]),
        .O(sfp_gpio_tri_i_6),
        .T(sfp_gpio_tri_t_6));
  IOBUF sfp_gpio_tri_iobuf_7
       (.I(sfp_gpio_tri_o_7),
        .IO(sfp_gpio_tri_io[7]),
        .O(sfp_gpio_tri_i_7),
        .T(sfp_gpio_tri_t_7));
  IOBUF sfp_gpio_tri_iobuf_8
       (.I(sfp_gpio_tri_o_8),
        .IO(sfp_gpio_tri_io[8]),
        .O(sfp_gpio_tri_i_8),
        .T(sfp_gpio_tri_t_8));
  IOBUF sfp_gpio_tri_iobuf_9
       (.I(sfp_gpio_tri_o_9),
        .IO(sfp_gpio_tri_io[9]),
        .O(sfp_gpio_tri_i_9),
        .T(sfp_gpio_tri_t_9));
endmodule
