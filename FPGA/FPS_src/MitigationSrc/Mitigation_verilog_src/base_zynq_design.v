//Copyright 1986-2016 Xilinx, Inc. All Rights Reserved.
//--------------------------------------------------------------------------------
//Tool Version: Vivado v.2016.1 (lin64) Build 1538259 Fri Apr  8 15:45:23 MDT 2016
//Date        : Wed Nov 16 10:35:44 2016
//Host        : localhost.localdomain running 64-bit CentOS Linux release 7.2.1511 (Core)
//Command     : generate_target base_zynq_design.bd
//Design      : base_zynq_design
//Purpose     : IP block netlist
//--------------------------------------------------------------------------------
`timescale 1 ps / 1 ps

module AuroraA_imp_14786E
   (CORE_STATUS0_channel_up,
    CORE_STATUS0_frame_err,
    CORE_STATUS0_hard_err,
    CORE_STATUS0_lane_up,
    CORE_STATUS0_rx_resetdone_out,
    CORE_STATUS0_soft_err,
    CORE_STATUS0_tx_lock,
    CORE_STATUS0_tx_resetdone_out,
    CORE_STATUS1_channel_up,
    CORE_STATUS1_frame_err,
    CORE_STATUS1_hard_err,
    CORE_STATUS1_lane_up,
    CORE_STATUS1_rx_resetdone_out,
    CORE_STATUS1_soft_err,
    CORE_STATUS1_tx_lock,
    CORE_STATUS1_tx_resetdone_out,
    CORE_STATUS2_channel_up,
    CORE_STATUS2_frame_err,
    CORE_STATUS2_hard_err,
    CORE_STATUS2_lane_up,
    CORE_STATUS2_rx_resetdone_out,
    CORE_STATUS2_soft_err,
    CORE_STATUS2_tx_lock,
    CORE_STATUS2_tx_resetdone_out,
    CORE_STATUS3_channel_up,
    CORE_STATUS3_frame_err,
    CORE_STATUS3_hard_err,
    CORE_STATUS3_lane_up,
    CORE_STATUS3_rx_resetdone_out,
    CORE_STATUS3_soft_err,
    CORE_STATUS3_tx_lock,
    CORE_STATUS3_tx_resetdone_out,
    GT0_DRP_IF0_daddr,
    GT0_DRP_IF0_den,
    GT0_DRP_IF0_di,
    GT0_DRP_IF0_do,
    GT0_DRP_IF0_drdy,
    GT0_DRP_IF0_dwe,
    GT0_DRP_IF1_daddr,
    GT0_DRP_IF1_den,
    GT0_DRP_IF1_di,
    GT0_DRP_IF1_do,
    GT0_DRP_IF1_drdy,
    GT0_DRP_IF1_dwe,
    GT0_DRP_IF2_daddr,
    GT0_DRP_IF2_den,
    GT0_DRP_IF2_di,
    GT0_DRP_IF2_do,
    GT0_DRP_IF2_drdy,
    GT0_DRP_IF2_dwe,
    GT0_DRP_IF3_daddr,
    GT0_DRP_IF3_den,
    GT0_DRP_IF3_di,
    GT0_DRP_IF3_do,
    GT0_DRP_IF3_drdy,
    GT0_DRP_IF3_dwe,
    GT_DIFF_REFCLK1_clk_n,
    GT_DIFF_REFCLK1_clk_p,
    GT_SERIAL_RX0_rxn,
    GT_SERIAL_RX0_rxp,
    GT_SERIAL_RX1_rxn,
    GT_SERIAL_RX1_rxp,
    GT_SERIAL_RX2_rxn,
    GT_SERIAL_RX2_rxp,
    GT_SERIAL_RX3_rxn,
    GT_SERIAL_RX3_rxp,
    GT_SERIAL_TX0_txn,
    GT_SERIAL_TX0_txp,
    GT_SERIAL_TX1_txn,
    GT_SERIAL_TX1_txp,
    GT_SERIAL_TX2_txn,
    GT_SERIAL_TX2_txp,
    GT_SERIAL_TX3_txn,
    GT_SERIAL_TX3_txp,
    USER_DATA_M_AXI_RX0_tdata,
    USER_DATA_M_AXI_RX0_tkeep,
    USER_DATA_M_AXI_RX0_tlast,
    USER_DATA_M_AXI_RX0_tvalid,
    USER_DATA_M_AXI_RX1_tdata,
    USER_DATA_M_AXI_RX1_tkeep,
    USER_DATA_M_AXI_RX1_tlast,
    USER_DATA_M_AXI_RX1_tvalid,
    USER_DATA_M_AXI_RX2_tdata,
    USER_DATA_M_AXI_RX2_tkeep,
    USER_DATA_M_AXI_RX2_tlast,
    USER_DATA_M_AXI_RX2_tvalid,
    USER_DATA_M_AXI_RX3_tdata,
    USER_DATA_M_AXI_RX3_tkeep,
    USER_DATA_M_AXI_RX3_tlast,
    USER_DATA_M_AXI_RX3_tvalid,
    USER_DATA_S_AXI_TX0_tdata,
    USER_DATA_S_AXI_TX0_tkeep,
    USER_DATA_S_AXI_TX0_tlast,
    USER_DATA_S_AXI_TX0_tready,
    USER_DATA_S_AXI_TX0_tvalid,
    USER_DATA_S_AXI_TX1_tdata,
    USER_DATA_S_AXI_TX1_tkeep,
    USER_DATA_S_AXI_TX1_tlast,
    USER_DATA_S_AXI_TX1_tready,
    USER_DATA_S_AXI_TX1_tvalid,
    USER_DATA_S_AXI_TX2_tdata,
    USER_DATA_S_AXI_TX2_tkeep,
    USER_DATA_S_AXI_TX2_tlast,
    USER_DATA_S_AXI_TX2_tready,
    USER_DATA_S_AXI_TX2_tvalid,
    USER_DATA_S_AXI_TX3_tdata,
    USER_DATA_S_AXI_TX3_tkeep,
    USER_DATA_S_AXI_TX3_tlast,
    USER_DATA_S_AXI_TX3_tready,
    USER_DATA_S_AXI_TX3_tvalid,
    auroraReset,
    auroraUserClk,
    gtxReset,
    init_clk_in);
  output CORE_STATUS0_channel_up;
  output CORE_STATUS0_frame_err;
  output CORE_STATUS0_hard_err;
  output [0:0]CORE_STATUS0_lane_up;
  output CORE_STATUS0_rx_resetdone_out;
  output CORE_STATUS0_soft_err;
  output CORE_STATUS0_tx_lock;
  output CORE_STATUS0_tx_resetdone_out;
  output CORE_STATUS1_channel_up;
  output CORE_STATUS1_frame_err;
  output CORE_STATUS1_hard_err;
  output [0:0]CORE_STATUS1_lane_up;
  output CORE_STATUS1_rx_resetdone_out;
  output CORE_STATUS1_soft_err;
  output CORE_STATUS1_tx_lock;
  output CORE_STATUS1_tx_resetdone_out;
  output CORE_STATUS2_channel_up;
  output CORE_STATUS2_frame_err;
  output CORE_STATUS2_hard_err;
  output [0:0]CORE_STATUS2_lane_up;
  output CORE_STATUS2_rx_resetdone_out;
  output CORE_STATUS2_soft_err;
  output CORE_STATUS2_tx_lock;
  output CORE_STATUS2_tx_resetdone_out;
  output CORE_STATUS3_channel_up;
  output CORE_STATUS3_frame_err;
  output CORE_STATUS3_hard_err;
  output [0:0]CORE_STATUS3_lane_up;
  output CORE_STATUS3_rx_resetdone_out;
  output CORE_STATUS3_soft_err;
  output CORE_STATUS3_tx_lock;
  output CORE_STATUS3_tx_resetdone_out;
  input [8:0]GT0_DRP_IF0_daddr;
  input GT0_DRP_IF0_den;
  input [15:0]GT0_DRP_IF0_di;
  output [15:0]GT0_DRP_IF0_do;
  output GT0_DRP_IF0_drdy;
  input GT0_DRP_IF0_dwe;
  input [8:0]GT0_DRP_IF1_daddr;
  input GT0_DRP_IF1_den;
  input [15:0]GT0_DRP_IF1_di;
  output [15:0]GT0_DRP_IF1_do;
  output GT0_DRP_IF1_drdy;
  input GT0_DRP_IF1_dwe;
  input [8:0]GT0_DRP_IF2_daddr;
  input GT0_DRP_IF2_den;
  input [15:0]GT0_DRP_IF2_di;
  output [15:0]GT0_DRP_IF2_do;
  output GT0_DRP_IF2_drdy;
  input GT0_DRP_IF2_dwe;
  input [8:0]GT0_DRP_IF3_daddr;
  input GT0_DRP_IF3_den;
  input [15:0]GT0_DRP_IF3_di;
  output [15:0]GT0_DRP_IF3_do;
  output GT0_DRP_IF3_drdy;
  input GT0_DRP_IF3_dwe;
  input GT_DIFF_REFCLK1_clk_n;
  input GT_DIFF_REFCLK1_clk_p;
  input [0:0]GT_SERIAL_RX0_rxn;
  input [0:0]GT_SERIAL_RX0_rxp;
  input [0:0]GT_SERIAL_RX1_rxn;
  input [0:0]GT_SERIAL_RX1_rxp;
  input [0:0]GT_SERIAL_RX2_rxn;
  input [0:0]GT_SERIAL_RX2_rxp;
  input [0:0]GT_SERIAL_RX3_rxn;
  input [0:0]GT_SERIAL_RX3_rxp;
  output [0:0]GT_SERIAL_TX0_txn;
  output [0:0]GT_SERIAL_TX0_txp;
  output [0:0]GT_SERIAL_TX1_txn;
  output [0:0]GT_SERIAL_TX1_txp;
  output [0:0]GT_SERIAL_TX2_txn;
  output [0:0]GT_SERIAL_TX2_txp;
  output [0:0]GT_SERIAL_TX3_txn;
  output [0:0]GT_SERIAL_TX3_txp;
  output [0:15]USER_DATA_M_AXI_RX0_tdata;
  output [0:1]USER_DATA_M_AXI_RX0_tkeep;
  output USER_DATA_M_AXI_RX0_tlast;
  output USER_DATA_M_AXI_RX0_tvalid;
  output [0:15]USER_DATA_M_AXI_RX1_tdata;
  output [0:1]USER_DATA_M_AXI_RX1_tkeep;
  output USER_DATA_M_AXI_RX1_tlast;
  output USER_DATA_M_AXI_RX1_tvalid;
  output [0:15]USER_DATA_M_AXI_RX2_tdata;
  output [0:1]USER_DATA_M_AXI_RX2_tkeep;
  output USER_DATA_M_AXI_RX2_tlast;
  output USER_DATA_M_AXI_RX2_tvalid;
  output [0:15]USER_DATA_M_AXI_RX3_tdata;
  output [0:1]USER_DATA_M_AXI_RX3_tkeep;
  output USER_DATA_M_AXI_RX3_tlast;
  output USER_DATA_M_AXI_RX3_tvalid;
  input [0:15]USER_DATA_S_AXI_TX0_tdata;
  input [0:1]USER_DATA_S_AXI_TX0_tkeep;
  input USER_DATA_S_AXI_TX0_tlast;
  output USER_DATA_S_AXI_TX0_tready;
  input USER_DATA_S_AXI_TX0_tvalid;
  input [0:15]USER_DATA_S_AXI_TX1_tdata;
  input [0:1]USER_DATA_S_AXI_TX1_tkeep;
  input USER_DATA_S_AXI_TX1_tlast;
  output USER_DATA_S_AXI_TX1_tready;
  input USER_DATA_S_AXI_TX1_tvalid;
  input [0:15]USER_DATA_S_AXI_TX2_tdata;
  input [0:1]USER_DATA_S_AXI_TX2_tkeep;
  input USER_DATA_S_AXI_TX2_tlast;
  output USER_DATA_S_AXI_TX2_tready;
  input USER_DATA_S_AXI_TX2_tvalid;
  input [0:15]USER_DATA_S_AXI_TX3_tdata;
  input [0:1]USER_DATA_S_AXI_TX3_tkeep;
  input USER_DATA_S_AXI_TX3_tlast;
  output USER_DATA_S_AXI_TX3_tready;
  input USER_DATA_S_AXI_TX3_tvalid;
  input auroraReset;
  output auroraUserClk;
  input gtxReset;
  input init_clk_in;

  wire [0:15]Conn10_TDATA;
  wire [0:1]Conn10_TKEEP;
  wire Conn10_TLAST;
  wire Conn10_TVALID;
  wire Conn11_CHANNEL_UP;
  wire Conn11_FRAME_ERR;
  wire Conn11_HARD_ERR;
  wire [0:0]Conn11_LANE_UP;
  wire Conn11_RX_RESETDONE_OUT;
  wire Conn11_SOFT_ERR;
  wire Conn11_TX_LOCK;
  wire Conn11_TX_RESETDONE_OUT;
  wire [0:0]Conn12_TXN;
  wire [0:0]Conn12_TXP;
  wire [0:15]Conn13_TDATA;
  wire [0:1]Conn13_TKEEP;
  wire Conn13_TLAST;
  wire Conn13_TREADY;
  wire Conn13_TVALID;
  wire [8:0]Conn14_DADDR;
  wire Conn14_DEN;
  wire [15:0]Conn14_DI;
  wire [15:0]Conn14_DO;
  wire Conn14_DRDY;
  wire Conn14_DWE;
  wire [0:0]Conn15_RXN;
  wire [0:0]Conn15_RXP;
  wire [0:15]Conn16_TDATA;
  wire [0:1]Conn16_TKEEP;
  wire Conn16_TLAST;
  wire Conn16_TVALID;
  wire Conn17_CHANNEL_UP;
  wire Conn17_FRAME_ERR;
  wire Conn17_HARD_ERR;
  wire [0:0]Conn17_LANE_UP;
  wire Conn17_RX_RESETDONE_OUT;
  wire Conn17_SOFT_ERR;
  wire Conn17_TX_LOCK;
  wire Conn17_TX_RESETDONE_OUT;
  wire [0:0]Conn18_TXN;
  wire [0:0]Conn18_TXP;
  wire [0:15]Conn1_TDATA;
  wire [0:1]Conn1_TKEEP;
  wire Conn1_TLAST;
  wire Conn1_TVALID;
  wire Conn2_CHANNEL_UP;
  wire Conn2_FRAME_ERR;
  wire Conn2_HARD_ERR;
  wire [0:0]Conn2_LANE_UP;
  wire Conn2_RX_RESETDONE_OUT;
  wire Conn2_SOFT_ERR;
  wire Conn2_TX_LOCK;
  wire Conn2_TX_RESETDONE_OUT;
  wire [0:0]Conn3_TXN;
  wire [0:0]Conn3_TXP;
  wire [0:15]Conn4_TDATA;
  wire [0:1]Conn4_TKEEP;
  wire Conn4_TLAST;
  wire Conn4_TREADY;
  wire Conn4_TVALID;
  wire [8:0]Conn5_DADDR;
  wire Conn5_DEN;
  wire [15:0]Conn5_DI;
  wire [15:0]Conn5_DO;
  wire Conn5_DRDY;
  wire Conn5_DWE;
  wire [0:0]Conn6_RXN;
  wire [0:0]Conn6_RXP;
  wire [0:15]Conn7_TDATA;
  wire [0:1]Conn7_TKEEP;
  wire Conn7_TLAST;
  wire Conn7_TREADY;
  wire Conn7_TVALID;
  wire [8:0]Conn8_DADDR;
  wire Conn8_DEN;
  wire [15:0]Conn8_DI;
  wire [15:0]Conn8_DO;
  wire Conn8_DRDY;
  wire Conn8_DWE;
  wire [0:0]Conn9_RXN;
  wire [0:0]Conn9_RXP;
  wire GT_DIFF_REFCLK1_1_CLK_N;
  wire GT_DIFF_REFCLK1_1_CLK_P;
  wire [0:0]GT_SERIAL_RX_1_RXN;
  wire [0:0]GT_SERIAL_RX_1_RXP;
  wire [0:15]USER_DATA_S_AXI_TX_1_TDATA;
  wire [0:1]USER_DATA_S_AXI_TX_1_TKEEP;
  wire USER_DATA_S_AXI_TX_1_TLAST;
  wire USER_DATA_S_AXI_TX_1_TREADY;
  wire USER_DATA_S_AXI_TX_1_TVALID;
  wire [0:0]Zero_dout;
  wire aurora_8b10b_0_CORE_STATUS_CHANNEL_UP;
  wire aurora_8b10b_0_CORE_STATUS_FRAME_ERR;
  wire aurora_8b10b_0_CORE_STATUS_HARD_ERR;
  wire [0:0]aurora_8b10b_0_CORE_STATUS_LANE_UP;
  wire aurora_8b10b_0_CORE_STATUS_RX_RESETDONE_OUT;
  wire aurora_8b10b_0_CORE_STATUS_SOFT_ERR;
  wire aurora_8b10b_0_CORE_STATUS_TX_LOCK;
  wire aurora_8b10b_0_CORE_STATUS_TX_RESETDONE_OUT;
  wire [0:0]aurora_8b10b_0_GT_SERIAL_TX_TXN;
  wire [0:0]aurora_8b10b_0_GT_SERIAL_TX_TXP;
  wire [0:15]aurora_8b10b_0_USER_DATA_M_AXI_RX_TDATA;
  wire [0:1]aurora_8b10b_0_USER_DATA_M_AXI_RX_TKEEP;
  wire aurora_8b10b_0_USER_DATA_M_AXI_RX_TLAST;
  wire aurora_8b10b_0_USER_DATA_M_AXI_RX_TVALID;
  wire aurora_8b10b_0_gt0_qplllock_out;
  wire aurora_8b10b_0_gt0_qpllrefclklost_out;
  wire aurora_8b10b_0_gt_qpllclk_quad1_out;
  wire aurora_8b10b_0_gt_qpllrefclk_quad1_out;
  wire aurora_8b10b_0_gt_refclk1_out;
  wire aurora_8b10b_0_pll_not_locked_out;
  wire aurora_8b10b_0_sync_clk_out;
  wire aurora_8b10b_0_user_clk_out;
  wire [8:0]drp_bridge_0_DRP1_DADDR;
  wire drp_bridge_0_DRP1_DEN;
  wire [15:0]drp_bridge_0_DRP1_DI;
  wire [15:0]drp_bridge_0_DRP1_DO;
  wire drp_bridge_0_DRP1_DRDY;
  wire drp_bridge_0_DRP1_DWE;
  wire gt_reset_1;
  wire [2:0]loopbackControl_dout;
  wire processing_system7_0_FCLK_CLK0;
  wire reset_1;

  assign CORE_STATUS0_channel_up = aurora_8b10b_0_CORE_STATUS_CHANNEL_UP;
  assign CORE_STATUS0_frame_err = aurora_8b10b_0_CORE_STATUS_FRAME_ERR;
  assign CORE_STATUS0_hard_err = aurora_8b10b_0_CORE_STATUS_HARD_ERR;
  assign CORE_STATUS0_lane_up[0] = aurora_8b10b_0_CORE_STATUS_LANE_UP;
  assign CORE_STATUS0_rx_resetdone_out = aurora_8b10b_0_CORE_STATUS_RX_RESETDONE_OUT;
  assign CORE_STATUS0_soft_err = aurora_8b10b_0_CORE_STATUS_SOFT_ERR;
  assign CORE_STATUS0_tx_lock = aurora_8b10b_0_CORE_STATUS_TX_LOCK;
  assign CORE_STATUS0_tx_resetdone_out = aurora_8b10b_0_CORE_STATUS_TX_RESETDONE_OUT;
  assign CORE_STATUS1_channel_up = Conn2_CHANNEL_UP;
  assign CORE_STATUS1_frame_err = Conn2_FRAME_ERR;
  assign CORE_STATUS1_hard_err = Conn2_HARD_ERR;
  assign CORE_STATUS1_lane_up[0] = Conn2_LANE_UP;
  assign CORE_STATUS1_rx_resetdone_out = Conn2_RX_RESETDONE_OUT;
  assign CORE_STATUS1_soft_err = Conn2_SOFT_ERR;
  assign CORE_STATUS1_tx_lock = Conn2_TX_LOCK;
  assign CORE_STATUS1_tx_resetdone_out = Conn2_TX_RESETDONE_OUT;
  assign CORE_STATUS2_channel_up = Conn11_CHANNEL_UP;
  assign CORE_STATUS2_frame_err = Conn11_FRAME_ERR;
  assign CORE_STATUS2_hard_err = Conn11_HARD_ERR;
  assign CORE_STATUS2_lane_up[0] = Conn11_LANE_UP;
  assign CORE_STATUS2_rx_resetdone_out = Conn11_RX_RESETDONE_OUT;
  assign CORE_STATUS2_soft_err = Conn11_SOFT_ERR;
  assign CORE_STATUS2_tx_lock = Conn11_TX_LOCK;
  assign CORE_STATUS2_tx_resetdone_out = Conn11_TX_RESETDONE_OUT;
  assign CORE_STATUS3_channel_up = Conn17_CHANNEL_UP;
  assign CORE_STATUS3_frame_err = Conn17_FRAME_ERR;
  assign CORE_STATUS3_hard_err = Conn17_HARD_ERR;
  assign CORE_STATUS3_lane_up[0] = Conn17_LANE_UP;
  assign CORE_STATUS3_rx_resetdone_out = Conn17_RX_RESETDONE_OUT;
  assign CORE_STATUS3_soft_err = Conn17_SOFT_ERR;
  assign CORE_STATUS3_tx_lock = Conn17_TX_LOCK;
  assign CORE_STATUS3_tx_resetdone_out = Conn17_TX_RESETDONE_OUT;
  assign Conn13_TDATA = USER_DATA_S_AXI_TX3_tdata[0:15];
  assign Conn13_TKEEP = USER_DATA_S_AXI_TX3_tkeep[0:1];
  assign Conn13_TLAST = USER_DATA_S_AXI_TX3_tlast;
  assign Conn13_TVALID = USER_DATA_S_AXI_TX3_tvalid;
  assign Conn14_DADDR = GT0_DRP_IF3_daddr[8:0];
  assign Conn14_DEN = GT0_DRP_IF3_den;
  assign Conn14_DI = GT0_DRP_IF3_di[15:0];
  assign Conn14_DWE = GT0_DRP_IF3_dwe;
  assign Conn15_RXN = GT_SERIAL_RX3_rxn[0];
  assign Conn15_RXP = GT_SERIAL_RX3_rxp[0];
  assign Conn4_TDATA = USER_DATA_S_AXI_TX1_tdata[0:15];
  assign Conn4_TKEEP = USER_DATA_S_AXI_TX1_tkeep[0:1];
  assign Conn4_TLAST = USER_DATA_S_AXI_TX1_tlast;
  assign Conn4_TVALID = USER_DATA_S_AXI_TX1_tvalid;
  assign Conn5_DADDR = GT0_DRP_IF1_daddr[8:0];
  assign Conn5_DEN = GT0_DRP_IF1_den;
  assign Conn5_DI = GT0_DRP_IF1_di[15:0];
  assign Conn5_DWE = GT0_DRP_IF1_dwe;
  assign Conn6_RXN = GT_SERIAL_RX1_rxn[0];
  assign Conn6_RXP = GT_SERIAL_RX1_rxp[0];
  assign Conn7_TDATA = USER_DATA_S_AXI_TX2_tdata[0:15];
  assign Conn7_TKEEP = USER_DATA_S_AXI_TX2_tkeep[0:1];
  assign Conn7_TLAST = USER_DATA_S_AXI_TX2_tlast;
  assign Conn7_TVALID = USER_DATA_S_AXI_TX2_tvalid;
  assign Conn8_DADDR = GT0_DRP_IF2_daddr[8:0];
  assign Conn8_DEN = GT0_DRP_IF2_den;
  assign Conn8_DI = GT0_DRP_IF2_di[15:0];
  assign Conn8_DWE = GT0_DRP_IF2_dwe;
  assign Conn9_RXN = GT_SERIAL_RX2_rxn[0];
  assign Conn9_RXP = GT_SERIAL_RX2_rxp[0];
  assign GT0_DRP_IF0_do[15:0] = drp_bridge_0_DRP1_DO;
  assign GT0_DRP_IF0_drdy = drp_bridge_0_DRP1_DRDY;
  assign GT0_DRP_IF1_do[15:0] = Conn5_DO;
  assign GT0_DRP_IF1_drdy = Conn5_DRDY;
  assign GT0_DRP_IF2_do[15:0] = Conn8_DO;
  assign GT0_DRP_IF2_drdy = Conn8_DRDY;
  assign GT0_DRP_IF3_do[15:0] = Conn14_DO;
  assign GT0_DRP_IF3_drdy = Conn14_DRDY;
  assign GT_DIFF_REFCLK1_1_CLK_N = GT_DIFF_REFCLK1_clk_n;
  assign GT_DIFF_REFCLK1_1_CLK_P = GT_DIFF_REFCLK1_clk_p;
  assign GT_SERIAL_RX_1_RXN = GT_SERIAL_RX0_rxn[0];
  assign GT_SERIAL_RX_1_RXP = GT_SERIAL_RX0_rxp[0];
  assign GT_SERIAL_TX0_txn[0] = aurora_8b10b_0_GT_SERIAL_TX_TXN;
  assign GT_SERIAL_TX0_txp[0] = aurora_8b10b_0_GT_SERIAL_TX_TXP;
  assign GT_SERIAL_TX1_txn[0] = Conn3_TXN;
  assign GT_SERIAL_TX1_txp[0] = Conn3_TXP;
  assign GT_SERIAL_TX2_txn[0] = Conn12_TXN;
  assign GT_SERIAL_TX2_txp[0] = Conn12_TXP;
  assign GT_SERIAL_TX3_txn[0] = Conn18_TXN;
  assign GT_SERIAL_TX3_txp[0] = Conn18_TXP;
  assign USER_DATA_M_AXI_RX0_tdata[0:15] = aurora_8b10b_0_USER_DATA_M_AXI_RX_TDATA;
  assign USER_DATA_M_AXI_RX0_tkeep[0:1] = aurora_8b10b_0_USER_DATA_M_AXI_RX_TKEEP;
  assign USER_DATA_M_AXI_RX0_tlast = aurora_8b10b_0_USER_DATA_M_AXI_RX_TLAST;
  assign USER_DATA_M_AXI_RX0_tvalid = aurora_8b10b_0_USER_DATA_M_AXI_RX_TVALID;
  assign USER_DATA_M_AXI_RX1_tdata[0:15] = Conn1_TDATA;
  assign USER_DATA_M_AXI_RX1_tkeep[0:1] = Conn1_TKEEP;
  assign USER_DATA_M_AXI_RX1_tlast = Conn1_TLAST;
  assign USER_DATA_M_AXI_RX1_tvalid = Conn1_TVALID;
  assign USER_DATA_M_AXI_RX2_tdata[0:15] = Conn10_TDATA;
  assign USER_DATA_M_AXI_RX2_tkeep[0:1] = Conn10_TKEEP;
  assign USER_DATA_M_AXI_RX2_tlast = Conn10_TLAST;
  assign USER_DATA_M_AXI_RX2_tvalid = Conn10_TVALID;
  assign USER_DATA_M_AXI_RX3_tdata[0:15] = Conn16_TDATA;
  assign USER_DATA_M_AXI_RX3_tkeep[0:1] = Conn16_TKEEP;
  assign USER_DATA_M_AXI_RX3_tlast = Conn16_TLAST;
  assign USER_DATA_M_AXI_RX3_tvalid = Conn16_TVALID;
  assign USER_DATA_S_AXI_TX0_tready = USER_DATA_S_AXI_TX_1_TREADY;
  assign USER_DATA_S_AXI_TX1_tready = Conn4_TREADY;
  assign USER_DATA_S_AXI_TX2_tready = Conn7_TREADY;
  assign USER_DATA_S_AXI_TX3_tready = Conn13_TREADY;
  assign USER_DATA_S_AXI_TX_1_TDATA = USER_DATA_S_AXI_TX0_tdata[0:15];
  assign USER_DATA_S_AXI_TX_1_TKEEP = USER_DATA_S_AXI_TX0_tkeep[0:1];
  assign USER_DATA_S_AXI_TX_1_TLAST = USER_DATA_S_AXI_TX0_tlast;
  assign USER_DATA_S_AXI_TX_1_TVALID = USER_DATA_S_AXI_TX0_tvalid;
  assign auroraUserClk = aurora_8b10b_0_user_clk_out;
  assign drp_bridge_0_DRP1_DADDR = GT0_DRP_IF0_daddr[8:0];
  assign drp_bridge_0_DRP1_DEN = GT0_DRP_IF0_den;
  assign drp_bridge_0_DRP1_DI = GT0_DRP_IF0_di[15:0];
  assign drp_bridge_0_DRP1_DWE = GT0_DRP_IF0_dwe;
  assign gt_reset_1 = gtxReset;
  assign processing_system7_0_FCLK_CLK0 = init_clk_in;
  assign reset_1 = auroraReset;
  base_zynq_design_xlconstant_0_1 Zero
       (.dout(Zero_dout));
  base_zynq_design_aurora_8b10b_0_0 aurora_8b10b_0
       (.channel_up(aurora_8b10b_0_CORE_STATUS_CHANNEL_UP),
        .drpaddr_in(drp_bridge_0_DRP1_DADDR),
        .drpclk_in(processing_system7_0_FCLK_CLK0),
        .drpdi_in(drp_bridge_0_DRP1_DI),
        .drpdo_out(drp_bridge_0_DRP1_DO),
        .drpen_in(drp_bridge_0_DRP1_DEN),
        .drprdy_out(drp_bridge_0_DRP1_DRDY),
        .drpwe_in(drp_bridge_0_DRP1_DWE),
        .frame_err(aurora_8b10b_0_CORE_STATUS_FRAME_ERR),
        .gt0_qplllock_out(aurora_8b10b_0_gt0_qplllock_out),
        .gt0_qpllrefclklost_out(aurora_8b10b_0_gt0_qpllrefclklost_out),
        .gt_qpllclk_quad1_out(aurora_8b10b_0_gt_qpllclk_quad1_out),
        .gt_qpllrefclk_quad1_out(aurora_8b10b_0_gt_qpllrefclk_quad1_out),
        .gt_refclk1_n(GT_DIFF_REFCLK1_1_CLK_N),
        .gt_refclk1_out(aurora_8b10b_0_gt_refclk1_out),
        .gt_refclk1_p(GT_DIFF_REFCLK1_1_CLK_P),
        .gt_reset(gt_reset_1),
        .hard_err(aurora_8b10b_0_CORE_STATUS_HARD_ERR),
        .init_clk_in(processing_system7_0_FCLK_CLK0),
        .lane_up(aurora_8b10b_0_CORE_STATUS_LANE_UP),
        .loopback(loopbackControl_dout),
        .m_axi_rx_tdata(aurora_8b10b_0_USER_DATA_M_AXI_RX_TDATA),
        .m_axi_rx_tkeep(aurora_8b10b_0_USER_DATA_M_AXI_RX_TKEEP),
        .m_axi_rx_tlast(aurora_8b10b_0_USER_DATA_M_AXI_RX_TLAST),
        .m_axi_rx_tvalid(aurora_8b10b_0_USER_DATA_M_AXI_RX_TVALID),
        .pll_not_locked_out(aurora_8b10b_0_pll_not_locked_out),
        .power_down(Zero_dout),
        .reset(reset_1),
        .rx_resetdone_out(aurora_8b10b_0_CORE_STATUS_RX_RESETDONE_OUT),
        .rxn(GT_SERIAL_RX_1_RXN),
        .rxp(GT_SERIAL_RX_1_RXP),
        .s_axi_tx_tdata(USER_DATA_S_AXI_TX_1_TDATA),
        .s_axi_tx_tkeep(USER_DATA_S_AXI_TX_1_TKEEP),
        .s_axi_tx_tlast(USER_DATA_S_AXI_TX_1_TLAST),
        .s_axi_tx_tready(USER_DATA_S_AXI_TX_1_TREADY),
        .s_axi_tx_tvalid(USER_DATA_S_AXI_TX_1_TVALID),
        .soft_err(aurora_8b10b_0_CORE_STATUS_SOFT_ERR),
        .sync_clk_out(aurora_8b10b_0_sync_clk_out),
        .tx_lock(aurora_8b10b_0_CORE_STATUS_TX_LOCK),
        .tx_resetdone_out(aurora_8b10b_0_CORE_STATUS_TX_RESETDONE_OUT),
        .txn(aurora_8b10b_0_GT_SERIAL_TX_TXN),
        .txp(aurora_8b10b_0_GT_SERIAL_TX_TXP),
        .user_clk_out(aurora_8b10b_0_user_clk_out));
  base_zynq_design_aurora_8b10b_1_0 aurora_8b10b_1
       (.channel_up(Conn2_CHANNEL_UP),
        .drpaddr_in(Conn5_DADDR),
        .drpclk_in(processing_system7_0_FCLK_CLK0),
        .drpdi_in(Conn5_DI),
        .drpdo_out(Conn5_DO),
        .drpen_in(Conn5_DEN),
        .drprdy_out(Conn5_DRDY),
        .drpwe_in(Conn5_DWE),
        .frame_err(Conn2_FRAME_ERR),
        .gt0_qplllock_in(aurora_8b10b_0_gt0_qplllock_out),
        .gt0_qpllrefclklost_in(aurora_8b10b_0_gt0_qpllrefclklost_out),
        .gt_qpllclk_quad1_in(aurora_8b10b_0_gt_qpllclk_quad1_out),
        .gt_qpllrefclk_quad1_in(aurora_8b10b_0_gt_qpllrefclk_quad1_out),
        .gt_refclk1(aurora_8b10b_0_gt_refclk1_out),
        .gt_reset(gt_reset_1),
        .hard_err(Conn2_HARD_ERR),
        .init_clk_in(processing_system7_0_FCLK_CLK0),
        .lane_up(Conn2_LANE_UP),
        .loopback(loopbackControl_dout),
        .m_axi_rx_tdata(Conn1_TDATA),
        .m_axi_rx_tkeep(Conn1_TKEEP),
        .m_axi_rx_tlast(Conn1_TLAST),
        .m_axi_rx_tvalid(Conn1_TVALID),
        .pll_not_locked(aurora_8b10b_0_pll_not_locked_out),
        .power_down(Zero_dout),
        .reset(reset_1),
        .rx_resetdone_out(Conn2_RX_RESETDONE_OUT),
        .rxn(Conn6_RXN),
        .rxp(Conn6_RXP),
        .s_axi_tx_tdata(Conn4_TDATA),
        .s_axi_tx_tkeep(Conn4_TKEEP),
        .s_axi_tx_tlast(Conn4_TLAST),
        .s_axi_tx_tready(Conn4_TREADY),
        .s_axi_tx_tvalid(Conn4_TVALID),
        .soft_err(Conn2_SOFT_ERR),
        .sync_clk(aurora_8b10b_0_sync_clk_out),
        .tx_lock(Conn2_TX_LOCK),
        .tx_resetdone_out(Conn2_TX_RESETDONE_OUT),
        .txn(Conn3_TXN),
        .txp(Conn3_TXP),
        .user_clk(aurora_8b10b_0_user_clk_out));
  base_zynq_design_aurora_8b10b_1_1 aurora_8b10b_2
       (.channel_up(Conn11_CHANNEL_UP),
        .drpaddr_in(Conn8_DADDR),
        .drpclk_in(processing_system7_0_FCLK_CLK0),
        .drpdi_in(Conn8_DI),
        .drpdo_out(Conn8_DO),
        .drpen_in(Conn8_DEN),
        .drprdy_out(Conn8_DRDY),
        .drpwe_in(Conn8_DWE),
        .frame_err(Conn11_FRAME_ERR),
        .gt0_qplllock_in(aurora_8b10b_0_gt0_qplllock_out),
        .gt0_qpllrefclklost_in(aurora_8b10b_0_gt0_qpllrefclklost_out),
        .gt_qpllclk_quad1_in(aurora_8b10b_0_gt_qpllclk_quad1_out),
        .gt_qpllrefclk_quad1_in(aurora_8b10b_0_gt_qpllrefclk_quad1_out),
        .gt_refclk1(aurora_8b10b_0_gt_refclk1_out),
        .gt_reset(gt_reset_1),
        .hard_err(Conn11_HARD_ERR),
        .init_clk_in(processing_system7_0_FCLK_CLK0),
        .lane_up(Conn11_LANE_UP),
        .loopback(loopbackControl_dout),
        .m_axi_rx_tdata(Conn10_TDATA),
        .m_axi_rx_tkeep(Conn10_TKEEP),
        .m_axi_rx_tlast(Conn10_TLAST),
        .m_axi_rx_tvalid(Conn10_TVALID),
        .pll_not_locked(aurora_8b10b_0_pll_not_locked_out),
        .power_down(Zero_dout),
        .reset(reset_1),
        .rx_resetdone_out(Conn11_RX_RESETDONE_OUT),
        .rxn(Conn9_RXN),
        .rxp(Conn9_RXP),
        .s_axi_tx_tdata(Conn7_TDATA),
        .s_axi_tx_tkeep(Conn7_TKEEP),
        .s_axi_tx_tlast(Conn7_TLAST),
        .s_axi_tx_tready(Conn7_TREADY),
        .s_axi_tx_tvalid(Conn7_TVALID),
        .soft_err(Conn11_SOFT_ERR),
        .sync_clk(aurora_8b10b_0_sync_clk_out),
        .tx_lock(Conn11_TX_LOCK),
        .tx_resetdone_out(Conn11_TX_RESETDONE_OUT),
        .txn(Conn12_TXN),
        .txp(Conn12_TXP),
        .user_clk(aurora_8b10b_0_user_clk_out));
  base_zynq_design_aurora_8b10b_1_2 aurora_8b10b_3
       (.channel_up(Conn17_CHANNEL_UP),
        .drpaddr_in(Conn14_DADDR),
        .drpclk_in(processing_system7_0_FCLK_CLK0),
        .drpdi_in(Conn14_DI),
        .drpdo_out(Conn14_DO),
        .drpen_in(Conn14_DEN),
        .drprdy_out(Conn14_DRDY),
        .drpwe_in(Conn14_DWE),
        .frame_err(Conn17_FRAME_ERR),
        .gt0_qplllock_in(aurora_8b10b_0_gt0_qplllock_out),
        .gt0_qpllrefclklost_in(aurora_8b10b_0_gt0_qpllrefclklost_out),
        .gt_qpllclk_quad1_in(aurora_8b10b_0_gt_qpllclk_quad1_out),
        .gt_qpllrefclk_quad1_in(aurora_8b10b_0_gt_qpllrefclk_quad1_out),
        .gt_refclk1(aurora_8b10b_0_gt_refclk1_out),
        .gt_reset(gt_reset_1),
        .hard_err(Conn17_HARD_ERR),
        .init_clk_in(processing_system7_0_FCLK_CLK0),
        .lane_up(Conn17_LANE_UP),
        .loopback(loopbackControl_dout),
        .m_axi_rx_tdata(Conn16_TDATA),
        .m_axi_rx_tkeep(Conn16_TKEEP),
        .m_axi_rx_tlast(Conn16_TLAST),
        .m_axi_rx_tvalid(Conn16_TVALID),
        .pll_not_locked(aurora_8b10b_0_pll_not_locked_out),
        .power_down(Zero_dout),
        .reset(reset_1),
        .rx_resetdone_out(Conn17_RX_RESETDONE_OUT),
        .rxn(Conn15_RXN),
        .rxp(Conn15_RXP),
        .s_axi_tx_tdata(Conn13_TDATA),
        .s_axi_tx_tkeep(Conn13_TKEEP),
        .s_axi_tx_tlast(Conn13_TLAST),
        .s_axi_tx_tready(Conn13_TREADY),
        .s_axi_tx_tvalid(Conn13_TVALID),
        .soft_err(Conn17_SOFT_ERR),
        .sync_clk(aurora_8b10b_0_sync_clk_out),
        .tx_lock(Conn17_TX_LOCK),
        .tx_resetdone_out(Conn17_TX_RESETDONE_OUT),
        .txn(Conn18_TXN),
        .txp(Conn18_TXP),
        .user_clk(aurora_8b10b_0_user_clk_out));
  base_zynq_design_xlconstant_0_0 loopbackControl
       (.dout(loopbackControl_dout));
endmodule

module AuroraB_imp_OB3XCY
   (CORE_STATUS0_channel_up,
    CORE_STATUS0_frame_err,
    CORE_STATUS0_hard_err,
    CORE_STATUS0_lane_up,
    CORE_STATUS0_rx_resetdone_out,
    CORE_STATUS0_soft_err,
    CORE_STATUS0_tx_lock,
    CORE_STATUS0_tx_resetdone_out,
    CORE_STATUS1_channel_up,
    CORE_STATUS1_frame_err,
    CORE_STATUS1_hard_err,
    CORE_STATUS1_lane_up,
    CORE_STATUS1_rx_resetdone_out,
    CORE_STATUS1_soft_err,
    CORE_STATUS1_tx_lock,
    CORE_STATUS1_tx_resetdone_out,
    CORE_STATUS2_channel_up,
    CORE_STATUS2_frame_err,
    CORE_STATUS2_hard_err,
    CORE_STATUS2_lane_up,
    CORE_STATUS2_rx_resetdone_out,
    CORE_STATUS2_soft_err,
    CORE_STATUS2_tx_lock,
    CORE_STATUS2_tx_resetdone_out,
    CORE_STATUS3_channel_up,
    CORE_STATUS3_frame_err,
    CORE_STATUS3_hard_err,
    CORE_STATUS3_lane_up,
    CORE_STATUS3_rx_resetdone_out,
    CORE_STATUS3_soft_err,
    CORE_STATUS3_tx_lock,
    CORE_STATUS3_tx_resetdone_out,
    CommonAuroraUserClk,
    GT0_DRP_IF0_daddr,
    GT0_DRP_IF0_den,
    GT0_DRP_IF0_di,
    GT0_DRP_IF0_do,
    GT0_DRP_IF0_drdy,
    GT0_DRP_IF0_dwe,
    GT0_DRP_IF1_daddr,
    GT0_DRP_IF1_den,
    GT0_DRP_IF1_di,
    GT0_DRP_IF1_do,
    GT0_DRP_IF1_drdy,
    GT0_DRP_IF1_dwe,
    GT0_DRP_IF2_daddr,
    GT0_DRP_IF2_den,
    GT0_DRP_IF2_di,
    GT0_DRP_IF2_do,
    GT0_DRP_IF2_drdy,
    GT0_DRP_IF2_dwe,
    GT0_DRP_IF3_daddr,
    GT0_DRP_IF3_den,
    GT0_DRP_IF3_di,
    GT0_DRP_IF3_do,
    GT0_DRP_IF3_drdy,
    GT0_DRP_IF3_dwe,
    GT_DIFF_REFCLK1_clk_n,
    GT_DIFF_REFCLK1_clk_p,
    GT_SERIAL_RX0_rxn,
    GT_SERIAL_RX0_rxp,
    GT_SERIAL_RX1_rxn,
    GT_SERIAL_RX1_rxp,
    GT_SERIAL_RX2_rxn,
    GT_SERIAL_RX2_rxp,
    GT_SERIAL_RX3_rxn,
    GT_SERIAL_RX3_rxp,
    GT_SERIAL_TX0_txn,
    GT_SERIAL_TX0_txp,
    GT_SERIAL_TX1_txn,
    GT_SERIAL_TX1_txp,
    GT_SERIAL_TX2_txn,
    GT_SERIAL_TX2_txp,
    GT_SERIAL_TX3_txn,
    GT_SERIAL_TX3_txp,
    USER_DATA_M_AXI_RX0_tdata,
    USER_DATA_M_AXI_RX0_tkeep,
    USER_DATA_M_AXI_RX0_tlast,
    USER_DATA_M_AXI_RX0_tready,
    USER_DATA_M_AXI_RX0_tvalid,
    USER_DATA_M_AXI_RX1_tdata,
    USER_DATA_M_AXI_RX1_tkeep,
    USER_DATA_M_AXI_RX1_tlast,
    USER_DATA_M_AXI_RX1_tready,
    USER_DATA_M_AXI_RX1_tvalid,
    USER_DATA_M_AXI_RX2_tdata,
    USER_DATA_M_AXI_RX2_tkeep,
    USER_DATA_M_AXI_RX2_tlast,
    USER_DATA_M_AXI_RX2_tready,
    USER_DATA_M_AXI_RX2_tvalid,
    USER_DATA_M_AXI_RX3_tdata,
    USER_DATA_M_AXI_RX3_tkeep,
    USER_DATA_M_AXI_RX3_tlast,
    USER_DATA_M_AXI_RX3_tready,
    USER_DATA_M_AXI_RX3_tvalid,
    USER_DATA_S_AXI_TX0_tdata,
    USER_DATA_S_AXI_TX0_tkeep,
    USER_DATA_S_AXI_TX0_tlast,
    USER_DATA_S_AXI_TX0_tready,
    USER_DATA_S_AXI_TX0_tvalid,
    USER_DATA_S_AXI_TX1_tdata,
    USER_DATA_S_AXI_TX1_tkeep,
    USER_DATA_S_AXI_TX1_tlast,
    USER_DATA_S_AXI_TX1_tready,
    USER_DATA_S_AXI_TX1_tvalid,
    USER_DATA_S_AXI_TX2_tdata,
    USER_DATA_S_AXI_TX2_tkeep,
    USER_DATA_S_AXI_TX2_tlast,
    USER_DATA_S_AXI_TX2_tready,
    USER_DATA_S_AXI_TX2_tvalid,
    USER_DATA_S_AXI_TX3_tdata,
    USER_DATA_S_AXI_TX3_tkeep,
    USER_DATA_S_AXI_TX3_tlast,
    USER_DATA_S_AXI_TX3_tready,
    USER_DATA_S_AXI_TX3_tvalid,
    auroraReset,
    gtxReset,
    init_clk_in);
  output CORE_STATUS0_channel_up;
  output CORE_STATUS0_frame_err;
  output CORE_STATUS0_hard_err;
  output [0:0]CORE_STATUS0_lane_up;
  output CORE_STATUS0_rx_resetdone_out;
  output CORE_STATUS0_soft_err;
  output CORE_STATUS0_tx_lock;
  output CORE_STATUS0_tx_resetdone_out;
  output CORE_STATUS1_channel_up;
  output CORE_STATUS1_frame_err;
  output CORE_STATUS1_hard_err;
  output [0:0]CORE_STATUS1_lane_up;
  output CORE_STATUS1_rx_resetdone_out;
  output CORE_STATUS1_soft_err;
  output CORE_STATUS1_tx_lock;
  output CORE_STATUS1_tx_resetdone_out;
  output CORE_STATUS2_channel_up;
  output CORE_STATUS2_frame_err;
  output CORE_STATUS2_hard_err;
  output [0:0]CORE_STATUS2_lane_up;
  output CORE_STATUS2_rx_resetdone_out;
  output CORE_STATUS2_soft_err;
  output CORE_STATUS2_tx_lock;
  output CORE_STATUS2_tx_resetdone_out;
  output CORE_STATUS3_channel_up;
  output CORE_STATUS3_frame_err;
  output CORE_STATUS3_hard_err;
  output [0:0]CORE_STATUS3_lane_up;
  output CORE_STATUS3_rx_resetdone_out;
  output CORE_STATUS3_soft_err;
  output CORE_STATUS3_tx_lock;
  output CORE_STATUS3_tx_resetdone_out;
  input CommonAuroraUserClk;
  input [8:0]GT0_DRP_IF0_daddr;
  input GT0_DRP_IF0_den;
  input [15:0]GT0_DRP_IF0_di;
  output [15:0]GT0_DRP_IF0_do;
  output GT0_DRP_IF0_drdy;
  input GT0_DRP_IF0_dwe;
  input [8:0]GT0_DRP_IF1_daddr;
  input GT0_DRP_IF1_den;
  input [15:0]GT0_DRP_IF1_di;
  output [15:0]GT0_DRP_IF1_do;
  output GT0_DRP_IF1_drdy;
  input GT0_DRP_IF1_dwe;
  input [8:0]GT0_DRP_IF2_daddr;
  input GT0_DRP_IF2_den;
  input [15:0]GT0_DRP_IF2_di;
  output [15:0]GT0_DRP_IF2_do;
  output GT0_DRP_IF2_drdy;
  input GT0_DRP_IF2_dwe;
  input [8:0]GT0_DRP_IF3_daddr;
  input GT0_DRP_IF3_den;
  input [15:0]GT0_DRP_IF3_di;
  output [15:0]GT0_DRP_IF3_do;
  output GT0_DRP_IF3_drdy;
  input GT0_DRP_IF3_dwe;
  input GT_DIFF_REFCLK1_clk_n;
  input GT_DIFF_REFCLK1_clk_p;
  input [0:0]GT_SERIAL_RX0_rxn;
  input [0:0]GT_SERIAL_RX0_rxp;
  input [0:0]GT_SERIAL_RX1_rxn;
  input [0:0]GT_SERIAL_RX1_rxp;
  input [0:0]GT_SERIAL_RX2_rxn;
  input [0:0]GT_SERIAL_RX2_rxp;
  input [0:0]GT_SERIAL_RX3_rxn;
  input [0:0]GT_SERIAL_RX3_rxp;
  output [0:0]GT_SERIAL_TX0_txn;
  output [0:0]GT_SERIAL_TX0_txp;
  output [0:0]GT_SERIAL_TX1_txn;
  output [0:0]GT_SERIAL_TX1_txp;
  output [0:0]GT_SERIAL_TX2_txn;
  output [0:0]GT_SERIAL_TX2_txp;
  output [0:0]GT_SERIAL_TX3_txn;
  output [0:0]GT_SERIAL_TX3_txp;
  output [15:0]USER_DATA_M_AXI_RX0_tdata;
  output [1:0]USER_DATA_M_AXI_RX0_tkeep;
  output USER_DATA_M_AXI_RX0_tlast;
  input USER_DATA_M_AXI_RX0_tready;
  output USER_DATA_M_AXI_RX0_tvalid;
  output [15:0]USER_DATA_M_AXI_RX1_tdata;
  output [1:0]USER_DATA_M_AXI_RX1_tkeep;
  output USER_DATA_M_AXI_RX1_tlast;
  input USER_DATA_M_AXI_RX1_tready;
  output USER_DATA_M_AXI_RX1_tvalid;
  output [15:0]USER_DATA_M_AXI_RX2_tdata;
  output [1:0]USER_DATA_M_AXI_RX2_tkeep;
  output USER_DATA_M_AXI_RX2_tlast;
  input USER_DATA_M_AXI_RX2_tready;
  output USER_DATA_M_AXI_RX2_tvalid;
  output [15:0]USER_DATA_M_AXI_RX3_tdata;
  output [1:0]USER_DATA_M_AXI_RX3_tkeep;
  output USER_DATA_M_AXI_RX3_tlast;
  input USER_DATA_M_AXI_RX3_tready;
  output USER_DATA_M_AXI_RX3_tvalid;
  input [0:15]USER_DATA_S_AXI_TX0_tdata;
  input [0:1]USER_DATA_S_AXI_TX0_tkeep;
  input USER_DATA_S_AXI_TX0_tlast;
  output USER_DATA_S_AXI_TX0_tready;
  input USER_DATA_S_AXI_TX0_tvalid;
  input [0:15]USER_DATA_S_AXI_TX1_tdata;
  input [0:1]USER_DATA_S_AXI_TX1_tkeep;
  input USER_DATA_S_AXI_TX1_tlast;
  output USER_DATA_S_AXI_TX1_tready;
  input USER_DATA_S_AXI_TX1_tvalid;
  input [0:15]USER_DATA_S_AXI_TX2_tdata;
  input [0:1]USER_DATA_S_AXI_TX2_tkeep;
  input USER_DATA_S_AXI_TX2_tlast;
  output USER_DATA_S_AXI_TX2_tready;
  input USER_DATA_S_AXI_TX2_tvalid;
  input [0:15]USER_DATA_S_AXI_TX3_tdata;
  input [0:1]USER_DATA_S_AXI_TX3_tkeep;
  input USER_DATA_S_AXI_TX3_tlast;
  output USER_DATA_S_AXI_TX3_tready;
  input USER_DATA_S_AXI_TX3_tvalid;
  input auroraReset;
  input gtxReset;
  input init_clk_in;

  wire ACLK_1;
  wire [0:0]ARESETN_1;
  wire Conn11_CHANNEL_UP;
  wire Conn11_FRAME_ERR;
  wire Conn11_HARD_ERR;
  wire [0:0]Conn11_LANE_UP;
  wire Conn11_RX_RESETDONE_OUT;
  wire Conn11_SOFT_ERR;
  wire Conn11_TX_LOCK;
  wire Conn11_TX_RESETDONE_OUT;
  wire [0:0]Conn12_TXN;
  wire [0:0]Conn12_TXP;
  wire [0:15]Conn13_TDATA;
  wire [0:1]Conn13_TKEEP;
  wire Conn13_TLAST;
  wire Conn13_TREADY;
  wire Conn13_TVALID;
  wire [8:0]Conn14_DADDR;
  wire Conn14_DEN;
  wire [15:0]Conn14_DI;
  wire [15:0]Conn14_DO;
  wire Conn14_DRDY;
  wire Conn14_DWE;
  wire [0:0]Conn15_RXN;
  wire [0:0]Conn15_RXP;
  wire Conn17_CHANNEL_UP;
  wire Conn17_FRAME_ERR;
  wire Conn17_HARD_ERR;
  wire [0:0]Conn17_LANE_UP;
  wire Conn17_RX_RESETDONE_OUT;
  wire Conn17_SOFT_ERR;
  wire Conn17_TX_LOCK;
  wire Conn17_TX_RESETDONE_OUT;
  wire [0:0]Conn18_TXN;
  wire [0:0]Conn18_TXP;
  wire Conn2_CHANNEL_UP;
  wire Conn2_FRAME_ERR;
  wire Conn2_HARD_ERR;
  wire [0:0]Conn2_LANE_UP;
  wire Conn2_RX_RESETDONE_OUT;
  wire Conn2_SOFT_ERR;
  wire Conn2_TX_LOCK;
  wire Conn2_TX_RESETDONE_OUT;
  wire [0:0]Conn3_TXN;
  wire [0:0]Conn3_TXP;
  wire [0:15]Conn4_TDATA;
  wire [0:1]Conn4_TKEEP;
  wire Conn4_TLAST;
  wire Conn4_TREADY;
  wire Conn4_TVALID;
  wire [8:0]Conn5_DADDR;
  wire Conn5_DEN;
  wire [15:0]Conn5_DI;
  wire [15:0]Conn5_DO;
  wire Conn5_DRDY;
  wire Conn5_DWE;
  wire [0:0]Conn6_RXN;
  wire [0:0]Conn6_RXP;
  wire [0:15]Conn7_TDATA;
  wire [0:1]Conn7_TKEEP;
  wire Conn7_TLAST;
  wire Conn7_TREADY;
  wire Conn7_TVALID;
  wire [8:0]Conn8_DADDR;
  wire Conn8_DEN;
  wire [15:0]Conn8_DI;
  wire [15:0]Conn8_DO;
  wire Conn8_DRDY;
  wire Conn8_DWE;
  wire [0:0]Conn9_RXN;
  wire [0:0]Conn9_RXP;
  wire GT_DIFF_REFCLK1_1_CLK_N;
  wire GT_DIFF_REFCLK1_1_CLK_P;
  wire [0:0]GT_SERIAL_RX_1_RXN;
  wire [0:0]GT_SERIAL_RX_1_RXP;
  wire [0:15]S00_AXIS_1_TDATA;
  wire [0:1]S00_AXIS_1_TKEEP;
  wire S00_AXIS_1_TLAST;
  wire S00_AXIS_1_TVALID;
  wire [0:15]S00_AXIS_2_TDATA;
  wire [0:1]S00_AXIS_2_TKEEP;
  wire S00_AXIS_2_TLAST;
  wire S00_AXIS_2_TVALID;
  wire [0:15]USER_DATA_S_AXI_TX_1_TDATA;
  wire [0:1]USER_DATA_S_AXI_TX_1_TKEEP;
  wire USER_DATA_S_AXI_TX_1_TLAST;
  wire USER_DATA_S_AXI_TX_1_TREADY;
  wire USER_DATA_S_AXI_TX_1_TVALID;
  wire [0:0]Zero_dout;
  wire aurora_8b10b_0_CORE_STATUS_CHANNEL_UP;
  wire aurora_8b10b_0_CORE_STATUS_FRAME_ERR;
  wire aurora_8b10b_0_CORE_STATUS_HARD_ERR;
  wire [0:0]aurora_8b10b_0_CORE_STATUS_LANE_UP;
  wire aurora_8b10b_0_CORE_STATUS_RX_RESETDONE_OUT;
  wire aurora_8b10b_0_CORE_STATUS_SOFT_ERR;
  wire aurora_8b10b_0_CORE_STATUS_TX_LOCK;
  wire aurora_8b10b_0_CORE_STATUS_TX_RESETDONE_OUT;
  wire [0:0]aurora_8b10b_0_GT_SERIAL_TX_TXN;
  wire [0:0]aurora_8b10b_0_GT_SERIAL_TX_TXP;
  wire aurora_8b10b_0_gt0_qplllock_out;
  wire aurora_8b10b_0_gt0_qpllrefclklost_out;
  wire aurora_8b10b_0_gt_qpllclk_quad1_out;
  wire aurora_8b10b_0_gt_qpllrefclk_quad1_out;
  wire aurora_8b10b_0_gt_refclk1_out;
  wire aurora_8b10b_0_pll_not_locked_out;
  wire aurora_8b10b_0_sync_clk_out;
  wire aurora_8b10b_0_user_clk_out;
  wire [0:15]aurora_8b10b_2_USER_DATA_M_AXI_RX_TDATA;
  wire [0:1]aurora_8b10b_2_USER_DATA_M_AXI_RX_TKEEP;
  wire aurora_8b10b_2_USER_DATA_M_AXI_RX_TLAST;
  wire aurora_8b10b_2_USER_DATA_M_AXI_RX_TVALID;
  wire [0:15]aurora_8b10b_3_USER_DATA_M_AXI_RX_TDATA;
  wire [0:1]aurora_8b10b_3_USER_DATA_M_AXI_RX_TKEEP;
  wire aurora_8b10b_3_USER_DATA_M_AXI_RX_TLAST;
  wire aurora_8b10b_3_USER_DATA_M_AXI_RX_TVALID;
  wire [15:0]axis_interconnect_0_M00_AXIS_TDATA;
  wire [1:0]axis_interconnect_0_M00_AXIS_TKEEP;
  wire axis_interconnect_0_M00_AXIS_TLAST;
  wire axis_interconnect_0_M00_AXIS_TREADY;
  wire axis_interconnect_0_M00_AXIS_TVALID;
  wire [15:0]axis_interconnect_1_M00_AXIS_TDATA;
  wire [1:0]axis_interconnect_1_M00_AXIS_TKEEP;
  wire axis_interconnect_1_M00_AXIS_TLAST;
  wire axis_interconnect_1_M00_AXIS_TREADY;
  wire axis_interconnect_1_M00_AXIS_TVALID;
  wire [15:0]axis_interconnect_2_M00_AXIS_TDATA;
  wire [1:0]axis_interconnect_2_M00_AXIS_TKEEP;
  wire axis_interconnect_2_M00_AXIS_TLAST;
  wire axis_interconnect_2_M00_AXIS_TREADY;
  wire axis_interconnect_2_M00_AXIS_TVALID;
  wire [15:0]axis_interconnect_3_M00_AXIS_TDATA;
  wire [1:0]axis_interconnect_3_M00_AXIS_TKEEP;
  wire axis_interconnect_3_M00_AXIS_TLAST;
  wire axis_interconnect_3_M00_AXIS_TREADY;
  wire axis_interconnect_3_M00_AXIS_TVALID;
  wire [8:0]drp_bridge_0_DRP1_DADDR;
  wire drp_bridge_0_DRP1_DEN;
  wire [15:0]drp_bridge_0_DRP1_DI;
  wire [15:0]drp_bridge_0_DRP1_DO;
  wire drp_bridge_0_DRP1_DRDY;
  wire drp_bridge_0_DRP1_DWE;
  wire gt_reset_1;
  wire [2:0]loopbackControl_dout;
  wire processing_system7_0_FCLK_CLK0;
  wire reset_1;

  assign ACLK_1 = CommonAuroraUserClk;
  assign CORE_STATUS0_channel_up = aurora_8b10b_0_CORE_STATUS_CHANNEL_UP;
  assign CORE_STATUS0_frame_err = aurora_8b10b_0_CORE_STATUS_FRAME_ERR;
  assign CORE_STATUS0_hard_err = aurora_8b10b_0_CORE_STATUS_HARD_ERR;
  assign CORE_STATUS0_lane_up[0] = aurora_8b10b_0_CORE_STATUS_LANE_UP;
  assign CORE_STATUS0_rx_resetdone_out = aurora_8b10b_0_CORE_STATUS_RX_RESETDONE_OUT;
  assign CORE_STATUS0_soft_err = aurora_8b10b_0_CORE_STATUS_SOFT_ERR;
  assign CORE_STATUS0_tx_lock = aurora_8b10b_0_CORE_STATUS_TX_LOCK;
  assign CORE_STATUS0_tx_resetdone_out = aurora_8b10b_0_CORE_STATUS_TX_RESETDONE_OUT;
  assign CORE_STATUS1_channel_up = Conn2_CHANNEL_UP;
  assign CORE_STATUS1_frame_err = Conn2_FRAME_ERR;
  assign CORE_STATUS1_hard_err = Conn2_HARD_ERR;
  assign CORE_STATUS1_lane_up[0] = Conn2_LANE_UP;
  assign CORE_STATUS1_rx_resetdone_out = Conn2_RX_RESETDONE_OUT;
  assign CORE_STATUS1_soft_err = Conn2_SOFT_ERR;
  assign CORE_STATUS1_tx_lock = Conn2_TX_LOCK;
  assign CORE_STATUS1_tx_resetdone_out = Conn2_TX_RESETDONE_OUT;
  assign CORE_STATUS2_channel_up = Conn11_CHANNEL_UP;
  assign CORE_STATUS2_frame_err = Conn11_FRAME_ERR;
  assign CORE_STATUS2_hard_err = Conn11_HARD_ERR;
  assign CORE_STATUS2_lane_up[0] = Conn11_LANE_UP;
  assign CORE_STATUS2_rx_resetdone_out = Conn11_RX_RESETDONE_OUT;
  assign CORE_STATUS2_soft_err = Conn11_SOFT_ERR;
  assign CORE_STATUS2_tx_lock = Conn11_TX_LOCK;
  assign CORE_STATUS2_tx_resetdone_out = Conn11_TX_RESETDONE_OUT;
  assign CORE_STATUS3_channel_up = Conn17_CHANNEL_UP;
  assign CORE_STATUS3_frame_err = Conn17_FRAME_ERR;
  assign CORE_STATUS3_hard_err = Conn17_HARD_ERR;
  assign CORE_STATUS3_lane_up[0] = Conn17_LANE_UP;
  assign CORE_STATUS3_rx_resetdone_out = Conn17_RX_RESETDONE_OUT;
  assign CORE_STATUS3_soft_err = Conn17_SOFT_ERR;
  assign CORE_STATUS3_tx_lock = Conn17_TX_LOCK;
  assign CORE_STATUS3_tx_resetdone_out = Conn17_TX_RESETDONE_OUT;
  assign Conn13_TDATA = USER_DATA_S_AXI_TX3_tdata[0:15];
  assign Conn13_TKEEP = USER_DATA_S_AXI_TX3_tkeep[0:1];
  assign Conn13_TLAST = USER_DATA_S_AXI_TX3_tlast;
  assign Conn13_TVALID = USER_DATA_S_AXI_TX3_tvalid;
  assign Conn14_DADDR = GT0_DRP_IF3_daddr[8:0];
  assign Conn14_DEN = GT0_DRP_IF3_den;
  assign Conn14_DI = GT0_DRP_IF3_di[15:0];
  assign Conn14_DWE = GT0_DRP_IF3_dwe;
  assign Conn15_RXN = GT_SERIAL_RX3_rxn[0];
  assign Conn15_RXP = GT_SERIAL_RX3_rxp[0];
  assign Conn4_TDATA = USER_DATA_S_AXI_TX1_tdata[0:15];
  assign Conn4_TKEEP = USER_DATA_S_AXI_TX1_tkeep[0:1];
  assign Conn4_TLAST = USER_DATA_S_AXI_TX1_tlast;
  assign Conn4_TVALID = USER_DATA_S_AXI_TX1_tvalid;
  assign Conn5_DADDR = GT0_DRP_IF1_daddr[8:0];
  assign Conn5_DEN = GT0_DRP_IF1_den;
  assign Conn5_DI = GT0_DRP_IF1_di[15:0];
  assign Conn5_DWE = GT0_DRP_IF1_dwe;
  assign Conn6_RXN = GT_SERIAL_RX1_rxn[0];
  assign Conn6_RXP = GT_SERIAL_RX1_rxp[0];
  assign Conn7_TDATA = USER_DATA_S_AXI_TX2_tdata[0:15];
  assign Conn7_TKEEP = USER_DATA_S_AXI_TX2_tkeep[0:1];
  assign Conn7_TLAST = USER_DATA_S_AXI_TX2_tlast;
  assign Conn7_TVALID = USER_DATA_S_AXI_TX2_tvalid;
  assign Conn8_DADDR = GT0_DRP_IF2_daddr[8:0];
  assign Conn8_DEN = GT0_DRP_IF2_den;
  assign Conn8_DI = GT0_DRP_IF2_di[15:0];
  assign Conn8_DWE = GT0_DRP_IF2_dwe;
  assign Conn9_RXN = GT_SERIAL_RX2_rxn[0];
  assign Conn9_RXP = GT_SERIAL_RX2_rxp[0];
  assign GT0_DRP_IF0_do[15:0] = drp_bridge_0_DRP1_DO;
  assign GT0_DRP_IF0_drdy = drp_bridge_0_DRP1_DRDY;
  assign GT0_DRP_IF1_do[15:0] = Conn5_DO;
  assign GT0_DRP_IF1_drdy = Conn5_DRDY;
  assign GT0_DRP_IF2_do[15:0] = Conn8_DO;
  assign GT0_DRP_IF2_drdy = Conn8_DRDY;
  assign GT0_DRP_IF3_do[15:0] = Conn14_DO;
  assign GT0_DRP_IF3_drdy = Conn14_DRDY;
  assign GT_DIFF_REFCLK1_1_CLK_N = GT_DIFF_REFCLK1_clk_n;
  assign GT_DIFF_REFCLK1_1_CLK_P = GT_DIFF_REFCLK1_clk_p;
  assign GT_SERIAL_RX_1_RXN = GT_SERIAL_RX0_rxn[0];
  assign GT_SERIAL_RX_1_RXP = GT_SERIAL_RX0_rxp[0];
  assign GT_SERIAL_TX0_txn[0] = aurora_8b10b_0_GT_SERIAL_TX_TXN;
  assign GT_SERIAL_TX0_txp[0] = aurora_8b10b_0_GT_SERIAL_TX_TXP;
  assign GT_SERIAL_TX1_txn[0] = Conn3_TXN;
  assign GT_SERIAL_TX1_txp[0] = Conn3_TXP;
  assign GT_SERIAL_TX2_txn[0] = Conn12_TXN;
  assign GT_SERIAL_TX2_txp[0] = Conn12_TXP;
  assign GT_SERIAL_TX3_txn[0] = Conn18_TXN;
  assign GT_SERIAL_TX3_txp[0] = Conn18_TXP;
  assign USER_DATA_M_AXI_RX0_tdata[15:0] = axis_interconnect_1_M00_AXIS_TDATA;
  assign USER_DATA_M_AXI_RX0_tkeep[1:0] = axis_interconnect_1_M00_AXIS_TKEEP;
  assign USER_DATA_M_AXI_RX0_tlast = axis_interconnect_1_M00_AXIS_TLAST;
  assign USER_DATA_M_AXI_RX0_tvalid = axis_interconnect_1_M00_AXIS_TVALID;
  assign USER_DATA_M_AXI_RX1_tdata[15:0] = axis_interconnect_2_M00_AXIS_TDATA;
  assign USER_DATA_M_AXI_RX1_tkeep[1:0] = axis_interconnect_2_M00_AXIS_TKEEP;
  assign USER_DATA_M_AXI_RX1_tlast = axis_interconnect_2_M00_AXIS_TLAST;
  assign USER_DATA_M_AXI_RX1_tvalid = axis_interconnect_2_M00_AXIS_TVALID;
  assign USER_DATA_M_AXI_RX2_tdata[15:0] = axis_interconnect_0_M00_AXIS_TDATA;
  assign USER_DATA_M_AXI_RX2_tkeep[1:0] = axis_interconnect_0_M00_AXIS_TKEEP;
  assign USER_DATA_M_AXI_RX2_tlast = axis_interconnect_0_M00_AXIS_TLAST;
  assign USER_DATA_M_AXI_RX2_tvalid = axis_interconnect_0_M00_AXIS_TVALID;
  assign USER_DATA_M_AXI_RX3_tdata[15:0] = axis_interconnect_3_M00_AXIS_TDATA;
  assign USER_DATA_M_AXI_RX3_tkeep[1:0] = axis_interconnect_3_M00_AXIS_TKEEP;
  assign USER_DATA_M_AXI_RX3_tlast = axis_interconnect_3_M00_AXIS_TLAST;
  assign USER_DATA_M_AXI_RX3_tvalid = axis_interconnect_3_M00_AXIS_TVALID;
  assign USER_DATA_S_AXI_TX0_tready = USER_DATA_S_AXI_TX_1_TREADY;
  assign USER_DATA_S_AXI_TX1_tready = Conn4_TREADY;
  assign USER_DATA_S_AXI_TX2_tready = Conn7_TREADY;
  assign USER_DATA_S_AXI_TX3_tready = Conn13_TREADY;
  assign USER_DATA_S_AXI_TX_1_TDATA = USER_DATA_S_AXI_TX0_tdata[0:15];
  assign USER_DATA_S_AXI_TX_1_TKEEP = USER_DATA_S_AXI_TX0_tkeep[0:1];
  assign USER_DATA_S_AXI_TX_1_TLAST = USER_DATA_S_AXI_TX0_tlast;
  assign USER_DATA_S_AXI_TX_1_TVALID = USER_DATA_S_AXI_TX0_tvalid;
  assign axis_interconnect_0_M00_AXIS_TREADY = USER_DATA_M_AXI_RX2_tready;
  assign axis_interconnect_1_M00_AXIS_TREADY = USER_DATA_M_AXI_RX0_tready;
  assign axis_interconnect_2_M00_AXIS_TREADY = USER_DATA_M_AXI_RX1_tready;
  assign axis_interconnect_3_M00_AXIS_TREADY = USER_DATA_M_AXI_RX3_tready;
  assign drp_bridge_0_DRP1_DADDR = GT0_DRP_IF0_daddr[8:0];
  assign drp_bridge_0_DRP1_DEN = GT0_DRP_IF0_den;
  assign drp_bridge_0_DRP1_DI = GT0_DRP_IF0_di[15:0];
  assign drp_bridge_0_DRP1_DWE = GT0_DRP_IF0_dwe;
  assign gt_reset_1 = gtxReset;
  assign processing_system7_0_FCLK_CLK0 = init_clk_in;
  assign reset_1 = auroraReset;
  base_zynq_design_xlconstant_0_3 One
       (.dout(ARESETN_1));
  base_zynq_design_Zero_0 Zero
       (.dout(Zero_dout));
  base_zynq_design_aurora_8b10b_0_1 aurora_8b10b_0
       (.channel_up(aurora_8b10b_0_CORE_STATUS_CHANNEL_UP),
        .drpaddr_in(drp_bridge_0_DRP1_DADDR),
        .drpclk_in(processing_system7_0_FCLK_CLK0),
        .drpdi_in(drp_bridge_0_DRP1_DI),
        .drpdo_out(drp_bridge_0_DRP1_DO),
        .drpen_in(drp_bridge_0_DRP1_DEN),
        .drprdy_out(drp_bridge_0_DRP1_DRDY),
        .drpwe_in(drp_bridge_0_DRP1_DWE),
        .frame_err(aurora_8b10b_0_CORE_STATUS_FRAME_ERR),
        .gt0_qplllock_out(aurora_8b10b_0_gt0_qplllock_out),
        .gt0_qpllrefclklost_out(aurora_8b10b_0_gt0_qpllrefclklost_out),
        .gt_qpllclk_quad1_out(aurora_8b10b_0_gt_qpllclk_quad1_out),
        .gt_qpllrefclk_quad1_out(aurora_8b10b_0_gt_qpllrefclk_quad1_out),
        .gt_refclk1_n(GT_DIFF_REFCLK1_1_CLK_N),
        .gt_refclk1_out(aurora_8b10b_0_gt_refclk1_out),
        .gt_refclk1_p(GT_DIFF_REFCLK1_1_CLK_P),
        .gt_reset(gt_reset_1),
        .hard_err(aurora_8b10b_0_CORE_STATUS_HARD_ERR),
        .init_clk_in(processing_system7_0_FCLK_CLK0),
        .lane_up(aurora_8b10b_0_CORE_STATUS_LANE_UP),
        .loopback(loopbackControl_dout),
        .m_axi_rx_tdata(S00_AXIS_1_TDATA),
        .m_axi_rx_tkeep(S00_AXIS_1_TKEEP),
        .m_axi_rx_tlast(S00_AXIS_1_TLAST),
        .m_axi_rx_tvalid(S00_AXIS_1_TVALID),
        .pll_not_locked_out(aurora_8b10b_0_pll_not_locked_out),
        .power_down(Zero_dout),
        .reset(reset_1),
        .rx_resetdone_out(aurora_8b10b_0_CORE_STATUS_RX_RESETDONE_OUT),
        .rxn(GT_SERIAL_RX_1_RXN),
        .rxp(GT_SERIAL_RX_1_RXP),
        .s_axi_tx_tdata(USER_DATA_S_AXI_TX_1_TDATA),
        .s_axi_tx_tkeep(USER_DATA_S_AXI_TX_1_TKEEP),
        .s_axi_tx_tlast(USER_DATA_S_AXI_TX_1_TLAST),
        .s_axi_tx_tready(USER_DATA_S_AXI_TX_1_TREADY),
        .s_axi_tx_tvalid(USER_DATA_S_AXI_TX_1_TVALID),
        .soft_err(aurora_8b10b_0_CORE_STATUS_SOFT_ERR),
        .sync_clk_out(aurora_8b10b_0_sync_clk_out),
        .tx_lock(aurora_8b10b_0_CORE_STATUS_TX_LOCK),
        .tx_resetdone_out(aurora_8b10b_0_CORE_STATUS_TX_RESETDONE_OUT),
        .txn(aurora_8b10b_0_GT_SERIAL_TX_TXN),
        .txp(aurora_8b10b_0_GT_SERIAL_TX_TXP),
        .user_clk_out(aurora_8b10b_0_user_clk_out));
  base_zynq_design_aurora_8b10b_1_3 aurora_8b10b_1
       (.channel_up(Conn2_CHANNEL_UP),
        .drpaddr_in(Conn5_DADDR),
        .drpclk_in(processing_system7_0_FCLK_CLK0),
        .drpdi_in(Conn5_DI),
        .drpdo_out(Conn5_DO),
        .drpen_in(Conn5_DEN),
        .drprdy_out(Conn5_DRDY),
        .drpwe_in(Conn5_DWE),
        .frame_err(Conn2_FRAME_ERR),
        .gt0_qplllock_in(aurora_8b10b_0_gt0_qplllock_out),
        .gt0_qpllrefclklost_in(aurora_8b10b_0_gt0_qpllrefclklost_out),
        .gt_qpllclk_quad1_in(aurora_8b10b_0_gt_qpllclk_quad1_out),
        .gt_qpllrefclk_quad1_in(aurora_8b10b_0_gt_qpllrefclk_quad1_out),
        .gt_refclk1(aurora_8b10b_0_gt_refclk1_out),
        .gt_reset(gt_reset_1),
        .hard_err(Conn2_HARD_ERR),
        .init_clk_in(processing_system7_0_FCLK_CLK0),
        .lane_up(Conn2_LANE_UP),
        .loopback(loopbackControl_dout),
        .m_axi_rx_tdata(S00_AXIS_2_TDATA),
        .m_axi_rx_tkeep(S00_AXIS_2_TKEEP),
        .m_axi_rx_tlast(S00_AXIS_2_TLAST),
        .m_axi_rx_tvalid(S00_AXIS_2_TVALID),
        .pll_not_locked(aurora_8b10b_0_pll_not_locked_out),
        .power_down(Zero_dout),
        .reset(reset_1),
        .rx_resetdone_out(Conn2_RX_RESETDONE_OUT),
        .rxn(Conn6_RXN),
        .rxp(Conn6_RXP),
        .s_axi_tx_tdata(Conn4_TDATA),
        .s_axi_tx_tkeep(Conn4_TKEEP),
        .s_axi_tx_tlast(Conn4_TLAST),
        .s_axi_tx_tready(Conn4_TREADY),
        .s_axi_tx_tvalid(Conn4_TVALID),
        .soft_err(Conn2_SOFT_ERR),
        .sync_clk(aurora_8b10b_0_sync_clk_out),
        .tx_lock(Conn2_TX_LOCK),
        .tx_resetdone_out(Conn2_TX_RESETDONE_OUT),
        .txn(Conn3_TXN),
        .txp(Conn3_TXP),
        .user_clk(aurora_8b10b_0_user_clk_out));
  base_zynq_design_aurora_8b10b_2_0 aurora_8b10b_2
       (.channel_up(Conn11_CHANNEL_UP),
        .drpaddr_in(Conn8_DADDR),
        .drpclk_in(processing_system7_0_FCLK_CLK0),
        .drpdi_in(Conn8_DI),
        .drpdo_out(Conn8_DO),
        .drpen_in(Conn8_DEN),
        .drprdy_out(Conn8_DRDY),
        .drpwe_in(Conn8_DWE),
        .frame_err(Conn11_FRAME_ERR),
        .gt0_qplllock_in(aurora_8b10b_0_gt0_qplllock_out),
        .gt0_qpllrefclklost_in(aurora_8b10b_0_gt0_qpllrefclklost_out),
        .gt_qpllclk_quad1_in(aurora_8b10b_0_gt_qpllclk_quad1_out),
        .gt_qpllrefclk_quad1_in(aurora_8b10b_0_gt_qpllrefclk_quad1_out),
        .gt_refclk1(aurora_8b10b_0_gt_refclk1_out),
        .gt_reset(gt_reset_1),
        .hard_err(Conn11_HARD_ERR),
        .init_clk_in(processing_system7_0_FCLK_CLK0),
        .lane_up(Conn11_LANE_UP),
        .loopback(loopbackControl_dout),
        .m_axi_rx_tdata(aurora_8b10b_2_USER_DATA_M_AXI_RX_TDATA),
        .m_axi_rx_tkeep(aurora_8b10b_2_USER_DATA_M_AXI_RX_TKEEP),
        .m_axi_rx_tlast(aurora_8b10b_2_USER_DATA_M_AXI_RX_TLAST),
        .m_axi_rx_tvalid(aurora_8b10b_2_USER_DATA_M_AXI_RX_TVALID),
        .pll_not_locked(aurora_8b10b_0_pll_not_locked_out),
        .power_down(Zero_dout),
        .reset(reset_1),
        .rx_resetdone_out(Conn11_RX_RESETDONE_OUT),
        .rxn(Conn9_RXN),
        .rxp(Conn9_RXP),
        .s_axi_tx_tdata(Conn7_TDATA),
        .s_axi_tx_tkeep(Conn7_TKEEP),
        .s_axi_tx_tlast(Conn7_TLAST),
        .s_axi_tx_tready(Conn7_TREADY),
        .s_axi_tx_tvalid(Conn7_TVALID),
        .soft_err(Conn11_SOFT_ERR),
        .sync_clk(aurora_8b10b_0_sync_clk_out),
        .tx_lock(Conn11_TX_LOCK),
        .tx_resetdone_out(Conn11_TX_RESETDONE_OUT),
        .txn(Conn12_TXN),
        .txp(Conn12_TXP),
        .user_clk(aurora_8b10b_0_user_clk_out));
  base_zynq_design_aurora_8b10b_3_0 aurora_8b10b_3
       (.channel_up(Conn17_CHANNEL_UP),
        .drpaddr_in(Conn14_DADDR),
        .drpclk_in(processing_system7_0_FCLK_CLK0),
        .drpdi_in(Conn14_DI),
        .drpdo_out(Conn14_DO),
        .drpen_in(Conn14_DEN),
        .drprdy_out(Conn14_DRDY),
        .drpwe_in(Conn14_DWE),
        .frame_err(Conn17_FRAME_ERR),
        .gt0_qplllock_in(aurora_8b10b_0_gt0_qplllock_out),
        .gt0_qpllrefclklost_in(aurora_8b10b_0_gt0_qpllrefclklost_out),
        .gt_qpllclk_quad1_in(aurora_8b10b_0_gt_qpllclk_quad1_out),
        .gt_qpllrefclk_quad1_in(aurora_8b10b_0_gt_qpllrefclk_quad1_out),
        .gt_refclk1(aurora_8b10b_0_gt_refclk1_out),
        .gt_reset(gt_reset_1),
        .hard_err(Conn17_HARD_ERR),
        .init_clk_in(processing_system7_0_FCLK_CLK0),
        .lane_up(Conn17_LANE_UP),
        .loopback(loopbackControl_dout),
        .m_axi_rx_tdata(aurora_8b10b_3_USER_DATA_M_AXI_RX_TDATA),
        .m_axi_rx_tkeep(aurora_8b10b_3_USER_DATA_M_AXI_RX_TKEEP),
        .m_axi_rx_tlast(aurora_8b10b_3_USER_DATA_M_AXI_RX_TLAST),
        .m_axi_rx_tvalid(aurora_8b10b_3_USER_DATA_M_AXI_RX_TVALID),
        .pll_not_locked(aurora_8b10b_0_pll_not_locked_out),
        .power_down(Zero_dout),
        .reset(reset_1),
        .rx_resetdone_out(Conn17_RX_RESETDONE_OUT),
        .rxn(Conn15_RXN),
        .rxp(Conn15_RXP),
        .s_axi_tx_tdata(Conn13_TDATA),
        .s_axi_tx_tkeep(Conn13_TKEEP),
        .s_axi_tx_tlast(Conn13_TLAST),
        .s_axi_tx_tready(Conn13_TREADY),
        .s_axi_tx_tvalid(Conn13_TVALID),
        .soft_err(Conn17_SOFT_ERR),
        .sync_clk(aurora_8b10b_0_sync_clk_out),
        .tx_lock(Conn17_TX_LOCK),
        .tx_resetdone_out(Conn17_TX_RESETDONE_OUT),
        .txn(Conn18_TXN),
        .txp(Conn18_TXP),
        .user_clk(aurora_8b10b_0_user_clk_out));
  base_zynq_design_axis_interconnect_0_2 axis_interconnect_0
       (.ACLK(ACLK_1),
        .ARESETN(ARESETN_1),
        .M00_AXIS_ACLK(ACLK_1),
        .M00_AXIS_ARESETN(ARESETN_1),
        .M00_AXIS_tdata(axis_interconnect_0_M00_AXIS_TDATA),
        .M00_AXIS_tkeep(axis_interconnect_0_M00_AXIS_TKEEP),
        .M00_AXIS_tlast(axis_interconnect_0_M00_AXIS_TLAST),
        .M00_AXIS_tready(axis_interconnect_0_M00_AXIS_TREADY),
        .M00_AXIS_tvalid(axis_interconnect_0_M00_AXIS_TVALID),
        .S00_AXIS_ACLK(aurora_8b10b_0_user_clk_out),
        .S00_AXIS_ARESETN(ARESETN_1),
        .S00_AXIS_tdata(aurora_8b10b_2_USER_DATA_M_AXI_RX_TDATA),
        .S00_AXIS_tkeep(aurora_8b10b_2_USER_DATA_M_AXI_RX_TKEEP),
        .S00_AXIS_tlast(aurora_8b10b_2_USER_DATA_M_AXI_RX_TLAST),
        .S00_AXIS_tvalid(aurora_8b10b_2_USER_DATA_M_AXI_RX_TVALID));
  base_zynq_design_axis_interconnect_0_3 axis_interconnect_1
       (.ACLK(ACLK_1),
        .ARESETN(ARESETN_1),
        .M00_AXIS_ACLK(ACLK_1),
        .M00_AXIS_ARESETN(ARESETN_1),
        .M00_AXIS_tdata(axis_interconnect_1_M00_AXIS_TDATA),
        .M00_AXIS_tkeep(axis_interconnect_1_M00_AXIS_TKEEP),
        .M00_AXIS_tlast(axis_interconnect_1_M00_AXIS_TLAST),
        .M00_AXIS_tready(axis_interconnect_1_M00_AXIS_TREADY),
        .M00_AXIS_tvalid(axis_interconnect_1_M00_AXIS_TVALID),
        .S00_AXIS_ACLK(aurora_8b10b_0_user_clk_out),
        .S00_AXIS_ARESETN(ARESETN_1),
        .S00_AXIS_tdata(S00_AXIS_1_TDATA),
        .S00_AXIS_tkeep(S00_AXIS_1_TKEEP),
        .S00_AXIS_tlast(S00_AXIS_1_TLAST),
        .S00_AXIS_tvalid(S00_AXIS_1_TVALID));
  base_zynq_design_axis_interconnect_0_4 axis_interconnect_2
       (.ACLK(ACLK_1),
        .ARESETN(ARESETN_1),
        .M00_AXIS_ACLK(ACLK_1),
        .M00_AXIS_ARESETN(ARESETN_1),
        .M00_AXIS_tdata(axis_interconnect_2_M00_AXIS_TDATA),
        .M00_AXIS_tkeep(axis_interconnect_2_M00_AXIS_TKEEP),
        .M00_AXIS_tlast(axis_interconnect_2_M00_AXIS_TLAST),
        .M00_AXIS_tready(axis_interconnect_2_M00_AXIS_TREADY),
        .M00_AXIS_tvalid(axis_interconnect_2_M00_AXIS_TVALID),
        .S00_AXIS_ACLK(aurora_8b10b_0_user_clk_out),
        .S00_AXIS_ARESETN(ARESETN_1),
        .S00_AXIS_tdata(S00_AXIS_2_TDATA),
        .S00_AXIS_tkeep(S00_AXIS_2_TKEEP),
        .S00_AXIS_tlast(S00_AXIS_2_TLAST),
        .S00_AXIS_tvalid(S00_AXIS_2_TVALID));
  base_zynq_design_axis_interconnect_0_5 axis_interconnect_3
       (.ACLK(ACLK_1),
        .ARESETN(ARESETN_1),
        .M00_AXIS_ACLK(ACLK_1),
        .M00_AXIS_ARESETN(ARESETN_1),
        .M00_AXIS_tdata(axis_interconnect_3_M00_AXIS_TDATA),
        .M00_AXIS_tkeep(axis_interconnect_3_M00_AXIS_TKEEP),
        .M00_AXIS_tlast(axis_interconnect_3_M00_AXIS_TLAST),
        .M00_AXIS_tready(axis_interconnect_3_M00_AXIS_TREADY),
        .M00_AXIS_tvalid(axis_interconnect_3_M00_AXIS_TVALID),
        .S00_AXIS_ACLK(aurora_8b10b_0_user_clk_out),
        .S00_AXIS_ARESETN(ARESETN_1),
        .S00_AXIS_tdata(aurora_8b10b_3_USER_DATA_M_AXI_RX_TDATA),
        .S00_AXIS_tkeep(aurora_8b10b_3_USER_DATA_M_AXI_RX_TKEEP),
        .S00_AXIS_tlast(aurora_8b10b_3_USER_DATA_M_AXI_RX_TLAST),
        .S00_AXIS_tvalid(aurora_8b10b_3_USER_DATA_M_AXI_RX_TVALID));
  base_zynq_design_loopbackControl_0 loopbackControl
       (.dout(loopbackControl_dout));
endmodule

module LogDMA_imp_GA7VJX
   (M_AXI_S2MM_awaddr,
    M_AXI_S2MM_awburst,
    M_AXI_S2MM_awcache,
    M_AXI_S2MM_awlen,
    M_AXI_S2MM_awlock,
    M_AXI_S2MM_awprot,
    M_AXI_S2MM_awqos,
    M_AXI_S2MM_awready,
    M_AXI_S2MM_awsize,
    M_AXI_S2MM_awvalid,
    M_AXI_S2MM_bready,
    M_AXI_S2MM_bresp,
    M_AXI_S2MM_bvalid,
    M_AXI_S2MM_wdata,
    M_AXI_S2MM_wlast,
    M_AXI_S2MM_wready,
    M_AXI_S2MM_wstrb,
    M_AXI_S2MM_wvalid,
    M_AXI_SG_araddr,
    M_AXI_SG_arburst,
    M_AXI_SG_arcache,
    M_AXI_SG_arlen,
    M_AXI_SG_arlock,
    M_AXI_SG_arprot,
    M_AXI_SG_arqos,
    M_AXI_SG_arready,
    M_AXI_SG_arsize,
    M_AXI_SG_arvalid,
    M_AXI_SG_awaddr,
    M_AXI_SG_awburst,
    M_AXI_SG_awcache,
    M_AXI_SG_awlen,
    M_AXI_SG_awlock,
    M_AXI_SG_awprot,
    M_AXI_SG_awqos,
    M_AXI_SG_awready,
    M_AXI_SG_awsize,
    M_AXI_SG_awvalid,
    M_AXI_SG_bready,
    M_AXI_SG_bresp,
    M_AXI_SG_bvalid,
    M_AXI_SG_rdata,
    M_AXI_SG_rlast,
    M_AXI_SG_rready,
    M_AXI_SG_rresp,
    M_AXI_SG_rvalid,
    M_AXI_SG_wdata,
    M_AXI_SG_wlast,
    M_AXI_SG_wready,
    M_AXI_SG_wstrb,
    M_AXI_SG_wvalid,
    S00_AXIS_tdata,
    S00_AXIS_tlast,
    S00_AXIS_tready,
    S00_AXIS_tvalid,
    S01_AXIS_tdata,
    S01_AXIS_tlast,
    S01_AXIS_tready,
    S01_AXIS_tvalid,
    S_AXI_LITE_araddr,
    S_AXI_LITE_arready,
    S_AXI_LITE_arvalid,
    S_AXI_LITE_awaddr,
    S_AXI_LITE_awready,
    S_AXI_LITE_awvalid,
    S_AXI_LITE_bready,
    S_AXI_LITE_bresp,
    S_AXI_LITE_bvalid,
    S_AXI_LITE_rdata,
    S_AXI_LITE_rready,
    S_AXI_LITE_rresp,
    S_AXI_LITE_rvalid,
    S_AXI_LITE_wdata,
    S_AXI_LITE_wready,
    S_AXI_LITE_wvalid,
    sys_ACLK,
    sys_ARESETN);
  output [31:0]M_AXI_S2MM_awaddr;
  output [1:0]M_AXI_S2MM_awburst;
  output [3:0]M_AXI_S2MM_awcache;
  output [3:0]M_AXI_S2MM_awlen;
  output [1:0]M_AXI_S2MM_awlock;
  output [2:0]M_AXI_S2MM_awprot;
  output [3:0]M_AXI_S2MM_awqos;
  input M_AXI_S2MM_awready;
  output [2:0]M_AXI_S2MM_awsize;
  output M_AXI_S2MM_awvalid;
  output M_AXI_S2MM_bready;
  input [1:0]M_AXI_S2MM_bresp;
  input M_AXI_S2MM_bvalid;
  output [63:0]M_AXI_S2MM_wdata;
  output M_AXI_S2MM_wlast;
  input M_AXI_S2MM_wready;
  output [7:0]M_AXI_S2MM_wstrb;
  output M_AXI_S2MM_wvalid;
  output [31:0]M_AXI_SG_araddr;
  output [1:0]M_AXI_SG_arburst;
  output [3:0]M_AXI_SG_arcache;
  output [3:0]M_AXI_SG_arlen;
  output [1:0]M_AXI_SG_arlock;
  output [2:0]M_AXI_SG_arprot;
  output [3:0]M_AXI_SG_arqos;
  input M_AXI_SG_arready;
  output [2:0]M_AXI_SG_arsize;
  output M_AXI_SG_arvalid;
  output [31:0]M_AXI_SG_awaddr;
  output [1:0]M_AXI_SG_awburst;
  output [3:0]M_AXI_SG_awcache;
  output [3:0]M_AXI_SG_awlen;
  output [1:0]M_AXI_SG_awlock;
  output [2:0]M_AXI_SG_awprot;
  output [3:0]M_AXI_SG_awqos;
  input M_AXI_SG_awready;
  output [2:0]M_AXI_SG_awsize;
  output M_AXI_SG_awvalid;
  output M_AXI_SG_bready;
  input [1:0]M_AXI_SG_bresp;
  input M_AXI_SG_bvalid;
  input [63:0]M_AXI_SG_rdata;
  input M_AXI_SG_rlast;
  output M_AXI_SG_rready;
  input [1:0]M_AXI_SG_rresp;
  input M_AXI_SG_rvalid;
  output [63:0]M_AXI_SG_wdata;
  output M_AXI_SG_wlast;
  input M_AXI_SG_wready;
  output [7:0]M_AXI_SG_wstrb;
  output M_AXI_SG_wvalid;
  input [31:0]S00_AXIS_tdata;
  input S00_AXIS_tlast;
  output S00_AXIS_tready;
  input S00_AXIS_tvalid;
  input [31:0]S01_AXIS_tdata;
  input S01_AXIS_tlast;
  output S01_AXIS_tready;
  input S01_AXIS_tvalid;
  input [31:0]S_AXI_LITE_araddr;
  output S_AXI_LITE_arready;
  input S_AXI_LITE_arvalid;
  input [31:0]S_AXI_LITE_awaddr;
  output S_AXI_LITE_awready;
  input S_AXI_LITE_awvalid;
  input S_AXI_LITE_bready;
  output [1:0]S_AXI_LITE_bresp;
  output S_AXI_LITE_bvalid;
  output [31:0]S_AXI_LITE_rdata;
  input S_AXI_LITE_rready;
  output [1:0]S_AXI_LITE_rresp;
  output S_AXI_LITE_rvalid;
  input [31:0]S_AXI_LITE_wdata;
  output S_AXI_LITE_wready;
  input S_AXI_LITE_wvalid;
  input sys_ACLK;
  input [0:0]sys_ARESETN;

  wire [31:0]Conn1_TDATA;
  wire Conn1_TLAST;
  wire Conn1_TREADY;
  wire Conn1_TVALID;
  wire [31:0]Conn2_TDATA;
  wire Conn2_TLAST;
  wire Conn2_TREADY;
  wire Conn2_TVALID;
  wire [0:0]S00_ARB_REQ_SUPPRESS_1;
  wire [31:0]S00_AXI_1_AWADDR;
  wire [1:0]S00_AXI_1_AWBURST;
  wire [3:0]S00_AXI_1_AWCACHE;
  wire [7:0]S00_AXI_1_AWLEN;
  wire [2:0]S00_AXI_1_AWPROT;
  wire S00_AXI_1_AWREADY;
  wire [2:0]S00_AXI_1_AWSIZE;
  wire S00_AXI_1_AWVALID;
  wire S00_AXI_1_BREADY;
  wire [1:0]S00_AXI_1_BRESP;
  wire S00_AXI_1_BVALID;
  wire [31:0]S00_AXI_1_WDATA;
  wire S00_AXI_1_WLAST;
  wire S00_AXI_1_WREADY;
  wire [3:0]S00_AXI_1_WSTRB;
  wire S00_AXI_1_WVALID;
  wire [31:0]S00_AXI_2_ARADDR;
  wire [1:0]S00_AXI_2_ARBURST;
  wire [3:0]S00_AXI_2_ARCACHE;
  wire [7:0]S00_AXI_2_ARLEN;
  wire [2:0]S00_AXI_2_ARPROT;
  wire S00_AXI_2_ARREADY;
  wire [2:0]S00_AXI_2_ARSIZE;
  wire S00_AXI_2_ARVALID;
  wire [31:0]S00_AXI_2_AWADDR;
  wire [1:0]S00_AXI_2_AWBURST;
  wire [3:0]S00_AXI_2_AWCACHE;
  wire [7:0]S00_AXI_2_AWLEN;
  wire [2:0]S00_AXI_2_AWPROT;
  wire S00_AXI_2_AWREADY;
  wire [2:0]S00_AXI_2_AWSIZE;
  wire S00_AXI_2_AWVALID;
  wire S00_AXI_2_BREADY;
  wire [1:0]S00_AXI_2_BRESP;
  wire S00_AXI_2_BVALID;
  wire [31:0]S00_AXI_2_RDATA;
  wire S00_AXI_2_RLAST;
  wire S00_AXI_2_RREADY;
  wire [1:0]S00_AXI_2_RRESP;
  wire S00_AXI_2_RVALID;
  wire [31:0]S00_AXI_2_WDATA;
  wire S00_AXI_2_WLAST;
  wire S00_AXI_2_WREADY;
  wire [3:0]S00_AXI_2_WSTRB;
  wire S00_AXI_2_WVALID;
  wire [31:0]axi_interconnect_0_M00_AXI_ARADDR;
  wire [1:0]axi_interconnect_0_M00_AXI_ARBURST;
  wire [3:0]axi_interconnect_0_M00_AXI_ARCACHE;
  wire [3:0]axi_interconnect_0_M00_AXI_ARLEN;
  wire [1:0]axi_interconnect_0_M00_AXI_ARLOCK;
  wire [2:0]axi_interconnect_0_M00_AXI_ARPROT;
  wire [3:0]axi_interconnect_0_M00_AXI_ARQOS;
  wire axi_interconnect_0_M00_AXI_ARREADY;
  wire [2:0]axi_interconnect_0_M00_AXI_ARSIZE;
  wire axi_interconnect_0_M00_AXI_ARVALID;
  wire [31:0]axi_interconnect_0_M00_AXI_AWADDR;
  wire [1:0]axi_interconnect_0_M00_AXI_AWBURST;
  wire [3:0]axi_interconnect_0_M00_AXI_AWCACHE;
  wire [3:0]axi_interconnect_0_M00_AXI_AWLEN;
  wire [1:0]axi_interconnect_0_M00_AXI_AWLOCK;
  wire [2:0]axi_interconnect_0_M00_AXI_AWPROT;
  wire [3:0]axi_interconnect_0_M00_AXI_AWQOS;
  wire axi_interconnect_0_M00_AXI_AWREADY;
  wire [2:0]axi_interconnect_0_M00_AXI_AWSIZE;
  wire axi_interconnect_0_M00_AXI_AWVALID;
  wire axi_interconnect_0_M00_AXI_BREADY;
  wire [1:0]axi_interconnect_0_M00_AXI_BRESP;
  wire axi_interconnect_0_M00_AXI_BVALID;
  wire [63:0]axi_interconnect_0_M00_AXI_RDATA;
  wire axi_interconnect_0_M00_AXI_RLAST;
  wire axi_interconnect_0_M00_AXI_RREADY;
  wire [1:0]axi_interconnect_0_M00_AXI_RRESP;
  wire axi_interconnect_0_M00_AXI_RVALID;
  wire [63:0]axi_interconnect_0_M00_AXI_WDATA;
  wire axi_interconnect_0_M00_AXI_WLAST;
  wire axi_interconnect_0_M00_AXI_WREADY;
  wire [7:0]axi_interconnect_0_M00_AXI_WSTRB;
  wire axi_interconnect_0_M00_AXI_WVALID;
  wire [31:0]axi_interconnect_1_M00_AXI_AWADDR;
  wire [1:0]axi_interconnect_1_M00_AXI_AWBURST;
  wire [3:0]axi_interconnect_1_M00_AXI_AWCACHE;
  wire [3:0]axi_interconnect_1_M00_AXI_AWLEN;
  wire [1:0]axi_interconnect_1_M00_AXI_AWLOCK;
  wire [2:0]axi_interconnect_1_M00_AXI_AWPROT;
  wire [3:0]axi_interconnect_1_M00_AXI_AWQOS;
  wire axi_interconnect_1_M00_AXI_AWREADY;
  wire [2:0]axi_interconnect_1_M00_AXI_AWSIZE;
  wire axi_interconnect_1_M00_AXI_AWVALID;
  wire axi_interconnect_1_M00_AXI_BREADY;
  wire [1:0]axi_interconnect_1_M00_AXI_BRESP;
  wire axi_interconnect_1_M00_AXI_BVALID;
  wire [63:0]axi_interconnect_1_M00_AXI_WDATA;
  wire axi_interconnect_1_M00_AXI_WLAST;
  wire axi_interconnect_1_M00_AXI_WREADY;
  wire [7:0]axi_interconnect_1_M00_AXI_WSTRB;
  wire axi_interconnect_1_M00_AXI_WVALID;
  wire [31:0]axis_interconnect_0_M00_AXIS_TDATA;
  wire [3:0]axis_interconnect_0_M00_AXIS_TKEEP;
  wire axis_interconnect_0_M00_AXIS_TLAST;
  wire axis_interconnect_0_M00_AXIS_TREADY;
  wire axis_interconnect_0_M00_AXIS_TVALID;
  wire processing_system7_0_FCLK_CLK0;
  wire [31:0]processing_system7_0_axi_periph_M10_AXI_ARADDR;
  wire processing_system7_0_axi_periph_M10_AXI_ARREADY;
  wire processing_system7_0_axi_periph_M10_AXI_ARVALID;
  wire [31:0]processing_system7_0_axi_periph_M10_AXI_AWADDR;
  wire processing_system7_0_axi_periph_M10_AXI_AWREADY;
  wire processing_system7_0_axi_periph_M10_AXI_AWVALID;
  wire processing_system7_0_axi_periph_M10_AXI_BREADY;
  wire [1:0]processing_system7_0_axi_periph_M10_AXI_BRESP;
  wire processing_system7_0_axi_periph_M10_AXI_BVALID;
  wire [31:0]processing_system7_0_axi_periph_M10_AXI_RDATA;
  wire processing_system7_0_axi_periph_M10_AXI_RREADY;
  wire [1:0]processing_system7_0_axi_periph_M10_AXI_RRESP;
  wire processing_system7_0_axi_periph_M10_AXI_RVALID;
  wire [31:0]processing_system7_0_axi_periph_M10_AXI_WDATA;
  wire processing_system7_0_axi_periph_M10_AXI_WREADY;
  wire processing_system7_0_axi_periph_M10_AXI_WVALID;
  wire [0:0]rst_processing_system7_0_50M_peripheral_aresetn;

  assign Conn1_TDATA = S00_AXIS_tdata[31:0];
  assign Conn1_TLAST = S00_AXIS_tlast;
  assign Conn1_TVALID = S00_AXIS_tvalid;
  assign Conn2_TDATA = S01_AXIS_tdata[31:0];
  assign Conn2_TLAST = S01_AXIS_tlast;
  assign Conn2_TVALID = S01_AXIS_tvalid;
  assign M_AXI_S2MM_awaddr[31:0] = axi_interconnect_1_M00_AXI_AWADDR;
  assign M_AXI_S2MM_awburst[1:0] = axi_interconnect_1_M00_AXI_AWBURST;
  assign M_AXI_S2MM_awcache[3:0] = axi_interconnect_1_M00_AXI_AWCACHE;
  assign M_AXI_S2MM_awlen[3:0] = axi_interconnect_1_M00_AXI_AWLEN;
  assign M_AXI_S2MM_awlock[1:0] = axi_interconnect_1_M00_AXI_AWLOCK;
  assign M_AXI_S2MM_awprot[2:0] = axi_interconnect_1_M00_AXI_AWPROT;
  assign M_AXI_S2MM_awqos[3:0] = axi_interconnect_1_M00_AXI_AWQOS;
  assign M_AXI_S2MM_awsize[2:0] = axi_interconnect_1_M00_AXI_AWSIZE;
  assign M_AXI_S2MM_awvalid = axi_interconnect_1_M00_AXI_AWVALID;
  assign M_AXI_S2MM_bready = axi_interconnect_1_M00_AXI_BREADY;
  assign M_AXI_S2MM_wdata[63:0] = axi_interconnect_1_M00_AXI_WDATA;
  assign M_AXI_S2MM_wlast = axi_interconnect_1_M00_AXI_WLAST;
  assign M_AXI_S2MM_wstrb[7:0] = axi_interconnect_1_M00_AXI_WSTRB;
  assign M_AXI_S2MM_wvalid = axi_interconnect_1_M00_AXI_WVALID;
  assign M_AXI_SG_araddr[31:0] = axi_interconnect_0_M00_AXI_ARADDR;
  assign M_AXI_SG_arburst[1:0] = axi_interconnect_0_M00_AXI_ARBURST;
  assign M_AXI_SG_arcache[3:0] = axi_interconnect_0_M00_AXI_ARCACHE;
  assign M_AXI_SG_arlen[3:0] = axi_interconnect_0_M00_AXI_ARLEN;
  assign M_AXI_SG_arlock[1:0] = axi_interconnect_0_M00_AXI_ARLOCK;
  assign M_AXI_SG_arprot[2:0] = axi_interconnect_0_M00_AXI_ARPROT;
  assign M_AXI_SG_arqos[3:0] = axi_interconnect_0_M00_AXI_ARQOS;
  assign M_AXI_SG_arsize[2:0] = axi_interconnect_0_M00_AXI_ARSIZE;
  assign M_AXI_SG_arvalid = axi_interconnect_0_M00_AXI_ARVALID;
  assign M_AXI_SG_awaddr[31:0] = axi_interconnect_0_M00_AXI_AWADDR;
  assign M_AXI_SG_awburst[1:0] = axi_interconnect_0_M00_AXI_AWBURST;
  assign M_AXI_SG_awcache[3:0] = axi_interconnect_0_M00_AXI_AWCACHE;
  assign M_AXI_SG_awlen[3:0] = axi_interconnect_0_M00_AXI_AWLEN;
  assign M_AXI_SG_awlock[1:0] = axi_interconnect_0_M00_AXI_AWLOCK;
  assign M_AXI_SG_awprot[2:0] = axi_interconnect_0_M00_AXI_AWPROT;
  assign M_AXI_SG_awqos[3:0] = axi_interconnect_0_M00_AXI_AWQOS;
  assign M_AXI_SG_awsize[2:0] = axi_interconnect_0_M00_AXI_AWSIZE;
  assign M_AXI_SG_awvalid = axi_interconnect_0_M00_AXI_AWVALID;
  assign M_AXI_SG_bready = axi_interconnect_0_M00_AXI_BREADY;
  assign M_AXI_SG_rready = axi_interconnect_0_M00_AXI_RREADY;
  assign M_AXI_SG_wdata[63:0] = axi_interconnect_0_M00_AXI_WDATA;
  assign M_AXI_SG_wlast = axi_interconnect_0_M00_AXI_WLAST;
  assign M_AXI_SG_wstrb[7:0] = axi_interconnect_0_M00_AXI_WSTRB;
  assign M_AXI_SG_wvalid = axi_interconnect_0_M00_AXI_WVALID;
  assign S00_AXIS_tready = Conn1_TREADY;
  assign S01_AXIS_tready = Conn2_TREADY;
  assign S_AXI_LITE_arready = processing_system7_0_axi_periph_M10_AXI_ARREADY;
  assign S_AXI_LITE_awready = processing_system7_0_axi_periph_M10_AXI_AWREADY;
  assign S_AXI_LITE_bresp[1:0] = processing_system7_0_axi_periph_M10_AXI_BRESP;
  assign S_AXI_LITE_bvalid = processing_system7_0_axi_periph_M10_AXI_BVALID;
  assign S_AXI_LITE_rdata[31:0] = processing_system7_0_axi_periph_M10_AXI_RDATA;
  assign S_AXI_LITE_rresp[1:0] = processing_system7_0_axi_periph_M10_AXI_RRESP;
  assign S_AXI_LITE_rvalid = processing_system7_0_axi_periph_M10_AXI_RVALID;
  assign S_AXI_LITE_wready = processing_system7_0_axi_periph_M10_AXI_WREADY;
  assign axi_interconnect_0_M00_AXI_ARREADY = M_AXI_SG_arready;
  assign axi_interconnect_0_M00_AXI_AWREADY = M_AXI_SG_awready;
  assign axi_interconnect_0_M00_AXI_BRESP = M_AXI_SG_bresp[1:0];
  assign axi_interconnect_0_M00_AXI_BVALID = M_AXI_SG_bvalid;
  assign axi_interconnect_0_M00_AXI_RDATA = M_AXI_SG_rdata[63:0];
  assign axi_interconnect_0_M00_AXI_RLAST = M_AXI_SG_rlast;
  assign axi_interconnect_0_M00_AXI_RRESP = M_AXI_SG_rresp[1:0];
  assign axi_interconnect_0_M00_AXI_RVALID = M_AXI_SG_rvalid;
  assign axi_interconnect_0_M00_AXI_WREADY = M_AXI_SG_wready;
  assign axi_interconnect_1_M00_AXI_AWREADY = M_AXI_S2MM_awready;
  assign axi_interconnect_1_M00_AXI_BRESP = M_AXI_S2MM_bresp[1:0];
  assign axi_interconnect_1_M00_AXI_BVALID = M_AXI_S2MM_bvalid;
  assign axi_interconnect_1_M00_AXI_WREADY = M_AXI_S2MM_wready;
  assign processing_system7_0_FCLK_CLK0 = sys_ACLK;
  assign processing_system7_0_axi_periph_M10_AXI_ARADDR = S_AXI_LITE_araddr[31:0];
  assign processing_system7_0_axi_periph_M10_AXI_ARVALID = S_AXI_LITE_arvalid;
  assign processing_system7_0_axi_periph_M10_AXI_AWADDR = S_AXI_LITE_awaddr[31:0];
  assign processing_system7_0_axi_periph_M10_AXI_AWVALID = S_AXI_LITE_awvalid;
  assign processing_system7_0_axi_periph_M10_AXI_BREADY = S_AXI_LITE_bready;
  assign processing_system7_0_axi_periph_M10_AXI_RREADY = S_AXI_LITE_rready;
  assign processing_system7_0_axi_periph_M10_AXI_WDATA = S_AXI_LITE_wdata[31:0];
  assign processing_system7_0_axi_periph_M10_AXI_WVALID = S_AXI_LITE_wvalid;
  assign rst_processing_system7_0_50M_peripheral_aresetn = sys_ARESETN[0];
  base_zynq_design_xlconstant_0_2 Zero
       (.dout(S00_ARB_REQ_SUPPRESS_1));
  base_zynq_design_axi_dma_0_0 axi_dma_0
       (.axi_resetn(rst_processing_system7_0_50M_peripheral_aresetn),
        .m_axi_s2mm_aclk(processing_system7_0_FCLK_CLK0),
        .m_axi_s2mm_awaddr(S00_AXI_1_AWADDR),
        .m_axi_s2mm_awburst(S00_AXI_1_AWBURST),
        .m_axi_s2mm_awcache(S00_AXI_1_AWCACHE),
        .m_axi_s2mm_awlen(S00_AXI_1_AWLEN),
        .m_axi_s2mm_awprot(S00_AXI_1_AWPROT),
        .m_axi_s2mm_awready(S00_AXI_1_AWREADY),
        .m_axi_s2mm_awsize(S00_AXI_1_AWSIZE),
        .m_axi_s2mm_awvalid(S00_AXI_1_AWVALID),
        .m_axi_s2mm_bready(S00_AXI_1_BREADY),
        .m_axi_s2mm_bresp(S00_AXI_1_BRESP),
        .m_axi_s2mm_bvalid(S00_AXI_1_BVALID),
        .m_axi_s2mm_wdata(S00_AXI_1_WDATA),
        .m_axi_s2mm_wlast(S00_AXI_1_WLAST),
        .m_axi_s2mm_wready(S00_AXI_1_WREADY),
        .m_axi_s2mm_wstrb(S00_AXI_1_WSTRB),
        .m_axi_s2mm_wvalid(S00_AXI_1_WVALID),
        .m_axi_sg_aclk(processing_system7_0_FCLK_CLK0),
        .m_axi_sg_araddr(S00_AXI_2_ARADDR),
        .m_axi_sg_arburst(S00_AXI_2_ARBURST),
        .m_axi_sg_arcache(S00_AXI_2_ARCACHE),
        .m_axi_sg_arlen(S00_AXI_2_ARLEN),
        .m_axi_sg_arprot(S00_AXI_2_ARPROT),
        .m_axi_sg_arready(S00_AXI_2_ARREADY),
        .m_axi_sg_arsize(S00_AXI_2_ARSIZE),
        .m_axi_sg_arvalid(S00_AXI_2_ARVALID),
        .m_axi_sg_awaddr(S00_AXI_2_AWADDR),
        .m_axi_sg_awburst(S00_AXI_2_AWBURST),
        .m_axi_sg_awcache(S00_AXI_2_AWCACHE),
        .m_axi_sg_awlen(S00_AXI_2_AWLEN),
        .m_axi_sg_awprot(S00_AXI_2_AWPROT),
        .m_axi_sg_awready(S00_AXI_2_AWREADY),
        .m_axi_sg_awsize(S00_AXI_2_AWSIZE),
        .m_axi_sg_awvalid(S00_AXI_2_AWVALID),
        .m_axi_sg_bready(S00_AXI_2_BREADY),
        .m_axi_sg_bresp(S00_AXI_2_BRESP),
        .m_axi_sg_bvalid(S00_AXI_2_BVALID),
        .m_axi_sg_rdata(S00_AXI_2_RDATA),
        .m_axi_sg_rlast(S00_AXI_2_RLAST),
        .m_axi_sg_rready(S00_AXI_2_RREADY),
        .m_axi_sg_rresp(S00_AXI_2_RRESP),
        .m_axi_sg_rvalid(S00_AXI_2_RVALID),
        .m_axi_sg_wdata(S00_AXI_2_WDATA),
        .m_axi_sg_wlast(S00_AXI_2_WLAST),
        .m_axi_sg_wready(S00_AXI_2_WREADY),
        .m_axi_sg_wstrb(S00_AXI_2_WSTRB),
        .m_axi_sg_wvalid(S00_AXI_2_WVALID),
        .s_axi_lite_aclk(processing_system7_0_FCLK_CLK0),
        .s_axi_lite_araddr(processing_system7_0_axi_periph_M10_AXI_ARADDR[9:0]),
        .s_axi_lite_arready(processing_system7_0_axi_periph_M10_AXI_ARREADY),
        .s_axi_lite_arvalid(processing_system7_0_axi_periph_M10_AXI_ARVALID),
        .s_axi_lite_awaddr(processing_system7_0_axi_periph_M10_AXI_AWADDR[9:0]),
        .s_axi_lite_awready(processing_system7_0_axi_periph_M10_AXI_AWREADY),
        .s_axi_lite_awvalid(processing_system7_0_axi_periph_M10_AXI_AWVALID),
        .s_axi_lite_bready(processing_system7_0_axi_periph_M10_AXI_BREADY),
        .s_axi_lite_bresp(processing_system7_0_axi_periph_M10_AXI_BRESP),
        .s_axi_lite_bvalid(processing_system7_0_axi_periph_M10_AXI_BVALID),
        .s_axi_lite_rdata(processing_system7_0_axi_periph_M10_AXI_RDATA),
        .s_axi_lite_rready(processing_system7_0_axi_periph_M10_AXI_RREADY),
        .s_axi_lite_rresp(processing_system7_0_axi_periph_M10_AXI_RRESP),
        .s_axi_lite_rvalid(processing_system7_0_axi_periph_M10_AXI_RVALID),
        .s_axi_lite_wdata(processing_system7_0_axi_periph_M10_AXI_WDATA),
        .s_axi_lite_wready(processing_system7_0_axi_periph_M10_AXI_WREADY),
        .s_axi_lite_wvalid(processing_system7_0_axi_periph_M10_AXI_WVALID),
        .s_axis_s2mm_tdata(axis_interconnect_0_M00_AXIS_TDATA),
        .s_axis_s2mm_tkeep(axis_interconnect_0_M00_AXIS_TKEEP),
        .s_axis_s2mm_tlast(axis_interconnect_0_M00_AXIS_TLAST),
        .s_axis_s2mm_tready(axis_interconnect_0_M00_AXIS_TREADY),
        .s_axis_s2mm_tvalid(axis_interconnect_0_M00_AXIS_TVALID));
  base_zynq_design_axi_interconnect_0_1 axi_interconnect_0
       (.ACLK(processing_system7_0_FCLK_CLK0),
        .ARESETN(rst_processing_system7_0_50M_peripheral_aresetn),
        .M00_ACLK(processing_system7_0_FCLK_CLK0),
        .M00_ARESETN(rst_processing_system7_0_50M_peripheral_aresetn),
        .M00_AXI_araddr(axi_interconnect_0_M00_AXI_ARADDR),
        .M00_AXI_arburst(axi_interconnect_0_M00_AXI_ARBURST),
        .M00_AXI_arcache(axi_interconnect_0_M00_AXI_ARCACHE),
        .M00_AXI_arlen(axi_interconnect_0_M00_AXI_ARLEN),
        .M00_AXI_arlock(axi_interconnect_0_M00_AXI_ARLOCK),
        .M00_AXI_arprot(axi_interconnect_0_M00_AXI_ARPROT),
        .M00_AXI_arqos(axi_interconnect_0_M00_AXI_ARQOS),
        .M00_AXI_arready(axi_interconnect_0_M00_AXI_ARREADY),
        .M00_AXI_arsize(axi_interconnect_0_M00_AXI_ARSIZE),
        .M00_AXI_arvalid(axi_interconnect_0_M00_AXI_ARVALID),
        .M00_AXI_awaddr(axi_interconnect_0_M00_AXI_AWADDR),
        .M00_AXI_awburst(axi_interconnect_0_M00_AXI_AWBURST),
        .M00_AXI_awcache(axi_interconnect_0_M00_AXI_AWCACHE),
        .M00_AXI_awlen(axi_interconnect_0_M00_AXI_AWLEN),
        .M00_AXI_awlock(axi_interconnect_0_M00_AXI_AWLOCK),
        .M00_AXI_awprot(axi_interconnect_0_M00_AXI_AWPROT),
        .M00_AXI_awqos(axi_interconnect_0_M00_AXI_AWQOS),
        .M00_AXI_awready(axi_interconnect_0_M00_AXI_AWREADY),
        .M00_AXI_awsize(axi_interconnect_0_M00_AXI_AWSIZE),
        .M00_AXI_awvalid(axi_interconnect_0_M00_AXI_AWVALID),
        .M00_AXI_bready(axi_interconnect_0_M00_AXI_BREADY),
        .M00_AXI_bresp(axi_interconnect_0_M00_AXI_BRESP),
        .M00_AXI_bvalid(axi_interconnect_0_M00_AXI_BVALID),
        .M00_AXI_rdata(axi_interconnect_0_M00_AXI_RDATA),
        .M00_AXI_rlast(axi_interconnect_0_M00_AXI_RLAST),
        .M00_AXI_rready(axi_interconnect_0_M00_AXI_RREADY),
        .M00_AXI_rresp(axi_interconnect_0_M00_AXI_RRESP),
        .M00_AXI_rvalid(axi_interconnect_0_M00_AXI_RVALID),
        .M00_AXI_wdata(axi_interconnect_0_M00_AXI_WDATA),
        .M00_AXI_wlast(axi_interconnect_0_M00_AXI_WLAST),
        .M00_AXI_wready(axi_interconnect_0_M00_AXI_WREADY),
        .M00_AXI_wstrb(axi_interconnect_0_M00_AXI_WSTRB),
        .M00_AXI_wvalid(axi_interconnect_0_M00_AXI_WVALID),
        .S00_ACLK(processing_system7_0_FCLK_CLK0),
        .S00_ARESETN(rst_processing_system7_0_50M_peripheral_aresetn),
        .S00_AXI_araddr(S00_AXI_2_ARADDR),
        .S00_AXI_arburst(S00_AXI_2_ARBURST),
        .S00_AXI_arcache(S00_AXI_2_ARCACHE),
        .S00_AXI_arlen(S00_AXI_2_ARLEN),
        .S00_AXI_arprot(S00_AXI_2_ARPROT),
        .S00_AXI_arready(S00_AXI_2_ARREADY),
        .S00_AXI_arsize(S00_AXI_2_ARSIZE),
        .S00_AXI_arvalid(S00_AXI_2_ARVALID),
        .S00_AXI_awaddr(S00_AXI_2_AWADDR),
        .S00_AXI_awburst(S00_AXI_2_AWBURST),
        .S00_AXI_awcache(S00_AXI_2_AWCACHE),
        .S00_AXI_awlen(S00_AXI_2_AWLEN),
        .S00_AXI_awprot(S00_AXI_2_AWPROT),
        .S00_AXI_awready(S00_AXI_2_AWREADY),
        .S00_AXI_awsize(S00_AXI_2_AWSIZE),
        .S00_AXI_awvalid(S00_AXI_2_AWVALID),
        .S00_AXI_bready(S00_AXI_2_BREADY),
        .S00_AXI_bresp(S00_AXI_2_BRESP),
        .S00_AXI_bvalid(S00_AXI_2_BVALID),
        .S00_AXI_rdata(S00_AXI_2_RDATA),
        .S00_AXI_rlast(S00_AXI_2_RLAST),
        .S00_AXI_rready(S00_AXI_2_RREADY),
        .S00_AXI_rresp(S00_AXI_2_RRESP),
        .S00_AXI_rvalid(S00_AXI_2_RVALID),
        .S00_AXI_wdata(S00_AXI_2_WDATA),
        .S00_AXI_wlast(S00_AXI_2_WLAST),
        .S00_AXI_wready(S00_AXI_2_WREADY),
        .S00_AXI_wstrb(S00_AXI_2_WSTRB),
        .S00_AXI_wvalid(S00_AXI_2_WVALID));
  base_zynq_design_axi_interconnect_0_0 axi_interconnect_1
       (.ACLK(processing_system7_0_FCLK_CLK0),
        .ARESETN(rst_processing_system7_0_50M_peripheral_aresetn),
        .M00_ACLK(processing_system7_0_FCLK_CLK0),
        .M00_ARESETN(rst_processing_system7_0_50M_peripheral_aresetn),
        .M00_AXI_awaddr(axi_interconnect_1_M00_AXI_AWADDR),
        .M00_AXI_awburst(axi_interconnect_1_M00_AXI_AWBURST),
        .M00_AXI_awcache(axi_interconnect_1_M00_AXI_AWCACHE),
        .M00_AXI_awlen(axi_interconnect_1_M00_AXI_AWLEN),
        .M00_AXI_awlock(axi_interconnect_1_M00_AXI_AWLOCK),
        .M00_AXI_awprot(axi_interconnect_1_M00_AXI_AWPROT),
        .M00_AXI_awqos(axi_interconnect_1_M00_AXI_AWQOS),
        .M00_AXI_awready(axi_interconnect_1_M00_AXI_AWREADY),
        .M00_AXI_awsize(axi_interconnect_1_M00_AXI_AWSIZE),
        .M00_AXI_awvalid(axi_interconnect_1_M00_AXI_AWVALID),
        .M00_AXI_bready(axi_interconnect_1_M00_AXI_BREADY),
        .M00_AXI_bresp(axi_interconnect_1_M00_AXI_BRESP),
        .M00_AXI_bvalid(axi_interconnect_1_M00_AXI_BVALID),
        .M00_AXI_wdata(axi_interconnect_1_M00_AXI_WDATA),
        .M00_AXI_wlast(axi_interconnect_1_M00_AXI_WLAST),
        .M00_AXI_wready(axi_interconnect_1_M00_AXI_WREADY),
        .M00_AXI_wstrb(axi_interconnect_1_M00_AXI_WSTRB),
        .M00_AXI_wvalid(axi_interconnect_1_M00_AXI_WVALID),
        .S00_ACLK(processing_system7_0_FCLK_CLK0),
        .S00_ARESETN(rst_processing_system7_0_50M_peripheral_aresetn),
        .S00_AXI_awaddr(S00_AXI_1_AWADDR),
        .S00_AXI_awburst(S00_AXI_1_AWBURST),
        .S00_AXI_awcache(S00_AXI_1_AWCACHE),
        .S00_AXI_awlen(S00_AXI_1_AWLEN),
        .S00_AXI_awprot(S00_AXI_1_AWPROT),
        .S00_AXI_awready(S00_AXI_1_AWREADY),
        .S00_AXI_awsize(S00_AXI_1_AWSIZE),
        .S00_AXI_awvalid(S00_AXI_1_AWVALID),
        .S00_AXI_bready(S00_AXI_1_BREADY),
        .S00_AXI_bresp(S00_AXI_1_BRESP),
        .S00_AXI_bvalid(S00_AXI_1_BVALID),
        .S00_AXI_wdata(S00_AXI_1_WDATA),
        .S00_AXI_wlast(S00_AXI_1_WLAST),
        .S00_AXI_wready(S00_AXI_1_WREADY),
        .S00_AXI_wstrb(S00_AXI_1_WSTRB),
        .S00_AXI_wvalid(S00_AXI_1_WVALID));
  base_zynq_design_axis_interconnect_0_0 axis_interconnect_0
       (.ACLK(processing_system7_0_FCLK_CLK0),
        .ARESETN(rst_processing_system7_0_50M_peripheral_aresetn),
        .M00_AXIS_ACLK(processing_system7_0_FCLK_CLK0),
        .M00_AXIS_ARESETN(rst_processing_system7_0_50M_peripheral_aresetn),
        .M00_AXIS_tdata(axis_interconnect_0_M00_AXIS_TDATA),
        .M00_AXIS_tkeep(axis_interconnect_0_M00_AXIS_TKEEP),
        .M00_AXIS_tlast(axis_interconnect_0_M00_AXIS_TLAST),
        .M00_AXIS_tready(axis_interconnect_0_M00_AXIS_TREADY),
        .M00_AXIS_tvalid(axis_interconnect_0_M00_AXIS_TVALID),
        .S00_ARB_REQ_SUPPRESS(S00_ARB_REQ_SUPPRESS_1),
        .S00_AXIS_ACLK(processing_system7_0_FCLK_CLK0),
        .S00_AXIS_ARESETN(rst_processing_system7_0_50M_peripheral_aresetn),
        .S00_AXIS_tdata(Conn1_TDATA),
        .S00_AXIS_tlast(Conn1_TLAST),
        .S00_AXIS_tready(Conn1_TREADY),
        .S00_AXIS_tvalid(Conn1_TVALID),
        .S01_ARB_REQ_SUPPRESS(S00_ARB_REQ_SUPPRESS_1),
        .S01_AXIS_ACLK(processing_system7_0_FCLK_CLK0),
        .S01_AXIS_ARESETN(rst_processing_system7_0_50M_peripheral_aresetn),
        .S01_AXIS_tdata(Conn2_TDATA),
        .S01_AXIS_tlast(Conn2_TLAST),
        .S01_AXIS_tready(Conn2_TREADY),
        .S01_AXIS_tvalid(Conn2_TVALID));
endmodule

(* CORE_GENERATION_INFO = "base_zynq_design,IP_Integrator,{x_ipVendor=xilinx.com,x_ipLibrary=BlockDiagram,x_ipName=base_zynq_design,x_ipVersion=1.00.a,x_ipLanguage=VERILOG,numBlks=86,numReposBlks=52,numNonXlnxBlks=0,numHierBlks=34,maxHierDepth=0,numSysgenBlks=0,numHlsBlks=0,numHdlrefBlks=0,numPkgbdBlks=0,bdsource=USER,da_axi4_cnt=34,da_board_cnt=2,da_bram_cntlr_cnt=4,da_ps7_cnt=3,synth_mode=Global}" *) (* HW_HANDOFF = "base_zynq_design.hwdef" *) 
module base_zynq_design
   (Aurora0_CORE_STATUS_channel_up,
    Aurora0_CORE_STATUS_frame_err,
    Aurora0_CORE_STATUS_hard_err,
    Aurora0_CORE_STATUS_lane_up,
    Aurora0_CORE_STATUS_rx_resetdone_out,
    Aurora0_CORE_STATUS_soft_err,
    Aurora0_CORE_STATUS_tx_lock,
    Aurora0_CORE_STATUS_tx_resetdone_out,
    Aurora0_RX_rxn,
    Aurora0_RX_rxp,
    Aurora0_TX_txn,
    Aurora0_TX_txp,
    Aurora0_USER_DATA_M_AXI_RX_tdata,
    Aurora0_USER_DATA_M_AXI_RX_tkeep,
    Aurora0_USER_DATA_M_AXI_RX_tlast,
    Aurora0_USER_DATA_M_AXI_RX_tvalid,
    Aurora0_USER_DATA_S_AXI_TX_tdata,
    Aurora0_USER_DATA_S_AXI_TX_tkeep,
    Aurora0_USER_DATA_S_AXI_TX_tlast,
    Aurora0_USER_DATA_S_AXI_TX_tready,
    Aurora0_USER_DATA_S_AXI_TX_tvalid,
    Aurora1_CORE_STATUS_channel_up,
    Aurora1_CORE_STATUS_frame_err,
    Aurora1_CORE_STATUS_hard_err,
    Aurora1_CORE_STATUS_lane_up,
    Aurora1_CORE_STATUS_rx_resetdone_out,
    Aurora1_CORE_STATUS_soft_err,
    Aurora1_CORE_STATUS_tx_lock,
    Aurora1_CORE_STATUS_tx_resetdone_out,
    Aurora1_RX_rxn,
    Aurora1_RX_rxp,
    Aurora1_TX_txn,
    Aurora1_TX_txp,
    Aurora1_USER_DATA_M_AXI_RX_tdata,
    Aurora1_USER_DATA_M_AXI_RX_tkeep,
    Aurora1_USER_DATA_M_AXI_RX_tlast,
    Aurora1_USER_DATA_M_AXI_RX_tvalid,
    Aurora1_USER_DATA_S_AXI_TX_tdata,
    Aurora1_USER_DATA_S_AXI_TX_tkeep,
    Aurora1_USER_DATA_S_AXI_TX_tlast,
    Aurora1_USER_DATA_S_AXI_TX_tready,
    Aurora1_USER_DATA_S_AXI_TX_tvalid,
    Aurora2_CORE_STATUS_channel_up,
    Aurora2_CORE_STATUS_frame_err,
    Aurora2_CORE_STATUS_hard_err,
    Aurora2_CORE_STATUS_lane_up,
    Aurora2_CORE_STATUS_rx_resetdone_out,
    Aurora2_CORE_STATUS_soft_err,
    Aurora2_CORE_STATUS_tx_lock,
    Aurora2_CORE_STATUS_tx_resetdone_out,
    Aurora2_RX_rxn,
    Aurora2_RX_rxp,
    Aurora2_TX_txn,
    Aurora2_TX_txp,
    Aurora2_USER_DATA_M_AXI_RX_tdata,
    Aurora2_USER_DATA_M_AXI_RX_tkeep,
    Aurora2_USER_DATA_M_AXI_RX_tlast,
    Aurora2_USER_DATA_M_AXI_RX_tvalid,
    Aurora2_USER_DATA_S_AXI_TX_tdata,
    Aurora2_USER_DATA_S_AXI_TX_tkeep,
    Aurora2_USER_DATA_S_AXI_TX_tlast,
    Aurora2_USER_DATA_S_AXI_TX_tready,
    Aurora2_USER_DATA_S_AXI_TX_tvalid,
    Aurora3_CORE_STATUS_channel_up,
    Aurora3_CORE_STATUS_frame_err,
    Aurora3_CORE_STATUS_hard_err,
    Aurora3_CORE_STATUS_lane_up,
    Aurora3_CORE_STATUS_rx_resetdone_out,
    Aurora3_CORE_STATUS_soft_err,
    Aurora3_CORE_STATUS_tx_lock,
    Aurora3_CORE_STATUS_tx_resetdone_out,
    Aurora3_RX_rxn,
    Aurora3_RX_rxp,
    Aurora3_TX_txn,
    Aurora3_TX_txp,
    Aurora3_USER_DATA_M_AXI_RX_tdata,
    Aurora3_USER_DATA_M_AXI_RX_tkeep,
    Aurora3_USER_DATA_M_AXI_RX_tlast,
    Aurora3_USER_DATA_M_AXI_RX_tvalid,
    Aurora3_USER_DATA_S_AXI_TX_tdata,
    Aurora3_USER_DATA_S_AXI_TX_tkeep,
    Aurora3_USER_DATA_S_AXI_TX_tlast,
    Aurora3_USER_DATA_S_AXI_TX_tready,
    Aurora3_USER_DATA_S_AXI_TX_tvalid,
    Aurora4_CORE_STATUS_channel_up,
    Aurora4_CORE_STATUS_frame_err,
    Aurora4_CORE_STATUS_hard_err,
    Aurora4_CORE_STATUS_lane_up,
    Aurora4_CORE_STATUS_rx_resetdone_out,
    Aurora4_CORE_STATUS_soft_err,
    Aurora4_CORE_STATUS_tx_lock,
    Aurora4_CORE_STATUS_tx_resetdone_out,
    Aurora4_RX_rxn,
    Aurora4_RX_rxp,
    Aurora4_TX_txn,
    Aurora4_TX_txp,
    Aurora4_USER_DATA_M_AXI_RX_tdata,
    Aurora4_USER_DATA_M_AXI_RX_tkeep,
    Aurora4_USER_DATA_M_AXI_RX_tlast,
    Aurora4_USER_DATA_M_AXI_RX_tready,
    Aurora4_USER_DATA_M_AXI_RX_tvalid,
    Aurora4_USER_DATA_S_AXI_TX_tdata,
    Aurora4_USER_DATA_S_AXI_TX_tkeep,
    Aurora4_USER_DATA_S_AXI_TX_tlast,
    Aurora4_USER_DATA_S_AXI_TX_tready,
    Aurora4_USER_DATA_S_AXI_TX_tvalid,
    Aurora5_CORE_STATUS_channel_up,
    Aurora5_CORE_STATUS_frame_err,
    Aurora5_CORE_STATUS_hard_err,
    Aurora5_CORE_STATUS_lane_up,
    Aurora5_CORE_STATUS_rx_resetdone_out,
    Aurora5_CORE_STATUS_soft_err,
    Aurora5_CORE_STATUS_tx_lock,
    Aurora5_CORE_STATUS_tx_resetdone_out,
    Aurora5_RX_rxn,
    Aurora5_RX_rxp,
    Aurora5_TX_txn,
    Aurora5_TX_txp,
    Aurora5_USER_DATA_M_AXI_RX_tdata,
    Aurora5_USER_DATA_M_AXI_RX_tkeep,
    Aurora5_USER_DATA_M_AXI_RX_tlast,
    Aurora5_USER_DATA_M_AXI_RX_tready,
    Aurora5_USER_DATA_M_AXI_RX_tvalid,
    Aurora5_USER_DATA_S_AXI_TX_tdata,
    Aurora5_USER_DATA_S_AXI_TX_tkeep,
    Aurora5_USER_DATA_S_AXI_TX_tlast,
    Aurora5_USER_DATA_S_AXI_TX_tready,
    Aurora5_USER_DATA_S_AXI_TX_tvalid,
    Aurora6_CORE_STATUS_channel_up,
    Aurora6_CORE_STATUS_frame_err,
    Aurora6_CORE_STATUS_hard_err,
    Aurora6_CORE_STATUS_lane_up,
    Aurora6_CORE_STATUS_rx_resetdone_out,
    Aurora6_CORE_STATUS_soft_err,
    Aurora6_CORE_STATUS_tx_lock,
    Aurora6_CORE_STATUS_tx_resetdone_out,
    Aurora6_RX_rxn,
    Aurora6_RX_rxp,
    Aurora6_TX_txn,
    Aurora6_TX_txp,
    Aurora6_USER_DATA_M_AXI_RX_tdata,
    Aurora6_USER_DATA_M_AXI_RX_tkeep,
    Aurora6_USER_DATA_M_AXI_RX_tlast,
    Aurora6_USER_DATA_M_AXI_RX_tready,
    Aurora6_USER_DATA_M_AXI_RX_tvalid,
    Aurora6_USER_DATA_S_AXI_TX_tdata,
    Aurora6_USER_DATA_S_AXI_TX_tkeep,
    Aurora6_USER_DATA_S_AXI_TX_tlast,
    Aurora6_USER_DATA_S_AXI_TX_tready,
    Aurora6_USER_DATA_S_AXI_TX_tvalid,
    Aurora7_CORE_STATUS_channel_up,
    Aurora7_CORE_STATUS_frame_err,
    Aurora7_CORE_STATUS_hard_err,
    Aurora7_CORE_STATUS_lane_up,
    Aurora7_CORE_STATUS_rx_resetdone_out,
    Aurora7_CORE_STATUS_soft_err,
    Aurora7_CORE_STATUS_tx_lock,
    Aurora7_CORE_STATUS_tx_resetdone_out,
    Aurora7_RX_rxn,
    Aurora7_RX_rxp,
    Aurora7_TX_txn,
    Aurora7_TX_txp,
    Aurora7_USER_DATA_M_AXI_RX_tdata,
    Aurora7_USER_DATA_M_AXI_RX_tkeep,
    Aurora7_USER_DATA_M_AXI_RX_tlast,
    Aurora7_USER_DATA_M_AXI_RX_tready,
    Aurora7_USER_DATA_M_AXI_RX_tvalid,
    Aurora7_USER_DATA_S_AXI_TX_tdata,
    Aurora7_USER_DATA_S_AXI_TX_tkeep,
    Aurora7_USER_DATA_S_AXI_TX_tlast,
    Aurora7_USER_DATA_S_AXI_TX_tready,
    Aurora7_USER_DATA_S_AXI_TX_tvalid,
    AuroraArefclk_clk_n,
    AuroraArefclk_clk_p,
    AuroraBrefclk_clk_n,
    AuroraBrefclk_clk_p,
    AuroraReset,
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
    DRP0_daddr,
    DRP0_den,
    DRP0_di,
    DRP0_do,
    DRP0_drdy,
    DRP0_dwe,
    FIXED_IO_ddr_vrn,
    FIXED_IO_ddr_vrp,
    FIXED_IO_mio,
    FIXED_IO_ps_clk,
    FIXED_IO_ps_porb,
    FIXED_IO_ps_srstb,
    GPIO_AURORA_A_tri_i,
    GPIO_AURORA_A_tri_o,
    GPIO_AURORA_A_tri_t,
    GPIO_AURORA_B_tri_i,
    GPIO_EVR_GTX_tri_i,
    GPIO_EVR_GTX_tri_o,
    GPIO_EVR_GTX_tri_t,
    GPIO_FAN_SPEED_tri_i,
    GPIO_FIRMWARE_DATE_tri_i,
    GPIO_MITIGATION_CONTROL_tri_o,
    GPIO_SFP_STATUS_tri_i,
    GPIO_STALE_NODES_HI_tri_i,
    GPIO_STALE_NODES_LO_tri_i,
    GPIO_SYSMON_READOUT_tri_i,
    GPIO_SYSMON_READOUT_tri_o,
    GPIO_SYSMON_READOUT_tri_t,
    GPIO_TIMESTAMP_SECONDS_tri_i,
    GPIO_TIMESTAMP_TICKS_tri_i,
    GTXreset,
    Log_AXIS_tdata,
    Log_AXIS_tlast,
    Log_AXIS_tready,
    Log_AXIS_tvalid,
    Marker_AXIS_tdata,
    Marker_AXIS_tlast,
    Marker_AXIS_tready,
    Marker_AXIS_tvalid,
    SFP_IIC_SELECT,
    SFP_IIC_scl_i,
    SFP_IIC_scl_o,
    SFP_IIC_scl_t,
    SFP_IIC_sda_i,
    SFP_IIC_sda_o,
    SFP_IIC_sda_t,
    auroraUserClk,
    mitigationBRAMctrl_addr,
    mitigationBRAMctrl_clk,
    mitigationBRAMctrl_din,
    mitigationBRAMctrl_dout,
    mitigationBRAMctrl_en,
    mitigationBRAMctrl_rst,
    mitigationBRAMctrl_we,
    mitigationClk,
    sysClk,
    sysReset_n);
  output Aurora0_CORE_STATUS_channel_up;
  output Aurora0_CORE_STATUS_frame_err;
  output Aurora0_CORE_STATUS_hard_err;
  output [0:0]Aurora0_CORE_STATUS_lane_up;
  output Aurora0_CORE_STATUS_rx_resetdone_out;
  output Aurora0_CORE_STATUS_soft_err;
  output Aurora0_CORE_STATUS_tx_lock;
  output Aurora0_CORE_STATUS_tx_resetdone_out;
  input [0:0]Aurora0_RX_rxn;
  input [0:0]Aurora0_RX_rxp;
  output [0:0]Aurora0_TX_txn;
  output [0:0]Aurora0_TX_txp;
  output [0:15]Aurora0_USER_DATA_M_AXI_RX_tdata;
  output [0:1]Aurora0_USER_DATA_M_AXI_RX_tkeep;
  output Aurora0_USER_DATA_M_AXI_RX_tlast;
  output Aurora0_USER_DATA_M_AXI_RX_tvalid;
  input [0:15]Aurora0_USER_DATA_S_AXI_TX_tdata;
  input [0:1]Aurora0_USER_DATA_S_AXI_TX_tkeep;
  input Aurora0_USER_DATA_S_AXI_TX_tlast;
  output Aurora0_USER_DATA_S_AXI_TX_tready;
  input Aurora0_USER_DATA_S_AXI_TX_tvalid;
  output Aurora1_CORE_STATUS_channel_up;
  output Aurora1_CORE_STATUS_frame_err;
  output Aurora1_CORE_STATUS_hard_err;
  output [0:0]Aurora1_CORE_STATUS_lane_up;
  output Aurora1_CORE_STATUS_rx_resetdone_out;
  output Aurora1_CORE_STATUS_soft_err;
  output Aurora1_CORE_STATUS_tx_lock;
  output Aurora1_CORE_STATUS_tx_resetdone_out;
  input [0:0]Aurora1_RX_rxn;
  input [0:0]Aurora1_RX_rxp;
  output [0:0]Aurora1_TX_txn;
  output [0:0]Aurora1_TX_txp;
  output [0:15]Aurora1_USER_DATA_M_AXI_RX_tdata;
  output [0:1]Aurora1_USER_DATA_M_AXI_RX_tkeep;
  output Aurora1_USER_DATA_M_AXI_RX_tlast;
  output Aurora1_USER_DATA_M_AXI_RX_tvalid;
  input [0:15]Aurora1_USER_DATA_S_AXI_TX_tdata;
  input [0:1]Aurora1_USER_DATA_S_AXI_TX_tkeep;
  input Aurora1_USER_DATA_S_AXI_TX_tlast;
  output Aurora1_USER_DATA_S_AXI_TX_tready;
  input Aurora1_USER_DATA_S_AXI_TX_tvalid;
  output Aurora2_CORE_STATUS_channel_up;
  output Aurora2_CORE_STATUS_frame_err;
  output Aurora2_CORE_STATUS_hard_err;
  output [0:0]Aurora2_CORE_STATUS_lane_up;
  output Aurora2_CORE_STATUS_rx_resetdone_out;
  output Aurora2_CORE_STATUS_soft_err;
  output Aurora2_CORE_STATUS_tx_lock;
  output Aurora2_CORE_STATUS_tx_resetdone_out;
  input [0:0]Aurora2_RX_rxn;
  input [0:0]Aurora2_RX_rxp;
  output [0:0]Aurora2_TX_txn;
  output [0:0]Aurora2_TX_txp;
  output [0:15]Aurora2_USER_DATA_M_AXI_RX_tdata;
  output [0:1]Aurora2_USER_DATA_M_AXI_RX_tkeep;
  output Aurora2_USER_DATA_M_AXI_RX_tlast;
  output Aurora2_USER_DATA_M_AXI_RX_tvalid;
  input [0:15]Aurora2_USER_DATA_S_AXI_TX_tdata;
  input [0:1]Aurora2_USER_DATA_S_AXI_TX_tkeep;
  input Aurora2_USER_DATA_S_AXI_TX_tlast;
  output Aurora2_USER_DATA_S_AXI_TX_tready;
  input Aurora2_USER_DATA_S_AXI_TX_tvalid;
  output Aurora3_CORE_STATUS_channel_up;
  output Aurora3_CORE_STATUS_frame_err;
  output Aurora3_CORE_STATUS_hard_err;
  output [0:0]Aurora3_CORE_STATUS_lane_up;
  output Aurora3_CORE_STATUS_rx_resetdone_out;
  output Aurora3_CORE_STATUS_soft_err;
  output Aurora3_CORE_STATUS_tx_lock;
  output Aurora3_CORE_STATUS_tx_resetdone_out;
  input [0:0]Aurora3_RX_rxn;
  input [0:0]Aurora3_RX_rxp;
  output [0:0]Aurora3_TX_txn;
  output [0:0]Aurora3_TX_txp;
  output [0:15]Aurora3_USER_DATA_M_AXI_RX_tdata;
  output [0:1]Aurora3_USER_DATA_M_AXI_RX_tkeep;
  output Aurora3_USER_DATA_M_AXI_RX_tlast;
  output Aurora3_USER_DATA_M_AXI_RX_tvalid;
  input [0:15]Aurora3_USER_DATA_S_AXI_TX_tdata;
  input [0:1]Aurora3_USER_DATA_S_AXI_TX_tkeep;
  input Aurora3_USER_DATA_S_AXI_TX_tlast;
  output Aurora3_USER_DATA_S_AXI_TX_tready;
  input Aurora3_USER_DATA_S_AXI_TX_tvalid;
  output Aurora4_CORE_STATUS_channel_up;
  output Aurora4_CORE_STATUS_frame_err;
  output Aurora4_CORE_STATUS_hard_err;
  output [0:0]Aurora4_CORE_STATUS_lane_up;
  output Aurora4_CORE_STATUS_rx_resetdone_out;
  output Aurora4_CORE_STATUS_soft_err;
  output Aurora4_CORE_STATUS_tx_lock;
  output Aurora4_CORE_STATUS_tx_resetdone_out;
  input [0:0]Aurora4_RX_rxn;
  input [0:0]Aurora4_RX_rxp;
  output [0:0]Aurora4_TX_txn;
  output [0:0]Aurora4_TX_txp;
  output [15:0]Aurora4_USER_DATA_M_AXI_RX_tdata;
  output [1:0]Aurora4_USER_DATA_M_AXI_RX_tkeep;
  output Aurora4_USER_DATA_M_AXI_RX_tlast;
  input Aurora4_USER_DATA_M_AXI_RX_tready;
  output Aurora4_USER_DATA_M_AXI_RX_tvalid;
  input [0:15]Aurora4_USER_DATA_S_AXI_TX_tdata;
  input [0:1]Aurora4_USER_DATA_S_AXI_TX_tkeep;
  input Aurora4_USER_DATA_S_AXI_TX_tlast;
  output Aurora4_USER_DATA_S_AXI_TX_tready;
  input Aurora4_USER_DATA_S_AXI_TX_tvalid;
  output Aurora5_CORE_STATUS_channel_up;
  output Aurora5_CORE_STATUS_frame_err;
  output Aurora5_CORE_STATUS_hard_err;
  output [0:0]Aurora5_CORE_STATUS_lane_up;
  output Aurora5_CORE_STATUS_rx_resetdone_out;
  output Aurora5_CORE_STATUS_soft_err;
  output Aurora5_CORE_STATUS_tx_lock;
  output Aurora5_CORE_STATUS_tx_resetdone_out;
  input [0:0]Aurora5_RX_rxn;
  input [0:0]Aurora5_RX_rxp;
  output [0:0]Aurora5_TX_txn;
  output [0:0]Aurora5_TX_txp;
  output [15:0]Aurora5_USER_DATA_M_AXI_RX_tdata;
  output [1:0]Aurora5_USER_DATA_M_AXI_RX_tkeep;
  output Aurora5_USER_DATA_M_AXI_RX_tlast;
  input Aurora5_USER_DATA_M_AXI_RX_tready;
  output Aurora5_USER_DATA_M_AXI_RX_tvalid;
  input [0:15]Aurora5_USER_DATA_S_AXI_TX_tdata;
  input [0:1]Aurora5_USER_DATA_S_AXI_TX_tkeep;
  input Aurora5_USER_DATA_S_AXI_TX_tlast;
  output Aurora5_USER_DATA_S_AXI_TX_tready;
  input Aurora5_USER_DATA_S_AXI_TX_tvalid;
  output Aurora6_CORE_STATUS_channel_up;
  output Aurora6_CORE_STATUS_frame_err;
  output Aurora6_CORE_STATUS_hard_err;
  output [0:0]Aurora6_CORE_STATUS_lane_up;
  output Aurora6_CORE_STATUS_rx_resetdone_out;
  output Aurora6_CORE_STATUS_soft_err;
  output Aurora6_CORE_STATUS_tx_lock;
  output Aurora6_CORE_STATUS_tx_resetdone_out;
  input [0:0]Aurora6_RX_rxn;
  input [0:0]Aurora6_RX_rxp;
  output [0:0]Aurora6_TX_txn;
  output [0:0]Aurora6_TX_txp;
  output [15:0]Aurora6_USER_DATA_M_AXI_RX_tdata;
  output [1:0]Aurora6_USER_DATA_M_AXI_RX_tkeep;
  output Aurora6_USER_DATA_M_AXI_RX_tlast;
  input Aurora6_USER_DATA_M_AXI_RX_tready;
  output Aurora6_USER_DATA_M_AXI_RX_tvalid;
  input [0:15]Aurora6_USER_DATA_S_AXI_TX_tdata;
  input [0:1]Aurora6_USER_DATA_S_AXI_TX_tkeep;
  input Aurora6_USER_DATA_S_AXI_TX_tlast;
  output Aurora6_USER_DATA_S_AXI_TX_tready;
  input Aurora6_USER_DATA_S_AXI_TX_tvalid;
  output Aurora7_CORE_STATUS_channel_up;
  output Aurora7_CORE_STATUS_frame_err;
  output Aurora7_CORE_STATUS_hard_err;
  output [0:0]Aurora7_CORE_STATUS_lane_up;
  output Aurora7_CORE_STATUS_rx_resetdone_out;
  output Aurora7_CORE_STATUS_soft_err;
  output Aurora7_CORE_STATUS_tx_lock;
  output Aurora7_CORE_STATUS_tx_resetdone_out;
  input [0:0]Aurora7_RX_rxn;
  input [0:0]Aurora7_RX_rxp;
  output [0:0]Aurora7_TX_txn;
  output [0:0]Aurora7_TX_txp;
  output [15:0]Aurora7_USER_DATA_M_AXI_RX_tdata;
  output [1:0]Aurora7_USER_DATA_M_AXI_RX_tkeep;
  output Aurora7_USER_DATA_M_AXI_RX_tlast;
  input Aurora7_USER_DATA_M_AXI_RX_tready;
  output Aurora7_USER_DATA_M_AXI_RX_tvalid;
  input [0:15]Aurora7_USER_DATA_S_AXI_TX_tdata;
  input [0:1]Aurora7_USER_DATA_S_AXI_TX_tkeep;
  input Aurora7_USER_DATA_S_AXI_TX_tlast;
  output Aurora7_USER_DATA_S_AXI_TX_tready;
  input Aurora7_USER_DATA_S_AXI_TX_tvalid;
  input AuroraArefclk_clk_n;
  input AuroraArefclk_clk_p;
  input AuroraBrefclk_clk_n;
  input AuroraBrefclk_clk_p;
  input AuroraReset;
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
  output [8:0]DRP0_daddr;
  output DRP0_den;
  output [15:0]DRP0_di;
  input [15:0]DRP0_do;
  input DRP0_drdy;
  output DRP0_dwe;
  inout FIXED_IO_ddr_vrn;
  inout FIXED_IO_ddr_vrp;
  inout [53:0]FIXED_IO_mio;
  inout FIXED_IO_ps_clk;
  inout FIXED_IO_ps_porb;
  inout FIXED_IO_ps_srstb;
  input [31:0]GPIO_AURORA_A_tri_i;
  output [31:0]GPIO_AURORA_A_tri_o;
  output [31:0]GPIO_AURORA_A_tri_t;
  input [31:0]GPIO_AURORA_B_tri_i;
  input [3:0]GPIO_EVR_GTX_tri_i;
  output [3:0]GPIO_EVR_GTX_tri_o;
  output [3:0]GPIO_EVR_GTX_tri_t;
  input [31:0]GPIO_FAN_SPEED_tri_i;
  input [31:0]GPIO_FIRMWARE_DATE_tri_i;
  output [31:0]GPIO_MITIGATION_CONTROL_tri_o;
  input [31:0]GPIO_SFP_STATUS_tri_i;
  input [31:0]GPIO_STALE_NODES_HI_tri_i;
  input [31:0]GPIO_STALE_NODES_LO_tri_i;
  input [15:0]GPIO_SYSMON_READOUT_tri_i;
  output [15:0]GPIO_SYSMON_READOUT_tri_o;
  output [15:0]GPIO_SYSMON_READOUT_tri_t;
  input [31:0]GPIO_TIMESTAMP_SECONDS_tri_i;
  input [31:0]GPIO_TIMESTAMP_TICKS_tri_i;
  input GTXreset;
  input [31:0]Log_AXIS_tdata;
  input Log_AXIS_tlast;
  output Log_AXIS_tready;
  input Log_AXIS_tvalid;
  input [31:0]Marker_AXIS_tdata;
  input Marker_AXIS_tlast;
  output Marker_AXIS_tready;
  input Marker_AXIS_tvalid;
  output [2:0]SFP_IIC_SELECT;
  input SFP_IIC_scl_i;
  output SFP_IIC_scl_o;
  output SFP_IIC_scl_t;
  input SFP_IIC_sda_i;
  output SFP_IIC_sda_o;
  output SFP_IIC_sda_t;
  output auroraUserClk;
  output [15:0]mitigationBRAMctrl_addr;
  output mitigationBRAMctrl_clk;
  output [31:0]mitigationBRAMctrl_din;
  input [31:0]mitigationBRAMctrl_dout;
  output mitigationBRAMctrl_en;
  output mitigationBRAMctrl_rst;
  output [3:0]mitigationBRAMctrl_we;
  output mitigationClk;
  output sysClk;
  output [0:0]sysReset_n;

  wire AuroraB_CORE_STATUS0_CHANNEL_UP;
  wire AuroraB_CORE_STATUS0_FRAME_ERR;
  wire AuroraB_CORE_STATUS0_HARD_ERR;
  wire [0:0]AuroraB_CORE_STATUS0_LANE_UP;
  wire AuroraB_CORE_STATUS0_RX_RESETDONE_OUT;
  wire AuroraB_CORE_STATUS0_SOFT_ERR;
  wire AuroraB_CORE_STATUS0_TX_LOCK;
  wire AuroraB_CORE_STATUS0_TX_RESETDONE_OUT;
  wire AuroraB_CORE_STATUS1_CHANNEL_UP;
  wire AuroraB_CORE_STATUS1_FRAME_ERR;
  wire AuroraB_CORE_STATUS1_HARD_ERR;
  wire [0:0]AuroraB_CORE_STATUS1_LANE_UP;
  wire AuroraB_CORE_STATUS1_RX_RESETDONE_OUT;
  wire AuroraB_CORE_STATUS1_SOFT_ERR;
  wire AuroraB_CORE_STATUS1_TX_LOCK;
  wire AuroraB_CORE_STATUS1_TX_RESETDONE_OUT;
  wire AuroraB_CORE_STATUS2_CHANNEL_UP;
  wire AuroraB_CORE_STATUS2_FRAME_ERR;
  wire AuroraB_CORE_STATUS2_HARD_ERR;
  wire [0:0]AuroraB_CORE_STATUS2_LANE_UP;
  wire AuroraB_CORE_STATUS2_RX_RESETDONE_OUT;
  wire AuroraB_CORE_STATUS2_SOFT_ERR;
  wire AuroraB_CORE_STATUS2_TX_LOCK;
  wire AuroraB_CORE_STATUS2_TX_RESETDONE_OUT;
  wire AuroraB_CORE_STATUS3_CHANNEL_UP;
  wire AuroraB_CORE_STATUS3_FRAME_ERR;
  wire AuroraB_CORE_STATUS3_HARD_ERR;
  wire [0:0]AuroraB_CORE_STATUS3_LANE_UP;
  wire AuroraB_CORE_STATUS3_RX_RESETDONE_OUT;
  wire AuroraB_CORE_STATUS3_SOFT_ERR;
  wire AuroraB_CORE_STATUS3_TX_LOCK;
  wire AuroraB_CORE_STATUS3_TX_RESETDONE_OUT;
  wire [0:0]AuroraB_GT_SERIAL_TX0_TXN;
  wire [0:0]AuroraB_GT_SERIAL_TX0_TXP;
  wire [0:0]AuroraB_GT_SERIAL_TX1_TXN;
  wire [0:0]AuroraB_GT_SERIAL_TX1_TXP;
  wire [0:0]AuroraB_GT_SERIAL_TX2_TXN;
  wire [0:0]AuroraB_GT_SERIAL_TX2_TXP;
  wire [0:0]AuroraB_GT_SERIAL_TX3_TXN;
  wire [0:0]AuroraB_GT_SERIAL_TX3_TXP;
  wire [15:0]AuroraB_USER_DATA_M_AXI_RX0_TDATA;
  wire [1:0]AuroraB_USER_DATA_M_AXI_RX0_TKEEP;
  wire AuroraB_USER_DATA_M_AXI_RX0_TLAST;
  wire AuroraB_USER_DATA_M_AXI_RX0_TREADY;
  wire AuroraB_USER_DATA_M_AXI_RX0_TVALID;
  wire [15:0]AuroraB_USER_DATA_M_AXI_RX1_TDATA;
  wire [1:0]AuroraB_USER_DATA_M_AXI_RX1_TKEEP;
  wire AuroraB_USER_DATA_M_AXI_RX1_TLAST;
  wire AuroraB_USER_DATA_M_AXI_RX1_TREADY;
  wire AuroraB_USER_DATA_M_AXI_RX1_TVALID;
  wire [15:0]AuroraB_USER_DATA_M_AXI_RX2_TDATA;
  wire [1:0]AuroraB_USER_DATA_M_AXI_RX2_TKEEP;
  wire AuroraB_USER_DATA_M_AXI_RX2_TLAST;
  wire AuroraB_USER_DATA_M_AXI_RX2_TREADY;
  wire AuroraB_USER_DATA_M_AXI_RX2_TVALID;
  wire [15:0]AuroraB_USER_DATA_M_AXI_RX3_TDATA;
  wire [1:0]AuroraB_USER_DATA_M_AXI_RX3_TKEEP;
  wire AuroraB_USER_DATA_M_AXI_RX3_TLAST;
  wire AuroraB_USER_DATA_M_AXI_RX3_TREADY;
  wire AuroraB_USER_DATA_M_AXI_RX3_TVALID;
  wire Aurora_CORE_STATUS1_CHANNEL_UP;
  wire Aurora_CORE_STATUS1_FRAME_ERR;
  wire Aurora_CORE_STATUS1_HARD_ERR;
  wire [0:0]Aurora_CORE_STATUS1_LANE_UP;
  wire Aurora_CORE_STATUS1_RX_RESETDONE_OUT;
  wire Aurora_CORE_STATUS1_SOFT_ERR;
  wire Aurora_CORE_STATUS1_TX_LOCK;
  wire Aurora_CORE_STATUS1_TX_RESETDONE_OUT;
  wire Aurora_CORE_STATUS2_CHANNEL_UP;
  wire Aurora_CORE_STATUS2_FRAME_ERR;
  wire Aurora_CORE_STATUS2_HARD_ERR;
  wire [0:0]Aurora_CORE_STATUS2_LANE_UP;
  wire Aurora_CORE_STATUS2_RX_RESETDONE_OUT;
  wire Aurora_CORE_STATUS2_SOFT_ERR;
  wire Aurora_CORE_STATUS2_TX_LOCK;
  wire Aurora_CORE_STATUS2_TX_RESETDONE_OUT;
  wire Aurora_CORE_STATUS_CHANNEL_UP;
  wire Aurora_CORE_STATUS_FRAME_ERR;
  wire Aurora_CORE_STATUS_HARD_ERR;
  wire [0:0]Aurora_CORE_STATUS_LANE_UP;
  wire Aurora_CORE_STATUS_RX_RESETDONE_OUT;
  wire Aurora_CORE_STATUS_SOFT_ERR;
  wire Aurora_CORE_STATUS_TX_LOCK;
  wire Aurora_CORE_STATUS_TX_RESETDONE_OUT;
  wire [0:0]Aurora_GT_SERIAL_TX1_TXN;
  wire [0:0]Aurora_GT_SERIAL_TX1_TXP;
  wire [0:0]Aurora_GT_SERIAL_TX2_TXN;
  wire [0:0]Aurora_GT_SERIAL_TX2_TXP;
  wire [0:0]Aurora_GT_SERIAL_TX_TXN;
  wire [0:0]Aurora_GT_SERIAL_TX_TXP;
  wire [0:15]Aurora_USER_DATA_M_AXI_RX1_TDATA;
  wire [0:1]Aurora_USER_DATA_M_AXI_RX1_TKEEP;
  wire Aurora_USER_DATA_M_AXI_RX1_TLAST;
  wire Aurora_USER_DATA_M_AXI_RX1_TVALID;
  wire [0:15]Aurora_USER_DATA_M_AXI_RX2_TDATA;
  wire [0:1]Aurora_USER_DATA_M_AXI_RX2_TKEEP;
  wire Aurora_USER_DATA_M_AXI_RX2_TLAST;
  wire Aurora_USER_DATA_M_AXI_RX2_TVALID;
  wire [0:15]Aurora_USER_DATA_M_AXI_RX_TDATA;
  wire [0:1]Aurora_USER_DATA_M_AXI_RX_TKEEP;
  wire Aurora_USER_DATA_M_AXI_RX_TLAST;
  wire Aurora_USER_DATA_M_AXI_RX_TVALID;
  wire Aurora_user_clk_out;
  wire [31:0]GPIO_AURORA_GPIO2_TRI_I;
  wire [31:0]GPIO_AURORA_GPIO_TRI_I;
  wire [31:0]GPIO_AURORA_GPIO_TRI_O;
  wire [31:0]GPIO_AURORA_GPIO_TRI_T;
  wire [3:0]GPIO_EVR_GTX_GPIO_TRI_I;
  wire [3:0]GPIO_EVR_GTX_GPIO_TRI_O;
  wire [3:0]GPIO_EVR_GTX_GPIO_TRI_T;
  wire [31:0]GPIO_GENERIC0_GPIO2_TRI_I;
  wire [31:0]GPIO_GENERIC0_GPIO_TRI_I;
  wire [31:0]GPIO_SFP_GPIO_TRI_I;
  wire [31:0]GPIO_STALE_NODES_GPIO2_TRI_I;
  wire [31:0]GPIO_STALE_NODES_GPIO_TRI_I;
  wire [15:0]GPIO_SYSMON_GPIO_TRI_I;
  wire [15:0]GPIO_SYSMON_GPIO_TRI_O;
  wire [15:0]GPIO_SYSMON_GPIO_TRI_T;
  wire GT_DIFF_REFCLK1_1_CLK_N;
  wire GT_DIFF_REFCLK1_1_CLK_P;
  wire GT_DIFF_REFCLK1_2_CLK_N;
  wire GT_DIFF_REFCLK1_2_CLK_P;
  wire [0:0]GT_SERIAL_RX0_1_RXN;
  wire [0:0]GT_SERIAL_RX0_1_RXP;
  wire [0:0]GT_SERIAL_RX1_1_RXN;
  wire [0:0]GT_SERIAL_RX1_1_RXP;
  wire [0:0]GT_SERIAL_RX2_1_RXN;
  wire [0:0]GT_SERIAL_RX2_1_RXP;
  wire [0:0]GT_SERIAL_RX3_1_RXN;
  wire [0:0]GT_SERIAL_RX3_1_RXP;
  wire [0:0]GT_SERIAL_RX_1_1_RXN;
  wire [0:0]GT_SERIAL_RX_1_1_RXP;
  wire [0:0]GT_SERIAL_RX_1_RXN;
  wire [0:0]GT_SERIAL_RX_1_RXP;
  wire [0:0]GT_SERIAL_RX_2_RXN;
  wire [0:0]GT_SERIAL_RX_2_RXP;
  wire [0:0]GT_SERIAL_RX_3_RXN;
  wire [0:0]GT_SERIAL_RX_3_RXP;
  wire [31:0]LogDMA_M00_AXI_ARADDR;
  wire [1:0]LogDMA_M00_AXI_ARBURST;
  wire [3:0]LogDMA_M00_AXI_ARCACHE;
  wire [3:0]LogDMA_M00_AXI_ARLEN;
  wire [1:0]LogDMA_M00_AXI_ARLOCK;
  wire [2:0]LogDMA_M00_AXI_ARPROT;
  wire [3:0]LogDMA_M00_AXI_ARQOS;
  wire LogDMA_M00_AXI_ARREADY;
  wire [2:0]LogDMA_M00_AXI_ARSIZE;
  wire LogDMA_M00_AXI_ARVALID;
  wire [31:0]LogDMA_M00_AXI_AWADDR;
  wire [1:0]LogDMA_M00_AXI_AWBURST;
  wire [3:0]LogDMA_M00_AXI_AWCACHE;
  wire [3:0]LogDMA_M00_AXI_AWLEN;
  wire [1:0]LogDMA_M00_AXI_AWLOCK;
  wire [2:0]LogDMA_M00_AXI_AWPROT;
  wire [3:0]LogDMA_M00_AXI_AWQOS;
  wire LogDMA_M00_AXI_AWREADY;
  wire [2:0]LogDMA_M00_AXI_AWSIZE;
  wire LogDMA_M00_AXI_AWVALID;
  wire LogDMA_M00_AXI_BREADY;
  wire [1:0]LogDMA_M00_AXI_BRESP;
  wire LogDMA_M00_AXI_BVALID;
  wire [63:0]LogDMA_M00_AXI_RDATA;
  wire LogDMA_M00_AXI_RLAST;
  wire LogDMA_M00_AXI_RREADY;
  wire [1:0]LogDMA_M00_AXI_RRESP;
  wire LogDMA_M00_AXI_RVALID;
  wire [63:0]LogDMA_M00_AXI_WDATA;
  wire LogDMA_M00_AXI_WLAST;
  wire LogDMA_M00_AXI_WREADY;
  wire [7:0]LogDMA_M00_AXI_WSTRB;
  wire LogDMA_M00_AXI_WVALID;
  wire [31:0]LogDMA_M_AXI_S2MM_AWADDR;
  wire [1:0]LogDMA_M_AXI_S2MM_AWBURST;
  wire [3:0]LogDMA_M_AXI_S2MM_AWCACHE;
  wire [3:0]LogDMA_M_AXI_S2MM_AWLEN;
  wire [1:0]LogDMA_M_AXI_S2MM_AWLOCK;
  wire [2:0]LogDMA_M_AXI_S2MM_AWPROT;
  wire [3:0]LogDMA_M_AXI_S2MM_AWQOS;
  wire LogDMA_M_AXI_S2MM_AWREADY;
  wire [2:0]LogDMA_M_AXI_S2MM_AWSIZE;
  wire LogDMA_M_AXI_S2MM_AWVALID;
  wire LogDMA_M_AXI_S2MM_BREADY;
  wire [1:0]LogDMA_M_AXI_S2MM_BRESP;
  wire LogDMA_M_AXI_S2MM_BVALID;
  wire [63:0]LogDMA_M_AXI_S2MM_WDATA;
  wire LogDMA_M_AXI_S2MM_WLAST;
  wire LogDMA_M_AXI_S2MM_WREADY;
  wire [7:0]LogDMA_M_AXI_S2MM_WSTRB;
  wire LogDMA_M_AXI_S2MM_WVALID;
  wire [31:0]S00_AXIS_1_TDATA;
  wire S00_AXIS_1_TLAST;
  wire S00_AXIS_1_TREADY;
  wire S00_AXIS_1_TVALID;
  wire [31:0]S01_AXIS_1_TDATA;
  wire S01_AXIS_1_TLAST;
  wire S01_AXIS_1_TREADY;
  wire S01_AXIS_1_TVALID;
  wire [0:15]USER_DATA_S_AXI_TX0_1_TDATA;
  wire [0:1]USER_DATA_S_AXI_TX0_1_TKEEP;
  wire USER_DATA_S_AXI_TX0_1_TLAST;
  wire USER_DATA_S_AXI_TX0_1_TREADY;
  wire USER_DATA_S_AXI_TX0_1_TVALID;
  wire [0:15]USER_DATA_S_AXI_TX1_1_TDATA;
  wire [0:1]USER_DATA_S_AXI_TX1_1_TKEEP;
  wire USER_DATA_S_AXI_TX1_1_TLAST;
  wire USER_DATA_S_AXI_TX1_1_TREADY;
  wire USER_DATA_S_AXI_TX1_1_TVALID;
  wire [0:15]USER_DATA_S_AXI_TX2_1_TDATA;
  wire [0:1]USER_DATA_S_AXI_TX2_1_TKEEP;
  wire USER_DATA_S_AXI_TX2_1_TLAST;
  wire USER_DATA_S_AXI_TX2_1_TREADY;
  wire USER_DATA_S_AXI_TX2_1_TVALID;
  wire [0:15]USER_DATA_S_AXI_TX3_1_TDATA;
  wire [0:1]USER_DATA_S_AXI_TX3_1_TKEEP;
  wire USER_DATA_S_AXI_TX3_1_TLAST;
  wire USER_DATA_S_AXI_TX3_1_TREADY;
  wire USER_DATA_S_AXI_TX3_1_TVALID;
  wire [0:15]USER_DATA_S_AXI_TX_1_1_TDATA;
  wire [0:1]USER_DATA_S_AXI_TX_1_1_TKEEP;
  wire USER_DATA_S_AXI_TX_1_1_TLAST;
  wire USER_DATA_S_AXI_TX_1_1_TREADY;
  wire USER_DATA_S_AXI_TX_1_1_TVALID;
  wire [0:15]USER_DATA_S_AXI_TX_1_TDATA;
  wire [0:1]USER_DATA_S_AXI_TX_1_TKEEP;
  wire USER_DATA_S_AXI_TX_1_TLAST;
  wire USER_DATA_S_AXI_TX_1_TREADY;
  wire USER_DATA_S_AXI_TX_1_TVALID;
  wire [0:15]USER_DATA_S_AXI_TX_2_TDATA;
  wire [0:1]USER_DATA_S_AXI_TX_2_TKEEP;
  wire USER_DATA_S_AXI_TX_2_TLAST;
  wire USER_DATA_S_AXI_TX_2_TREADY;
  wire USER_DATA_S_AXI_TX_2_TVALID;
  wire [0:15]USER_DATA_S_AXI_TX_3_TDATA;
  wire [0:1]USER_DATA_S_AXI_TX_3_TKEEP;
  wire USER_DATA_S_AXI_TX_3_TLAST;
  wire USER_DATA_S_AXI_TX_3_TREADY;
  wire USER_DATA_S_AXI_TX_3_TVALID;
  wire aurora_8b10b_0_CORE_STATUS_CHANNEL_UP;
  wire aurora_8b10b_0_CORE_STATUS_FRAME_ERR;
  wire aurora_8b10b_0_CORE_STATUS_HARD_ERR;
  wire [0:0]aurora_8b10b_0_CORE_STATUS_LANE_UP;
  wire aurora_8b10b_0_CORE_STATUS_RX_RESETDONE_OUT;
  wire aurora_8b10b_0_CORE_STATUS_SOFT_ERR;
  wire aurora_8b10b_0_CORE_STATUS_TX_LOCK;
  wire aurora_8b10b_0_CORE_STATUS_TX_RESETDONE_OUT;
  wire [0:0]aurora_8b10b_0_GT_SERIAL_TX_TXN;
  wire [0:0]aurora_8b10b_0_GT_SERIAL_TX_TXP;
  wire [0:15]aurora_8b10b_0_USER_DATA_M_AXI_RX_TDATA;
  wire [0:1]aurora_8b10b_0_USER_DATA_M_AXI_RX_TKEEP;
  wire aurora_8b10b_0_USER_DATA_M_AXI_RX_TLAST;
  wire aurora_8b10b_0_USER_DATA_M_AXI_RX_TVALID;
  wire [15:0]axi_bram_ctrl_0_BRAM_PORTA_ADDR;
  wire axi_bram_ctrl_0_BRAM_PORTA_CLK;
  wire [31:0]axi_bram_ctrl_0_BRAM_PORTA_DIN;
  wire [31:0]axi_bram_ctrl_0_BRAM_PORTA_DOUT;
  wire axi_bram_ctrl_0_BRAM_PORTA_EN;
  wire axi_bram_ctrl_0_BRAM_PORTA_RST;
  wire [3:0]axi_bram_ctrl_0_BRAM_PORTA_WE;
  wire [31:0]axi_gpio_0_GPIO1_TRI_O;
  wire [31:0]axi_gpio_0_GPIO2_TRI_I;
  wire [31:0]axi_gpio_0_GPIO_TRI_I;
  wire axi_iic_0_IIC_SCL_I;
  wire axi_iic_0_IIC_SCL_O;
  wire axi_iic_0_IIC_SCL_T;
  wire axi_iic_0_IIC_SDA_I;
  wire axi_iic_0_IIC_SDA_O;
  wire axi_iic_0_IIC_SDA_T;
  wire [2:0]axi_iic_0_gpo;
  wire [8:0]drp_bridge_0_DRP0_DADDR;
  wire drp_bridge_0_DRP0_DEN;
  wire [15:0]drp_bridge_0_DRP0_DI;
  wire [15:0]drp_bridge_0_DRP0_DO;
  wire drp_bridge_0_DRP0_DRDY;
  wire drp_bridge_0_DRP0_DWE;
  wire [8:0]drp_bridge_0_DRP1_DADDR;
  wire drp_bridge_0_DRP1_DEN;
  wire [15:0]drp_bridge_0_DRP1_DI;
  wire [15:0]drp_bridge_0_DRP1_DO;
  wire drp_bridge_0_DRP1_DRDY;
  wire drp_bridge_0_DRP1_DWE;
  wire [8:0]drp_bridge_0_DRP2_DADDR;
  wire drp_bridge_0_DRP2_DEN;
  wire [15:0]drp_bridge_0_DRP2_DI;
  wire [15:0]drp_bridge_0_DRP2_DO;
  wire drp_bridge_0_DRP2_DRDY;
  wire drp_bridge_0_DRP2_DWE;
  wire [8:0]drp_bridge_0_DRP3_DADDR;
  wire drp_bridge_0_DRP3_DEN;
  wire [15:0]drp_bridge_0_DRP3_DI;
  wire [15:0]drp_bridge_0_DRP3_DO;
  wire drp_bridge_0_DRP3_DRDY;
  wire drp_bridge_0_DRP3_DWE;
  wire [8:0]drp_bridge_0_DRP4_DADDR;
  wire drp_bridge_0_DRP4_DEN;
  wire [15:0]drp_bridge_0_DRP4_DI;
  wire [15:0]drp_bridge_0_DRP4_DO;
  wire drp_bridge_0_DRP4_DRDY;
  wire drp_bridge_0_DRP4_DWE;
  wire [8:0]drp_bridge_0_DRP5_DADDR;
  wire drp_bridge_0_DRP5_DEN;
  wire [15:0]drp_bridge_0_DRP5_DI;
  wire [15:0]drp_bridge_0_DRP5_DO;
  wire drp_bridge_0_DRP5_DRDY;
  wire drp_bridge_0_DRP5_DWE;
  wire [8:0]drp_bridge_0_DRP6_DADDR;
  wire drp_bridge_0_DRP6_DEN;
  wire [15:0]drp_bridge_0_DRP6_DI;
  wire [15:0]drp_bridge_0_DRP6_DO;
  wire drp_bridge_0_DRP6_DRDY;
  wire drp_bridge_0_DRP6_DWE;
  wire [8:0]drp_bridge_0_DRP7_DADDR;
  wire drp_bridge_0_DRP7_DEN;
  wire [15:0]drp_bridge_0_DRP7_DI;
  wire [15:0]drp_bridge_0_DRP7_DO;
  wire drp_bridge_0_DRP7_DRDY;
  wire drp_bridge_0_DRP7_DWE;
  wire [8:0]drp_bridge_0_DRP8_DADDR;
  wire drp_bridge_0_DRP8_DEN;
  wire [15:0]drp_bridge_0_DRP8_DI;
  wire [15:0]drp_bridge_0_DRP8_DO;
  wire drp_bridge_0_DRP8_DRDY;
  wire drp_bridge_0_DRP8_DWE;
  wire gt_reset_1;
  wire [14:0]processing_system7_0_DDR_ADDR;
  wire [2:0]processing_system7_0_DDR_BA;
  wire processing_system7_0_DDR_CAS_N;
  wire processing_system7_0_DDR_CKE;
  wire processing_system7_0_DDR_CK_N;
  wire processing_system7_0_DDR_CK_P;
  wire processing_system7_0_DDR_CS_N;
  wire [3:0]processing_system7_0_DDR_DM;
  wire [31:0]processing_system7_0_DDR_DQ;
  wire [3:0]processing_system7_0_DDR_DQS_N;
  wire [3:0]processing_system7_0_DDR_DQS_P;
  wire processing_system7_0_DDR_ODT;
  wire processing_system7_0_DDR_RAS_N;
  wire processing_system7_0_DDR_RESET_N;
  wire processing_system7_0_DDR_WE_N;
  wire processing_system7_0_FCLK_CLK0;
  wire processing_system7_0_FCLK_CLK1;
  wire processing_system7_0_FCLK_RESET0_N;
  wire processing_system7_0_FIXED_IO_DDR_VRN;
  wire processing_system7_0_FIXED_IO_DDR_VRP;
  wire [53:0]processing_system7_0_FIXED_IO_MIO;
  wire processing_system7_0_FIXED_IO_PS_CLK;
  wire processing_system7_0_FIXED_IO_PS_PORB;
  wire processing_system7_0_FIXED_IO_PS_SRSTB;
  wire [31:0]processing_system7_0_M_AXI_GP0_ARADDR;
  wire [1:0]processing_system7_0_M_AXI_GP0_ARBURST;
  wire [3:0]processing_system7_0_M_AXI_GP0_ARCACHE;
  wire [11:0]processing_system7_0_M_AXI_GP0_ARID;
  wire [3:0]processing_system7_0_M_AXI_GP0_ARLEN;
  wire [1:0]processing_system7_0_M_AXI_GP0_ARLOCK;
  wire [2:0]processing_system7_0_M_AXI_GP0_ARPROT;
  wire [3:0]processing_system7_0_M_AXI_GP0_ARQOS;
  wire processing_system7_0_M_AXI_GP0_ARREADY;
  wire [2:0]processing_system7_0_M_AXI_GP0_ARSIZE;
  wire processing_system7_0_M_AXI_GP0_ARVALID;
  wire [31:0]processing_system7_0_M_AXI_GP0_AWADDR;
  wire [1:0]processing_system7_0_M_AXI_GP0_AWBURST;
  wire [3:0]processing_system7_0_M_AXI_GP0_AWCACHE;
  wire [11:0]processing_system7_0_M_AXI_GP0_AWID;
  wire [3:0]processing_system7_0_M_AXI_GP0_AWLEN;
  wire [1:0]processing_system7_0_M_AXI_GP0_AWLOCK;
  wire [2:0]processing_system7_0_M_AXI_GP0_AWPROT;
  wire [3:0]processing_system7_0_M_AXI_GP0_AWQOS;
  wire processing_system7_0_M_AXI_GP0_AWREADY;
  wire [2:0]processing_system7_0_M_AXI_GP0_AWSIZE;
  wire processing_system7_0_M_AXI_GP0_AWVALID;
  wire [11:0]processing_system7_0_M_AXI_GP0_BID;
  wire processing_system7_0_M_AXI_GP0_BREADY;
  wire [1:0]processing_system7_0_M_AXI_GP0_BRESP;
  wire processing_system7_0_M_AXI_GP0_BVALID;
  wire [31:0]processing_system7_0_M_AXI_GP0_RDATA;
  wire [11:0]processing_system7_0_M_AXI_GP0_RID;
  wire processing_system7_0_M_AXI_GP0_RLAST;
  wire processing_system7_0_M_AXI_GP0_RREADY;
  wire [1:0]processing_system7_0_M_AXI_GP0_RRESP;
  wire processing_system7_0_M_AXI_GP0_RVALID;
  wire [31:0]processing_system7_0_M_AXI_GP0_WDATA;
  wire [11:0]processing_system7_0_M_AXI_GP0_WID;
  wire processing_system7_0_M_AXI_GP0_WLAST;
  wire processing_system7_0_M_AXI_GP0_WREADY;
  wire [3:0]processing_system7_0_M_AXI_GP0_WSTRB;
  wire processing_system7_0_M_AXI_GP0_WVALID;
  wire [31:0]processing_system7_0_axi_periph_M00_AXI_ARADDR;
  wire processing_system7_0_axi_periph_M00_AXI_ARREADY;
  wire [0:0]processing_system7_0_axi_periph_M00_AXI_ARVALID;
  wire [31:0]processing_system7_0_axi_periph_M00_AXI_AWADDR;
  wire processing_system7_0_axi_periph_M00_AXI_AWREADY;
  wire [0:0]processing_system7_0_axi_periph_M00_AXI_AWVALID;
  wire [0:0]processing_system7_0_axi_periph_M00_AXI_BREADY;
  wire [1:0]processing_system7_0_axi_periph_M00_AXI_BRESP;
  wire processing_system7_0_axi_periph_M00_AXI_BVALID;
  wire [31:0]processing_system7_0_axi_periph_M00_AXI_RDATA;
  wire [0:0]processing_system7_0_axi_periph_M00_AXI_RREADY;
  wire [1:0]processing_system7_0_axi_periph_M00_AXI_RRESP;
  wire processing_system7_0_axi_periph_M00_AXI_RVALID;
  wire [31:0]processing_system7_0_axi_periph_M00_AXI_WDATA;
  wire processing_system7_0_axi_periph_M00_AXI_WREADY;
  wire [3:0]processing_system7_0_axi_periph_M00_AXI_WSTRB;
  wire [0:0]processing_system7_0_axi_periph_M00_AXI_WVALID;
  wire [31:0]processing_system7_0_axi_periph_M01_AXI_ARADDR;
  wire processing_system7_0_axi_periph_M01_AXI_ARREADY;
  wire processing_system7_0_axi_periph_M01_AXI_ARVALID;
  wire [31:0]processing_system7_0_axi_periph_M01_AXI_AWADDR;
  wire processing_system7_0_axi_periph_M01_AXI_AWREADY;
  wire processing_system7_0_axi_periph_M01_AXI_AWVALID;
  wire processing_system7_0_axi_periph_M01_AXI_BREADY;
  wire [1:0]processing_system7_0_axi_periph_M01_AXI_BRESP;
  wire processing_system7_0_axi_periph_M01_AXI_BVALID;
  wire [31:0]processing_system7_0_axi_periph_M01_AXI_RDATA;
  wire processing_system7_0_axi_periph_M01_AXI_RREADY;
  wire [1:0]processing_system7_0_axi_periph_M01_AXI_RRESP;
  wire processing_system7_0_axi_periph_M01_AXI_RVALID;
  wire [31:0]processing_system7_0_axi_periph_M01_AXI_WDATA;
  wire processing_system7_0_axi_periph_M01_AXI_WREADY;
  wire [3:0]processing_system7_0_axi_periph_M01_AXI_WSTRB;
  wire processing_system7_0_axi_periph_M01_AXI_WVALID;
  wire [31:0]processing_system7_0_axi_periph_M02_AXI_ARADDR;
  wire processing_system7_0_axi_periph_M02_AXI_ARREADY;
  wire processing_system7_0_axi_periph_M02_AXI_ARVALID;
  wire [31:0]processing_system7_0_axi_periph_M02_AXI_AWADDR;
  wire processing_system7_0_axi_periph_M02_AXI_AWREADY;
  wire processing_system7_0_axi_periph_M02_AXI_AWVALID;
  wire processing_system7_0_axi_periph_M02_AXI_BREADY;
  wire [1:0]processing_system7_0_axi_periph_M02_AXI_BRESP;
  wire processing_system7_0_axi_periph_M02_AXI_BVALID;
  wire [31:0]processing_system7_0_axi_periph_M02_AXI_RDATA;
  wire processing_system7_0_axi_periph_M02_AXI_RREADY;
  wire [1:0]processing_system7_0_axi_periph_M02_AXI_RRESP;
  wire processing_system7_0_axi_periph_M02_AXI_RVALID;
  wire [31:0]processing_system7_0_axi_periph_M02_AXI_WDATA;
  wire processing_system7_0_axi_periph_M02_AXI_WREADY;
  wire [3:0]processing_system7_0_axi_periph_M02_AXI_WSTRB;
  wire processing_system7_0_axi_periph_M02_AXI_WVALID;
  wire [31:0]processing_system7_0_axi_periph_M03_AXI_ARADDR;
  wire [2:0]processing_system7_0_axi_periph_M03_AXI_ARPROT;
  wire processing_system7_0_axi_periph_M03_AXI_ARREADY;
  wire processing_system7_0_axi_periph_M03_AXI_ARVALID;
  wire [31:0]processing_system7_0_axi_periph_M03_AXI_AWADDR;
  wire [2:0]processing_system7_0_axi_periph_M03_AXI_AWPROT;
  wire processing_system7_0_axi_periph_M03_AXI_AWREADY;
  wire processing_system7_0_axi_periph_M03_AXI_AWVALID;
  wire processing_system7_0_axi_periph_M03_AXI_BREADY;
  wire [1:0]processing_system7_0_axi_periph_M03_AXI_BRESP;
  wire processing_system7_0_axi_periph_M03_AXI_BVALID;
  wire [31:0]processing_system7_0_axi_periph_M03_AXI_RDATA;
  wire processing_system7_0_axi_periph_M03_AXI_RREADY;
  wire [1:0]processing_system7_0_axi_periph_M03_AXI_RRESP;
  wire processing_system7_0_axi_periph_M03_AXI_RVALID;
  wire [31:0]processing_system7_0_axi_periph_M03_AXI_WDATA;
  wire processing_system7_0_axi_periph_M03_AXI_WREADY;
  wire [3:0]processing_system7_0_axi_periph_M03_AXI_WSTRB;
  wire processing_system7_0_axi_periph_M03_AXI_WVALID;
  wire [31:0]processing_system7_0_axi_periph_M04_AXI_ARADDR;
  wire processing_system7_0_axi_periph_M04_AXI_ARREADY;
  wire processing_system7_0_axi_periph_M04_AXI_ARVALID;
  wire [31:0]processing_system7_0_axi_periph_M04_AXI_AWADDR;
  wire processing_system7_0_axi_periph_M04_AXI_AWREADY;
  wire processing_system7_0_axi_periph_M04_AXI_AWVALID;
  wire processing_system7_0_axi_periph_M04_AXI_BREADY;
  wire [1:0]processing_system7_0_axi_periph_M04_AXI_BRESP;
  wire processing_system7_0_axi_periph_M04_AXI_BVALID;
  wire [31:0]processing_system7_0_axi_periph_M04_AXI_RDATA;
  wire processing_system7_0_axi_periph_M04_AXI_RREADY;
  wire [1:0]processing_system7_0_axi_periph_M04_AXI_RRESP;
  wire processing_system7_0_axi_periph_M04_AXI_RVALID;
  wire [31:0]processing_system7_0_axi_periph_M04_AXI_WDATA;
  wire processing_system7_0_axi_periph_M04_AXI_WREADY;
  wire [3:0]processing_system7_0_axi_periph_M04_AXI_WSTRB;
  wire processing_system7_0_axi_periph_M04_AXI_WVALID;
  wire [31:0]processing_system7_0_axi_periph_M05_AXI_ARADDR;
  wire processing_system7_0_axi_periph_M05_AXI_ARREADY;
  wire processing_system7_0_axi_periph_M05_AXI_ARVALID;
  wire [31:0]processing_system7_0_axi_periph_M05_AXI_AWADDR;
  wire processing_system7_0_axi_periph_M05_AXI_AWREADY;
  wire processing_system7_0_axi_periph_M05_AXI_AWVALID;
  wire processing_system7_0_axi_periph_M05_AXI_BREADY;
  wire [1:0]processing_system7_0_axi_periph_M05_AXI_BRESP;
  wire processing_system7_0_axi_periph_M05_AXI_BVALID;
  wire [31:0]processing_system7_0_axi_periph_M05_AXI_RDATA;
  wire processing_system7_0_axi_periph_M05_AXI_RREADY;
  wire [1:0]processing_system7_0_axi_periph_M05_AXI_RRESP;
  wire processing_system7_0_axi_periph_M05_AXI_RVALID;
  wire [31:0]processing_system7_0_axi_periph_M05_AXI_WDATA;
  wire processing_system7_0_axi_periph_M05_AXI_WREADY;
  wire [3:0]processing_system7_0_axi_periph_M05_AXI_WSTRB;
  wire processing_system7_0_axi_periph_M05_AXI_WVALID;
  wire [31:0]processing_system7_0_axi_periph_M06_AXI_ARADDR;
  wire processing_system7_0_axi_periph_M06_AXI_ARREADY;
  wire processing_system7_0_axi_periph_M06_AXI_ARVALID;
  wire [31:0]processing_system7_0_axi_periph_M06_AXI_AWADDR;
  wire processing_system7_0_axi_periph_M06_AXI_AWREADY;
  wire processing_system7_0_axi_periph_M06_AXI_AWVALID;
  wire processing_system7_0_axi_periph_M06_AXI_BREADY;
  wire [1:0]processing_system7_0_axi_periph_M06_AXI_BRESP;
  wire processing_system7_0_axi_periph_M06_AXI_BVALID;
  wire [31:0]processing_system7_0_axi_periph_M06_AXI_RDATA;
  wire processing_system7_0_axi_periph_M06_AXI_RREADY;
  wire [1:0]processing_system7_0_axi_periph_M06_AXI_RRESP;
  wire processing_system7_0_axi_periph_M06_AXI_RVALID;
  wire [31:0]processing_system7_0_axi_periph_M06_AXI_WDATA;
  wire processing_system7_0_axi_periph_M06_AXI_WREADY;
  wire [3:0]processing_system7_0_axi_periph_M06_AXI_WSTRB;
  wire processing_system7_0_axi_periph_M06_AXI_WVALID;
  wire [31:0]processing_system7_0_axi_periph_M07_AXI_ARADDR;
  wire processing_system7_0_axi_periph_M07_AXI_ARREADY;
  wire processing_system7_0_axi_periph_M07_AXI_ARVALID;
  wire [31:0]processing_system7_0_axi_periph_M07_AXI_AWADDR;
  wire processing_system7_0_axi_periph_M07_AXI_AWREADY;
  wire processing_system7_0_axi_periph_M07_AXI_AWVALID;
  wire processing_system7_0_axi_periph_M07_AXI_BREADY;
  wire [1:0]processing_system7_0_axi_periph_M07_AXI_BRESP;
  wire processing_system7_0_axi_periph_M07_AXI_BVALID;
  wire [31:0]processing_system7_0_axi_periph_M07_AXI_RDATA;
  wire processing_system7_0_axi_periph_M07_AXI_RREADY;
  wire [1:0]processing_system7_0_axi_periph_M07_AXI_RRESP;
  wire processing_system7_0_axi_periph_M07_AXI_RVALID;
  wire [31:0]processing_system7_0_axi_periph_M07_AXI_WDATA;
  wire processing_system7_0_axi_periph_M07_AXI_WREADY;
  wire [3:0]processing_system7_0_axi_periph_M07_AXI_WSTRB;
  wire processing_system7_0_axi_periph_M07_AXI_WVALID;
  wire [31:0]processing_system7_0_axi_periph_M08_AXI_ARADDR;
  wire [2:0]processing_system7_0_axi_periph_M08_AXI_ARPROT;
  wire processing_system7_0_axi_periph_M08_AXI_ARREADY;
  wire processing_system7_0_axi_periph_M08_AXI_ARVALID;
  wire [31:0]processing_system7_0_axi_periph_M08_AXI_AWADDR;
  wire [2:0]processing_system7_0_axi_periph_M08_AXI_AWPROT;
  wire processing_system7_0_axi_periph_M08_AXI_AWREADY;
  wire processing_system7_0_axi_periph_M08_AXI_AWVALID;
  wire processing_system7_0_axi_periph_M08_AXI_BREADY;
  wire [1:0]processing_system7_0_axi_periph_M08_AXI_BRESP;
  wire processing_system7_0_axi_periph_M08_AXI_BVALID;
  wire [31:0]processing_system7_0_axi_periph_M08_AXI_RDATA;
  wire processing_system7_0_axi_periph_M08_AXI_RREADY;
  wire [1:0]processing_system7_0_axi_periph_M08_AXI_RRESP;
  wire processing_system7_0_axi_periph_M08_AXI_RVALID;
  wire [31:0]processing_system7_0_axi_periph_M08_AXI_WDATA;
  wire processing_system7_0_axi_periph_M08_AXI_WREADY;
  wire [3:0]processing_system7_0_axi_periph_M08_AXI_WSTRB;
  wire processing_system7_0_axi_periph_M08_AXI_WVALID;
  wire [31:0]processing_system7_0_axi_periph_M09_AXI_ARADDR;
  wire processing_system7_0_axi_periph_M09_AXI_ARREADY;
  wire processing_system7_0_axi_periph_M09_AXI_ARVALID;
  wire [31:0]processing_system7_0_axi_periph_M09_AXI_AWADDR;
  wire processing_system7_0_axi_periph_M09_AXI_AWREADY;
  wire processing_system7_0_axi_periph_M09_AXI_AWVALID;
  wire processing_system7_0_axi_periph_M09_AXI_BREADY;
  wire [1:0]processing_system7_0_axi_periph_M09_AXI_BRESP;
  wire processing_system7_0_axi_periph_M09_AXI_BVALID;
  wire [31:0]processing_system7_0_axi_periph_M09_AXI_RDATA;
  wire processing_system7_0_axi_periph_M09_AXI_RREADY;
  wire [1:0]processing_system7_0_axi_periph_M09_AXI_RRESP;
  wire processing_system7_0_axi_periph_M09_AXI_RVALID;
  wire [31:0]processing_system7_0_axi_periph_M09_AXI_WDATA;
  wire processing_system7_0_axi_periph_M09_AXI_WREADY;
  wire [3:0]processing_system7_0_axi_periph_M09_AXI_WSTRB;
  wire processing_system7_0_axi_periph_M09_AXI_WVALID;
  wire [31:0]processing_system7_0_axi_periph_M10_AXI_ARADDR;
  wire processing_system7_0_axi_periph_M10_AXI_ARREADY;
  wire processing_system7_0_axi_periph_M10_AXI_ARVALID;
  wire [31:0]processing_system7_0_axi_periph_M10_AXI_AWADDR;
  wire processing_system7_0_axi_periph_M10_AXI_AWREADY;
  wire processing_system7_0_axi_periph_M10_AXI_AWVALID;
  wire processing_system7_0_axi_periph_M10_AXI_BREADY;
  wire [1:0]processing_system7_0_axi_periph_M10_AXI_BRESP;
  wire processing_system7_0_axi_periph_M10_AXI_BVALID;
  wire [31:0]processing_system7_0_axi_periph_M10_AXI_RDATA;
  wire processing_system7_0_axi_periph_M10_AXI_RREADY;
  wire [1:0]processing_system7_0_axi_periph_M10_AXI_RRESP;
  wire processing_system7_0_axi_periph_M10_AXI_RVALID;
  wire [31:0]processing_system7_0_axi_periph_M10_AXI_WDATA;
  wire processing_system7_0_axi_periph_M10_AXI_WREADY;
  wire processing_system7_0_axi_periph_M10_AXI_WVALID;
  wire [31:0]processing_system7_0_axi_periph_M11_AXI_ARADDR;
  wire processing_system7_0_axi_periph_M11_AXI_ARREADY;
  wire processing_system7_0_axi_periph_M11_AXI_ARVALID;
  wire [31:0]processing_system7_0_axi_periph_M11_AXI_AWADDR;
  wire processing_system7_0_axi_periph_M11_AXI_AWREADY;
  wire processing_system7_0_axi_periph_M11_AXI_AWVALID;
  wire processing_system7_0_axi_periph_M11_AXI_BREADY;
  wire [1:0]processing_system7_0_axi_periph_M11_AXI_BRESP;
  wire processing_system7_0_axi_periph_M11_AXI_BVALID;
  wire [31:0]processing_system7_0_axi_periph_M11_AXI_RDATA;
  wire processing_system7_0_axi_periph_M11_AXI_RREADY;
  wire [1:0]processing_system7_0_axi_periph_M11_AXI_RRESP;
  wire processing_system7_0_axi_periph_M11_AXI_RVALID;
  wire [31:0]processing_system7_0_axi_periph_M11_AXI_WDATA;
  wire processing_system7_0_axi_periph_M11_AXI_WREADY;
  wire [3:0]processing_system7_0_axi_periph_M11_AXI_WSTRB;
  wire processing_system7_0_axi_periph_M11_AXI_WVALID;
  wire [31:0]processing_system7_0_axi_periph_M12_AXI_ARADDR;
  wire processing_system7_0_axi_periph_M12_AXI_ARREADY;
  wire processing_system7_0_axi_periph_M12_AXI_ARVALID;
  wire [31:0]processing_system7_0_axi_periph_M12_AXI_AWADDR;
  wire processing_system7_0_axi_periph_M12_AXI_AWREADY;
  wire processing_system7_0_axi_periph_M12_AXI_AWVALID;
  wire processing_system7_0_axi_periph_M12_AXI_BREADY;
  wire [1:0]processing_system7_0_axi_periph_M12_AXI_BRESP;
  wire processing_system7_0_axi_periph_M12_AXI_BVALID;
  wire [31:0]processing_system7_0_axi_periph_M12_AXI_RDATA;
  wire processing_system7_0_axi_periph_M12_AXI_RREADY;
  wire [1:0]processing_system7_0_axi_periph_M12_AXI_RRESP;
  wire processing_system7_0_axi_periph_M12_AXI_RVALID;
  wire [31:0]processing_system7_0_axi_periph_M12_AXI_WDATA;
  wire processing_system7_0_axi_periph_M12_AXI_WREADY;
  wire [3:0]processing_system7_0_axi_periph_M12_AXI_WSTRB;
  wire processing_system7_0_axi_periph_M12_AXI_WVALID;
  wire reset_1;
  wire [0:0]rst_processing_system7_0_50M_interconnect_aresetn;
  wire [0:0]rst_processing_system7_0_50M_peripheral_aresetn;

  assign Aurora0_CORE_STATUS_channel_up = aurora_8b10b_0_CORE_STATUS_CHANNEL_UP;
  assign Aurora0_CORE_STATUS_frame_err = aurora_8b10b_0_CORE_STATUS_FRAME_ERR;
  assign Aurora0_CORE_STATUS_hard_err = aurora_8b10b_0_CORE_STATUS_HARD_ERR;
  assign Aurora0_CORE_STATUS_lane_up[0] = aurora_8b10b_0_CORE_STATUS_LANE_UP;
  assign Aurora0_CORE_STATUS_rx_resetdone_out = aurora_8b10b_0_CORE_STATUS_RX_RESETDONE_OUT;
  assign Aurora0_CORE_STATUS_soft_err = aurora_8b10b_0_CORE_STATUS_SOFT_ERR;
  assign Aurora0_CORE_STATUS_tx_lock = aurora_8b10b_0_CORE_STATUS_TX_LOCK;
  assign Aurora0_CORE_STATUS_tx_resetdone_out = aurora_8b10b_0_CORE_STATUS_TX_RESETDONE_OUT;
  assign Aurora0_TX_txn[0] = aurora_8b10b_0_GT_SERIAL_TX_TXN;
  assign Aurora0_TX_txp[0] = aurora_8b10b_0_GT_SERIAL_TX_TXP;
  assign Aurora0_USER_DATA_M_AXI_RX_tdata[0:15] = aurora_8b10b_0_USER_DATA_M_AXI_RX_TDATA;
  assign Aurora0_USER_DATA_M_AXI_RX_tkeep[0:1] = aurora_8b10b_0_USER_DATA_M_AXI_RX_TKEEP;
  assign Aurora0_USER_DATA_M_AXI_RX_tlast = aurora_8b10b_0_USER_DATA_M_AXI_RX_TLAST;
  assign Aurora0_USER_DATA_M_AXI_RX_tvalid = aurora_8b10b_0_USER_DATA_M_AXI_RX_TVALID;
  assign Aurora0_USER_DATA_S_AXI_TX_tready = USER_DATA_S_AXI_TX_1_TREADY;
  assign Aurora1_CORE_STATUS_channel_up = Aurora_CORE_STATUS1_CHANNEL_UP;
  assign Aurora1_CORE_STATUS_frame_err = Aurora_CORE_STATUS1_FRAME_ERR;
  assign Aurora1_CORE_STATUS_hard_err = Aurora_CORE_STATUS1_HARD_ERR;
  assign Aurora1_CORE_STATUS_lane_up[0] = Aurora_CORE_STATUS1_LANE_UP;
  assign Aurora1_CORE_STATUS_rx_resetdone_out = Aurora_CORE_STATUS1_RX_RESETDONE_OUT;
  assign Aurora1_CORE_STATUS_soft_err = Aurora_CORE_STATUS1_SOFT_ERR;
  assign Aurora1_CORE_STATUS_tx_lock = Aurora_CORE_STATUS1_TX_LOCK;
  assign Aurora1_CORE_STATUS_tx_resetdone_out = Aurora_CORE_STATUS1_TX_RESETDONE_OUT;
  assign Aurora1_TX_txn[0] = Aurora_GT_SERIAL_TX1_TXN;
  assign Aurora1_TX_txp[0] = Aurora_GT_SERIAL_TX1_TXP;
  assign Aurora1_USER_DATA_M_AXI_RX_tdata[0:15] = Aurora_USER_DATA_M_AXI_RX1_TDATA;
  assign Aurora1_USER_DATA_M_AXI_RX_tkeep[0:1] = Aurora_USER_DATA_M_AXI_RX1_TKEEP;
  assign Aurora1_USER_DATA_M_AXI_RX_tlast = Aurora_USER_DATA_M_AXI_RX1_TLAST;
  assign Aurora1_USER_DATA_M_AXI_RX_tvalid = Aurora_USER_DATA_M_AXI_RX1_TVALID;
  assign Aurora1_USER_DATA_S_AXI_TX_tready = USER_DATA_S_AXI_TX_2_TREADY;
  assign Aurora2_CORE_STATUS_channel_up = Aurora_CORE_STATUS_CHANNEL_UP;
  assign Aurora2_CORE_STATUS_frame_err = Aurora_CORE_STATUS_FRAME_ERR;
  assign Aurora2_CORE_STATUS_hard_err = Aurora_CORE_STATUS_HARD_ERR;
  assign Aurora2_CORE_STATUS_lane_up[0] = Aurora_CORE_STATUS_LANE_UP;
  assign Aurora2_CORE_STATUS_rx_resetdone_out = Aurora_CORE_STATUS_RX_RESETDONE_OUT;
  assign Aurora2_CORE_STATUS_soft_err = Aurora_CORE_STATUS_SOFT_ERR;
  assign Aurora2_CORE_STATUS_tx_lock = Aurora_CORE_STATUS_TX_LOCK;
  assign Aurora2_CORE_STATUS_tx_resetdone_out = Aurora_CORE_STATUS_TX_RESETDONE_OUT;
  assign Aurora2_TX_txn[0] = Aurora_GT_SERIAL_TX_TXN;
  assign Aurora2_TX_txp[0] = Aurora_GT_SERIAL_TX_TXP;
  assign Aurora2_USER_DATA_M_AXI_RX_tdata[0:15] = Aurora_USER_DATA_M_AXI_RX_TDATA;
  assign Aurora2_USER_DATA_M_AXI_RX_tkeep[0:1] = Aurora_USER_DATA_M_AXI_RX_TKEEP;
  assign Aurora2_USER_DATA_M_AXI_RX_tlast = Aurora_USER_DATA_M_AXI_RX_TLAST;
  assign Aurora2_USER_DATA_M_AXI_RX_tvalid = Aurora_USER_DATA_M_AXI_RX_TVALID;
  assign Aurora2_USER_DATA_S_AXI_TX_tready = USER_DATA_S_AXI_TX_3_TREADY;
  assign Aurora3_CORE_STATUS_channel_up = Aurora_CORE_STATUS2_CHANNEL_UP;
  assign Aurora3_CORE_STATUS_frame_err = Aurora_CORE_STATUS2_FRAME_ERR;
  assign Aurora3_CORE_STATUS_hard_err = Aurora_CORE_STATUS2_HARD_ERR;
  assign Aurora3_CORE_STATUS_lane_up[0] = Aurora_CORE_STATUS2_LANE_UP;
  assign Aurora3_CORE_STATUS_rx_resetdone_out = Aurora_CORE_STATUS2_RX_RESETDONE_OUT;
  assign Aurora3_CORE_STATUS_soft_err = Aurora_CORE_STATUS2_SOFT_ERR;
  assign Aurora3_CORE_STATUS_tx_lock = Aurora_CORE_STATUS2_TX_LOCK;
  assign Aurora3_CORE_STATUS_tx_resetdone_out = Aurora_CORE_STATUS2_TX_RESETDONE_OUT;
  assign Aurora3_TX_txn[0] = Aurora_GT_SERIAL_TX2_TXN;
  assign Aurora3_TX_txp[0] = Aurora_GT_SERIAL_TX2_TXP;
  assign Aurora3_USER_DATA_M_AXI_RX_tdata[0:15] = Aurora_USER_DATA_M_AXI_RX2_TDATA;
  assign Aurora3_USER_DATA_M_AXI_RX_tkeep[0:1] = Aurora_USER_DATA_M_AXI_RX2_TKEEP;
  assign Aurora3_USER_DATA_M_AXI_RX_tlast = Aurora_USER_DATA_M_AXI_RX2_TLAST;
  assign Aurora3_USER_DATA_M_AXI_RX_tvalid = Aurora_USER_DATA_M_AXI_RX2_TVALID;
  assign Aurora3_USER_DATA_S_AXI_TX_tready = USER_DATA_S_AXI_TX_1_1_TREADY;
  assign Aurora4_CORE_STATUS_channel_up = AuroraB_CORE_STATUS0_CHANNEL_UP;
  assign Aurora4_CORE_STATUS_frame_err = AuroraB_CORE_STATUS0_FRAME_ERR;
  assign Aurora4_CORE_STATUS_hard_err = AuroraB_CORE_STATUS0_HARD_ERR;
  assign Aurora4_CORE_STATUS_lane_up[0] = AuroraB_CORE_STATUS0_LANE_UP;
  assign Aurora4_CORE_STATUS_rx_resetdone_out = AuroraB_CORE_STATUS0_RX_RESETDONE_OUT;
  assign Aurora4_CORE_STATUS_soft_err = AuroraB_CORE_STATUS0_SOFT_ERR;
  assign Aurora4_CORE_STATUS_tx_lock = AuroraB_CORE_STATUS0_TX_LOCK;
  assign Aurora4_CORE_STATUS_tx_resetdone_out = AuroraB_CORE_STATUS0_TX_RESETDONE_OUT;
  assign Aurora4_TX_txn[0] = AuroraB_GT_SERIAL_TX0_TXN;
  assign Aurora4_TX_txp[0] = AuroraB_GT_SERIAL_TX0_TXP;
  assign Aurora4_USER_DATA_M_AXI_RX_tdata[15:0] = AuroraB_USER_DATA_M_AXI_RX0_TDATA;
  assign Aurora4_USER_DATA_M_AXI_RX_tkeep[1:0] = AuroraB_USER_DATA_M_AXI_RX0_TKEEP;
  assign Aurora4_USER_DATA_M_AXI_RX_tlast = AuroraB_USER_DATA_M_AXI_RX0_TLAST;
  assign Aurora4_USER_DATA_M_AXI_RX_tvalid = AuroraB_USER_DATA_M_AXI_RX0_TVALID;
  assign Aurora4_USER_DATA_S_AXI_TX_tready = USER_DATA_S_AXI_TX0_1_TREADY;
  assign Aurora5_CORE_STATUS_channel_up = AuroraB_CORE_STATUS1_CHANNEL_UP;
  assign Aurora5_CORE_STATUS_frame_err = AuroraB_CORE_STATUS1_FRAME_ERR;
  assign Aurora5_CORE_STATUS_hard_err = AuroraB_CORE_STATUS1_HARD_ERR;
  assign Aurora5_CORE_STATUS_lane_up[0] = AuroraB_CORE_STATUS1_LANE_UP;
  assign Aurora5_CORE_STATUS_rx_resetdone_out = AuroraB_CORE_STATUS1_RX_RESETDONE_OUT;
  assign Aurora5_CORE_STATUS_soft_err = AuroraB_CORE_STATUS1_SOFT_ERR;
  assign Aurora5_CORE_STATUS_tx_lock = AuroraB_CORE_STATUS1_TX_LOCK;
  assign Aurora5_CORE_STATUS_tx_resetdone_out = AuroraB_CORE_STATUS1_TX_RESETDONE_OUT;
  assign Aurora5_TX_txn[0] = AuroraB_GT_SERIAL_TX1_TXN;
  assign Aurora5_TX_txp[0] = AuroraB_GT_SERIAL_TX1_TXP;
  assign Aurora5_USER_DATA_M_AXI_RX_tdata[15:0] = AuroraB_USER_DATA_M_AXI_RX1_TDATA;
  assign Aurora5_USER_DATA_M_AXI_RX_tkeep[1:0] = AuroraB_USER_DATA_M_AXI_RX1_TKEEP;
  assign Aurora5_USER_DATA_M_AXI_RX_tlast = AuroraB_USER_DATA_M_AXI_RX1_TLAST;
  assign Aurora5_USER_DATA_M_AXI_RX_tvalid = AuroraB_USER_DATA_M_AXI_RX1_TVALID;
  assign Aurora5_USER_DATA_S_AXI_TX_tready = USER_DATA_S_AXI_TX1_1_TREADY;
  assign Aurora6_CORE_STATUS_channel_up = AuroraB_CORE_STATUS2_CHANNEL_UP;
  assign Aurora6_CORE_STATUS_frame_err = AuroraB_CORE_STATUS2_FRAME_ERR;
  assign Aurora6_CORE_STATUS_hard_err = AuroraB_CORE_STATUS2_HARD_ERR;
  assign Aurora6_CORE_STATUS_lane_up[0] = AuroraB_CORE_STATUS2_LANE_UP;
  assign Aurora6_CORE_STATUS_rx_resetdone_out = AuroraB_CORE_STATUS2_RX_RESETDONE_OUT;
  assign Aurora6_CORE_STATUS_soft_err = AuroraB_CORE_STATUS2_SOFT_ERR;
  assign Aurora6_CORE_STATUS_tx_lock = AuroraB_CORE_STATUS2_TX_LOCK;
  assign Aurora6_CORE_STATUS_tx_resetdone_out = AuroraB_CORE_STATUS2_TX_RESETDONE_OUT;
  assign Aurora6_TX_txn[0] = AuroraB_GT_SERIAL_TX2_TXN;
  assign Aurora6_TX_txp[0] = AuroraB_GT_SERIAL_TX2_TXP;
  assign Aurora6_USER_DATA_M_AXI_RX_tdata[15:0] = AuroraB_USER_DATA_M_AXI_RX2_TDATA;
  assign Aurora6_USER_DATA_M_AXI_RX_tkeep[1:0] = AuroraB_USER_DATA_M_AXI_RX2_TKEEP;
  assign Aurora6_USER_DATA_M_AXI_RX_tlast = AuroraB_USER_DATA_M_AXI_RX2_TLAST;
  assign Aurora6_USER_DATA_M_AXI_RX_tvalid = AuroraB_USER_DATA_M_AXI_RX2_TVALID;
  assign Aurora6_USER_DATA_S_AXI_TX_tready = USER_DATA_S_AXI_TX2_1_TREADY;
  assign Aurora7_CORE_STATUS_channel_up = AuroraB_CORE_STATUS3_CHANNEL_UP;
  assign Aurora7_CORE_STATUS_frame_err = AuroraB_CORE_STATUS3_FRAME_ERR;
  assign Aurora7_CORE_STATUS_hard_err = AuroraB_CORE_STATUS3_HARD_ERR;
  assign Aurora7_CORE_STATUS_lane_up[0] = AuroraB_CORE_STATUS3_LANE_UP;
  assign Aurora7_CORE_STATUS_rx_resetdone_out = AuroraB_CORE_STATUS3_RX_RESETDONE_OUT;
  assign Aurora7_CORE_STATUS_soft_err = AuroraB_CORE_STATUS3_SOFT_ERR;
  assign Aurora7_CORE_STATUS_tx_lock = AuroraB_CORE_STATUS3_TX_LOCK;
  assign Aurora7_CORE_STATUS_tx_resetdone_out = AuroraB_CORE_STATUS3_TX_RESETDONE_OUT;
  assign Aurora7_TX_txn[0] = AuroraB_GT_SERIAL_TX3_TXN;
  assign Aurora7_TX_txp[0] = AuroraB_GT_SERIAL_TX3_TXP;
  assign Aurora7_USER_DATA_M_AXI_RX_tdata[15:0] = AuroraB_USER_DATA_M_AXI_RX3_TDATA;
  assign Aurora7_USER_DATA_M_AXI_RX_tkeep[1:0] = AuroraB_USER_DATA_M_AXI_RX3_TKEEP;
  assign Aurora7_USER_DATA_M_AXI_RX_tlast = AuroraB_USER_DATA_M_AXI_RX3_TLAST;
  assign Aurora7_USER_DATA_M_AXI_RX_tvalid = AuroraB_USER_DATA_M_AXI_RX3_TVALID;
  assign Aurora7_USER_DATA_S_AXI_TX_tready = USER_DATA_S_AXI_TX3_1_TREADY;
  assign AuroraB_USER_DATA_M_AXI_RX0_TREADY = Aurora4_USER_DATA_M_AXI_RX_tready;
  assign AuroraB_USER_DATA_M_AXI_RX1_TREADY = Aurora5_USER_DATA_M_AXI_RX_tready;
  assign AuroraB_USER_DATA_M_AXI_RX2_TREADY = Aurora6_USER_DATA_M_AXI_RX_tready;
  assign AuroraB_USER_DATA_M_AXI_RX3_TREADY = Aurora7_USER_DATA_M_AXI_RX_tready;
  assign DRP0_daddr[8:0] = drp_bridge_0_DRP0_DADDR;
  assign DRP0_den = drp_bridge_0_DRP0_DEN;
  assign DRP0_di[15:0] = drp_bridge_0_DRP0_DI;
  assign DRP0_dwe = drp_bridge_0_DRP0_DWE;
  assign GPIO_AURORA_A_tri_o[31:0] = GPIO_AURORA_GPIO_TRI_O;
  assign GPIO_AURORA_A_tri_t[31:0] = GPIO_AURORA_GPIO_TRI_T;
  assign GPIO_AURORA_GPIO2_TRI_I = GPIO_AURORA_B_tri_i[31:0];
  assign GPIO_AURORA_GPIO_TRI_I = GPIO_AURORA_A_tri_i[31:0];
  assign GPIO_EVR_GTX_GPIO_TRI_I = GPIO_EVR_GTX_tri_i[3:0];
  assign GPIO_EVR_GTX_tri_o[3:0] = GPIO_EVR_GTX_GPIO_TRI_O;
  assign GPIO_EVR_GTX_tri_t[3:0] = GPIO_EVR_GTX_GPIO_TRI_T;
  assign GPIO_GENERIC0_GPIO2_TRI_I = GPIO_FAN_SPEED_tri_i[31:0];
  assign GPIO_GENERIC0_GPIO_TRI_I = GPIO_FIRMWARE_DATE_tri_i[31:0];
  assign GPIO_MITIGATION_CONTROL_tri_o[31:0] = axi_gpio_0_GPIO1_TRI_O;
  assign GPIO_SFP_GPIO_TRI_I = GPIO_SFP_STATUS_tri_i[31:0];
  assign GPIO_STALE_NODES_GPIO2_TRI_I = GPIO_STALE_NODES_HI_tri_i[31:0];
  assign GPIO_STALE_NODES_GPIO_TRI_I = GPIO_STALE_NODES_LO_tri_i[31:0];
  assign GPIO_SYSMON_GPIO_TRI_I = GPIO_SYSMON_READOUT_tri_i[15:0];
  assign GPIO_SYSMON_READOUT_tri_o[15:0] = GPIO_SYSMON_GPIO_TRI_O;
  assign GPIO_SYSMON_READOUT_tri_t[15:0] = GPIO_SYSMON_GPIO_TRI_T;
  assign GT_DIFF_REFCLK1_1_CLK_N = AuroraArefclk_clk_n;
  assign GT_DIFF_REFCLK1_1_CLK_P = AuroraArefclk_clk_p;
  assign GT_DIFF_REFCLK1_2_CLK_N = AuroraBrefclk_clk_n;
  assign GT_DIFF_REFCLK1_2_CLK_P = AuroraBrefclk_clk_p;
  assign GT_SERIAL_RX0_1_RXN = Aurora4_RX_rxn[0];
  assign GT_SERIAL_RX0_1_RXP = Aurora4_RX_rxp[0];
  assign GT_SERIAL_RX1_1_RXN = Aurora5_RX_rxn[0];
  assign GT_SERIAL_RX1_1_RXP = Aurora5_RX_rxp[0];
  assign GT_SERIAL_RX2_1_RXN = Aurora6_RX_rxn[0];
  assign GT_SERIAL_RX2_1_RXP = Aurora6_RX_rxp[0];
  assign GT_SERIAL_RX3_1_RXN = Aurora7_RX_rxn[0];
  assign GT_SERIAL_RX3_1_RXP = Aurora7_RX_rxp[0];
  assign GT_SERIAL_RX_1_1_RXN = Aurora3_RX_rxn[0];
  assign GT_SERIAL_RX_1_1_RXP = Aurora3_RX_rxp[0];
  assign GT_SERIAL_RX_1_RXN = Aurora0_RX_rxn[0];
  assign GT_SERIAL_RX_1_RXP = Aurora0_RX_rxp[0];
  assign GT_SERIAL_RX_2_RXN = Aurora1_RX_rxn[0];
  assign GT_SERIAL_RX_2_RXP = Aurora1_RX_rxp[0];
  assign GT_SERIAL_RX_3_RXN = Aurora2_RX_rxn[0];
  assign GT_SERIAL_RX_3_RXP = Aurora2_RX_rxp[0];
  assign Log_AXIS_tready = S00_AXIS_1_TREADY;
  assign Marker_AXIS_tready = S01_AXIS_1_TREADY;
  assign S00_AXIS_1_TDATA = Log_AXIS_tdata[31:0];
  assign S00_AXIS_1_TLAST = Log_AXIS_tlast;
  assign S00_AXIS_1_TVALID = Log_AXIS_tvalid;
  assign S01_AXIS_1_TDATA = Marker_AXIS_tdata[31:0];
  assign S01_AXIS_1_TLAST = Marker_AXIS_tlast;
  assign S01_AXIS_1_TVALID = Marker_AXIS_tvalid;
  assign SFP_IIC_SELECT[2:0] = axi_iic_0_gpo;
  assign SFP_IIC_scl_o = axi_iic_0_IIC_SCL_O;
  assign SFP_IIC_scl_t = axi_iic_0_IIC_SCL_T;
  assign SFP_IIC_sda_o = axi_iic_0_IIC_SDA_O;
  assign SFP_IIC_sda_t = axi_iic_0_IIC_SDA_T;
  assign USER_DATA_S_AXI_TX0_1_TDATA = Aurora4_USER_DATA_S_AXI_TX_tdata[0:15];
  assign USER_DATA_S_AXI_TX0_1_TKEEP = Aurora4_USER_DATA_S_AXI_TX_tkeep[0:1];
  assign USER_DATA_S_AXI_TX0_1_TLAST = Aurora4_USER_DATA_S_AXI_TX_tlast;
  assign USER_DATA_S_AXI_TX0_1_TVALID = Aurora4_USER_DATA_S_AXI_TX_tvalid;
  assign USER_DATA_S_AXI_TX1_1_TDATA = Aurora5_USER_DATA_S_AXI_TX_tdata[0:15];
  assign USER_DATA_S_AXI_TX1_1_TKEEP = Aurora5_USER_DATA_S_AXI_TX_tkeep[0:1];
  assign USER_DATA_S_AXI_TX1_1_TLAST = Aurora5_USER_DATA_S_AXI_TX_tlast;
  assign USER_DATA_S_AXI_TX1_1_TVALID = Aurora5_USER_DATA_S_AXI_TX_tvalid;
  assign USER_DATA_S_AXI_TX2_1_TDATA = Aurora6_USER_DATA_S_AXI_TX_tdata[0:15];
  assign USER_DATA_S_AXI_TX2_1_TKEEP = Aurora6_USER_DATA_S_AXI_TX_tkeep[0:1];
  assign USER_DATA_S_AXI_TX2_1_TLAST = Aurora6_USER_DATA_S_AXI_TX_tlast;
  assign USER_DATA_S_AXI_TX2_1_TVALID = Aurora6_USER_DATA_S_AXI_TX_tvalid;
  assign USER_DATA_S_AXI_TX3_1_TDATA = Aurora7_USER_DATA_S_AXI_TX_tdata[0:15];
  assign USER_DATA_S_AXI_TX3_1_TKEEP = Aurora7_USER_DATA_S_AXI_TX_tkeep[0:1];
  assign USER_DATA_S_AXI_TX3_1_TLAST = Aurora7_USER_DATA_S_AXI_TX_tlast;
  assign USER_DATA_S_AXI_TX3_1_TVALID = Aurora7_USER_DATA_S_AXI_TX_tvalid;
  assign USER_DATA_S_AXI_TX_1_1_TDATA = Aurora3_USER_DATA_S_AXI_TX_tdata[0:15];
  assign USER_DATA_S_AXI_TX_1_1_TKEEP = Aurora3_USER_DATA_S_AXI_TX_tkeep[0:1];
  assign USER_DATA_S_AXI_TX_1_1_TLAST = Aurora3_USER_DATA_S_AXI_TX_tlast;
  assign USER_DATA_S_AXI_TX_1_1_TVALID = Aurora3_USER_DATA_S_AXI_TX_tvalid;
  assign USER_DATA_S_AXI_TX_1_TDATA = Aurora0_USER_DATA_S_AXI_TX_tdata[0:15];
  assign USER_DATA_S_AXI_TX_1_TKEEP = Aurora0_USER_DATA_S_AXI_TX_tkeep[0:1];
  assign USER_DATA_S_AXI_TX_1_TLAST = Aurora0_USER_DATA_S_AXI_TX_tlast;
  assign USER_DATA_S_AXI_TX_1_TVALID = Aurora0_USER_DATA_S_AXI_TX_tvalid;
  assign USER_DATA_S_AXI_TX_2_TDATA = Aurora1_USER_DATA_S_AXI_TX_tdata[0:15];
  assign USER_DATA_S_AXI_TX_2_TKEEP = Aurora1_USER_DATA_S_AXI_TX_tkeep[0:1];
  assign USER_DATA_S_AXI_TX_2_TLAST = Aurora1_USER_DATA_S_AXI_TX_tlast;
  assign USER_DATA_S_AXI_TX_2_TVALID = Aurora1_USER_DATA_S_AXI_TX_tvalid;
  assign USER_DATA_S_AXI_TX_3_TDATA = Aurora2_USER_DATA_S_AXI_TX_tdata[0:15];
  assign USER_DATA_S_AXI_TX_3_TKEEP = Aurora2_USER_DATA_S_AXI_TX_tkeep[0:1];
  assign USER_DATA_S_AXI_TX_3_TLAST = Aurora2_USER_DATA_S_AXI_TX_tlast;
  assign USER_DATA_S_AXI_TX_3_TVALID = Aurora2_USER_DATA_S_AXI_TX_tvalid;
  assign auroraUserClk = Aurora_user_clk_out;
  assign axi_bram_ctrl_0_BRAM_PORTA_DOUT = mitigationBRAMctrl_dout[31:0];
  assign axi_gpio_0_GPIO2_TRI_I = GPIO_TIMESTAMP_TICKS_tri_i[31:0];
  assign axi_gpio_0_GPIO_TRI_I = GPIO_TIMESTAMP_SECONDS_tri_i[31:0];
  assign axi_iic_0_IIC_SCL_I = SFP_IIC_scl_i;
  assign axi_iic_0_IIC_SDA_I = SFP_IIC_sda_i;
  assign drp_bridge_0_DRP0_DO = DRP0_do[15:0];
  assign drp_bridge_0_DRP0_DRDY = DRP0_drdy;
  assign gt_reset_1 = GTXreset;
  assign mitigationBRAMctrl_addr[15:0] = axi_bram_ctrl_0_BRAM_PORTA_ADDR;
  assign mitigationBRAMctrl_clk = axi_bram_ctrl_0_BRAM_PORTA_CLK;
  assign mitigationBRAMctrl_din[31:0] = axi_bram_ctrl_0_BRAM_PORTA_DIN;
  assign mitigationBRAMctrl_en = axi_bram_ctrl_0_BRAM_PORTA_EN;
  assign mitigationBRAMctrl_rst = axi_bram_ctrl_0_BRAM_PORTA_RST;
  assign mitigationBRAMctrl_we[3:0] = axi_bram_ctrl_0_BRAM_PORTA_WE;
  assign mitigationClk = processing_system7_0_FCLK_CLK1;
  assign reset_1 = AuroraReset;
  assign sysClk = processing_system7_0_FCLK_CLK0;
  assign sysReset_n[0] = rst_processing_system7_0_50M_peripheral_aresetn;
  AuroraA_imp_14786E AuroraA
       (.CORE_STATUS0_channel_up(aurora_8b10b_0_CORE_STATUS_CHANNEL_UP),
        .CORE_STATUS0_frame_err(aurora_8b10b_0_CORE_STATUS_FRAME_ERR),
        .CORE_STATUS0_hard_err(aurora_8b10b_0_CORE_STATUS_HARD_ERR),
        .CORE_STATUS0_lane_up(aurora_8b10b_0_CORE_STATUS_LANE_UP),
        .CORE_STATUS0_rx_resetdone_out(aurora_8b10b_0_CORE_STATUS_RX_RESETDONE_OUT),
        .CORE_STATUS0_soft_err(aurora_8b10b_0_CORE_STATUS_SOFT_ERR),
        .CORE_STATUS0_tx_lock(aurora_8b10b_0_CORE_STATUS_TX_LOCK),
        .CORE_STATUS0_tx_resetdone_out(aurora_8b10b_0_CORE_STATUS_TX_RESETDONE_OUT),
        .CORE_STATUS1_channel_up(Aurora_CORE_STATUS1_CHANNEL_UP),
        .CORE_STATUS1_frame_err(Aurora_CORE_STATUS1_FRAME_ERR),
        .CORE_STATUS1_hard_err(Aurora_CORE_STATUS1_HARD_ERR),
        .CORE_STATUS1_lane_up(Aurora_CORE_STATUS1_LANE_UP),
        .CORE_STATUS1_rx_resetdone_out(Aurora_CORE_STATUS1_RX_RESETDONE_OUT),
        .CORE_STATUS1_soft_err(Aurora_CORE_STATUS1_SOFT_ERR),
        .CORE_STATUS1_tx_lock(Aurora_CORE_STATUS1_TX_LOCK),
        .CORE_STATUS1_tx_resetdone_out(Aurora_CORE_STATUS1_TX_RESETDONE_OUT),
        .CORE_STATUS2_channel_up(Aurora_CORE_STATUS_CHANNEL_UP),
        .CORE_STATUS2_frame_err(Aurora_CORE_STATUS_FRAME_ERR),
        .CORE_STATUS2_hard_err(Aurora_CORE_STATUS_HARD_ERR),
        .CORE_STATUS2_lane_up(Aurora_CORE_STATUS_LANE_UP),
        .CORE_STATUS2_rx_resetdone_out(Aurora_CORE_STATUS_RX_RESETDONE_OUT),
        .CORE_STATUS2_soft_err(Aurora_CORE_STATUS_SOFT_ERR),
        .CORE_STATUS2_tx_lock(Aurora_CORE_STATUS_TX_LOCK),
        .CORE_STATUS2_tx_resetdone_out(Aurora_CORE_STATUS_TX_RESETDONE_OUT),
        .CORE_STATUS3_channel_up(Aurora_CORE_STATUS2_CHANNEL_UP),
        .CORE_STATUS3_frame_err(Aurora_CORE_STATUS2_FRAME_ERR),
        .CORE_STATUS3_hard_err(Aurora_CORE_STATUS2_HARD_ERR),
        .CORE_STATUS3_lane_up(Aurora_CORE_STATUS2_LANE_UP),
        .CORE_STATUS3_rx_resetdone_out(Aurora_CORE_STATUS2_RX_RESETDONE_OUT),
        .CORE_STATUS3_soft_err(Aurora_CORE_STATUS2_SOFT_ERR),
        .CORE_STATUS3_tx_lock(Aurora_CORE_STATUS2_TX_LOCK),
        .CORE_STATUS3_tx_resetdone_out(Aurora_CORE_STATUS2_TX_RESETDONE_OUT),
        .GT0_DRP_IF0_daddr(drp_bridge_0_DRP1_DADDR),
        .GT0_DRP_IF0_den(drp_bridge_0_DRP1_DEN),
        .GT0_DRP_IF0_di(drp_bridge_0_DRP1_DI),
        .GT0_DRP_IF0_do(drp_bridge_0_DRP1_DO),
        .GT0_DRP_IF0_drdy(drp_bridge_0_DRP1_DRDY),
        .GT0_DRP_IF0_dwe(drp_bridge_0_DRP1_DWE),
        .GT0_DRP_IF1_daddr(drp_bridge_0_DRP2_DADDR),
        .GT0_DRP_IF1_den(drp_bridge_0_DRP2_DEN),
        .GT0_DRP_IF1_di(drp_bridge_0_DRP2_DI),
        .GT0_DRP_IF1_do(drp_bridge_0_DRP2_DO),
        .GT0_DRP_IF1_drdy(drp_bridge_0_DRP2_DRDY),
        .GT0_DRP_IF1_dwe(drp_bridge_0_DRP2_DWE),
        .GT0_DRP_IF2_daddr(drp_bridge_0_DRP3_DADDR),
        .GT0_DRP_IF2_den(drp_bridge_0_DRP3_DEN),
        .GT0_DRP_IF2_di(drp_bridge_0_DRP3_DI),
        .GT0_DRP_IF2_do(drp_bridge_0_DRP3_DO),
        .GT0_DRP_IF2_drdy(drp_bridge_0_DRP3_DRDY),
        .GT0_DRP_IF2_dwe(drp_bridge_0_DRP3_DWE),
        .GT0_DRP_IF3_daddr(drp_bridge_0_DRP4_DADDR),
        .GT0_DRP_IF3_den(drp_bridge_0_DRP4_DEN),
        .GT0_DRP_IF3_di(drp_bridge_0_DRP4_DI),
        .GT0_DRP_IF3_do(drp_bridge_0_DRP4_DO),
        .GT0_DRP_IF3_drdy(drp_bridge_0_DRP4_DRDY),
        .GT0_DRP_IF3_dwe(drp_bridge_0_DRP4_DWE),
        .GT_DIFF_REFCLK1_clk_n(GT_DIFF_REFCLK1_1_CLK_N),
        .GT_DIFF_REFCLK1_clk_p(GT_DIFF_REFCLK1_1_CLK_P),
        .GT_SERIAL_RX0_rxn(GT_SERIAL_RX_1_RXN),
        .GT_SERIAL_RX0_rxp(GT_SERIAL_RX_1_RXP),
        .GT_SERIAL_RX1_rxn(GT_SERIAL_RX_2_RXN),
        .GT_SERIAL_RX1_rxp(GT_SERIAL_RX_2_RXP),
        .GT_SERIAL_RX2_rxn(GT_SERIAL_RX_3_RXN),
        .GT_SERIAL_RX2_rxp(GT_SERIAL_RX_3_RXP),
        .GT_SERIAL_RX3_rxn(GT_SERIAL_RX_1_1_RXN),
        .GT_SERIAL_RX3_rxp(GT_SERIAL_RX_1_1_RXP),
        .GT_SERIAL_TX0_txn(aurora_8b10b_0_GT_SERIAL_TX_TXN),
        .GT_SERIAL_TX0_txp(aurora_8b10b_0_GT_SERIAL_TX_TXP),
        .GT_SERIAL_TX1_txn(Aurora_GT_SERIAL_TX1_TXN),
        .GT_SERIAL_TX1_txp(Aurora_GT_SERIAL_TX1_TXP),
        .GT_SERIAL_TX2_txn(Aurora_GT_SERIAL_TX_TXN),
        .GT_SERIAL_TX2_txp(Aurora_GT_SERIAL_TX_TXP),
        .GT_SERIAL_TX3_txn(Aurora_GT_SERIAL_TX2_TXN),
        .GT_SERIAL_TX3_txp(Aurora_GT_SERIAL_TX2_TXP),
        .USER_DATA_M_AXI_RX0_tdata(aurora_8b10b_0_USER_DATA_M_AXI_RX_TDATA),
        .USER_DATA_M_AXI_RX0_tkeep(aurora_8b10b_0_USER_DATA_M_AXI_RX_TKEEP),
        .USER_DATA_M_AXI_RX0_tlast(aurora_8b10b_0_USER_DATA_M_AXI_RX_TLAST),
        .USER_DATA_M_AXI_RX0_tvalid(aurora_8b10b_0_USER_DATA_M_AXI_RX_TVALID),
        .USER_DATA_M_AXI_RX1_tdata(Aurora_USER_DATA_M_AXI_RX1_TDATA),
        .USER_DATA_M_AXI_RX1_tkeep(Aurora_USER_DATA_M_AXI_RX1_TKEEP),
        .USER_DATA_M_AXI_RX1_tlast(Aurora_USER_DATA_M_AXI_RX1_TLAST),
        .USER_DATA_M_AXI_RX1_tvalid(Aurora_USER_DATA_M_AXI_RX1_TVALID),
        .USER_DATA_M_AXI_RX2_tdata(Aurora_USER_DATA_M_AXI_RX_TDATA),
        .USER_DATA_M_AXI_RX2_tkeep(Aurora_USER_DATA_M_AXI_RX_TKEEP),
        .USER_DATA_M_AXI_RX2_tlast(Aurora_USER_DATA_M_AXI_RX_TLAST),
        .USER_DATA_M_AXI_RX2_tvalid(Aurora_USER_DATA_M_AXI_RX_TVALID),
        .USER_DATA_M_AXI_RX3_tdata(Aurora_USER_DATA_M_AXI_RX2_TDATA),
        .USER_DATA_M_AXI_RX3_tkeep(Aurora_USER_DATA_M_AXI_RX2_TKEEP),
        .USER_DATA_M_AXI_RX3_tlast(Aurora_USER_DATA_M_AXI_RX2_TLAST),
        .USER_DATA_M_AXI_RX3_tvalid(Aurora_USER_DATA_M_AXI_RX2_TVALID),
        .USER_DATA_S_AXI_TX0_tdata(USER_DATA_S_AXI_TX_1_TDATA),
        .USER_DATA_S_AXI_TX0_tkeep(USER_DATA_S_AXI_TX_1_TKEEP),
        .USER_DATA_S_AXI_TX0_tlast(USER_DATA_S_AXI_TX_1_TLAST),
        .USER_DATA_S_AXI_TX0_tready(USER_DATA_S_AXI_TX_1_TREADY),
        .USER_DATA_S_AXI_TX0_tvalid(USER_DATA_S_AXI_TX_1_TVALID),
        .USER_DATA_S_AXI_TX1_tdata(USER_DATA_S_AXI_TX_2_TDATA),
        .USER_DATA_S_AXI_TX1_tkeep(USER_DATA_S_AXI_TX_2_TKEEP),
        .USER_DATA_S_AXI_TX1_tlast(USER_DATA_S_AXI_TX_2_TLAST),
        .USER_DATA_S_AXI_TX1_tready(USER_DATA_S_AXI_TX_2_TREADY),
        .USER_DATA_S_AXI_TX1_tvalid(USER_DATA_S_AXI_TX_2_TVALID),
        .USER_DATA_S_AXI_TX2_tdata(USER_DATA_S_AXI_TX_3_TDATA),
        .USER_DATA_S_AXI_TX2_tkeep(USER_DATA_S_AXI_TX_3_TKEEP),
        .USER_DATA_S_AXI_TX2_tlast(USER_DATA_S_AXI_TX_3_TLAST),
        .USER_DATA_S_AXI_TX2_tready(USER_DATA_S_AXI_TX_3_TREADY),
        .USER_DATA_S_AXI_TX2_tvalid(USER_DATA_S_AXI_TX_3_TVALID),
        .USER_DATA_S_AXI_TX3_tdata(USER_DATA_S_AXI_TX_1_1_TDATA),
        .USER_DATA_S_AXI_TX3_tkeep(USER_DATA_S_AXI_TX_1_1_TKEEP),
        .USER_DATA_S_AXI_TX3_tlast(USER_DATA_S_AXI_TX_1_1_TLAST),
        .USER_DATA_S_AXI_TX3_tready(USER_DATA_S_AXI_TX_1_1_TREADY),
        .USER_DATA_S_AXI_TX3_tvalid(USER_DATA_S_AXI_TX_1_1_TVALID),
        .auroraReset(reset_1),
        .auroraUserClk(Aurora_user_clk_out),
        .gtxReset(gt_reset_1),
        .init_clk_in(processing_system7_0_FCLK_CLK0));
  AuroraB_imp_OB3XCY AuroraB
       (.CORE_STATUS0_channel_up(AuroraB_CORE_STATUS0_CHANNEL_UP),
        .CORE_STATUS0_frame_err(AuroraB_CORE_STATUS0_FRAME_ERR),
        .CORE_STATUS0_hard_err(AuroraB_CORE_STATUS0_HARD_ERR),
        .CORE_STATUS0_lane_up(AuroraB_CORE_STATUS0_LANE_UP),
        .CORE_STATUS0_rx_resetdone_out(AuroraB_CORE_STATUS0_RX_RESETDONE_OUT),
        .CORE_STATUS0_soft_err(AuroraB_CORE_STATUS0_SOFT_ERR),
        .CORE_STATUS0_tx_lock(AuroraB_CORE_STATUS0_TX_LOCK),
        .CORE_STATUS0_tx_resetdone_out(AuroraB_CORE_STATUS0_TX_RESETDONE_OUT),
        .CORE_STATUS1_channel_up(AuroraB_CORE_STATUS1_CHANNEL_UP),
        .CORE_STATUS1_frame_err(AuroraB_CORE_STATUS1_FRAME_ERR),
        .CORE_STATUS1_hard_err(AuroraB_CORE_STATUS1_HARD_ERR),
        .CORE_STATUS1_lane_up(AuroraB_CORE_STATUS1_LANE_UP),
        .CORE_STATUS1_rx_resetdone_out(AuroraB_CORE_STATUS1_RX_RESETDONE_OUT),
        .CORE_STATUS1_soft_err(AuroraB_CORE_STATUS1_SOFT_ERR),
        .CORE_STATUS1_tx_lock(AuroraB_CORE_STATUS1_TX_LOCK),
        .CORE_STATUS1_tx_resetdone_out(AuroraB_CORE_STATUS1_TX_RESETDONE_OUT),
        .CORE_STATUS2_channel_up(AuroraB_CORE_STATUS2_CHANNEL_UP),
        .CORE_STATUS2_frame_err(AuroraB_CORE_STATUS2_FRAME_ERR),
        .CORE_STATUS2_hard_err(AuroraB_CORE_STATUS2_HARD_ERR),
        .CORE_STATUS2_lane_up(AuroraB_CORE_STATUS2_LANE_UP),
        .CORE_STATUS2_rx_resetdone_out(AuroraB_CORE_STATUS2_RX_RESETDONE_OUT),
        .CORE_STATUS2_soft_err(AuroraB_CORE_STATUS2_SOFT_ERR),
        .CORE_STATUS2_tx_lock(AuroraB_CORE_STATUS2_TX_LOCK),
        .CORE_STATUS2_tx_resetdone_out(AuroraB_CORE_STATUS2_TX_RESETDONE_OUT),
        .CORE_STATUS3_channel_up(AuroraB_CORE_STATUS3_CHANNEL_UP),
        .CORE_STATUS3_frame_err(AuroraB_CORE_STATUS3_FRAME_ERR),
        .CORE_STATUS3_hard_err(AuroraB_CORE_STATUS3_HARD_ERR),
        .CORE_STATUS3_lane_up(AuroraB_CORE_STATUS3_LANE_UP),
        .CORE_STATUS3_rx_resetdone_out(AuroraB_CORE_STATUS3_RX_RESETDONE_OUT),
        .CORE_STATUS3_soft_err(AuroraB_CORE_STATUS3_SOFT_ERR),
        .CORE_STATUS3_tx_lock(AuroraB_CORE_STATUS3_TX_LOCK),
        .CORE_STATUS3_tx_resetdone_out(AuroraB_CORE_STATUS3_TX_RESETDONE_OUT),
        .CommonAuroraUserClk(Aurora_user_clk_out),
        .GT0_DRP_IF0_daddr(drp_bridge_0_DRP5_DADDR),
        .GT0_DRP_IF0_den(drp_bridge_0_DRP5_DEN),
        .GT0_DRP_IF0_di(drp_bridge_0_DRP5_DI),
        .GT0_DRP_IF0_do(drp_bridge_0_DRP5_DO),
        .GT0_DRP_IF0_drdy(drp_bridge_0_DRP5_DRDY),
        .GT0_DRP_IF0_dwe(drp_bridge_0_DRP5_DWE),
        .GT0_DRP_IF1_daddr(drp_bridge_0_DRP6_DADDR),
        .GT0_DRP_IF1_den(drp_bridge_0_DRP6_DEN),
        .GT0_DRP_IF1_di(drp_bridge_0_DRP6_DI),
        .GT0_DRP_IF1_do(drp_bridge_0_DRP6_DO),
        .GT0_DRP_IF1_drdy(drp_bridge_0_DRP6_DRDY),
        .GT0_DRP_IF1_dwe(drp_bridge_0_DRP6_DWE),
        .GT0_DRP_IF2_daddr(drp_bridge_0_DRP7_DADDR),
        .GT0_DRP_IF2_den(drp_bridge_0_DRP7_DEN),
        .GT0_DRP_IF2_di(drp_bridge_0_DRP7_DI),
        .GT0_DRP_IF2_do(drp_bridge_0_DRP7_DO),
        .GT0_DRP_IF2_drdy(drp_bridge_0_DRP7_DRDY),
        .GT0_DRP_IF2_dwe(drp_bridge_0_DRP7_DWE),
        .GT0_DRP_IF3_daddr(drp_bridge_0_DRP8_DADDR),
        .GT0_DRP_IF3_den(drp_bridge_0_DRP8_DEN),
        .GT0_DRP_IF3_di(drp_bridge_0_DRP8_DI),
        .GT0_DRP_IF3_do(drp_bridge_0_DRP8_DO),
        .GT0_DRP_IF3_drdy(drp_bridge_0_DRP8_DRDY),
        .GT0_DRP_IF3_dwe(drp_bridge_0_DRP8_DWE),
        .GT_DIFF_REFCLK1_clk_n(GT_DIFF_REFCLK1_2_CLK_N),
        .GT_DIFF_REFCLK1_clk_p(GT_DIFF_REFCLK1_2_CLK_P),
        .GT_SERIAL_RX0_rxn(GT_SERIAL_RX0_1_RXN),
        .GT_SERIAL_RX0_rxp(GT_SERIAL_RX0_1_RXP),
        .GT_SERIAL_RX1_rxn(GT_SERIAL_RX1_1_RXN),
        .GT_SERIAL_RX1_rxp(GT_SERIAL_RX1_1_RXP),
        .GT_SERIAL_RX2_rxn(GT_SERIAL_RX2_1_RXN),
        .GT_SERIAL_RX2_rxp(GT_SERIAL_RX2_1_RXP),
        .GT_SERIAL_RX3_rxn(GT_SERIAL_RX3_1_RXN),
        .GT_SERIAL_RX3_rxp(GT_SERIAL_RX3_1_RXP),
        .GT_SERIAL_TX0_txn(AuroraB_GT_SERIAL_TX0_TXN),
        .GT_SERIAL_TX0_txp(AuroraB_GT_SERIAL_TX0_TXP),
        .GT_SERIAL_TX1_txn(AuroraB_GT_SERIAL_TX1_TXN),
        .GT_SERIAL_TX1_txp(AuroraB_GT_SERIAL_TX1_TXP),
        .GT_SERIAL_TX2_txn(AuroraB_GT_SERIAL_TX2_TXN),
        .GT_SERIAL_TX2_txp(AuroraB_GT_SERIAL_TX2_TXP),
        .GT_SERIAL_TX3_txn(AuroraB_GT_SERIAL_TX3_TXN),
        .GT_SERIAL_TX3_txp(AuroraB_GT_SERIAL_TX3_TXP),
        .USER_DATA_M_AXI_RX0_tdata(AuroraB_USER_DATA_M_AXI_RX0_TDATA),
        .USER_DATA_M_AXI_RX0_tkeep(AuroraB_USER_DATA_M_AXI_RX0_TKEEP),
        .USER_DATA_M_AXI_RX0_tlast(AuroraB_USER_DATA_M_AXI_RX0_TLAST),
        .USER_DATA_M_AXI_RX0_tready(AuroraB_USER_DATA_M_AXI_RX0_TREADY),
        .USER_DATA_M_AXI_RX0_tvalid(AuroraB_USER_DATA_M_AXI_RX0_TVALID),
        .USER_DATA_M_AXI_RX1_tdata(AuroraB_USER_DATA_M_AXI_RX1_TDATA),
        .USER_DATA_M_AXI_RX1_tkeep(AuroraB_USER_DATA_M_AXI_RX1_TKEEP),
        .USER_DATA_M_AXI_RX1_tlast(AuroraB_USER_DATA_M_AXI_RX1_TLAST),
        .USER_DATA_M_AXI_RX1_tready(AuroraB_USER_DATA_M_AXI_RX1_TREADY),
        .USER_DATA_M_AXI_RX1_tvalid(AuroraB_USER_DATA_M_AXI_RX1_TVALID),
        .USER_DATA_M_AXI_RX2_tdata(AuroraB_USER_DATA_M_AXI_RX2_TDATA),
        .USER_DATA_M_AXI_RX2_tkeep(AuroraB_USER_DATA_M_AXI_RX2_TKEEP),
        .USER_DATA_M_AXI_RX2_tlast(AuroraB_USER_DATA_M_AXI_RX2_TLAST),
        .USER_DATA_M_AXI_RX2_tready(AuroraB_USER_DATA_M_AXI_RX2_TREADY),
        .USER_DATA_M_AXI_RX2_tvalid(AuroraB_USER_DATA_M_AXI_RX2_TVALID),
        .USER_DATA_M_AXI_RX3_tdata(AuroraB_USER_DATA_M_AXI_RX3_TDATA),
        .USER_DATA_M_AXI_RX3_tkeep(AuroraB_USER_DATA_M_AXI_RX3_TKEEP),
        .USER_DATA_M_AXI_RX3_tlast(AuroraB_USER_DATA_M_AXI_RX3_TLAST),
        .USER_DATA_M_AXI_RX3_tready(AuroraB_USER_DATA_M_AXI_RX3_TREADY),
        .USER_DATA_M_AXI_RX3_tvalid(AuroraB_USER_DATA_M_AXI_RX3_TVALID),
        .USER_DATA_S_AXI_TX0_tdata(USER_DATA_S_AXI_TX0_1_TDATA),
        .USER_DATA_S_AXI_TX0_tkeep(USER_DATA_S_AXI_TX0_1_TKEEP),
        .USER_DATA_S_AXI_TX0_tlast(USER_DATA_S_AXI_TX0_1_TLAST),
        .USER_DATA_S_AXI_TX0_tready(USER_DATA_S_AXI_TX0_1_TREADY),
        .USER_DATA_S_AXI_TX0_tvalid(USER_DATA_S_AXI_TX0_1_TVALID),
        .USER_DATA_S_AXI_TX1_tdata(USER_DATA_S_AXI_TX1_1_TDATA),
        .USER_DATA_S_AXI_TX1_tkeep(USER_DATA_S_AXI_TX1_1_TKEEP),
        .USER_DATA_S_AXI_TX1_tlast(USER_DATA_S_AXI_TX1_1_TLAST),
        .USER_DATA_S_AXI_TX1_tready(USER_DATA_S_AXI_TX1_1_TREADY),
        .USER_DATA_S_AXI_TX1_tvalid(USER_DATA_S_AXI_TX1_1_TVALID),
        .USER_DATA_S_AXI_TX2_tdata(USER_DATA_S_AXI_TX2_1_TDATA),
        .USER_DATA_S_AXI_TX2_tkeep(USER_DATA_S_AXI_TX2_1_TKEEP),
        .USER_DATA_S_AXI_TX2_tlast(USER_DATA_S_AXI_TX2_1_TLAST),
        .USER_DATA_S_AXI_TX2_tready(USER_DATA_S_AXI_TX2_1_TREADY),
        .USER_DATA_S_AXI_TX2_tvalid(USER_DATA_S_AXI_TX2_1_TVALID),
        .USER_DATA_S_AXI_TX3_tdata(USER_DATA_S_AXI_TX3_1_TDATA),
        .USER_DATA_S_AXI_TX3_tkeep(USER_DATA_S_AXI_TX3_1_TKEEP),
        .USER_DATA_S_AXI_TX3_tlast(USER_DATA_S_AXI_TX3_1_TLAST),
        .USER_DATA_S_AXI_TX3_tready(USER_DATA_S_AXI_TX3_1_TREADY),
        .USER_DATA_S_AXI_TX3_tvalid(USER_DATA_S_AXI_TX3_1_TVALID),
        .auroraReset(reset_1),
        .gtxReset(gt_reset_1),
        .init_clk_in(processing_system7_0_FCLK_CLK0));
  base_zynq_design_axi_gpio_0_3 GPIO_AURORA
       (.gpio2_io_i(GPIO_AURORA_GPIO2_TRI_I),
        .gpio_io_i(GPIO_AURORA_GPIO_TRI_I),
        .gpio_io_o(GPIO_AURORA_GPIO_TRI_O),
        .gpio_io_t(GPIO_AURORA_GPIO_TRI_T),
        .s_axi_aclk(processing_system7_0_FCLK_CLK0),
        .s_axi_araddr(processing_system7_0_axi_periph_M05_AXI_ARADDR[8:0]),
        .s_axi_aresetn(rst_processing_system7_0_50M_peripheral_aresetn),
        .s_axi_arready(processing_system7_0_axi_periph_M05_AXI_ARREADY),
        .s_axi_arvalid(processing_system7_0_axi_periph_M05_AXI_ARVALID),
        .s_axi_awaddr(processing_system7_0_axi_periph_M05_AXI_AWADDR[8:0]),
        .s_axi_awready(processing_system7_0_axi_periph_M05_AXI_AWREADY),
        .s_axi_awvalid(processing_system7_0_axi_periph_M05_AXI_AWVALID),
        .s_axi_bready(processing_system7_0_axi_periph_M05_AXI_BREADY),
        .s_axi_bresp(processing_system7_0_axi_periph_M05_AXI_BRESP),
        .s_axi_bvalid(processing_system7_0_axi_periph_M05_AXI_BVALID),
        .s_axi_rdata(processing_system7_0_axi_periph_M05_AXI_RDATA),
        .s_axi_rready(processing_system7_0_axi_periph_M05_AXI_RREADY),
        .s_axi_rresp(processing_system7_0_axi_periph_M05_AXI_RRESP),
        .s_axi_rvalid(processing_system7_0_axi_periph_M05_AXI_RVALID),
        .s_axi_wdata(processing_system7_0_axi_periph_M05_AXI_WDATA),
        .s_axi_wready(processing_system7_0_axi_periph_M05_AXI_WREADY),
        .s_axi_wstrb(processing_system7_0_axi_periph_M05_AXI_WSTRB),
        .s_axi_wvalid(processing_system7_0_axi_periph_M05_AXI_WVALID));
  base_zynq_design_axi_gpio_0_2 GPIO_EVR_GTX
       (.gpio_io_i(GPIO_EVR_GTX_GPIO_TRI_I),
        .gpio_io_o(GPIO_EVR_GTX_GPIO_TRI_O),
        .gpio_io_t(GPIO_EVR_GTX_GPIO_TRI_T),
        .s_axi_aclk(processing_system7_0_FCLK_CLK0),
        .s_axi_araddr(processing_system7_0_axi_periph_M04_AXI_ARADDR[8:0]),
        .s_axi_aresetn(rst_processing_system7_0_50M_peripheral_aresetn),
        .s_axi_arready(processing_system7_0_axi_periph_M04_AXI_ARREADY),
        .s_axi_arvalid(processing_system7_0_axi_periph_M04_AXI_ARVALID),
        .s_axi_awaddr(processing_system7_0_axi_periph_M04_AXI_AWADDR[8:0]),
        .s_axi_awready(processing_system7_0_axi_periph_M04_AXI_AWREADY),
        .s_axi_awvalid(processing_system7_0_axi_periph_M04_AXI_AWVALID),
        .s_axi_bready(processing_system7_0_axi_periph_M04_AXI_BREADY),
        .s_axi_bresp(processing_system7_0_axi_periph_M04_AXI_BRESP),
        .s_axi_bvalid(processing_system7_0_axi_periph_M04_AXI_BVALID),
        .s_axi_rdata(processing_system7_0_axi_periph_M04_AXI_RDATA),
        .s_axi_rready(processing_system7_0_axi_periph_M04_AXI_RREADY),
        .s_axi_rresp(processing_system7_0_axi_periph_M04_AXI_RRESP),
        .s_axi_rvalid(processing_system7_0_axi_periph_M04_AXI_RVALID),
        .s_axi_wdata(processing_system7_0_axi_periph_M04_AXI_WDATA),
        .s_axi_wready(processing_system7_0_axi_periph_M04_AXI_WREADY),
        .s_axi_wstrb(processing_system7_0_axi_periph_M04_AXI_WSTRB),
        .s_axi_wvalid(processing_system7_0_axi_periph_M04_AXI_WVALID));
  base_zynq_design_axi_gpio_0_1 GPIO_GENERIC0
       (.gpio2_io_i(GPIO_GENERIC0_GPIO2_TRI_I),
        .gpio_io_i(GPIO_GENERIC0_GPIO_TRI_I),
        .s_axi_aclk(processing_system7_0_FCLK_CLK0),
        .s_axi_araddr(processing_system7_0_axi_periph_M01_AXI_ARADDR[8:0]),
        .s_axi_aresetn(rst_processing_system7_0_50M_peripheral_aresetn),
        .s_axi_arready(processing_system7_0_axi_periph_M01_AXI_ARREADY),
        .s_axi_arvalid(processing_system7_0_axi_periph_M01_AXI_ARVALID),
        .s_axi_awaddr(processing_system7_0_axi_periph_M01_AXI_AWADDR[8:0]),
        .s_axi_awready(processing_system7_0_axi_periph_M01_AXI_AWREADY),
        .s_axi_awvalid(processing_system7_0_axi_periph_M01_AXI_AWVALID),
        .s_axi_bready(processing_system7_0_axi_periph_M01_AXI_BREADY),
        .s_axi_bresp(processing_system7_0_axi_periph_M01_AXI_BRESP),
        .s_axi_bvalid(processing_system7_0_axi_periph_M01_AXI_BVALID),
        .s_axi_rdata(processing_system7_0_axi_periph_M01_AXI_RDATA),
        .s_axi_rready(processing_system7_0_axi_periph_M01_AXI_RREADY),
        .s_axi_rresp(processing_system7_0_axi_periph_M01_AXI_RRESP),
        .s_axi_rvalid(processing_system7_0_axi_periph_M01_AXI_RVALID),
        .s_axi_wdata(processing_system7_0_axi_periph_M01_AXI_WDATA),
        .s_axi_wready(processing_system7_0_axi_periph_M01_AXI_WREADY),
        .s_axi_wstrb(processing_system7_0_axi_periph_M01_AXI_WSTRB),
        .s_axi_wvalid(processing_system7_0_axi_periph_M01_AXI_WVALID));
  base_zynq_design_axi_gpio_0_5 GPIO_MITIGATION
       (.gpio_io_o(axi_gpio_0_GPIO1_TRI_O),
        .s_axi_aclk(processing_system7_0_FCLK_CLK0),
        .s_axi_araddr(processing_system7_0_axi_periph_M09_AXI_ARADDR[8:0]),
        .s_axi_aresetn(rst_processing_system7_0_50M_peripheral_aresetn),
        .s_axi_arready(processing_system7_0_axi_periph_M09_AXI_ARREADY),
        .s_axi_arvalid(processing_system7_0_axi_periph_M09_AXI_ARVALID),
        .s_axi_awaddr(processing_system7_0_axi_periph_M09_AXI_AWADDR[8:0]),
        .s_axi_awready(processing_system7_0_axi_periph_M09_AXI_AWREADY),
        .s_axi_awvalid(processing_system7_0_axi_periph_M09_AXI_AWVALID),
        .s_axi_bready(processing_system7_0_axi_periph_M09_AXI_BREADY),
        .s_axi_bresp(processing_system7_0_axi_periph_M09_AXI_BRESP),
        .s_axi_bvalid(processing_system7_0_axi_periph_M09_AXI_BVALID),
        .s_axi_rdata(processing_system7_0_axi_periph_M09_AXI_RDATA),
        .s_axi_rready(processing_system7_0_axi_periph_M09_AXI_RREADY),
        .s_axi_rresp(processing_system7_0_axi_periph_M09_AXI_RRESP),
        .s_axi_rvalid(processing_system7_0_axi_periph_M09_AXI_RVALID),
        .s_axi_wdata(processing_system7_0_axi_periph_M09_AXI_WDATA),
        .s_axi_wready(processing_system7_0_axi_periph_M09_AXI_WREADY),
        .s_axi_wstrb(processing_system7_0_axi_periph_M09_AXI_WSTRB),
        .s_axi_wvalid(processing_system7_0_axi_periph_M09_AXI_WVALID));
  base_zynq_design_axi_gpio_0_0 GPIO_SFP_STATUS
       (.gpio_io_i(GPIO_SFP_GPIO_TRI_I),
        .s_axi_aclk(processing_system7_0_FCLK_CLK0),
        .s_axi_araddr(processing_system7_0_axi_periph_M00_AXI_ARADDR[8:0]),
        .s_axi_aresetn(rst_processing_system7_0_50M_peripheral_aresetn),
        .s_axi_arready(processing_system7_0_axi_periph_M00_AXI_ARREADY),
        .s_axi_arvalid(processing_system7_0_axi_periph_M00_AXI_ARVALID),
        .s_axi_awaddr(processing_system7_0_axi_periph_M00_AXI_AWADDR[8:0]),
        .s_axi_awready(processing_system7_0_axi_periph_M00_AXI_AWREADY),
        .s_axi_awvalid(processing_system7_0_axi_periph_M00_AXI_AWVALID),
        .s_axi_bready(processing_system7_0_axi_periph_M00_AXI_BREADY),
        .s_axi_bresp(processing_system7_0_axi_periph_M00_AXI_BRESP),
        .s_axi_bvalid(processing_system7_0_axi_periph_M00_AXI_BVALID),
        .s_axi_rdata(processing_system7_0_axi_periph_M00_AXI_RDATA),
        .s_axi_rready(processing_system7_0_axi_periph_M00_AXI_RREADY),
        .s_axi_rresp(processing_system7_0_axi_periph_M00_AXI_RRESP),
        .s_axi_rvalid(processing_system7_0_axi_periph_M00_AXI_RVALID),
        .s_axi_wdata(processing_system7_0_axi_periph_M00_AXI_WDATA),
        .s_axi_wready(processing_system7_0_axi_periph_M00_AXI_WREADY),
        .s_axi_wstrb(processing_system7_0_axi_periph_M00_AXI_WSTRB),
        .s_axi_wvalid(processing_system7_0_axi_periph_M00_AXI_WVALID));
  base_zynq_design_axi_gpio_0_7 GPIO_STALE_NODES
       (.gpio2_io_i(GPIO_STALE_NODES_GPIO2_TRI_I),
        .gpio_io_i(GPIO_STALE_NODES_GPIO_TRI_I),
        .s_axi_aclk(processing_system7_0_FCLK_CLK0),
        .s_axi_araddr(processing_system7_0_axi_periph_M12_AXI_ARADDR[8:0]),
        .s_axi_aresetn(rst_processing_system7_0_50M_peripheral_aresetn),
        .s_axi_arready(processing_system7_0_axi_periph_M12_AXI_ARREADY),
        .s_axi_arvalid(processing_system7_0_axi_periph_M12_AXI_ARVALID),
        .s_axi_awaddr(processing_system7_0_axi_periph_M12_AXI_AWADDR[8:0]),
        .s_axi_awready(processing_system7_0_axi_periph_M12_AXI_AWREADY),
        .s_axi_awvalid(processing_system7_0_axi_periph_M12_AXI_AWVALID),
        .s_axi_bready(processing_system7_0_axi_periph_M12_AXI_BREADY),
        .s_axi_bresp(processing_system7_0_axi_periph_M12_AXI_BRESP),
        .s_axi_bvalid(processing_system7_0_axi_periph_M12_AXI_BVALID),
        .s_axi_rdata(processing_system7_0_axi_periph_M12_AXI_RDATA),
        .s_axi_rready(processing_system7_0_axi_periph_M12_AXI_RREADY),
        .s_axi_rresp(processing_system7_0_axi_periph_M12_AXI_RRESP),
        .s_axi_rvalid(processing_system7_0_axi_periph_M12_AXI_RVALID),
        .s_axi_wdata(processing_system7_0_axi_periph_M12_AXI_WDATA),
        .s_axi_wready(processing_system7_0_axi_periph_M12_AXI_WREADY),
        .s_axi_wstrb(processing_system7_0_axi_periph_M12_AXI_WSTRB),
        .s_axi_wvalid(processing_system7_0_axi_periph_M12_AXI_WVALID));
  base_zynq_design_axi_gpio_0_6 GPIO_SYSMON
       (.gpio_io_i(GPIO_SYSMON_GPIO_TRI_I),
        .gpio_io_o(GPIO_SYSMON_GPIO_TRI_O),
        .gpio_io_t(GPIO_SYSMON_GPIO_TRI_T),
        .s_axi_aclk(processing_system7_0_FCLK_CLK0),
        .s_axi_araddr(processing_system7_0_axi_periph_M11_AXI_ARADDR[8:0]),
        .s_axi_aresetn(rst_processing_system7_0_50M_peripheral_aresetn),
        .s_axi_arready(processing_system7_0_axi_periph_M11_AXI_ARREADY),
        .s_axi_arvalid(processing_system7_0_axi_periph_M11_AXI_ARVALID),
        .s_axi_awaddr(processing_system7_0_axi_periph_M11_AXI_AWADDR[8:0]),
        .s_axi_awready(processing_system7_0_axi_periph_M11_AXI_AWREADY),
        .s_axi_awvalid(processing_system7_0_axi_periph_M11_AXI_AWVALID),
        .s_axi_bready(processing_system7_0_axi_periph_M11_AXI_BREADY),
        .s_axi_bresp(processing_system7_0_axi_periph_M11_AXI_BRESP),
        .s_axi_bvalid(processing_system7_0_axi_periph_M11_AXI_BVALID),
        .s_axi_rdata(processing_system7_0_axi_periph_M11_AXI_RDATA),
        .s_axi_rready(processing_system7_0_axi_periph_M11_AXI_RREADY),
        .s_axi_rresp(processing_system7_0_axi_periph_M11_AXI_RRESP),
        .s_axi_rvalid(processing_system7_0_axi_periph_M11_AXI_RVALID),
        .s_axi_wdata(processing_system7_0_axi_periph_M11_AXI_WDATA),
        .s_axi_wready(processing_system7_0_axi_periph_M11_AXI_WREADY),
        .s_axi_wstrb(processing_system7_0_axi_periph_M11_AXI_WSTRB),
        .s_axi_wvalid(processing_system7_0_axi_periph_M11_AXI_WVALID));
  base_zynq_design_axi_gpio_0_4 GPIO_TIMESTAMP
       (.gpio2_io_i(axi_gpio_0_GPIO2_TRI_I),
        .gpio_io_i(axi_gpio_0_GPIO_TRI_I),
        .s_axi_aclk(processing_system7_0_FCLK_CLK0),
        .s_axi_araddr(processing_system7_0_axi_periph_M07_AXI_ARADDR[8:0]),
        .s_axi_aresetn(rst_processing_system7_0_50M_peripheral_aresetn),
        .s_axi_arready(processing_system7_0_axi_periph_M07_AXI_ARREADY),
        .s_axi_arvalid(processing_system7_0_axi_periph_M07_AXI_ARVALID),
        .s_axi_awaddr(processing_system7_0_axi_periph_M07_AXI_AWADDR[8:0]),
        .s_axi_awready(processing_system7_0_axi_periph_M07_AXI_AWREADY),
        .s_axi_awvalid(processing_system7_0_axi_periph_M07_AXI_AWVALID),
        .s_axi_bready(processing_system7_0_axi_periph_M07_AXI_BREADY),
        .s_axi_bresp(processing_system7_0_axi_periph_M07_AXI_BRESP),
        .s_axi_bvalid(processing_system7_0_axi_periph_M07_AXI_BVALID),
        .s_axi_rdata(processing_system7_0_axi_periph_M07_AXI_RDATA),
        .s_axi_rready(processing_system7_0_axi_periph_M07_AXI_RREADY),
        .s_axi_rresp(processing_system7_0_axi_periph_M07_AXI_RRESP),
        .s_axi_rvalid(processing_system7_0_axi_periph_M07_AXI_RVALID),
        .s_axi_wdata(processing_system7_0_axi_periph_M07_AXI_WDATA),
        .s_axi_wready(processing_system7_0_axi_periph_M07_AXI_WREADY),
        .s_axi_wstrb(processing_system7_0_axi_periph_M07_AXI_WSTRB),
        .s_axi_wvalid(processing_system7_0_axi_periph_M07_AXI_WVALID));
  LogDMA_imp_GA7VJX LogDMA
       (.M_AXI_S2MM_awaddr(LogDMA_M_AXI_S2MM_AWADDR),
        .M_AXI_S2MM_awburst(LogDMA_M_AXI_S2MM_AWBURST),
        .M_AXI_S2MM_awcache(LogDMA_M_AXI_S2MM_AWCACHE),
        .M_AXI_S2MM_awlen(LogDMA_M_AXI_S2MM_AWLEN),
        .M_AXI_S2MM_awlock(LogDMA_M_AXI_S2MM_AWLOCK),
        .M_AXI_S2MM_awprot(LogDMA_M_AXI_S2MM_AWPROT),
        .M_AXI_S2MM_awqos(LogDMA_M_AXI_S2MM_AWQOS),
        .M_AXI_S2MM_awready(LogDMA_M_AXI_S2MM_AWREADY),
        .M_AXI_S2MM_awsize(LogDMA_M_AXI_S2MM_AWSIZE),
        .M_AXI_S2MM_awvalid(LogDMA_M_AXI_S2MM_AWVALID),
        .M_AXI_S2MM_bready(LogDMA_M_AXI_S2MM_BREADY),
        .M_AXI_S2MM_bresp(LogDMA_M_AXI_S2MM_BRESP),
        .M_AXI_S2MM_bvalid(LogDMA_M_AXI_S2MM_BVALID),
        .M_AXI_S2MM_wdata(LogDMA_M_AXI_S2MM_WDATA),
        .M_AXI_S2MM_wlast(LogDMA_M_AXI_S2MM_WLAST),
        .M_AXI_S2MM_wready(LogDMA_M_AXI_S2MM_WREADY),
        .M_AXI_S2MM_wstrb(LogDMA_M_AXI_S2MM_WSTRB),
        .M_AXI_S2MM_wvalid(LogDMA_M_AXI_S2MM_WVALID),
        .M_AXI_SG_araddr(LogDMA_M00_AXI_ARADDR),
        .M_AXI_SG_arburst(LogDMA_M00_AXI_ARBURST),
        .M_AXI_SG_arcache(LogDMA_M00_AXI_ARCACHE),
        .M_AXI_SG_arlen(LogDMA_M00_AXI_ARLEN),
        .M_AXI_SG_arlock(LogDMA_M00_AXI_ARLOCK),
        .M_AXI_SG_arprot(LogDMA_M00_AXI_ARPROT),
        .M_AXI_SG_arqos(LogDMA_M00_AXI_ARQOS),
        .M_AXI_SG_arready(LogDMA_M00_AXI_ARREADY),
        .M_AXI_SG_arsize(LogDMA_M00_AXI_ARSIZE),
        .M_AXI_SG_arvalid(LogDMA_M00_AXI_ARVALID),
        .M_AXI_SG_awaddr(LogDMA_M00_AXI_AWADDR),
        .M_AXI_SG_awburst(LogDMA_M00_AXI_AWBURST),
        .M_AXI_SG_awcache(LogDMA_M00_AXI_AWCACHE),
        .M_AXI_SG_awlen(LogDMA_M00_AXI_AWLEN),
        .M_AXI_SG_awlock(LogDMA_M00_AXI_AWLOCK),
        .M_AXI_SG_awprot(LogDMA_M00_AXI_AWPROT),
        .M_AXI_SG_awqos(LogDMA_M00_AXI_AWQOS),
        .M_AXI_SG_awready(LogDMA_M00_AXI_AWREADY),
        .M_AXI_SG_awsize(LogDMA_M00_AXI_AWSIZE),
        .M_AXI_SG_awvalid(LogDMA_M00_AXI_AWVALID),
        .M_AXI_SG_bready(LogDMA_M00_AXI_BREADY),
        .M_AXI_SG_bresp(LogDMA_M00_AXI_BRESP),
        .M_AXI_SG_bvalid(LogDMA_M00_AXI_BVALID),
        .M_AXI_SG_rdata(LogDMA_M00_AXI_RDATA),
        .M_AXI_SG_rlast(LogDMA_M00_AXI_RLAST),
        .M_AXI_SG_rready(LogDMA_M00_AXI_RREADY),
        .M_AXI_SG_rresp(LogDMA_M00_AXI_RRESP),
        .M_AXI_SG_rvalid(LogDMA_M00_AXI_RVALID),
        .M_AXI_SG_wdata(LogDMA_M00_AXI_WDATA),
        .M_AXI_SG_wlast(LogDMA_M00_AXI_WLAST),
        .M_AXI_SG_wready(LogDMA_M00_AXI_WREADY),
        .M_AXI_SG_wstrb(LogDMA_M00_AXI_WSTRB),
        .M_AXI_SG_wvalid(LogDMA_M00_AXI_WVALID),
        .S00_AXIS_tdata(S00_AXIS_1_TDATA),
        .S00_AXIS_tlast(S00_AXIS_1_TLAST),
        .S00_AXIS_tready(S00_AXIS_1_TREADY),
        .S00_AXIS_tvalid(S00_AXIS_1_TVALID),
        .S01_AXIS_tdata(S01_AXIS_1_TDATA),
        .S01_AXIS_tlast(S01_AXIS_1_TLAST),
        .S01_AXIS_tready(S01_AXIS_1_TREADY),
        .S01_AXIS_tvalid(S01_AXIS_1_TVALID),
        .S_AXI_LITE_araddr(processing_system7_0_axi_periph_M10_AXI_ARADDR),
        .S_AXI_LITE_arready(processing_system7_0_axi_periph_M10_AXI_ARREADY),
        .S_AXI_LITE_arvalid(processing_system7_0_axi_periph_M10_AXI_ARVALID),
        .S_AXI_LITE_awaddr(processing_system7_0_axi_periph_M10_AXI_AWADDR),
        .S_AXI_LITE_awready(processing_system7_0_axi_periph_M10_AXI_AWREADY),
        .S_AXI_LITE_awvalid(processing_system7_0_axi_periph_M10_AXI_AWVALID),
        .S_AXI_LITE_bready(processing_system7_0_axi_periph_M10_AXI_BREADY),
        .S_AXI_LITE_bresp(processing_system7_0_axi_periph_M10_AXI_BRESP),
        .S_AXI_LITE_bvalid(processing_system7_0_axi_periph_M10_AXI_BVALID),
        .S_AXI_LITE_rdata(processing_system7_0_axi_periph_M10_AXI_RDATA),
        .S_AXI_LITE_rready(processing_system7_0_axi_periph_M10_AXI_RREADY),
        .S_AXI_LITE_rresp(processing_system7_0_axi_periph_M10_AXI_RRESP),
        .S_AXI_LITE_rvalid(processing_system7_0_axi_periph_M10_AXI_RVALID),
        .S_AXI_LITE_wdata(processing_system7_0_axi_periph_M10_AXI_WDATA),
        .S_AXI_LITE_wready(processing_system7_0_axi_periph_M10_AXI_WREADY),
        .S_AXI_LITE_wvalid(processing_system7_0_axi_periph_M10_AXI_WVALID),
        .sys_ACLK(processing_system7_0_FCLK_CLK0),
        .sys_ARESETN(rst_processing_system7_0_50M_peripheral_aresetn));
  base_zynq_design_axi_iic_0_0 SFP_IIC
       (.gpo(axi_iic_0_gpo),
        .s_axi_aclk(processing_system7_0_FCLK_CLK0),
        .s_axi_araddr(processing_system7_0_axi_periph_M02_AXI_ARADDR[8:0]),
        .s_axi_aresetn(rst_processing_system7_0_50M_peripheral_aresetn),
        .s_axi_arready(processing_system7_0_axi_periph_M02_AXI_ARREADY),
        .s_axi_arvalid(processing_system7_0_axi_periph_M02_AXI_ARVALID),
        .s_axi_awaddr(processing_system7_0_axi_periph_M02_AXI_AWADDR[8:0]),
        .s_axi_awready(processing_system7_0_axi_periph_M02_AXI_AWREADY),
        .s_axi_awvalid(processing_system7_0_axi_periph_M02_AXI_AWVALID),
        .s_axi_bready(processing_system7_0_axi_periph_M02_AXI_BREADY),
        .s_axi_bresp(processing_system7_0_axi_periph_M02_AXI_BRESP),
        .s_axi_bvalid(processing_system7_0_axi_periph_M02_AXI_BVALID),
        .s_axi_rdata(processing_system7_0_axi_periph_M02_AXI_RDATA),
        .s_axi_rready(processing_system7_0_axi_periph_M02_AXI_RREADY),
        .s_axi_rresp(processing_system7_0_axi_periph_M02_AXI_RRESP),
        .s_axi_rvalid(processing_system7_0_axi_periph_M02_AXI_RVALID),
        .s_axi_wdata(processing_system7_0_axi_periph_M02_AXI_WDATA),
        .s_axi_wready(processing_system7_0_axi_periph_M02_AXI_WREADY),
        .s_axi_wstrb(processing_system7_0_axi_periph_M02_AXI_WSTRB),
        .s_axi_wvalid(processing_system7_0_axi_periph_M02_AXI_WVALID),
        .scl_i(axi_iic_0_IIC_SCL_I),
        .scl_o(axi_iic_0_IIC_SCL_O),
        .scl_t(axi_iic_0_IIC_SCL_T),
        .sda_i(axi_iic_0_IIC_SDA_I),
        .sda_o(axi_iic_0_IIC_SDA_O),
        .sda_t(axi_iic_0_IIC_SDA_T));
  base_zynq_design_drp_bridge_0_1 drp_bridge_0
       (.AXI_aclk(processing_system7_0_FCLK_CLK0),
        .AXI_aresetn(rst_processing_system7_0_50M_peripheral_aresetn),
        .S_AXI_araddr(processing_system7_0_axi_periph_M03_AXI_ARADDR),
        .S_AXI_arprot(processing_system7_0_axi_periph_M03_AXI_ARPROT),
        .S_AXI_arready(processing_system7_0_axi_periph_M03_AXI_ARREADY),
        .S_AXI_arvalid(processing_system7_0_axi_periph_M03_AXI_ARVALID),
        .S_AXI_awaddr(processing_system7_0_axi_periph_M03_AXI_AWADDR),
        .S_AXI_awprot(processing_system7_0_axi_periph_M03_AXI_AWPROT),
        .S_AXI_awready(processing_system7_0_axi_periph_M03_AXI_AWREADY),
        .S_AXI_awvalid(processing_system7_0_axi_periph_M03_AXI_AWVALID),
        .S_AXI_bready(processing_system7_0_axi_periph_M03_AXI_BREADY),
        .S_AXI_bresp(processing_system7_0_axi_periph_M03_AXI_BRESP),
        .S_AXI_bvalid(processing_system7_0_axi_periph_M03_AXI_BVALID),
        .S_AXI_rdata(processing_system7_0_axi_periph_M03_AXI_RDATA),
        .S_AXI_rready(processing_system7_0_axi_periph_M03_AXI_RREADY),
        .S_AXI_rresp(processing_system7_0_axi_periph_M03_AXI_RRESP),
        .S_AXI_rvalid(processing_system7_0_axi_periph_M03_AXI_RVALID),
        .S_AXI_wdata(processing_system7_0_axi_periph_M03_AXI_WDATA),
        .S_AXI_wready(processing_system7_0_axi_periph_M03_AXI_WREADY),
        .S_AXI_wstrb(processing_system7_0_axi_periph_M03_AXI_WSTRB),
        .S_AXI_wvalid(processing_system7_0_axi_periph_M03_AXI_WVALID),
        .drp0_addr(drp_bridge_0_DRP0_DADDR),
        .drp0_di(drp_bridge_0_DRP0_DI),
        .drp0_do(drp_bridge_0_DRP0_DO),
        .drp0_en(drp_bridge_0_DRP0_DEN),
        .drp0_rdy(drp_bridge_0_DRP0_DRDY),
        .drp0_we(drp_bridge_0_DRP0_DWE),
        .drp1_addr(drp_bridge_0_DRP1_DADDR),
        .drp1_di(drp_bridge_0_DRP1_DI),
        .drp1_do(drp_bridge_0_DRP1_DO),
        .drp1_en(drp_bridge_0_DRP1_DEN),
        .drp1_rdy(drp_bridge_0_DRP1_DRDY),
        .drp1_we(drp_bridge_0_DRP1_DWE),
        .drp2_addr(drp_bridge_0_DRP2_DADDR),
        .drp2_di(drp_bridge_0_DRP2_DI),
        .drp2_do(drp_bridge_0_DRP2_DO),
        .drp2_en(drp_bridge_0_DRP2_DEN),
        .drp2_rdy(drp_bridge_0_DRP2_DRDY),
        .drp2_we(drp_bridge_0_DRP2_DWE),
        .drp3_addr(drp_bridge_0_DRP3_DADDR),
        .drp3_di(drp_bridge_0_DRP3_DI),
        .drp3_do(drp_bridge_0_DRP3_DO),
        .drp3_en(drp_bridge_0_DRP3_DEN),
        .drp3_rdy(drp_bridge_0_DRP3_DRDY),
        .drp3_we(drp_bridge_0_DRP3_DWE),
        .drp4_addr(drp_bridge_0_DRP4_DADDR),
        .drp4_di(drp_bridge_0_DRP4_DI),
        .drp4_do(drp_bridge_0_DRP4_DO),
        .drp4_en(drp_bridge_0_DRP4_DEN),
        .drp4_rdy(drp_bridge_0_DRP4_DRDY),
        .drp4_we(drp_bridge_0_DRP4_DWE),
        .drp5_addr(drp_bridge_0_DRP5_DADDR),
        .drp5_di(drp_bridge_0_DRP5_DI),
        .drp5_do(drp_bridge_0_DRP5_DO),
        .drp5_en(drp_bridge_0_DRP5_DEN),
        .drp5_rdy(drp_bridge_0_DRP5_DRDY),
        .drp5_we(drp_bridge_0_DRP5_DWE),
        .drp6_addr(drp_bridge_0_DRP6_DADDR),
        .drp6_di(drp_bridge_0_DRP6_DI),
        .drp6_do(drp_bridge_0_DRP6_DO),
        .drp6_en(drp_bridge_0_DRP6_DEN),
        .drp6_rdy(drp_bridge_0_DRP6_DRDY),
        .drp6_we(drp_bridge_0_DRP6_DWE),
        .drp7_addr(drp_bridge_0_DRP7_DADDR),
        .drp7_di(drp_bridge_0_DRP7_DI),
        .drp7_do(drp_bridge_0_DRP7_DO),
        .drp7_en(drp_bridge_0_DRP7_DEN),
        .drp7_rdy(drp_bridge_0_DRP7_DRDY),
        .drp7_we(drp_bridge_0_DRP7_DWE),
        .drp8_addr(drp_bridge_0_DRP8_DADDR),
        .drp8_di(drp_bridge_0_DRP8_DI),
        .drp8_do(drp_bridge_0_DRP8_DO),
        .drp8_en(drp_bridge_0_DRP8_DEN),
        .drp8_rdy(drp_bridge_0_DRP8_DRDY),
        .drp8_we(drp_bridge_0_DRP8_DWE));
  base_zynq_design_axi_bram_ctrl_0_0 mitigationBRAMctrl
       (.bram_addr_a(axi_bram_ctrl_0_BRAM_PORTA_ADDR),
        .bram_clk_a(axi_bram_ctrl_0_BRAM_PORTA_CLK),
        .bram_en_a(axi_bram_ctrl_0_BRAM_PORTA_EN),
        .bram_rddata_a(axi_bram_ctrl_0_BRAM_PORTA_DOUT),
        .bram_rst_a(axi_bram_ctrl_0_BRAM_PORTA_RST),
        .bram_we_a(axi_bram_ctrl_0_BRAM_PORTA_WE),
        .bram_wrdata_a(axi_bram_ctrl_0_BRAM_PORTA_DIN),
        .s_axi_aclk(processing_system7_0_FCLK_CLK0),
        .s_axi_araddr(processing_system7_0_axi_periph_M08_AXI_ARADDR[15:0]),
        .s_axi_aresetn(rst_processing_system7_0_50M_peripheral_aresetn),
        .s_axi_arprot(processing_system7_0_axi_periph_M08_AXI_ARPROT),
        .s_axi_arready(processing_system7_0_axi_periph_M08_AXI_ARREADY),
        .s_axi_arvalid(processing_system7_0_axi_periph_M08_AXI_ARVALID),
        .s_axi_awaddr(processing_system7_0_axi_periph_M08_AXI_AWADDR[15:0]),
        .s_axi_awprot(processing_system7_0_axi_periph_M08_AXI_AWPROT),
        .s_axi_awready(processing_system7_0_axi_periph_M08_AXI_AWREADY),
        .s_axi_awvalid(processing_system7_0_axi_periph_M08_AXI_AWVALID),
        .s_axi_bready(processing_system7_0_axi_periph_M08_AXI_BREADY),
        .s_axi_bresp(processing_system7_0_axi_periph_M08_AXI_BRESP),
        .s_axi_bvalid(processing_system7_0_axi_periph_M08_AXI_BVALID),
        .s_axi_rdata(processing_system7_0_axi_periph_M08_AXI_RDATA),
        .s_axi_rready(processing_system7_0_axi_periph_M08_AXI_RREADY),
        .s_axi_rresp(processing_system7_0_axi_periph_M08_AXI_RRESP),
        .s_axi_rvalid(processing_system7_0_axi_periph_M08_AXI_RVALID),
        .s_axi_wdata(processing_system7_0_axi_periph_M08_AXI_WDATA),
        .s_axi_wready(processing_system7_0_axi_periph_M08_AXI_WREADY),
        .s_axi_wstrb(processing_system7_0_axi_periph_M08_AXI_WSTRB),
        .s_axi_wvalid(processing_system7_0_axi_periph_M08_AXI_WVALID));
  base_zynq_design_processing_system7_0_0 processing_system7_0
       (.DDR_Addr(DDR_addr[14:0]),
        .DDR_BankAddr(DDR_ba[2:0]),
        .DDR_CAS_n(DDR_cas_n),
        .DDR_CKE(DDR_cke),
        .DDR_CS_n(DDR_cs_n),
        .DDR_Clk(DDR_ck_p),
        .DDR_Clk_n(DDR_ck_n),
        .DDR_DM(DDR_dm[3:0]),
        .DDR_DQ(DDR_dq[31:0]),
        .DDR_DQS(DDR_dqs_p[3:0]),
        .DDR_DQS_n(DDR_dqs_n[3:0]),
        .DDR_DRSTB(DDR_reset_n),
        .DDR_ODT(DDR_odt),
        .DDR_RAS_n(DDR_ras_n),
        .DDR_VRN(FIXED_IO_ddr_vrn),
        .DDR_VRP(FIXED_IO_ddr_vrp),
        .DDR_WEB(DDR_we_n),
        .FCLK_CLK0(processing_system7_0_FCLK_CLK0),
        .FCLK_CLK1(processing_system7_0_FCLK_CLK1),
        .FCLK_RESET0_N(processing_system7_0_FCLK_RESET0_N),
        .MIO(FIXED_IO_mio[53:0]),
        .M_AXI_GP0_ACLK(processing_system7_0_FCLK_CLK0),
        .M_AXI_GP0_ARADDR(processing_system7_0_M_AXI_GP0_ARADDR),
        .M_AXI_GP0_ARBURST(processing_system7_0_M_AXI_GP0_ARBURST),
        .M_AXI_GP0_ARCACHE(processing_system7_0_M_AXI_GP0_ARCACHE),
        .M_AXI_GP0_ARID(processing_system7_0_M_AXI_GP0_ARID),
        .M_AXI_GP0_ARLEN(processing_system7_0_M_AXI_GP0_ARLEN),
        .M_AXI_GP0_ARLOCK(processing_system7_0_M_AXI_GP0_ARLOCK),
        .M_AXI_GP0_ARPROT(processing_system7_0_M_AXI_GP0_ARPROT),
        .M_AXI_GP0_ARQOS(processing_system7_0_M_AXI_GP0_ARQOS),
        .M_AXI_GP0_ARREADY(processing_system7_0_M_AXI_GP0_ARREADY),
        .M_AXI_GP0_ARSIZE(processing_system7_0_M_AXI_GP0_ARSIZE),
        .M_AXI_GP0_ARVALID(processing_system7_0_M_AXI_GP0_ARVALID),
        .M_AXI_GP0_AWADDR(processing_system7_0_M_AXI_GP0_AWADDR),
        .M_AXI_GP0_AWBURST(processing_system7_0_M_AXI_GP0_AWBURST),
        .M_AXI_GP0_AWCACHE(processing_system7_0_M_AXI_GP0_AWCACHE),
        .M_AXI_GP0_AWID(processing_system7_0_M_AXI_GP0_AWID),
        .M_AXI_GP0_AWLEN(processing_system7_0_M_AXI_GP0_AWLEN),
        .M_AXI_GP0_AWLOCK(processing_system7_0_M_AXI_GP0_AWLOCK),
        .M_AXI_GP0_AWPROT(processing_system7_0_M_AXI_GP0_AWPROT),
        .M_AXI_GP0_AWQOS(processing_system7_0_M_AXI_GP0_AWQOS),
        .M_AXI_GP0_AWREADY(processing_system7_0_M_AXI_GP0_AWREADY),
        .M_AXI_GP0_AWSIZE(processing_system7_0_M_AXI_GP0_AWSIZE),
        .M_AXI_GP0_AWVALID(processing_system7_0_M_AXI_GP0_AWVALID),
        .M_AXI_GP0_BID(processing_system7_0_M_AXI_GP0_BID),
        .M_AXI_GP0_BREADY(processing_system7_0_M_AXI_GP0_BREADY),
        .M_AXI_GP0_BRESP(processing_system7_0_M_AXI_GP0_BRESP),
        .M_AXI_GP0_BVALID(processing_system7_0_M_AXI_GP0_BVALID),
        .M_AXI_GP0_RDATA(processing_system7_0_M_AXI_GP0_RDATA),
        .M_AXI_GP0_RID(processing_system7_0_M_AXI_GP0_RID),
        .M_AXI_GP0_RLAST(processing_system7_0_M_AXI_GP0_RLAST),
        .M_AXI_GP0_RREADY(processing_system7_0_M_AXI_GP0_RREADY),
        .M_AXI_GP0_RRESP(processing_system7_0_M_AXI_GP0_RRESP),
        .M_AXI_GP0_RVALID(processing_system7_0_M_AXI_GP0_RVALID),
        .M_AXI_GP0_WDATA(processing_system7_0_M_AXI_GP0_WDATA),
        .M_AXI_GP0_WID(processing_system7_0_M_AXI_GP0_WID),
        .M_AXI_GP0_WLAST(processing_system7_0_M_AXI_GP0_WLAST),
        .M_AXI_GP0_WREADY(processing_system7_0_M_AXI_GP0_WREADY),
        .M_AXI_GP0_WSTRB(processing_system7_0_M_AXI_GP0_WSTRB),
        .M_AXI_GP0_WVALID(processing_system7_0_M_AXI_GP0_WVALID),
        .PS_CLK(FIXED_IO_ps_clk),
        .PS_PORB(FIXED_IO_ps_porb),
        .PS_SRSTB(FIXED_IO_ps_srstb),
        .S_AXI_HP0_ACLK(processing_system7_0_FCLK_CLK0),
        .S_AXI_HP0_ARADDR(LogDMA_M00_AXI_ARADDR),
        .S_AXI_HP0_ARBURST(LogDMA_M00_AXI_ARBURST),
        .S_AXI_HP0_ARCACHE(LogDMA_M00_AXI_ARCACHE),
        .S_AXI_HP0_ARID({1'b0,1'b0,1'b0,1'b0,1'b0,1'b0}),
        .S_AXI_HP0_ARLEN(LogDMA_M00_AXI_ARLEN),
        .S_AXI_HP0_ARLOCK(LogDMA_M00_AXI_ARLOCK),
        .S_AXI_HP0_ARPROT(LogDMA_M00_AXI_ARPROT),
        .S_AXI_HP0_ARQOS(LogDMA_M00_AXI_ARQOS),
        .S_AXI_HP0_ARREADY(LogDMA_M00_AXI_ARREADY),
        .S_AXI_HP0_ARSIZE(LogDMA_M00_AXI_ARSIZE),
        .S_AXI_HP0_ARVALID(LogDMA_M00_AXI_ARVALID),
        .S_AXI_HP0_AWADDR(LogDMA_M00_AXI_AWADDR),
        .S_AXI_HP0_AWBURST(LogDMA_M00_AXI_AWBURST),
        .S_AXI_HP0_AWCACHE(LogDMA_M00_AXI_AWCACHE),
        .S_AXI_HP0_AWID({1'b0,1'b0,1'b0,1'b0,1'b0,1'b0}),
        .S_AXI_HP0_AWLEN(LogDMA_M00_AXI_AWLEN),
        .S_AXI_HP0_AWLOCK(LogDMA_M00_AXI_AWLOCK),
        .S_AXI_HP0_AWPROT(LogDMA_M00_AXI_AWPROT),
        .S_AXI_HP0_AWQOS(LogDMA_M00_AXI_AWQOS),
        .S_AXI_HP0_AWREADY(LogDMA_M00_AXI_AWREADY),
        .S_AXI_HP0_AWSIZE(LogDMA_M00_AXI_AWSIZE),
        .S_AXI_HP0_AWVALID(LogDMA_M00_AXI_AWVALID),
        .S_AXI_HP0_BREADY(LogDMA_M00_AXI_BREADY),
        .S_AXI_HP0_BRESP(LogDMA_M00_AXI_BRESP),
        .S_AXI_HP0_BVALID(LogDMA_M00_AXI_BVALID),
        .S_AXI_HP0_RDATA(LogDMA_M00_AXI_RDATA),
        .S_AXI_HP0_RDISSUECAP1_EN(1'b0),
        .S_AXI_HP0_RLAST(LogDMA_M00_AXI_RLAST),
        .S_AXI_HP0_RREADY(LogDMA_M00_AXI_RREADY),
        .S_AXI_HP0_RRESP(LogDMA_M00_AXI_RRESP),
        .S_AXI_HP0_RVALID(LogDMA_M00_AXI_RVALID),
        .S_AXI_HP0_WDATA(LogDMA_M00_AXI_WDATA),
        .S_AXI_HP0_WID({1'b0,1'b0,1'b0,1'b0,1'b0,1'b0}),
        .S_AXI_HP0_WLAST(LogDMA_M00_AXI_WLAST),
        .S_AXI_HP0_WREADY(LogDMA_M00_AXI_WREADY),
        .S_AXI_HP0_WRISSUECAP1_EN(1'b0),
        .S_AXI_HP0_WSTRB(LogDMA_M00_AXI_WSTRB),
        .S_AXI_HP0_WVALID(LogDMA_M00_AXI_WVALID),
        .S_AXI_HP1_ACLK(processing_system7_0_FCLK_CLK0),
        .S_AXI_HP1_ARADDR({1'b0,1'b0,1'b0,1'b0,1'b0,1'b0,1'b0,1'b0,1'b0,1'b0,1'b0,1'b0,1'b0,1'b0,1'b0,1'b0,1'b0,1'b0,1'b0,1'b0,1'b0,1'b0,1'b0,1'b0,1'b0,1'b0,1'b0,1'b0,1'b0,1'b0,1'b0,1'b0}),
        .S_AXI_HP1_ARBURST({1'b0,1'b0}),
        .S_AXI_HP1_ARCACHE({1'b0,1'b0,1'b0,1'b0}),
        .S_AXI_HP1_ARID({1'b0,1'b0,1'b0,1'b0,1'b0,1'b0}),
        .S_AXI_HP1_ARLEN({1'b0,1'b0,1'b0,1'b0}),
        .S_AXI_HP1_ARLOCK({1'b0,1'b0}),
        .S_AXI_HP1_ARPROT({1'b0,1'b0,1'b0}),
        .S_AXI_HP1_ARQOS({1'b0,1'b0,1'b0,1'b0}),
        .S_AXI_HP1_ARSIZE({1'b0,1'b0,1'b0}),
        .S_AXI_HP1_ARVALID(1'b0),
        .S_AXI_HP1_AWADDR(LogDMA_M_AXI_S2MM_AWADDR),
        .S_AXI_HP1_AWBURST(LogDMA_M_AXI_S2MM_AWBURST),
        .S_AXI_HP1_AWCACHE(LogDMA_M_AXI_S2MM_AWCACHE),
        .S_AXI_HP1_AWID({1'b0,1'b0,1'b0,1'b0,1'b0,1'b0}),
        .S_AXI_HP1_AWLEN(LogDMA_M_AXI_S2MM_AWLEN),
        .S_AXI_HP1_AWLOCK(LogDMA_M_AXI_S2MM_AWLOCK),
        .S_AXI_HP1_AWPROT(LogDMA_M_AXI_S2MM_AWPROT),
        .S_AXI_HP1_AWQOS(LogDMA_M_AXI_S2MM_AWQOS),
        .S_AXI_HP1_AWREADY(LogDMA_M_AXI_S2MM_AWREADY),
        .S_AXI_HP1_AWSIZE(LogDMA_M_AXI_S2MM_AWSIZE),
        .S_AXI_HP1_AWVALID(LogDMA_M_AXI_S2MM_AWVALID),
        .S_AXI_HP1_BREADY(LogDMA_M_AXI_S2MM_BREADY),
        .S_AXI_HP1_BRESP(LogDMA_M_AXI_S2MM_BRESP),
        .S_AXI_HP1_BVALID(LogDMA_M_AXI_S2MM_BVALID),
        .S_AXI_HP1_RDISSUECAP1_EN(1'b0),
        .S_AXI_HP1_RREADY(1'b0),
        .S_AXI_HP1_WDATA(LogDMA_M_AXI_S2MM_WDATA),
        .S_AXI_HP1_WID({1'b0,1'b0,1'b0,1'b0,1'b0,1'b0}),
        .S_AXI_HP1_WLAST(LogDMA_M_AXI_S2MM_WLAST),
        .S_AXI_HP1_WREADY(LogDMA_M_AXI_S2MM_WREADY),
        .S_AXI_HP1_WRISSUECAP1_EN(1'b0),
        .S_AXI_HP1_WSTRB(LogDMA_M_AXI_S2MM_WSTRB),
        .S_AXI_HP1_WVALID(LogDMA_M_AXI_S2MM_WVALID));
  base_zynq_design_processing_system7_0_axi_periph_0 processing_system7_0_axi_periph
       (.ACLK(processing_system7_0_FCLK_CLK0),
        .ARESETN(rst_processing_system7_0_50M_interconnect_aresetn),
        .M00_ACLK(processing_system7_0_FCLK_CLK0),
        .M00_ARESETN(rst_processing_system7_0_50M_peripheral_aresetn),
        .M00_AXI_araddr(processing_system7_0_axi_periph_M00_AXI_ARADDR),
        .M00_AXI_arready(processing_system7_0_axi_periph_M00_AXI_ARREADY),
        .M00_AXI_arvalid(processing_system7_0_axi_periph_M00_AXI_ARVALID),
        .M00_AXI_awaddr(processing_system7_0_axi_periph_M00_AXI_AWADDR),
        .M00_AXI_awready(processing_system7_0_axi_periph_M00_AXI_AWREADY),
        .M00_AXI_awvalid(processing_system7_0_axi_periph_M00_AXI_AWVALID),
        .M00_AXI_bready(processing_system7_0_axi_periph_M00_AXI_BREADY),
        .M00_AXI_bresp(processing_system7_0_axi_periph_M00_AXI_BRESP),
        .M00_AXI_bvalid(processing_system7_0_axi_periph_M00_AXI_BVALID),
        .M00_AXI_rdata(processing_system7_0_axi_periph_M00_AXI_RDATA),
        .M00_AXI_rready(processing_system7_0_axi_periph_M00_AXI_RREADY),
        .M00_AXI_rresp(processing_system7_0_axi_periph_M00_AXI_RRESP),
        .M00_AXI_rvalid(processing_system7_0_axi_periph_M00_AXI_RVALID),
        .M00_AXI_wdata(processing_system7_0_axi_periph_M00_AXI_WDATA),
        .M00_AXI_wready(processing_system7_0_axi_periph_M00_AXI_WREADY),
        .M00_AXI_wstrb(processing_system7_0_axi_periph_M00_AXI_WSTRB),
        .M00_AXI_wvalid(processing_system7_0_axi_periph_M00_AXI_WVALID),
        .M01_ACLK(processing_system7_0_FCLK_CLK0),
        .M01_ARESETN(rst_processing_system7_0_50M_peripheral_aresetn),
        .M01_AXI_araddr(processing_system7_0_axi_periph_M01_AXI_ARADDR),
        .M01_AXI_arready(processing_system7_0_axi_periph_M01_AXI_ARREADY),
        .M01_AXI_arvalid(processing_system7_0_axi_periph_M01_AXI_ARVALID),
        .M01_AXI_awaddr(processing_system7_0_axi_periph_M01_AXI_AWADDR),
        .M01_AXI_awready(processing_system7_0_axi_periph_M01_AXI_AWREADY),
        .M01_AXI_awvalid(processing_system7_0_axi_periph_M01_AXI_AWVALID),
        .M01_AXI_bready(processing_system7_0_axi_periph_M01_AXI_BREADY),
        .M01_AXI_bresp(processing_system7_0_axi_periph_M01_AXI_BRESP),
        .M01_AXI_bvalid(processing_system7_0_axi_periph_M01_AXI_BVALID),
        .M01_AXI_rdata(processing_system7_0_axi_periph_M01_AXI_RDATA),
        .M01_AXI_rready(processing_system7_0_axi_periph_M01_AXI_RREADY),
        .M01_AXI_rresp(processing_system7_0_axi_periph_M01_AXI_RRESP),
        .M01_AXI_rvalid(processing_system7_0_axi_periph_M01_AXI_RVALID),
        .M01_AXI_wdata(processing_system7_0_axi_periph_M01_AXI_WDATA),
        .M01_AXI_wready(processing_system7_0_axi_periph_M01_AXI_WREADY),
        .M01_AXI_wstrb(processing_system7_0_axi_periph_M01_AXI_WSTRB),
        .M01_AXI_wvalid(processing_system7_0_axi_periph_M01_AXI_WVALID),
        .M02_ACLK(processing_system7_0_FCLK_CLK0),
        .M02_ARESETN(rst_processing_system7_0_50M_peripheral_aresetn),
        .M02_AXI_araddr(processing_system7_0_axi_periph_M02_AXI_ARADDR),
        .M02_AXI_arready(processing_system7_0_axi_periph_M02_AXI_ARREADY),
        .M02_AXI_arvalid(processing_system7_0_axi_periph_M02_AXI_ARVALID),
        .M02_AXI_awaddr(processing_system7_0_axi_periph_M02_AXI_AWADDR),
        .M02_AXI_awready(processing_system7_0_axi_periph_M02_AXI_AWREADY),
        .M02_AXI_awvalid(processing_system7_0_axi_periph_M02_AXI_AWVALID),
        .M02_AXI_bready(processing_system7_0_axi_periph_M02_AXI_BREADY),
        .M02_AXI_bresp(processing_system7_0_axi_periph_M02_AXI_BRESP),
        .M02_AXI_bvalid(processing_system7_0_axi_periph_M02_AXI_BVALID),
        .M02_AXI_rdata(processing_system7_0_axi_periph_M02_AXI_RDATA),
        .M02_AXI_rready(processing_system7_0_axi_periph_M02_AXI_RREADY),
        .M02_AXI_rresp(processing_system7_0_axi_periph_M02_AXI_RRESP),
        .M02_AXI_rvalid(processing_system7_0_axi_periph_M02_AXI_RVALID),
        .M02_AXI_wdata(processing_system7_0_axi_periph_M02_AXI_WDATA),
        .M02_AXI_wready(processing_system7_0_axi_periph_M02_AXI_WREADY),
        .M02_AXI_wstrb(processing_system7_0_axi_periph_M02_AXI_WSTRB),
        .M02_AXI_wvalid(processing_system7_0_axi_periph_M02_AXI_WVALID),
        .M03_ACLK(processing_system7_0_FCLK_CLK0),
        .M03_ARESETN(rst_processing_system7_0_50M_peripheral_aresetn),
        .M03_AXI_araddr(processing_system7_0_axi_periph_M03_AXI_ARADDR),
        .M03_AXI_arprot(processing_system7_0_axi_periph_M03_AXI_ARPROT),
        .M03_AXI_arready(processing_system7_0_axi_periph_M03_AXI_ARREADY),
        .M03_AXI_arvalid(processing_system7_0_axi_periph_M03_AXI_ARVALID),
        .M03_AXI_awaddr(processing_system7_0_axi_periph_M03_AXI_AWADDR),
        .M03_AXI_awprot(processing_system7_0_axi_periph_M03_AXI_AWPROT),
        .M03_AXI_awready(processing_system7_0_axi_periph_M03_AXI_AWREADY),
        .M03_AXI_awvalid(processing_system7_0_axi_periph_M03_AXI_AWVALID),
        .M03_AXI_bready(processing_system7_0_axi_periph_M03_AXI_BREADY),
        .M03_AXI_bresp(processing_system7_0_axi_periph_M03_AXI_BRESP),
        .M03_AXI_bvalid(processing_system7_0_axi_periph_M03_AXI_BVALID),
        .M03_AXI_rdata(processing_system7_0_axi_periph_M03_AXI_RDATA),
        .M03_AXI_rready(processing_system7_0_axi_periph_M03_AXI_RREADY),
        .M03_AXI_rresp(processing_system7_0_axi_periph_M03_AXI_RRESP),
        .M03_AXI_rvalid(processing_system7_0_axi_periph_M03_AXI_RVALID),
        .M03_AXI_wdata(processing_system7_0_axi_periph_M03_AXI_WDATA),
        .M03_AXI_wready(processing_system7_0_axi_periph_M03_AXI_WREADY),
        .M03_AXI_wstrb(processing_system7_0_axi_periph_M03_AXI_WSTRB),
        .M03_AXI_wvalid(processing_system7_0_axi_periph_M03_AXI_WVALID),
        .M04_ACLK(processing_system7_0_FCLK_CLK0),
        .M04_ARESETN(rst_processing_system7_0_50M_peripheral_aresetn),
        .M04_AXI_araddr(processing_system7_0_axi_periph_M04_AXI_ARADDR),
        .M04_AXI_arready(processing_system7_0_axi_periph_M04_AXI_ARREADY),
        .M04_AXI_arvalid(processing_system7_0_axi_periph_M04_AXI_ARVALID),
        .M04_AXI_awaddr(processing_system7_0_axi_periph_M04_AXI_AWADDR),
        .M04_AXI_awready(processing_system7_0_axi_periph_M04_AXI_AWREADY),
        .M04_AXI_awvalid(processing_system7_0_axi_periph_M04_AXI_AWVALID),
        .M04_AXI_bready(processing_system7_0_axi_periph_M04_AXI_BREADY),
        .M04_AXI_bresp(processing_system7_0_axi_periph_M04_AXI_BRESP),
        .M04_AXI_bvalid(processing_system7_0_axi_periph_M04_AXI_BVALID),
        .M04_AXI_rdata(processing_system7_0_axi_periph_M04_AXI_RDATA),
        .M04_AXI_rready(processing_system7_0_axi_periph_M04_AXI_RREADY),
        .M04_AXI_rresp(processing_system7_0_axi_periph_M04_AXI_RRESP),
        .M04_AXI_rvalid(processing_system7_0_axi_periph_M04_AXI_RVALID),
        .M04_AXI_wdata(processing_system7_0_axi_periph_M04_AXI_WDATA),
        .M04_AXI_wready(processing_system7_0_axi_periph_M04_AXI_WREADY),
        .M04_AXI_wstrb(processing_system7_0_axi_periph_M04_AXI_WSTRB),
        .M04_AXI_wvalid(processing_system7_0_axi_periph_M04_AXI_WVALID),
        .M05_ACLK(processing_system7_0_FCLK_CLK0),
        .M05_ARESETN(rst_processing_system7_0_50M_peripheral_aresetn),
        .M05_AXI_araddr(processing_system7_0_axi_periph_M05_AXI_ARADDR),
        .M05_AXI_arready(processing_system7_0_axi_periph_M05_AXI_ARREADY),
        .M05_AXI_arvalid(processing_system7_0_axi_periph_M05_AXI_ARVALID),
        .M05_AXI_awaddr(processing_system7_0_axi_periph_M05_AXI_AWADDR),
        .M05_AXI_awready(processing_system7_0_axi_periph_M05_AXI_AWREADY),
        .M05_AXI_awvalid(processing_system7_0_axi_periph_M05_AXI_AWVALID),
        .M05_AXI_bready(processing_system7_0_axi_periph_M05_AXI_BREADY),
        .M05_AXI_bresp(processing_system7_0_axi_periph_M05_AXI_BRESP),
        .M05_AXI_bvalid(processing_system7_0_axi_periph_M05_AXI_BVALID),
        .M05_AXI_rdata(processing_system7_0_axi_periph_M05_AXI_RDATA),
        .M05_AXI_rready(processing_system7_0_axi_periph_M05_AXI_RREADY),
        .M05_AXI_rresp(processing_system7_0_axi_periph_M05_AXI_RRESP),
        .M05_AXI_rvalid(processing_system7_0_axi_periph_M05_AXI_RVALID),
        .M05_AXI_wdata(processing_system7_0_axi_periph_M05_AXI_WDATA),
        .M05_AXI_wready(processing_system7_0_axi_periph_M05_AXI_WREADY),
        .M05_AXI_wstrb(processing_system7_0_axi_periph_M05_AXI_WSTRB),
        .M05_AXI_wvalid(processing_system7_0_axi_periph_M05_AXI_WVALID),
        .M06_ACLK(processing_system7_0_FCLK_CLK0),
        .M06_ARESETN(rst_processing_system7_0_50M_peripheral_aresetn),
        .M06_AXI_araddr(processing_system7_0_axi_periph_M06_AXI_ARADDR),
        .M06_AXI_arready(processing_system7_0_axi_periph_M06_AXI_ARREADY),
        .M06_AXI_arvalid(processing_system7_0_axi_periph_M06_AXI_ARVALID),
        .M06_AXI_awaddr(processing_system7_0_axi_periph_M06_AXI_AWADDR),
        .M06_AXI_awready(processing_system7_0_axi_periph_M06_AXI_AWREADY),
        .M06_AXI_awvalid(processing_system7_0_axi_periph_M06_AXI_AWVALID),
        .M06_AXI_bready(processing_system7_0_axi_periph_M06_AXI_BREADY),
        .M06_AXI_bresp(processing_system7_0_axi_periph_M06_AXI_BRESP),
        .M06_AXI_bvalid(processing_system7_0_axi_periph_M06_AXI_BVALID),
        .M06_AXI_rdata(processing_system7_0_axi_periph_M06_AXI_RDATA),
        .M06_AXI_rready(processing_system7_0_axi_periph_M06_AXI_RREADY),
        .M06_AXI_rresp(processing_system7_0_axi_periph_M06_AXI_RRESP),
        .M06_AXI_rvalid(processing_system7_0_axi_periph_M06_AXI_RVALID),
        .M06_AXI_wdata(processing_system7_0_axi_periph_M06_AXI_WDATA),
        .M06_AXI_wready(processing_system7_0_axi_periph_M06_AXI_WREADY),
        .M06_AXI_wstrb(processing_system7_0_axi_periph_M06_AXI_WSTRB),
        .M06_AXI_wvalid(processing_system7_0_axi_periph_M06_AXI_WVALID),
        .M07_ACLK(processing_system7_0_FCLK_CLK0),
        .M07_ARESETN(rst_processing_system7_0_50M_peripheral_aresetn),
        .M07_AXI_araddr(processing_system7_0_axi_periph_M07_AXI_ARADDR),
        .M07_AXI_arready(processing_system7_0_axi_periph_M07_AXI_ARREADY),
        .M07_AXI_arvalid(processing_system7_0_axi_periph_M07_AXI_ARVALID),
        .M07_AXI_awaddr(processing_system7_0_axi_periph_M07_AXI_AWADDR),
        .M07_AXI_awready(processing_system7_0_axi_periph_M07_AXI_AWREADY),
        .M07_AXI_awvalid(processing_system7_0_axi_periph_M07_AXI_AWVALID),
        .M07_AXI_bready(processing_system7_0_axi_periph_M07_AXI_BREADY),
        .M07_AXI_bresp(processing_system7_0_axi_periph_M07_AXI_BRESP),
        .M07_AXI_bvalid(processing_system7_0_axi_periph_M07_AXI_BVALID),
        .M07_AXI_rdata(processing_system7_0_axi_periph_M07_AXI_RDATA),
        .M07_AXI_rready(processing_system7_0_axi_periph_M07_AXI_RREADY),
        .M07_AXI_rresp(processing_system7_0_axi_periph_M07_AXI_RRESP),
        .M07_AXI_rvalid(processing_system7_0_axi_periph_M07_AXI_RVALID),
        .M07_AXI_wdata(processing_system7_0_axi_periph_M07_AXI_WDATA),
        .M07_AXI_wready(processing_system7_0_axi_periph_M07_AXI_WREADY),
        .M07_AXI_wstrb(processing_system7_0_axi_periph_M07_AXI_WSTRB),
        .M07_AXI_wvalid(processing_system7_0_axi_periph_M07_AXI_WVALID),
        .M08_ACLK(processing_system7_0_FCLK_CLK0),
        .M08_ARESETN(rst_processing_system7_0_50M_peripheral_aresetn),
        .M08_AXI_araddr(processing_system7_0_axi_periph_M08_AXI_ARADDR),
        .M08_AXI_arprot(processing_system7_0_axi_periph_M08_AXI_ARPROT),
        .M08_AXI_arready(processing_system7_0_axi_periph_M08_AXI_ARREADY),
        .M08_AXI_arvalid(processing_system7_0_axi_periph_M08_AXI_ARVALID),
        .M08_AXI_awaddr(processing_system7_0_axi_periph_M08_AXI_AWADDR),
        .M08_AXI_awprot(processing_system7_0_axi_periph_M08_AXI_AWPROT),
        .M08_AXI_awready(processing_system7_0_axi_periph_M08_AXI_AWREADY),
        .M08_AXI_awvalid(processing_system7_0_axi_periph_M08_AXI_AWVALID),
        .M08_AXI_bready(processing_system7_0_axi_periph_M08_AXI_BREADY),
        .M08_AXI_bresp(processing_system7_0_axi_periph_M08_AXI_BRESP),
        .M08_AXI_bvalid(processing_system7_0_axi_periph_M08_AXI_BVALID),
        .M08_AXI_rdata(processing_system7_0_axi_periph_M08_AXI_RDATA),
        .M08_AXI_rready(processing_system7_0_axi_periph_M08_AXI_RREADY),
        .M08_AXI_rresp(processing_system7_0_axi_periph_M08_AXI_RRESP),
        .M08_AXI_rvalid(processing_system7_0_axi_periph_M08_AXI_RVALID),
        .M08_AXI_wdata(processing_system7_0_axi_periph_M08_AXI_WDATA),
        .M08_AXI_wready(processing_system7_0_axi_periph_M08_AXI_WREADY),
        .M08_AXI_wstrb(processing_system7_0_axi_periph_M08_AXI_WSTRB),
        .M08_AXI_wvalid(processing_system7_0_axi_periph_M08_AXI_WVALID),
        .M09_ACLK(processing_system7_0_FCLK_CLK0),
        .M09_ARESETN(rst_processing_system7_0_50M_peripheral_aresetn),
        .M09_AXI_araddr(processing_system7_0_axi_periph_M09_AXI_ARADDR),
        .M09_AXI_arready(processing_system7_0_axi_periph_M09_AXI_ARREADY),
        .M09_AXI_arvalid(processing_system7_0_axi_periph_M09_AXI_ARVALID),
        .M09_AXI_awaddr(processing_system7_0_axi_periph_M09_AXI_AWADDR),
        .M09_AXI_awready(processing_system7_0_axi_periph_M09_AXI_AWREADY),
        .M09_AXI_awvalid(processing_system7_0_axi_periph_M09_AXI_AWVALID),
        .M09_AXI_bready(processing_system7_0_axi_periph_M09_AXI_BREADY),
        .M09_AXI_bresp(processing_system7_0_axi_periph_M09_AXI_BRESP),
        .M09_AXI_bvalid(processing_system7_0_axi_periph_M09_AXI_BVALID),
        .M09_AXI_rdata(processing_system7_0_axi_periph_M09_AXI_RDATA),
        .M09_AXI_rready(processing_system7_0_axi_periph_M09_AXI_RREADY),
        .M09_AXI_rresp(processing_system7_0_axi_periph_M09_AXI_RRESP),
        .M09_AXI_rvalid(processing_system7_0_axi_periph_M09_AXI_RVALID),
        .M09_AXI_wdata(processing_system7_0_axi_periph_M09_AXI_WDATA),
        .M09_AXI_wready(processing_system7_0_axi_periph_M09_AXI_WREADY),
        .M09_AXI_wstrb(processing_system7_0_axi_periph_M09_AXI_WSTRB),
        .M09_AXI_wvalid(processing_system7_0_axi_periph_M09_AXI_WVALID),
        .M10_ACLK(processing_system7_0_FCLK_CLK0),
        .M10_ARESETN(rst_processing_system7_0_50M_peripheral_aresetn),
        .M10_AXI_araddr(processing_system7_0_axi_periph_M10_AXI_ARADDR),
        .M10_AXI_arready(processing_system7_0_axi_periph_M10_AXI_ARREADY),
        .M10_AXI_arvalid(processing_system7_0_axi_periph_M10_AXI_ARVALID),
        .M10_AXI_awaddr(processing_system7_0_axi_periph_M10_AXI_AWADDR),
        .M10_AXI_awready(processing_system7_0_axi_periph_M10_AXI_AWREADY),
        .M10_AXI_awvalid(processing_system7_0_axi_periph_M10_AXI_AWVALID),
        .M10_AXI_bready(processing_system7_0_axi_periph_M10_AXI_BREADY),
        .M10_AXI_bresp(processing_system7_0_axi_periph_M10_AXI_BRESP),
        .M10_AXI_bvalid(processing_system7_0_axi_periph_M10_AXI_BVALID),
        .M10_AXI_rdata(processing_system7_0_axi_periph_M10_AXI_RDATA),
        .M10_AXI_rready(processing_system7_0_axi_periph_M10_AXI_RREADY),
        .M10_AXI_rresp(processing_system7_0_axi_periph_M10_AXI_RRESP),
        .M10_AXI_rvalid(processing_system7_0_axi_periph_M10_AXI_RVALID),
        .M10_AXI_wdata(processing_system7_0_axi_periph_M10_AXI_WDATA),
        .M10_AXI_wready(processing_system7_0_axi_periph_M10_AXI_WREADY),
        .M10_AXI_wvalid(processing_system7_0_axi_periph_M10_AXI_WVALID),
        .M11_ACLK(processing_system7_0_FCLK_CLK0),
        .M11_ARESETN(rst_processing_system7_0_50M_peripheral_aresetn),
        .M11_AXI_araddr(processing_system7_0_axi_periph_M11_AXI_ARADDR),
        .M11_AXI_arready(processing_system7_0_axi_periph_M11_AXI_ARREADY),
        .M11_AXI_arvalid(processing_system7_0_axi_periph_M11_AXI_ARVALID),
        .M11_AXI_awaddr(processing_system7_0_axi_periph_M11_AXI_AWADDR),
        .M11_AXI_awready(processing_system7_0_axi_periph_M11_AXI_AWREADY),
        .M11_AXI_awvalid(processing_system7_0_axi_periph_M11_AXI_AWVALID),
        .M11_AXI_bready(processing_system7_0_axi_periph_M11_AXI_BREADY),
        .M11_AXI_bresp(processing_system7_0_axi_periph_M11_AXI_BRESP),
        .M11_AXI_bvalid(processing_system7_0_axi_periph_M11_AXI_BVALID),
        .M11_AXI_rdata(processing_system7_0_axi_periph_M11_AXI_RDATA),
        .M11_AXI_rready(processing_system7_0_axi_periph_M11_AXI_RREADY),
        .M11_AXI_rresp(processing_system7_0_axi_periph_M11_AXI_RRESP),
        .M11_AXI_rvalid(processing_system7_0_axi_periph_M11_AXI_RVALID),
        .M11_AXI_wdata(processing_system7_0_axi_periph_M11_AXI_WDATA),
        .M11_AXI_wready(processing_system7_0_axi_periph_M11_AXI_WREADY),
        .M11_AXI_wstrb(processing_system7_0_axi_periph_M11_AXI_WSTRB),
        .M11_AXI_wvalid(processing_system7_0_axi_periph_M11_AXI_WVALID),
        .M12_ACLK(processing_system7_0_FCLK_CLK0),
        .M12_ARESETN(rst_processing_system7_0_50M_peripheral_aresetn),
        .M12_AXI_araddr(processing_system7_0_axi_periph_M12_AXI_ARADDR),
        .M12_AXI_arready(processing_system7_0_axi_periph_M12_AXI_ARREADY),
        .M12_AXI_arvalid(processing_system7_0_axi_periph_M12_AXI_ARVALID),
        .M12_AXI_awaddr(processing_system7_0_axi_periph_M12_AXI_AWADDR),
        .M12_AXI_awready(processing_system7_0_axi_periph_M12_AXI_AWREADY),
        .M12_AXI_awvalid(processing_system7_0_axi_periph_M12_AXI_AWVALID),
        .M12_AXI_bready(processing_system7_0_axi_periph_M12_AXI_BREADY),
        .M12_AXI_bresp(processing_system7_0_axi_periph_M12_AXI_BRESP),
        .M12_AXI_bvalid(processing_system7_0_axi_periph_M12_AXI_BVALID),
        .M12_AXI_rdata(processing_system7_0_axi_periph_M12_AXI_RDATA),
        .M12_AXI_rready(processing_system7_0_axi_periph_M12_AXI_RREADY),
        .M12_AXI_rresp(processing_system7_0_axi_periph_M12_AXI_RRESP),
        .M12_AXI_rvalid(processing_system7_0_axi_periph_M12_AXI_RVALID),
        .M12_AXI_wdata(processing_system7_0_axi_periph_M12_AXI_WDATA),
        .M12_AXI_wready(processing_system7_0_axi_periph_M12_AXI_WREADY),
        .M12_AXI_wstrb(processing_system7_0_axi_periph_M12_AXI_WSTRB),
        .M12_AXI_wvalid(processing_system7_0_axi_periph_M12_AXI_WVALID),
        .S00_ACLK(processing_system7_0_FCLK_CLK0),
        .S00_ARESETN(rst_processing_system7_0_50M_peripheral_aresetn),
        .S00_AXI_araddr(processing_system7_0_M_AXI_GP0_ARADDR),
        .S00_AXI_arburst(processing_system7_0_M_AXI_GP0_ARBURST),
        .S00_AXI_arcache(processing_system7_0_M_AXI_GP0_ARCACHE),
        .S00_AXI_arid(processing_system7_0_M_AXI_GP0_ARID),
        .S00_AXI_arlen(processing_system7_0_M_AXI_GP0_ARLEN),
        .S00_AXI_arlock(processing_system7_0_M_AXI_GP0_ARLOCK),
        .S00_AXI_arprot(processing_system7_0_M_AXI_GP0_ARPROT),
        .S00_AXI_arqos(processing_system7_0_M_AXI_GP0_ARQOS),
        .S00_AXI_arready(processing_system7_0_M_AXI_GP0_ARREADY),
        .S00_AXI_arsize(processing_system7_0_M_AXI_GP0_ARSIZE),
        .S00_AXI_arvalid(processing_system7_0_M_AXI_GP0_ARVALID),
        .S00_AXI_awaddr(processing_system7_0_M_AXI_GP0_AWADDR),
        .S00_AXI_awburst(processing_system7_0_M_AXI_GP0_AWBURST),
        .S00_AXI_awcache(processing_system7_0_M_AXI_GP0_AWCACHE),
        .S00_AXI_awid(processing_system7_0_M_AXI_GP0_AWID),
        .S00_AXI_awlen(processing_system7_0_M_AXI_GP0_AWLEN),
        .S00_AXI_awlock(processing_system7_0_M_AXI_GP0_AWLOCK),
        .S00_AXI_awprot(processing_system7_0_M_AXI_GP0_AWPROT),
        .S00_AXI_awqos(processing_system7_0_M_AXI_GP0_AWQOS),
        .S00_AXI_awready(processing_system7_0_M_AXI_GP0_AWREADY),
        .S00_AXI_awsize(processing_system7_0_M_AXI_GP0_AWSIZE),
        .S00_AXI_awvalid(processing_system7_0_M_AXI_GP0_AWVALID),
        .S00_AXI_bid(processing_system7_0_M_AXI_GP0_BID),
        .S00_AXI_bready(processing_system7_0_M_AXI_GP0_BREADY),
        .S00_AXI_bresp(processing_system7_0_M_AXI_GP0_BRESP),
        .S00_AXI_bvalid(processing_system7_0_M_AXI_GP0_BVALID),
        .S00_AXI_rdata(processing_system7_0_M_AXI_GP0_RDATA),
        .S00_AXI_rid(processing_system7_0_M_AXI_GP0_RID),
        .S00_AXI_rlast(processing_system7_0_M_AXI_GP0_RLAST),
        .S00_AXI_rready(processing_system7_0_M_AXI_GP0_RREADY),
        .S00_AXI_rresp(processing_system7_0_M_AXI_GP0_RRESP),
        .S00_AXI_rvalid(processing_system7_0_M_AXI_GP0_RVALID),
        .S00_AXI_wdata(processing_system7_0_M_AXI_GP0_WDATA),
        .S00_AXI_wid(processing_system7_0_M_AXI_GP0_WID),
        .S00_AXI_wlast(processing_system7_0_M_AXI_GP0_WLAST),
        .S00_AXI_wready(processing_system7_0_M_AXI_GP0_WREADY),
        .S00_AXI_wstrb(processing_system7_0_M_AXI_GP0_WSTRB),
        .S00_AXI_wvalid(processing_system7_0_M_AXI_GP0_WVALID));
  base_zynq_design_rst_processing_system7_0_50M_0 rst_processing_system7_0_50M
       (.aux_reset_in(1'b1),
        .dcm_locked(1'b1),
        .ext_reset_in(processing_system7_0_FCLK_RESET0_N),
        .interconnect_aresetn(rst_processing_system7_0_50M_interconnect_aresetn),
        .mb_debug_sys_rst(1'b0),
        .peripheral_aresetn(rst_processing_system7_0_50M_peripheral_aresetn),
        .slowest_sync_clk(processing_system7_0_FCLK_CLK0));
  base_zynq_design_xadc_wiz_0_0 xadc_wiz_0
       (.s_axi_aclk(processing_system7_0_FCLK_CLK0),
        .s_axi_araddr(processing_system7_0_axi_periph_M06_AXI_ARADDR[10:0]),
        .s_axi_aresetn(rst_processing_system7_0_50M_peripheral_aresetn),
        .s_axi_arready(processing_system7_0_axi_periph_M06_AXI_ARREADY),
        .s_axi_arvalid(processing_system7_0_axi_periph_M06_AXI_ARVALID),
        .s_axi_awaddr(processing_system7_0_axi_periph_M06_AXI_AWADDR[10:0]),
        .s_axi_awready(processing_system7_0_axi_periph_M06_AXI_AWREADY),
        .s_axi_awvalid(processing_system7_0_axi_periph_M06_AXI_AWVALID),
        .s_axi_bready(processing_system7_0_axi_periph_M06_AXI_BREADY),
        .s_axi_bresp(processing_system7_0_axi_periph_M06_AXI_BRESP),
        .s_axi_bvalid(processing_system7_0_axi_periph_M06_AXI_BVALID),
        .s_axi_rdata(processing_system7_0_axi_periph_M06_AXI_RDATA),
        .s_axi_rready(processing_system7_0_axi_periph_M06_AXI_RREADY),
        .s_axi_rresp(processing_system7_0_axi_periph_M06_AXI_RRESP),
        .s_axi_rvalid(processing_system7_0_axi_periph_M06_AXI_RVALID),
        .s_axi_wdata(processing_system7_0_axi_periph_M06_AXI_WDATA),
        .s_axi_wready(processing_system7_0_axi_periph_M06_AXI_WREADY),
        .s_axi_wstrb(processing_system7_0_axi_periph_M06_AXI_WSTRB),
        .s_axi_wvalid(processing_system7_0_axi_periph_M06_AXI_WVALID),
        .vn_in(1'b0),
        .vp_in(1'b0));
endmodule

module base_zynq_design_axi_interconnect_0_0
   (ACLK,
    ARESETN,
    M00_ACLK,
    M00_ARESETN,
    M00_AXI_awaddr,
    M00_AXI_awburst,
    M00_AXI_awcache,
    M00_AXI_awlen,
    M00_AXI_awlock,
    M00_AXI_awprot,
    M00_AXI_awqos,
    M00_AXI_awready,
    M00_AXI_awsize,
    M00_AXI_awvalid,
    M00_AXI_bready,
    M00_AXI_bresp,
    M00_AXI_bvalid,
    M00_AXI_wdata,
    M00_AXI_wlast,
    M00_AXI_wready,
    M00_AXI_wstrb,
    M00_AXI_wvalid,
    S00_ACLK,
    S00_ARESETN,
    S00_AXI_awaddr,
    S00_AXI_awburst,
    S00_AXI_awcache,
    S00_AXI_awlen,
    S00_AXI_awprot,
    S00_AXI_awready,
    S00_AXI_awsize,
    S00_AXI_awvalid,
    S00_AXI_bready,
    S00_AXI_bresp,
    S00_AXI_bvalid,
    S00_AXI_wdata,
    S00_AXI_wlast,
    S00_AXI_wready,
    S00_AXI_wstrb,
    S00_AXI_wvalid);
  input ACLK;
  input [0:0]ARESETN;
  input M00_ACLK;
  input [0:0]M00_ARESETN;
  output [31:0]M00_AXI_awaddr;
  output [1:0]M00_AXI_awburst;
  output [3:0]M00_AXI_awcache;
  output [3:0]M00_AXI_awlen;
  output [1:0]M00_AXI_awlock;
  output [2:0]M00_AXI_awprot;
  output [3:0]M00_AXI_awqos;
  input M00_AXI_awready;
  output [2:0]M00_AXI_awsize;
  output M00_AXI_awvalid;
  output M00_AXI_bready;
  input [1:0]M00_AXI_bresp;
  input M00_AXI_bvalid;
  output [63:0]M00_AXI_wdata;
  output M00_AXI_wlast;
  input M00_AXI_wready;
  output [7:0]M00_AXI_wstrb;
  output M00_AXI_wvalid;
  input S00_ACLK;
  input [0:0]S00_ARESETN;
  input [31:0]S00_AXI_awaddr;
  input [1:0]S00_AXI_awburst;
  input [3:0]S00_AXI_awcache;
  input [7:0]S00_AXI_awlen;
  input [2:0]S00_AXI_awprot;
  output S00_AXI_awready;
  input [2:0]S00_AXI_awsize;
  input S00_AXI_awvalid;
  input S00_AXI_bready;
  output [1:0]S00_AXI_bresp;
  output S00_AXI_bvalid;
  input [31:0]S00_AXI_wdata;
  input S00_AXI_wlast;
  output S00_AXI_wready;
  input [3:0]S00_AXI_wstrb;
  input S00_AXI_wvalid;

  wire S00_ACLK_1;
  wire [0:0]S00_ARESETN_1;
  wire axi_interconnect_1_ACLK_net;
  wire [0:0]axi_interconnect_1_ARESETN_net;
  wire [31:0]axi_interconnect_1_to_s00_couplers_AWADDR;
  wire [1:0]axi_interconnect_1_to_s00_couplers_AWBURST;
  wire [3:0]axi_interconnect_1_to_s00_couplers_AWCACHE;
  wire [7:0]axi_interconnect_1_to_s00_couplers_AWLEN;
  wire [2:0]axi_interconnect_1_to_s00_couplers_AWPROT;
  wire axi_interconnect_1_to_s00_couplers_AWREADY;
  wire [2:0]axi_interconnect_1_to_s00_couplers_AWSIZE;
  wire axi_interconnect_1_to_s00_couplers_AWVALID;
  wire axi_interconnect_1_to_s00_couplers_BREADY;
  wire [1:0]axi_interconnect_1_to_s00_couplers_BRESP;
  wire axi_interconnect_1_to_s00_couplers_BVALID;
  wire [31:0]axi_interconnect_1_to_s00_couplers_WDATA;
  wire axi_interconnect_1_to_s00_couplers_WLAST;
  wire axi_interconnect_1_to_s00_couplers_WREADY;
  wire [3:0]axi_interconnect_1_to_s00_couplers_WSTRB;
  wire axi_interconnect_1_to_s00_couplers_WVALID;
  wire [31:0]s00_couplers_to_axi_interconnect_1_AWADDR;
  wire [1:0]s00_couplers_to_axi_interconnect_1_AWBURST;
  wire [3:0]s00_couplers_to_axi_interconnect_1_AWCACHE;
  wire [3:0]s00_couplers_to_axi_interconnect_1_AWLEN;
  wire [1:0]s00_couplers_to_axi_interconnect_1_AWLOCK;
  wire [2:0]s00_couplers_to_axi_interconnect_1_AWPROT;
  wire [3:0]s00_couplers_to_axi_interconnect_1_AWQOS;
  wire s00_couplers_to_axi_interconnect_1_AWREADY;
  wire [2:0]s00_couplers_to_axi_interconnect_1_AWSIZE;
  wire s00_couplers_to_axi_interconnect_1_AWVALID;
  wire s00_couplers_to_axi_interconnect_1_BREADY;
  wire [1:0]s00_couplers_to_axi_interconnect_1_BRESP;
  wire s00_couplers_to_axi_interconnect_1_BVALID;
  wire [63:0]s00_couplers_to_axi_interconnect_1_WDATA;
  wire s00_couplers_to_axi_interconnect_1_WLAST;
  wire s00_couplers_to_axi_interconnect_1_WREADY;
  wire [7:0]s00_couplers_to_axi_interconnect_1_WSTRB;
  wire s00_couplers_to_axi_interconnect_1_WVALID;

  assign M00_AXI_awaddr[31:0] = s00_couplers_to_axi_interconnect_1_AWADDR;
  assign M00_AXI_awburst[1:0] = s00_couplers_to_axi_interconnect_1_AWBURST;
  assign M00_AXI_awcache[3:0] = s00_couplers_to_axi_interconnect_1_AWCACHE;
  assign M00_AXI_awlen[3:0] = s00_couplers_to_axi_interconnect_1_AWLEN;
  assign M00_AXI_awlock[1:0] = s00_couplers_to_axi_interconnect_1_AWLOCK;
  assign M00_AXI_awprot[2:0] = s00_couplers_to_axi_interconnect_1_AWPROT;
  assign M00_AXI_awqos[3:0] = s00_couplers_to_axi_interconnect_1_AWQOS;
  assign M00_AXI_awsize[2:0] = s00_couplers_to_axi_interconnect_1_AWSIZE;
  assign M00_AXI_awvalid = s00_couplers_to_axi_interconnect_1_AWVALID;
  assign M00_AXI_bready = s00_couplers_to_axi_interconnect_1_BREADY;
  assign M00_AXI_wdata[63:0] = s00_couplers_to_axi_interconnect_1_WDATA;
  assign M00_AXI_wlast = s00_couplers_to_axi_interconnect_1_WLAST;
  assign M00_AXI_wstrb[7:0] = s00_couplers_to_axi_interconnect_1_WSTRB;
  assign M00_AXI_wvalid = s00_couplers_to_axi_interconnect_1_WVALID;
  assign S00_ACLK_1 = S00_ACLK;
  assign S00_ARESETN_1 = S00_ARESETN[0];
  assign S00_AXI_awready = axi_interconnect_1_to_s00_couplers_AWREADY;
  assign S00_AXI_bresp[1:0] = axi_interconnect_1_to_s00_couplers_BRESP;
  assign S00_AXI_bvalid = axi_interconnect_1_to_s00_couplers_BVALID;
  assign S00_AXI_wready = axi_interconnect_1_to_s00_couplers_WREADY;
  assign axi_interconnect_1_ACLK_net = M00_ACLK;
  assign axi_interconnect_1_ARESETN_net = M00_ARESETN[0];
  assign axi_interconnect_1_to_s00_couplers_AWADDR = S00_AXI_awaddr[31:0];
  assign axi_interconnect_1_to_s00_couplers_AWBURST = S00_AXI_awburst[1:0];
  assign axi_interconnect_1_to_s00_couplers_AWCACHE = S00_AXI_awcache[3:0];
  assign axi_interconnect_1_to_s00_couplers_AWLEN = S00_AXI_awlen[7:0];
  assign axi_interconnect_1_to_s00_couplers_AWPROT = S00_AXI_awprot[2:0];
  assign axi_interconnect_1_to_s00_couplers_AWSIZE = S00_AXI_awsize[2:0];
  assign axi_interconnect_1_to_s00_couplers_AWVALID = S00_AXI_awvalid;
  assign axi_interconnect_1_to_s00_couplers_BREADY = S00_AXI_bready;
  assign axi_interconnect_1_to_s00_couplers_WDATA = S00_AXI_wdata[31:0];
  assign axi_interconnect_1_to_s00_couplers_WLAST = S00_AXI_wlast;
  assign axi_interconnect_1_to_s00_couplers_WSTRB = S00_AXI_wstrb[3:0];
  assign axi_interconnect_1_to_s00_couplers_WVALID = S00_AXI_wvalid;
  assign s00_couplers_to_axi_interconnect_1_AWREADY = M00_AXI_awready;
  assign s00_couplers_to_axi_interconnect_1_BRESP = M00_AXI_bresp[1:0];
  assign s00_couplers_to_axi_interconnect_1_BVALID = M00_AXI_bvalid;
  assign s00_couplers_to_axi_interconnect_1_WREADY = M00_AXI_wready;
  s00_couplers_imp_GKK9VK s00_couplers
       (.M_ACLK(axi_interconnect_1_ACLK_net),
        .M_ARESETN(axi_interconnect_1_ARESETN_net),
        .M_AXI_awaddr(s00_couplers_to_axi_interconnect_1_AWADDR),
        .M_AXI_awburst(s00_couplers_to_axi_interconnect_1_AWBURST),
        .M_AXI_awcache(s00_couplers_to_axi_interconnect_1_AWCACHE),
        .M_AXI_awlen(s00_couplers_to_axi_interconnect_1_AWLEN),
        .M_AXI_awlock(s00_couplers_to_axi_interconnect_1_AWLOCK),
        .M_AXI_awprot(s00_couplers_to_axi_interconnect_1_AWPROT),
        .M_AXI_awqos(s00_couplers_to_axi_interconnect_1_AWQOS),
        .M_AXI_awready(s00_couplers_to_axi_interconnect_1_AWREADY),
        .M_AXI_awsize(s00_couplers_to_axi_interconnect_1_AWSIZE),
        .M_AXI_awvalid(s00_couplers_to_axi_interconnect_1_AWVALID),
        .M_AXI_bready(s00_couplers_to_axi_interconnect_1_BREADY),
        .M_AXI_bresp(s00_couplers_to_axi_interconnect_1_BRESP),
        .M_AXI_bvalid(s00_couplers_to_axi_interconnect_1_BVALID),
        .M_AXI_wdata(s00_couplers_to_axi_interconnect_1_WDATA),
        .M_AXI_wlast(s00_couplers_to_axi_interconnect_1_WLAST),
        .M_AXI_wready(s00_couplers_to_axi_interconnect_1_WREADY),
        .M_AXI_wstrb(s00_couplers_to_axi_interconnect_1_WSTRB),
        .M_AXI_wvalid(s00_couplers_to_axi_interconnect_1_WVALID),
        .S_ACLK(S00_ACLK_1),
        .S_ARESETN(S00_ARESETN_1),
        .S_AXI_awaddr(axi_interconnect_1_to_s00_couplers_AWADDR),
        .S_AXI_awburst(axi_interconnect_1_to_s00_couplers_AWBURST),
        .S_AXI_awcache(axi_interconnect_1_to_s00_couplers_AWCACHE),
        .S_AXI_awlen(axi_interconnect_1_to_s00_couplers_AWLEN),
        .S_AXI_awprot(axi_interconnect_1_to_s00_couplers_AWPROT),
        .S_AXI_awready(axi_interconnect_1_to_s00_couplers_AWREADY),
        .S_AXI_awsize(axi_interconnect_1_to_s00_couplers_AWSIZE),
        .S_AXI_awvalid(axi_interconnect_1_to_s00_couplers_AWVALID),
        .S_AXI_bready(axi_interconnect_1_to_s00_couplers_BREADY),
        .S_AXI_bresp(axi_interconnect_1_to_s00_couplers_BRESP),
        .S_AXI_bvalid(axi_interconnect_1_to_s00_couplers_BVALID),
        .S_AXI_wdata(axi_interconnect_1_to_s00_couplers_WDATA),
        .S_AXI_wlast(axi_interconnect_1_to_s00_couplers_WLAST),
        .S_AXI_wready(axi_interconnect_1_to_s00_couplers_WREADY),
        .S_AXI_wstrb(axi_interconnect_1_to_s00_couplers_WSTRB),
        .S_AXI_wvalid(axi_interconnect_1_to_s00_couplers_WVALID));
endmodule

module base_zynq_design_axi_interconnect_0_1
   (ACLK,
    ARESETN,
    M00_ACLK,
    M00_ARESETN,
    M00_AXI_araddr,
    M00_AXI_arburst,
    M00_AXI_arcache,
    M00_AXI_arlen,
    M00_AXI_arlock,
    M00_AXI_arprot,
    M00_AXI_arqos,
    M00_AXI_arready,
    M00_AXI_arsize,
    M00_AXI_arvalid,
    M00_AXI_awaddr,
    M00_AXI_awburst,
    M00_AXI_awcache,
    M00_AXI_awlen,
    M00_AXI_awlock,
    M00_AXI_awprot,
    M00_AXI_awqos,
    M00_AXI_awready,
    M00_AXI_awsize,
    M00_AXI_awvalid,
    M00_AXI_bready,
    M00_AXI_bresp,
    M00_AXI_bvalid,
    M00_AXI_rdata,
    M00_AXI_rlast,
    M00_AXI_rready,
    M00_AXI_rresp,
    M00_AXI_rvalid,
    M00_AXI_wdata,
    M00_AXI_wlast,
    M00_AXI_wready,
    M00_AXI_wstrb,
    M00_AXI_wvalid,
    S00_ACLK,
    S00_ARESETN,
    S00_AXI_araddr,
    S00_AXI_arburst,
    S00_AXI_arcache,
    S00_AXI_arlen,
    S00_AXI_arprot,
    S00_AXI_arready,
    S00_AXI_arsize,
    S00_AXI_arvalid,
    S00_AXI_awaddr,
    S00_AXI_awburst,
    S00_AXI_awcache,
    S00_AXI_awlen,
    S00_AXI_awprot,
    S00_AXI_awready,
    S00_AXI_awsize,
    S00_AXI_awvalid,
    S00_AXI_bready,
    S00_AXI_bresp,
    S00_AXI_bvalid,
    S00_AXI_rdata,
    S00_AXI_rlast,
    S00_AXI_rready,
    S00_AXI_rresp,
    S00_AXI_rvalid,
    S00_AXI_wdata,
    S00_AXI_wlast,
    S00_AXI_wready,
    S00_AXI_wstrb,
    S00_AXI_wvalid);
  input ACLK;
  input [0:0]ARESETN;
  input M00_ACLK;
  input [0:0]M00_ARESETN;
  output [31:0]M00_AXI_araddr;
  output [1:0]M00_AXI_arburst;
  output [3:0]M00_AXI_arcache;
  output [3:0]M00_AXI_arlen;
  output [1:0]M00_AXI_arlock;
  output [2:0]M00_AXI_arprot;
  output [3:0]M00_AXI_arqos;
  input M00_AXI_arready;
  output [2:0]M00_AXI_arsize;
  output M00_AXI_arvalid;
  output [31:0]M00_AXI_awaddr;
  output [1:0]M00_AXI_awburst;
  output [3:0]M00_AXI_awcache;
  output [3:0]M00_AXI_awlen;
  output [1:0]M00_AXI_awlock;
  output [2:0]M00_AXI_awprot;
  output [3:0]M00_AXI_awqos;
  input M00_AXI_awready;
  output [2:0]M00_AXI_awsize;
  output M00_AXI_awvalid;
  output M00_AXI_bready;
  input [1:0]M00_AXI_bresp;
  input M00_AXI_bvalid;
  input [63:0]M00_AXI_rdata;
  input M00_AXI_rlast;
  output M00_AXI_rready;
  input [1:0]M00_AXI_rresp;
  input M00_AXI_rvalid;
  output [63:0]M00_AXI_wdata;
  output M00_AXI_wlast;
  input M00_AXI_wready;
  output [7:0]M00_AXI_wstrb;
  output M00_AXI_wvalid;
  input S00_ACLK;
  input [0:0]S00_ARESETN;
  input [31:0]S00_AXI_araddr;
  input [1:0]S00_AXI_arburst;
  input [3:0]S00_AXI_arcache;
  input [7:0]S00_AXI_arlen;
  input [2:0]S00_AXI_arprot;
  output S00_AXI_arready;
  input [2:0]S00_AXI_arsize;
  input S00_AXI_arvalid;
  input [31:0]S00_AXI_awaddr;
  input [1:0]S00_AXI_awburst;
  input [3:0]S00_AXI_awcache;
  input [7:0]S00_AXI_awlen;
  input [2:0]S00_AXI_awprot;
  output S00_AXI_awready;
  input [2:0]S00_AXI_awsize;
  input S00_AXI_awvalid;
  input S00_AXI_bready;
  output [1:0]S00_AXI_bresp;
  output S00_AXI_bvalid;
  output [31:0]S00_AXI_rdata;
  output S00_AXI_rlast;
  input S00_AXI_rready;
  output [1:0]S00_AXI_rresp;
  output S00_AXI_rvalid;
  input [31:0]S00_AXI_wdata;
  input S00_AXI_wlast;
  output S00_AXI_wready;
  input [3:0]S00_AXI_wstrb;
  input S00_AXI_wvalid;

  wire S00_ACLK_1;
  wire [0:0]S00_ARESETN_1;
  wire axi_interconnect_0_ACLK_net;
  wire [0:0]axi_interconnect_0_ARESETN_net;
  wire [31:0]axi_interconnect_0_to_s00_couplers_ARADDR;
  wire [1:0]axi_interconnect_0_to_s00_couplers_ARBURST;
  wire [3:0]axi_interconnect_0_to_s00_couplers_ARCACHE;
  wire [7:0]axi_interconnect_0_to_s00_couplers_ARLEN;
  wire [2:0]axi_interconnect_0_to_s00_couplers_ARPROT;
  wire axi_interconnect_0_to_s00_couplers_ARREADY;
  wire [2:0]axi_interconnect_0_to_s00_couplers_ARSIZE;
  wire axi_interconnect_0_to_s00_couplers_ARVALID;
  wire [31:0]axi_interconnect_0_to_s00_couplers_AWADDR;
  wire [1:0]axi_interconnect_0_to_s00_couplers_AWBURST;
  wire [3:0]axi_interconnect_0_to_s00_couplers_AWCACHE;
  wire [7:0]axi_interconnect_0_to_s00_couplers_AWLEN;
  wire [2:0]axi_interconnect_0_to_s00_couplers_AWPROT;
  wire axi_interconnect_0_to_s00_couplers_AWREADY;
  wire [2:0]axi_interconnect_0_to_s00_couplers_AWSIZE;
  wire axi_interconnect_0_to_s00_couplers_AWVALID;
  wire axi_interconnect_0_to_s00_couplers_BREADY;
  wire [1:0]axi_interconnect_0_to_s00_couplers_BRESP;
  wire axi_interconnect_0_to_s00_couplers_BVALID;
  wire [31:0]axi_interconnect_0_to_s00_couplers_RDATA;
  wire axi_interconnect_0_to_s00_couplers_RLAST;
  wire axi_interconnect_0_to_s00_couplers_RREADY;
  wire [1:0]axi_interconnect_0_to_s00_couplers_RRESP;
  wire axi_interconnect_0_to_s00_couplers_RVALID;
  wire [31:0]axi_interconnect_0_to_s00_couplers_WDATA;
  wire axi_interconnect_0_to_s00_couplers_WLAST;
  wire axi_interconnect_0_to_s00_couplers_WREADY;
  wire [3:0]axi_interconnect_0_to_s00_couplers_WSTRB;
  wire axi_interconnect_0_to_s00_couplers_WVALID;
  wire [31:0]s00_couplers_to_axi_interconnect_0_ARADDR;
  wire [1:0]s00_couplers_to_axi_interconnect_0_ARBURST;
  wire [3:0]s00_couplers_to_axi_interconnect_0_ARCACHE;
  wire [3:0]s00_couplers_to_axi_interconnect_0_ARLEN;
  wire [1:0]s00_couplers_to_axi_interconnect_0_ARLOCK;
  wire [2:0]s00_couplers_to_axi_interconnect_0_ARPROT;
  wire [3:0]s00_couplers_to_axi_interconnect_0_ARQOS;
  wire s00_couplers_to_axi_interconnect_0_ARREADY;
  wire [2:0]s00_couplers_to_axi_interconnect_0_ARSIZE;
  wire s00_couplers_to_axi_interconnect_0_ARVALID;
  wire [31:0]s00_couplers_to_axi_interconnect_0_AWADDR;
  wire [1:0]s00_couplers_to_axi_interconnect_0_AWBURST;
  wire [3:0]s00_couplers_to_axi_interconnect_0_AWCACHE;
  wire [3:0]s00_couplers_to_axi_interconnect_0_AWLEN;
  wire [1:0]s00_couplers_to_axi_interconnect_0_AWLOCK;
  wire [2:0]s00_couplers_to_axi_interconnect_0_AWPROT;
  wire [3:0]s00_couplers_to_axi_interconnect_0_AWQOS;
  wire s00_couplers_to_axi_interconnect_0_AWREADY;
  wire [2:0]s00_couplers_to_axi_interconnect_0_AWSIZE;
  wire s00_couplers_to_axi_interconnect_0_AWVALID;
  wire s00_couplers_to_axi_interconnect_0_BREADY;
  wire [1:0]s00_couplers_to_axi_interconnect_0_BRESP;
  wire s00_couplers_to_axi_interconnect_0_BVALID;
  wire [63:0]s00_couplers_to_axi_interconnect_0_RDATA;
  wire s00_couplers_to_axi_interconnect_0_RLAST;
  wire s00_couplers_to_axi_interconnect_0_RREADY;
  wire [1:0]s00_couplers_to_axi_interconnect_0_RRESP;
  wire s00_couplers_to_axi_interconnect_0_RVALID;
  wire [63:0]s00_couplers_to_axi_interconnect_0_WDATA;
  wire s00_couplers_to_axi_interconnect_0_WLAST;
  wire s00_couplers_to_axi_interconnect_0_WREADY;
  wire [7:0]s00_couplers_to_axi_interconnect_0_WSTRB;
  wire s00_couplers_to_axi_interconnect_0_WVALID;

  assign M00_AXI_araddr[31:0] = s00_couplers_to_axi_interconnect_0_ARADDR;
  assign M00_AXI_arburst[1:0] = s00_couplers_to_axi_interconnect_0_ARBURST;
  assign M00_AXI_arcache[3:0] = s00_couplers_to_axi_interconnect_0_ARCACHE;
  assign M00_AXI_arlen[3:0] = s00_couplers_to_axi_interconnect_0_ARLEN;
  assign M00_AXI_arlock[1:0] = s00_couplers_to_axi_interconnect_0_ARLOCK;
  assign M00_AXI_arprot[2:0] = s00_couplers_to_axi_interconnect_0_ARPROT;
  assign M00_AXI_arqos[3:0] = s00_couplers_to_axi_interconnect_0_ARQOS;
  assign M00_AXI_arsize[2:0] = s00_couplers_to_axi_interconnect_0_ARSIZE;
  assign M00_AXI_arvalid = s00_couplers_to_axi_interconnect_0_ARVALID;
  assign M00_AXI_awaddr[31:0] = s00_couplers_to_axi_interconnect_0_AWADDR;
  assign M00_AXI_awburst[1:0] = s00_couplers_to_axi_interconnect_0_AWBURST;
  assign M00_AXI_awcache[3:0] = s00_couplers_to_axi_interconnect_0_AWCACHE;
  assign M00_AXI_awlen[3:0] = s00_couplers_to_axi_interconnect_0_AWLEN;
  assign M00_AXI_awlock[1:0] = s00_couplers_to_axi_interconnect_0_AWLOCK;
  assign M00_AXI_awprot[2:0] = s00_couplers_to_axi_interconnect_0_AWPROT;
  assign M00_AXI_awqos[3:0] = s00_couplers_to_axi_interconnect_0_AWQOS;
  assign M00_AXI_awsize[2:0] = s00_couplers_to_axi_interconnect_0_AWSIZE;
  assign M00_AXI_awvalid = s00_couplers_to_axi_interconnect_0_AWVALID;
  assign M00_AXI_bready = s00_couplers_to_axi_interconnect_0_BREADY;
  assign M00_AXI_rready = s00_couplers_to_axi_interconnect_0_RREADY;
  assign M00_AXI_wdata[63:0] = s00_couplers_to_axi_interconnect_0_WDATA;
  assign M00_AXI_wlast = s00_couplers_to_axi_interconnect_0_WLAST;
  assign M00_AXI_wstrb[7:0] = s00_couplers_to_axi_interconnect_0_WSTRB;
  assign M00_AXI_wvalid = s00_couplers_to_axi_interconnect_0_WVALID;
  assign S00_ACLK_1 = S00_ACLK;
  assign S00_ARESETN_1 = S00_ARESETN[0];
  assign S00_AXI_arready = axi_interconnect_0_to_s00_couplers_ARREADY;
  assign S00_AXI_awready = axi_interconnect_0_to_s00_couplers_AWREADY;
  assign S00_AXI_bresp[1:0] = axi_interconnect_0_to_s00_couplers_BRESP;
  assign S00_AXI_bvalid = axi_interconnect_0_to_s00_couplers_BVALID;
  assign S00_AXI_rdata[31:0] = axi_interconnect_0_to_s00_couplers_RDATA;
  assign S00_AXI_rlast = axi_interconnect_0_to_s00_couplers_RLAST;
  assign S00_AXI_rresp[1:0] = axi_interconnect_0_to_s00_couplers_RRESP;
  assign S00_AXI_rvalid = axi_interconnect_0_to_s00_couplers_RVALID;
  assign S00_AXI_wready = axi_interconnect_0_to_s00_couplers_WREADY;
  assign axi_interconnect_0_ACLK_net = M00_ACLK;
  assign axi_interconnect_0_ARESETN_net = M00_ARESETN[0];
  assign axi_interconnect_0_to_s00_couplers_ARADDR = S00_AXI_araddr[31:0];
  assign axi_interconnect_0_to_s00_couplers_ARBURST = S00_AXI_arburst[1:0];
  assign axi_interconnect_0_to_s00_couplers_ARCACHE = S00_AXI_arcache[3:0];
  assign axi_interconnect_0_to_s00_couplers_ARLEN = S00_AXI_arlen[7:0];
  assign axi_interconnect_0_to_s00_couplers_ARPROT = S00_AXI_arprot[2:0];
  assign axi_interconnect_0_to_s00_couplers_ARSIZE = S00_AXI_arsize[2:0];
  assign axi_interconnect_0_to_s00_couplers_ARVALID = S00_AXI_arvalid;
  assign axi_interconnect_0_to_s00_couplers_AWADDR = S00_AXI_awaddr[31:0];
  assign axi_interconnect_0_to_s00_couplers_AWBURST = S00_AXI_awburst[1:0];
  assign axi_interconnect_0_to_s00_couplers_AWCACHE = S00_AXI_awcache[3:0];
  assign axi_interconnect_0_to_s00_couplers_AWLEN = S00_AXI_awlen[7:0];
  assign axi_interconnect_0_to_s00_couplers_AWPROT = S00_AXI_awprot[2:0];
  assign axi_interconnect_0_to_s00_couplers_AWSIZE = S00_AXI_awsize[2:0];
  assign axi_interconnect_0_to_s00_couplers_AWVALID = S00_AXI_awvalid;
  assign axi_interconnect_0_to_s00_couplers_BREADY = S00_AXI_bready;
  assign axi_interconnect_0_to_s00_couplers_RREADY = S00_AXI_rready;
  assign axi_interconnect_0_to_s00_couplers_WDATA = S00_AXI_wdata[31:0];
  assign axi_interconnect_0_to_s00_couplers_WLAST = S00_AXI_wlast;
  assign axi_interconnect_0_to_s00_couplers_WSTRB = S00_AXI_wstrb[3:0];
  assign axi_interconnect_0_to_s00_couplers_WVALID = S00_AXI_wvalid;
  assign s00_couplers_to_axi_interconnect_0_ARREADY = M00_AXI_arready;
  assign s00_couplers_to_axi_interconnect_0_AWREADY = M00_AXI_awready;
  assign s00_couplers_to_axi_interconnect_0_BRESP = M00_AXI_bresp[1:0];
  assign s00_couplers_to_axi_interconnect_0_BVALID = M00_AXI_bvalid;
  assign s00_couplers_to_axi_interconnect_0_RDATA = M00_AXI_rdata[63:0];
  assign s00_couplers_to_axi_interconnect_0_RLAST = M00_AXI_rlast;
  assign s00_couplers_to_axi_interconnect_0_RRESP = M00_AXI_rresp[1:0];
  assign s00_couplers_to_axi_interconnect_0_RVALID = M00_AXI_rvalid;
  assign s00_couplers_to_axi_interconnect_0_WREADY = M00_AXI_wready;
  s00_couplers_imp_26EUIW s00_couplers
       (.M_ACLK(axi_interconnect_0_ACLK_net),
        .M_ARESETN(axi_interconnect_0_ARESETN_net),
        .M_AXI_araddr(s00_couplers_to_axi_interconnect_0_ARADDR),
        .M_AXI_arburst(s00_couplers_to_axi_interconnect_0_ARBURST),
        .M_AXI_arcache(s00_couplers_to_axi_interconnect_0_ARCACHE),
        .M_AXI_arlen(s00_couplers_to_axi_interconnect_0_ARLEN),
        .M_AXI_arlock(s00_couplers_to_axi_interconnect_0_ARLOCK),
        .M_AXI_arprot(s00_couplers_to_axi_interconnect_0_ARPROT),
        .M_AXI_arqos(s00_couplers_to_axi_interconnect_0_ARQOS),
        .M_AXI_arready(s00_couplers_to_axi_interconnect_0_ARREADY),
        .M_AXI_arsize(s00_couplers_to_axi_interconnect_0_ARSIZE),
        .M_AXI_arvalid(s00_couplers_to_axi_interconnect_0_ARVALID),
        .M_AXI_awaddr(s00_couplers_to_axi_interconnect_0_AWADDR),
        .M_AXI_awburst(s00_couplers_to_axi_interconnect_0_AWBURST),
        .M_AXI_awcache(s00_couplers_to_axi_interconnect_0_AWCACHE),
        .M_AXI_awlen(s00_couplers_to_axi_interconnect_0_AWLEN),
        .M_AXI_awlock(s00_couplers_to_axi_interconnect_0_AWLOCK),
        .M_AXI_awprot(s00_couplers_to_axi_interconnect_0_AWPROT),
        .M_AXI_awqos(s00_couplers_to_axi_interconnect_0_AWQOS),
        .M_AXI_awready(s00_couplers_to_axi_interconnect_0_AWREADY),
        .M_AXI_awsize(s00_couplers_to_axi_interconnect_0_AWSIZE),
        .M_AXI_awvalid(s00_couplers_to_axi_interconnect_0_AWVALID),
        .M_AXI_bready(s00_couplers_to_axi_interconnect_0_BREADY),
        .M_AXI_bresp(s00_couplers_to_axi_interconnect_0_BRESP),
        .M_AXI_bvalid(s00_couplers_to_axi_interconnect_0_BVALID),
        .M_AXI_rdata(s00_couplers_to_axi_interconnect_0_RDATA),
        .M_AXI_rlast(s00_couplers_to_axi_interconnect_0_RLAST),
        .M_AXI_rready(s00_couplers_to_axi_interconnect_0_RREADY),
        .M_AXI_rresp(s00_couplers_to_axi_interconnect_0_RRESP),
        .M_AXI_rvalid(s00_couplers_to_axi_interconnect_0_RVALID),
        .M_AXI_wdata(s00_couplers_to_axi_interconnect_0_WDATA),
        .M_AXI_wlast(s00_couplers_to_axi_interconnect_0_WLAST),
        .M_AXI_wready(s00_couplers_to_axi_interconnect_0_WREADY),
        .M_AXI_wstrb(s00_couplers_to_axi_interconnect_0_WSTRB),
        .M_AXI_wvalid(s00_couplers_to_axi_interconnect_0_WVALID),
        .S_ACLK(S00_ACLK_1),
        .S_ARESETN(S00_ARESETN_1),
        .S_AXI_araddr(axi_interconnect_0_to_s00_couplers_ARADDR),
        .S_AXI_arburst(axi_interconnect_0_to_s00_couplers_ARBURST),
        .S_AXI_arcache(axi_interconnect_0_to_s00_couplers_ARCACHE),
        .S_AXI_arlen(axi_interconnect_0_to_s00_couplers_ARLEN),
        .S_AXI_arprot(axi_interconnect_0_to_s00_couplers_ARPROT),
        .S_AXI_arready(axi_interconnect_0_to_s00_couplers_ARREADY),
        .S_AXI_arsize(axi_interconnect_0_to_s00_couplers_ARSIZE),
        .S_AXI_arvalid(axi_interconnect_0_to_s00_couplers_ARVALID),
        .S_AXI_awaddr(axi_interconnect_0_to_s00_couplers_AWADDR),
        .S_AXI_awburst(axi_interconnect_0_to_s00_couplers_AWBURST),
        .S_AXI_awcache(axi_interconnect_0_to_s00_couplers_AWCACHE),
        .S_AXI_awlen(axi_interconnect_0_to_s00_couplers_AWLEN),
        .S_AXI_awprot(axi_interconnect_0_to_s00_couplers_AWPROT),
        .S_AXI_awready(axi_interconnect_0_to_s00_couplers_AWREADY),
        .S_AXI_awsize(axi_interconnect_0_to_s00_couplers_AWSIZE),
        .S_AXI_awvalid(axi_interconnect_0_to_s00_couplers_AWVALID),
        .S_AXI_bready(axi_interconnect_0_to_s00_couplers_BREADY),
        .S_AXI_bresp(axi_interconnect_0_to_s00_couplers_BRESP),
        .S_AXI_bvalid(axi_interconnect_0_to_s00_couplers_BVALID),
        .S_AXI_rdata(axi_interconnect_0_to_s00_couplers_RDATA),
        .S_AXI_rlast(axi_interconnect_0_to_s00_couplers_RLAST),
        .S_AXI_rready(axi_interconnect_0_to_s00_couplers_RREADY),
        .S_AXI_rresp(axi_interconnect_0_to_s00_couplers_RRESP),
        .S_AXI_rvalid(axi_interconnect_0_to_s00_couplers_RVALID),
        .S_AXI_wdata(axi_interconnect_0_to_s00_couplers_WDATA),
        .S_AXI_wlast(axi_interconnect_0_to_s00_couplers_WLAST),
        .S_AXI_wready(axi_interconnect_0_to_s00_couplers_WREADY),
        .S_AXI_wstrb(axi_interconnect_0_to_s00_couplers_WSTRB),
        .S_AXI_wvalid(axi_interconnect_0_to_s00_couplers_WVALID));
endmodule

module base_zynq_design_axis_interconnect_0_0
   (ACLK,
    ARESETN,
    M00_AXIS_ACLK,
    M00_AXIS_ARESETN,
    M00_AXIS_tdata,
    M00_AXIS_tkeep,
    M00_AXIS_tlast,
    M00_AXIS_tready,
    M00_AXIS_tvalid,
    S00_ARB_REQ_SUPPRESS,
    S00_AXIS_ACLK,
    S00_AXIS_ARESETN,
    S00_AXIS_tdata,
    S00_AXIS_tlast,
    S00_AXIS_tready,
    S00_AXIS_tvalid,
    S01_ARB_REQ_SUPPRESS,
    S01_AXIS_ACLK,
    S01_AXIS_ARESETN,
    S01_AXIS_tdata,
    S01_AXIS_tlast,
    S01_AXIS_tready,
    S01_AXIS_tvalid);
  input ACLK;
  input [0:0]ARESETN;
  input M00_AXIS_ACLK;
  input [0:0]M00_AXIS_ARESETN;
  output [31:0]M00_AXIS_tdata;
  output [3:0]M00_AXIS_tkeep;
  output M00_AXIS_tlast;
  input M00_AXIS_tready;
  output M00_AXIS_tvalid;
  input [0:0]S00_ARB_REQ_SUPPRESS;
  input S00_AXIS_ACLK;
  input [0:0]S00_AXIS_ARESETN;
  input [31:0]S00_AXIS_tdata;
  input S00_AXIS_tlast;
  output S00_AXIS_tready;
  input S00_AXIS_tvalid;
  input [0:0]S01_ARB_REQ_SUPPRESS;
  input S01_AXIS_ACLK;
  input [0:0]S01_AXIS_ARESETN;
  input [31:0]S01_AXIS_tdata;
  input S01_AXIS_tlast;
  output S01_AXIS_tready;
  input S01_AXIS_tvalid;

  wire axis_interconnect_0_ACLK_net;
  wire [0:0]axis_interconnect_0_ARESETN_net;
  wire [31:0]axis_interconnect_0_to_s00_couplers_TDATA;
  wire axis_interconnect_0_to_s00_couplers_TLAST;
  wire axis_interconnect_0_to_s00_couplers_TREADY;
  wire axis_interconnect_0_to_s00_couplers_TVALID;
  wire [31:0]axis_interconnect_0_to_s01_couplers_TDATA;
  wire axis_interconnect_0_to_s01_couplers_TLAST;
  wire axis_interconnect_0_to_s01_couplers_TREADY;
  wire axis_interconnect_0_to_s01_couplers_TVALID;
  wire [31:0]m00_couplers_to_axis_interconnect_0_TDATA;
  wire [3:0]m00_couplers_to_axis_interconnect_0_TKEEP;
  wire m00_couplers_to_axis_interconnect_0_TLAST;
  wire m00_couplers_to_axis_interconnect_0_TREADY;
  wire m00_couplers_to_axis_interconnect_0_TVALID;
  wire [0:0]s00_arb_req_suppress_to_s_arb_req_suppress_concat;
  wire [31:0]s00_couplers_to_xbar_TDATA;
  wire s00_couplers_to_xbar_TLAST;
  wire [0:0]s00_couplers_to_xbar_TREADY;
  wire s00_couplers_to_xbar_TVALID;
  wire [0:0]s01_arb_req_suppress_to_s_arb_req_suppress_concat;
  wire [31:0]s01_couplers_to_xbar_TDATA;
  wire s01_couplers_to_xbar_TLAST;
  wire [1:1]s01_couplers_to_xbar_TREADY;
  wire s01_couplers_to_xbar_TVALID;
  wire [1:0]s_arb_req_suppress_concat_dout;
  wire [31:0]xbar_to_m00_couplers_TDATA;
  wire [0:0]xbar_to_m00_couplers_TLAST;
  wire xbar_to_m00_couplers_TREADY;
  wire [0:0]xbar_to_m00_couplers_TVALID;

  assign M00_AXIS_tdata[31:0] = m00_couplers_to_axis_interconnect_0_TDATA;
  assign M00_AXIS_tkeep[3:0] = m00_couplers_to_axis_interconnect_0_TKEEP;
  assign M00_AXIS_tlast = m00_couplers_to_axis_interconnect_0_TLAST;
  assign M00_AXIS_tvalid = m00_couplers_to_axis_interconnect_0_TVALID;
  assign S00_AXIS_tready = axis_interconnect_0_to_s00_couplers_TREADY;
  assign S01_AXIS_tready = axis_interconnect_0_to_s01_couplers_TREADY;
  assign axis_interconnect_0_ACLK_net = ACLK;
  assign axis_interconnect_0_ARESETN_net = ARESETN[0];
  assign axis_interconnect_0_to_s00_couplers_TDATA = S00_AXIS_tdata[31:0];
  assign axis_interconnect_0_to_s00_couplers_TLAST = S00_AXIS_tlast;
  assign axis_interconnect_0_to_s00_couplers_TVALID = S00_AXIS_tvalid;
  assign axis_interconnect_0_to_s01_couplers_TDATA = S01_AXIS_tdata[31:0];
  assign axis_interconnect_0_to_s01_couplers_TLAST = S01_AXIS_tlast;
  assign axis_interconnect_0_to_s01_couplers_TVALID = S01_AXIS_tvalid;
  assign m00_couplers_to_axis_interconnect_0_TREADY = M00_AXIS_tready;
  assign s00_arb_req_suppress_to_s_arb_req_suppress_concat = S00_ARB_REQ_SUPPRESS[0];
  assign s01_arb_req_suppress_to_s_arb_req_suppress_concat = S01_ARB_REQ_SUPPRESS[0];
  m00_couplers_imp_1AOQ6J3 m00_couplers
       (.M_AXIS_ACLK(axis_interconnect_0_ACLK_net),
        .M_AXIS_ARESETN(axis_interconnect_0_ARESETN_net),
        .M_AXIS_tdata(m00_couplers_to_axis_interconnect_0_TDATA),
        .M_AXIS_tkeep(m00_couplers_to_axis_interconnect_0_TKEEP),
        .M_AXIS_tlast(m00_couplers_to_axis_interconnect_0_TLAST),
        .M_AXIS_tready(m00_couplers_to_axis_interconnect_0_TREADY),
        .M_AXIS_tvalid(m00_couplers_to_axis_interconnect_0_TVALID),
        .S_AXIS_ACLK(axis_interconnect_0_ACLK_net),
        .S_AXIS_ARESETN(axis_interconnect_0_ARESETN_net),
        .S_AXIS_tdata(xbar_to_m00_couplers_TDATA),
        .S_AXIS_tlast(xbar_to_m00_couplers_TLAST),
        .S_AXIS_tready(xbar_to_m00_couplers_TREADY),
        .S_AXIS_tvalid(xbar_to_m00_couplers_TVALID));
  s00_couplers_imp_1FWH62H s00_couplers
       (.M_AXIS_ACLK(axis_interconnect_0_ACLK_net),
        .M_AXIS_ARESETN(axis_interconnect_0_ARESETN_net),
        .M_AXIS_tdata(s00_couplers_to_xbar_TDATA),
        .M_AXIS_tlast(s00_couplers_to_xbar_TLAST),
        .M_AXIS_tready(s00_couplers_to_xbar_TREADY),
        .M_AXIS_tvalid(s00_couplers_to_xbar_TVALID),
        .S_AXIS_ACLK(axis_interconnect_0_ACLK_net),
        .S_AXIS_ARESETN(axis_interconnect_0_ARESETN_net),
        .S_AXIS_tdata(axis_interconnect_0_to_s00_couplers_TDATA),
        .S_AXIS_tlast(axis_interconnect_0_to_s00_couplers_TLAST),
        .S_AXIS_tready(axis_interconnect_0_to_s00_couplers_TREADY),
        .S_AXIS_tvalid(axis_interconnect_0_to_s00_couplers_TVALID));
  s01_couplers_imp_PJOTIM s01_couplers
       (.M_AXIS_ACLK(axis_interconnect_0_ACLK_net),
        .M_AXIS_ARESETN(axis_interconnect_0_ARESETN_net),
        .M_AXIS_tdata(s01_couplers_to_xbar_TDATA),
        .M_AXIS_tlast(s01_couplers_to_xbar_TLAST),
        .M_AXIS_tready(s01_couplers_to_xbar_TREADY),
        .M_AXIS_tvalid(s01_couplers_to_xbar_TVALID),
        .S_AXIS_ACLK(axis_interconnect_0_ACLK_net),
        .S_AXIS_ARESETN(axis_interconnect_0_ARESETN_net),
        .S_AXIS_tdata(axis_interconnect_0_to_s01_couplers_TDATA),
        .S_AXIS_tlast(axis_interconnect_0_to_s01_couplers_TLAST),
        .S_AXIS_tready(axis_interconnect_0_to_s01_couplers_TREADY),
        .S_AXIS_tvalid(axis_interconnect_0_to_s01_couplers_TVALID));
  base_zynq_design_s_arb_req_suppress_concat_0 s_arb_req_suppress_concat
       (.In0(s00_arb_req_suppress_to_s_arb_req_suppress_concat),
        .In1(s01_arb_req_suppress_to_s_arb_req_suppress_concat),
        .dout(s_arb_req_suppress_concat_dout));
  base_zynq_design_xbar_1 xbar
       (.aclk(axis_interconnect_0_ACLK_net),
        .aresetn(axis_interconnect_0_ARESETN_net),
        .m_axis_tdata(xbar_to_m00_couplers_TDATA),
        .m_axis_tlast(xbar_to_m00_couplers_TLAST),
        .m_axis_tready(xbar_to_m00_couplers_TREADY),
        .m_axis_tvalid(xbar_to_m00_couplers_TVALID),
        .s_axis_tdata({s01_couplers_to_xbar_TDATA,s00_couplers_to_xbar_TDATA}),
        .s_axis_tlast({s01_couplers_to_xbar_TLAST,s00_couplers_to_xbar_TLAST}),
        .s_axis_tready({s01_couplers_to_xbar_TREADY,s00_couplers_to_xbar_TREADY}),
        .s_axis_tvalid({s01_couplers_to_xbar_TVALID,s00_couplers_to_xbar_TVALID}),
        .s_req_suppress(s_arb_req_suppress_concat_dout));
endmodule

module base_zynq_design_axis_interconnect_0_2
   (ACLK,
    ARESETN,
    M00_AXIS_ACLK,
    M00_AXIS_ARESETN,
    M00_AXIS_tdata,
    M00_AXIS_tkeep,
    M00_AXIS_tlast,
    M00_AXIS_tready,
    M00_AXIS_tvalid,
    S00_AXIS_ACLK,
    S00_AXIS_ARESETN,
    S00_AXIS_tdata,
    S00_AXIS_tkeep,
    S00_AXIS_tlast,
    S00_AXIS_tvalid);
  input ACLK;
  input [0:0]ARESETN;
  input M00_AXIS_ACLK;
  input [0:0]M00_AXIS_ARESETN;
  output [15:0]M00_AXIS_tdata;
  output [1:0]M00_AXIS_tkeep;
  output M00_AXIS_tlast;
  input M00_AXIS_tready;
  output M00_AXIS_tvalid;
  input S00_AXIS_ACLK;
  input [0:0]S00_AXIS_ARESETN;
  input [0:15]S00_AXIS_tdata;
  input [0:1]S00_AXIS_tkeep;
  input S00_AXIS_tlast;
  input S00_AXIS_tvalid;

  wire M00_AXIS_ACLK_1;
  wire [0:0]M00_AXIS_ARESETN_1;
  wire S00_AXIS_ACLK_1;
  wire [0:0]S00_AXIS_ARESETN_1;
  wire [0:15]axis_interconnect_0_to_s00_couplers_TDATA;
  wire [0:1]axis_interconnect_0_to_s00_couplers_TKEEP;
  wire axis_interconnect_0_to_s00_couplers_TLAST;
  wire axis_interconnect_0_to_s00_couplers_TVALID;
  wire [15:0]s00_couplers_to_axis_interconnect_0_TDATA;
  wire [1:0]s00_couplers_to_axis_interconnect_0_TKEEP;
  wire s00_couplers_to_axis_interconnect_0_TLAST;
  wire s00_couplers_to_axis_interconnect_0_TREADY;
  wire s00_couplers_to_axis_interconnect_0_TVALID;

  assign M00_AXIS_ACLK_1 = M00_AXIS_ACLK;
  assign M00_AXIS_ARESETN_1 = M00_AXIS_ARESETN[0];
  assign M00_AXIS_tdata[15:0] = s00_couplers_to_axis_interconnect_0_TDATA;
  assign M00_AXIS_tkeep[1:0] = s00_couplers_to_axis_interconnect_0_TKEEP;
  assign M00_AXIS_tlast = s00_couplers_to_axis_interconnect_0_TLAST;
  assign M00_AXIS_tvalid = s00_couplers_to_axis_interconnect_0_TVALID;
  assign S00_AXIS_ACLK_1 = S00_AXIS_ACLK;
  assign S00_AXIS_ARESETN_1 = S00_AXIS_ARESETN[0];
  assign axis_interconnect_0_to_s00_couplers_TDATA = S00_AXIS_tdata[0:15];
  assign axis_interconnect_0_to_s00_couplers_TKEEP = S00_AXIS_tkeep[0:1];
  assign axis_interconnect_0_to_s00_couplers_TLAST = S00_AXIS_tlast;
  assign axis_interconnect_0_to_s00_couplers_TVALID = S00_AXIS_tvalid;
  assign s00_couplers_to_axis_interconnect_0_TREADY = M00_AXIS_tready;
  s00_couplers_imp_13JUP3T s00_couplers
       (.M_AXIS_ACLK(M00_AXIS_ACLK_1),
        .M_AXIS_ARESETN(M00_AXIS_ARESETN_1),
        .M_AXIS_tdata(s00_couplers_to_axis_interconnect_0_TDATA),
        .M_AXIS_tkeep(s00_couplers_to_axis_interconnect_0_TKEEP),
        .M_AXIS_tlast(s00_couplers_to_axis_interconnect_0_TLAST),
        .M_AXIS_tready(s00_couplers_to_axis_interconnect_0_TREADY),
        .M_AXIS_tvalid(s00_couplers_to_axis_interconnect_0_TVALID),
        .S_AXIS_ACLK(S00_AXIS_ACLK_1),
        .S_AXIS_ARESETN(S00_AXIS_ARESETN_1),
        .S_AXIS_tdata(axis_interconnect_0_to_s00_couplers_TDATA),
        .S_AXIS_tkeep(axis_interconnect_0_to_s00_couplers_TKEEP),
        .S_AXIS_tlast(axis_interconnect_0_to_s00_couplers_TLAST),
        .S_AXIS_tvalid(axis_interconnect_0_to_s00_couplers_TVALID));
endmodule

module base_zynq_design_axis_interconnect_0_3
   (ACLK,
    ARESETN,
    M00_AXIS_ACLK,
    M00_AXIS_ARESETN,
    M00_AXIS_tdata,
    M00_AXIS_tkeep,
    M00_AXIS_tlast,
    M00_AXIS_tready,
    M00_AXIS_tvalid,
    S00_AXIS_ACLK,
    S00_AXIS_ARESETN,
    S00_AXIS_tdata,
    S00_AXIS_tkeep,
    S00_AXIS_tlast,
    S00_AXIS_tvalid);
  input ACLK;
  input [0:0]ARESETN;
  input M00_AXIS_ACLK;
  input [0:0]M00_AXIS_ARESETN;
  output [15:0]M00_AXIS_tdata;
  output [1:0]M00_AXIS_tkeep;
  output M00_AXIS_tlast;
  input M00_AXIS_tready;
  output M00_AXIS_tvalid;
  input S00_AXIS_ACLK;
  input [0:0]S00_AXIS_ARESETN;
  input [0:15]S00_AXIS_tdata;
  input [0:1]S00_AXIS_tkeep;
  input S00_AXIS_tlast;
  input S00_AXIS_tvalid;

  wire M00_AXIS_ACLK_1;
  wire [0:0]M00_AXIS_ARESETN_1;
  wire S00_AXIS_ACLK_1;
  wire [0:0]S00_AXIS_ARESETN_1;
  wire [0:15]axis_interconnect_1_to_s00_couplers_TDATA;
  wire [0:1]axis_interconnect_1_to_s00_couplers_TKEEP;
  wire axis_interconnect_1_to_s00_couplers_TLAST;
  wire axis_interconnect_1_to_s00_couplers_TVALID;
  wire [15:0]s00_couplers_to_axis_interconnect_1_TDATA;
  wire [1:0]s00_couplers_to_axis_interconnect_1_TKEEP;
  wire s00_couplers_to_axis_interconnect_1_TLAST;
  wire s00_couplers_to_axis_interconnect_1_TREADY;
  wire s00_couplers_to_axis_interconnect_1_TVALID;

  assign M00_AXIS_ACLK_1 = M00_AXIS_ACLK;
  assign M00_AXIS_ARESETN_1 = M00_AXIS_ARESETN[0];
  assign M00_AXIS_tdata[15:0] = s00_couplers_to_axis_interconnect_1_TDATA;
  assign M00_AXIS_tkeep[1:0] = s00_couplers_to_axis_interconnect_1_TKEEP;
  assign M00_AXIS_tlast = s00_couplers_to_axis_interconnect_1_TLAST;
  assign M00_AXIS_tvalid = s00_couplers_to_axis_interconnect_1_TVALID;
  assign S00_AXIS_ACLK_1 = S00_AXIS_ACLK;
  assign S00_AXIS_ARESETN_1 = S00_AXIS_ARESETN[0];
  assign axis_interconnect_1_to_s00_couplers_TDATA = S00_AXIS_tdata[0:15];
  assign axis_interconnect_1_to_s00_couplers_TKEEP = S00_AXIS_tkeep[0:1];
  assign axis_interconnect_1_to_s00_couplers_TLAST = S00_AXIS_tlast;
  assign axis_interconnect_1_to_s00_couplers_TVALID = S00_AXIS_tvalid;
  assign s00_couplers_to_axis_interconnect_1_TREADY = M00_AXIS_tready;
  s00_couplers_imp_1DN61ZL s00_couplers
       (.M_AXIS_ACLK(M00_AXIS_ACLK_1),
        .M_AXIS_ARESETN(M00_AXIS_ARESETN_1),
        .M_AXIS_tdata(s00_couplers_to_axis_interconnect_1_TDATA),
        .M_AXIS_tkeep(s00_couplers_to_axis_interconnect_1_TKEEP),
        .M_AXIS_tlast(s00_couplers_to_axis_interconnect_1_TLAST),
        .M_AXIS_tready(s00_couplers_to_axis_interconnect_1_TREADY),
        .M_AXIS_tvalid(s00_couplers_to_axis_interconnect_1_TVALID),
        .S_AXIS_ACLK(S00_AXIS_ACLK_1),
        .S_AXIS_ARESETN(S00_AXIS_ARESETN_1),
        .S_AXIS_tdata(axis_interconnect_1_to_s00_couplers_TDATA),
        .S_AXIS_tkeep(axis_interconnect_1_to_s00_couplers_TKEEP),
        .S_AXIS_tlast(axis_interconnect_1_to_s00_couplers_TLAST),
        .S_AXIS_tvalid(axis_interconnect_1_to_s00_couplers_TVALID));
endmodule

module base_zynq_design_axis_interconnect_0_4
   (ACLK,
    ARESETN,
    M00_AXIS_ACLK,
    M00_AXIS_ARESETN,
    M00_AXIS_tdata,
    M00_AXIS_tkeep,
    M00_AXIS_tlast,
    M00_AXIS_tready,
    M00_AXIS_tvalid,
    S00_AXIS_ACLK,
    S00_AXIS_ARESETN,
    S00_AXIS_tdata,
    S00_AXIS_tkeep,
    S00_AXIS_tlast,
    S00_AXIS_tvalid);
  input ACLK;
  input [0:0]ARESETN;
  input M00_AXIS_ACLK;
  input [0:0]M00_AXIS_ARESETN;
  output [15:0]M00_AXIS_tdata;
  output [1:0]M00_AXIS_tkeep;
  output M00_AXIS_tlast;
  input M00_AXIS_tready;
  output M00_AXIS_tvalid;
  input S00_AXIS_ACLK;
  input [0:0]S00_AXIS_ARESETN;
  input [0:15]S00_AXIS_tdata;
  input [0:1]S00_AXIS_tkeep;
  input S00_AXIS_tlast;
  input S00_AXIS_tvalid;

  wire M00_AXIS_ACLK_1;
  wire [0:0]M00_AXIS_ARESETN_1;
  wire S00_AXIS_ACLK_1;
  wire [0:0]S00_AXIS_ARESETN_1;
  wire [0:15]axis_interconnect_2_to_s00_couplers_TDATA;
  wire [0:1]axis_interconnect_2_to_s00_couplers_TKEEP;
  wire axis_interconnect_2_to_s00_couplers_TLAST;
  wire axis_interconnect_2_to_s00_couplers_TVALID;
  wire [15:0]s00_couplers_to_axis_interconnect_2_TDATA;
  wire [1:0]s00_couplers_to_axis_interconnect_2_TKEEP;
  wire s00_couplers_to_axis_interconnect_2_TLAST;
  wire s00_couplers_to_axis_interconnect_2_TREADY;
  wire s00_couplers_to_axis_interconnect_2_TVALID;

  assign M00_AXIS_ACLK_1 = M00_AXIS_ACLK;
  assign M00_AXIS_ARESETN_1 = M00_AXIS_ARESETN[0];
  assign M00_AXIS_tdata[15:0] = s00_couplers_to_axis_interconnect_2_TDATA;
  assign M00_AXIS_tkeep[1:0] = s00_couplers_to_axis_interconnect_2_TKEEP;
  assign M00_AXIS_tlast = s00_couplers_to_axis_interconnect_2_TLAST;
  assign M00_AXIS_tvalid = s00_couplers_to_axis_interconnect_2_TVALID;
  assign S00_AXIS_ACLK_1 = S00_AXIS_ACLK;
  assign S00_AXIS_ARESETN_1 = S00_AXIS_ARESETN[0];
  assign axis_interconnect_2_to_s00_couplers_TDATA = S00_AXIS_tdata[0:15];
  assign axis_interconnect_2_to_s00_couplers_TKEEP = S00_AXIS_tkeep[0:1];
  assign axis_interconnect_2_to_s00_couplers_TLAST = S00_AXIS_tlast;
  assign axis_interconnect_2_to_s00_couplers_TVALID = S00_AXIS_tvalid;
  assign s00_couplers_to_axis_interconnect_2_TREADY = M00_AXIS_tready;
  s00_couplers_imp_1WC458P s00_couplers
       (.M_AXIS_ACLK(M00_AXIS_ACLK_1),
        .M_AXIS_ARESETN(M00_AXIS_ARESETN_1),
        .M_AXIS_tdata(s00_couplers_to_axis_interconnect_2_TDATA),
        .M_AXIS_tkeep(s00_couplers_to_axis_interconnect_2_TKEEP),
        .M_AXIS_tlast(s00_couplers_to_axis_interconnect_2_TLAST),
        .M_AXIS_tready(s00_couplers_to_axis_interconnect_2_TREADY),
        .M_AXIS_tvalid(s00_couplers_to_axis_interconnect_2_TVALID),
        .S_AXIS_ACLK(S00_AXIS_ACLK_1),
        .S_AXIS_ARESETN(S00_AXIS_ARESETN_1),
        .S_AXIS_tdata(axis_interconnect_2_to_s00_couplers_TDATA),
        .S_AXIS_tkeep(axis_interconnect_2_to_s00_couplers_TKEEP),
        .S_AXIS_tlast(axis_interconnect_2_to_s00_couplers_TLAST),
        .S_AXIS_tvalid(axis_interconnect_2_to_s00_couplers_TVALID));
endmodule

module base_zynq_design_axis_interconnect_0_5
   (ACLK,
    ARESETN,
    M00_AXIS_ACLK,
    M00_AXIS_ARESETN,
    M00_AXIS_tdata,
    M00_AXIS_tkeep,
    M00_AXIS_tlast,
    M00_AXIS_tready,
    M00_AXIS_tvalid,
    S00_AXIS_ACLK,
    S00_AXIS_ARESETN,
    S00_AXIS_tdata,
    S00_AXIS_tkeep,
    S00_AXIS_tlast,
    S00_AXIS_tvalid);
  input ACLK;
  input [0:0]ARESETN;
  input M00_AXIS_ACLK;
  input [0:0]M00_AXIS_ARESETN;
  output [15:0]M00_AXIS_tdata;
  output [1:0]M00_AXIS_tkeep;
  output M00_AXIS_tlast;
  input M00_AXIS_tready;
  output M00_AXIS_tvalid;
  input S00_AXIS_ACLK;
  input [0:0]S00_AXIS_ARESETN;
  input [0:15]S00_AXIS_tdata;
  input [0:1]S00_AXIS_tkeep;
  input S00_AXIS_tlast;
  input S00_AXIS_tvalid;

  wire M00_AXIS_ACLK_1;
  wire [0:0]M00_AXIS_ARESETN_1;
  wire S00_AXIS_ACLK_1;
  wire [0:0]S00_AXIS_ARESETN_1;
  wire [0:15]axis_interconnect_3_to_s00_couplers_TDATA;
  wire [0:1]axis_interconnect_3_to_s00_couplers_TKEEP;
  wire axis_interconnect_3_to_s00_couplers_TLAST;
  wire axis_interconnect_3_to_s00_couplers_TVALID;
  wire [15:0]s00_couplers_to_axis_interconnect_3_TDATA;
  wire [1:0]s00_couplers_to_axis_interconnect_3_TKEEP;
  wire s00_couplers_to_axis_interconnect_3_TLAST;
  wire s00_couplers_to_axis_interconnect_3_TREADY;
  wire s00_couplers_to_axis_interconnect_3_TVALID;

  assign M00_AXIS_ACLK_1 = M00_AXIS_ACLK;
  assign M00_AXIS_ARESETN_1 = M00_AXIS_ARESETN[0];
  assign M00_AXIS_tdata[15:0] = s00_couplers_to_axis_interconnect_3_TDATA;
  assign M00_AXIS_tkeep[1:0] = s00_couplers_to_axis_interconnect_3_TKEEP;
  assign M00_AXIS_tlast = s00_couplers_to_axis_interconnect_3_TLAST;
  assign M00_AXIS_tvalid = s00_couplers_to_axis_interconnect_3_TVALID;
  assign S00_AXIS_ACLK_1 = S00_AXIS_ACLK;
  assign S00_AXIS_ARESETN_1 = S00_AXIS_ARESETN[0];
  assign axis_interconnect_3_to_s00_couplers_TDATA = S00_AXIS_tdata[0:15];
  assign axis_interconnect_3_to_s00_couplers_TKEEP = S00_AXIS_tkeep[0:1];
  assign axis_interconnect_3_to_s00_couplers_TLAST = S00_AXIS_tlast;
  assign axis_interconnect_3_to_s00_couplers_TVALID = S00_AXIS_tvalid;
  assign s00_couplers_to_axis_interconnect_3_TREADY = M00_AXIS_tready;
  s00_couplers_imp_1K3E9VL s00_couplers
       (.M_AXIS_ACLK(M00_AXIS_ACLK_1),
        .M_AXIS_ARESETN(M00_AXIS_ARESETN_1),
        .M_AXIS_tdata(s00_couplers_to_axis_interconnect_3_TDATA),
        .M_AXIS_tkeep(s00_couplers_to_axis_interconnect_3_TKEEP),
        .M_AXIS_tlast(s00_couplers_to_axis_interconnect_3_TLAST),
        .M_AXIS_tready(s00_couplers_to_axis_interconnect_3_TREADY),
        .M_AXIS_tvalid(s00_couplers_to_axis_interconnect_3_TVALID),
        .S_AXIS_ACLK(S00_AXIS_ACLK_1),
        .S_AXIS_ARESETN(S00_AXIS_ARESETN_1),
        .S_AXIS_tdata(axis_interconnect_3_to_s00_couplers_TDATA),
        .S_AXIS_tkeep(axis_interconnect_3_to_s00_couplers_TKEEP),
        .S_AXIS_tlast(axis_interconnect_3_to_s00_couplers_TLAST),
        .S_AXIS_tvalid(axis_interconnect_3_to_s00_couplers_TVALID));
endmodule

module base_zynq_design_processing_system7_0_axi_periph_0
   (ACLK,
    ARESETN,
    M00_ACLK,
    M00_ARESETN,
    M00_AXI_araddr,
    M00_AXI_arready,
    M00_AXI_arvalid,
    M00_AXI_awaddr,
    M00_AXI_awready,
    M00_AXI_awvalid,
    M00_AXI_bready,
    M00_AXI_bresp,
    M00_AXI_bvalid,
    M00_AXI_rdata,
    M00_AXI_rready,
    M00_AXI_rresp,
    M00_AXI_rvalid,
    M00_AXI_wdata,
    M00_AXI_wready,
    M00_AXI_wstrb,
    M00_AXI_wvalid,
    M01_ACLK,
    M01_ARESETN,
    M01_AXI_araddr,
    M01_AXI_arready,
    M01_AXI_arvalid,
    M01_AXI_awaddr,
    M01_AXI_awready,
    M01_AXI_awvalid,
    M01_AXI_bready,
    M01_AXI_bresp,
    M01_AXI_bvalid,
    M01_AXI_rdata,
    M01_AXI_rready,
    M01_AXI_rresp,
    M01_AXI_rvalid,
    M01_AXI_wdata,
    M01_AXI_wready,
    M01_AXI_wstrb,
    M01_AXI_wvalid,
    M02_ACLK,
    M02_ARESETN,
    M02_AXI_araddr,
    M02_AXI_arready,
    M02_AXI_arvalid,
    M02_AXI_awaddr,
    M02_AXI_awready,
    M02_AXI_awvalid,
    M02_AXI_bready,
    M02_AXI_bresp,
    M02_AXI_bvalid,
    M02_AXI_rdata,
    M02_AXI_rready,
    M02_AXI_rresp,
    M02_AXI_rvalid,
    M02_AXI_wdata,
    M02_AXI_wready,
    M02_AXI_wstrb,
    M02_AXI_wvalid,
    M03_ACLK,
    M03_ARESETN,
    M03_AXI_araddr,
    M03_AXI_arprot,
    M03_AXI_arready,
    M03_AXI_arvalid,
    M03_AXI_awaddr,
    M03_AXI_awprot,
    M03_AXI_awready,
    M03_AXI_awvalid,
    M03_AXI_bready,
    M03_AXI_bresp,
    M03_AXI_bvalid,
    M03_AXI_rdata,
    M03_AXI_rready,
    M03_AXI_rresp,
    M03_AXI_rvalid,
    M03_AXI_wdata,
    M03_AXI_wready,
    M03_AXI_wstrb,
    M03_AXI_wvalid,
    M04_ACLK,
    M04_ARESETN,
    M04_AXI_araddr,
    M04_AXI_arready,
    M04_AXI_arvalid,
    M04_AXI_awaddr,
    M04_AXI_awready,
    M04_AXI_awvalid,
    M04_AXI_bready,
    M04_AXI_bresp,
    M04_AXI_bvalid,
    M04_AXI_rdata,
    M04_AXI_rready,
    M04_AXI_rresp,
    M04_AXI_rvalid,
    M04_AXI_wdata,
    M04_AXI_wready,
    M04_AXI_wstrb,
    M04_AXI_wvalid,
    M05_ACLK,
    M05_ARESETN,
    M05_AXI_araddr,
    M05_AXI_arready,
    M05_AXI_arvalid,
    M05_AXI_awaddr,
    M05_AXI_awready,
    M05_AXI_awvalid,
    M05_AXI_bready,
    M05_AXI_bresp,
    M05_AXI_bvalid,
    M05_AXI_rdata,
    M05_AXI_rready,
    M05_AXI_rresp,
    M05_AXI_rvalid,
    M05_AXI_wdata,
    M05_AXI_wready,
    M05_AXI_wstrb,
    M05_AXI_wvalid,
    M06_ACLK,
    M06_ARESETN,
    M06_AXI_araddr,
    M06_AXI_arready,
    M06_AXI_arvalid,
    M06_AXI_awaddr,
    M06_AXI_awready,
    M06_AXI_awvalid,
    M06_AXI_bready,
    M06_AXI_bresp,
    M06_AXI_bvalid,
    M06_AXI_rdata,
    M06_AXI_rready,
    M06_AXI_rresp,
    M06_AXI_rvalid,
    M06_AXI_wdata,
    M06_AXI_wready,
    M06_AXI_wstrb,
    M06_AXI_wvalid,
    M07_ACLK,
    M07_ARESETN,
    M07_AXI_araddr,
    M07_AXI_arready,
    M07_AXI_arvalid,
    M07_AXI_awaddr,
    M07_AXI_awready,
    M07_AXI_awvalid,
    M07_AXI_bready,
    M07_AXI_bresp,
    M07_AXI_bvalid,
    M07_AXI_rdata,
    M07_AXI_rready,
    M07_AXI_rresp,
    M07_AXI_rvalid,
    M07_AXI_wdata,
    M07_AXI_wready,
    M07_AXI_wstrb,
    M07_AXI_wvalid,
    M08_ACLK,
    M08_ARESETN,
    M08_AXI_araddr,
    M08_AXI_arprot,
    M08_AXI_arready,
    M08_AXI_arvalid,
    M08_AXI_awaddr,
    M08_AXI_awprot,
    M08_AXI_awready,
    M08_AXI_awvalid,
    M08_AXI_bready,
    M08_AXI_bresp,
    M08_AXI_bvalid,
    M08_AXI_rdata,
    M08_AXI_rready,
    M08_AXI_rresp,
    M08_AXI_rvalid,
    M08_AXI_wdata,
    M08_AXI_wready,
    M08_AXI_wstrb,
    M08_AXI_wvalid,
    M09_ACLK,
    M09_ARESETN,
    M09_AXI_araddr,
    M09_AXI_arready,
    M09_AXI_arvalid,
    M09_AXI_awaddr,
    M09_AXI_awready,
    M09_AXI_awvalid,
    M09_AXI_bready,
    M09_AXI_bresp,
    M09_AXI_bvalid,
    M09_AXI_rdata,
    M09_AXI_rready,
    M09_AXI_rresp,
    M09_AXI_rvalid,
    M09_AXI_wdata,
    M09_AXI_wready,
    M09_AXI_wstrb,
    M09_AXI_wvalid,
    M10_ACLK,
    M10_ARESETN,
    M10_AXI_araddr,
    M10_AXI_arready,
    M10_AXI_arvalid,
    M10_AXI_awaddr,
    M10_AXI_awready,
    M10_AXI_awvalid,
    M10_AXI_bready,
    M10_AXI_bresp,
    M10_AXI_bvalid,
    M10_AXI_rdata,
    M10_AXI_rready,
    M10_AXI_rresp,
    M10_AXI_rvalid,
    M10_AXI_wdata,
    M10_AXI_wready,
    M10_AXI_wvalid,
    M11_ACLK,
    M11_ARESETN,
    M11_AXI_araddr,
    M11_AXI_arready,
    M11_AXI_arvalid,
    M11_AXI_awaddr,
    M11_AXI_awready,
    M11_AXI_awvalid,
    M11_AXI_bready,
    M11_AXI_bresp,
    M11_AXI_bvalid,
    M11_AXI_rdata,
    M11_AXI_rready,
    M11_AXI_rresp,
    M11_AXI_rvalid,
    M11_AXI_wdata,
    M11_AXI_wready,
    M11_AXI_wstrb,
    M11_AXI_wvalid,
    M12_ACLK,
    M12_ARESETN,
    M12_AXI_araddr,
    M12_AXI_arready,
    M12_AXI_arvalid,
    M12_AXI_awaddr,
    M12_AXI_awready,
    M12_AXI_awvalid,
    M12_AXI_bready,
    M12_AXI_bresp,
    M12_AXI_bvalid,
    M12_AXI_rdata,
    M12_AXI_rready,
    M12_AXI_rresp,
    M12_AXI_rvalid,
    M12_AXI_wdata,
    M12_AXI_wready,
    M12_AXI_wstrb,
    M12_AXI_wvalid,
    S00_ACLK,
    S00_ARESETN,
    S00_AXI_araddr,
    S00_AXI_arburst,
    S00_AXI_arcache,
    S00_AXI_arid,
    S00_AXI_arlen,
    S00_AXI_arlock,
    S00_AXI_arprot,
    S00_AXI_arqos,
    S00_AXI_arready,
    S00_AXI_arsize,
    S00_AXI_arvalid,
    S00_AXI_awaddr,
    S00_AXI_awburst,
    S00_AXI_awcache,
    S00_AXI_awid,
    S00_AXI_awlen,
    S00_AXI_awlock,
    S00_AXI_awprot,
    S00_AXI_awqos,
    S00_AXI_awready,
    S00_AXI_awsize,
    S00_AXI_awvalid,
    S00_AXI_bid,
    S00_AXI_bready,
    S00_AXI_bresp,
    S00_AXI_bvalid,
    S00_AXI_rdata,
    S00_AXI_rid,
    S00_AXI_rlast,
    S00_AXI_rready,
    S00_AXI_rresp,
    S00_AXI_rvalid,
    S00_AXI_wdata,
    S00_AXI_wid,
    S00_AXI_wlast,
    S00_AXI_wready,
    S00_AXI_wstrb,
    S00_AXI_wvalid);
  input ACLK;
  input [0:0]ARESETN;
  input M00_ACLK;
  input [0:0]M00_ARESETN;
  output [31:0]M00_AXI_araddr;
  input [0:0]M00_AXI_arready;
  output [0:0]M00_AXI_arvalid;
  output [31:0]M00_AXI_awaddr;
  input [0:0]M00_AXI_awready;
  output [0:0]M00_AXI_awvalid;
  output [0:0]M00_AXI_bready;
  input [1:0]M00_AXI_bresp;
  input [0:0]M00_AXI_bvalid;
  input [31:0]M00_AXI_rdata;
  output [0:0]M00_AXI_rready;
  input [1:0]M00_AXI_rresp;
  input [0:0]M00_AXI_rvalid;
  output [31:0]M00_AXI_wdata;
  input [0:0]M00_AXI_wready;
  output [3:0]M00_AXI_wstrb;
  output [0:0]M00_AXI_wvalid;
  input M01_ACLK;
  input [0:0]M01_ARESETN;
  output [31:0]M01_AXI_araddr;
  input M01_AXI_arready;
  output M01_AXI_arvalid;
  output [31:0]M01_AXI_awaddr;
  input M01_AXI_awready;
  output M01_AXI_awvalid;
  output M01_AXI_bready;
  input [1:0]M01_AXI_bresp;
  input M01_AXI_bvalid;
  input [31:0]M01_AXI_rdata;
  output M01_AXI_rready;
  input [1:0]M01_AXI_rresp;
  input M01_AXI_rvalid;
  output [31:0]M01_AXI_wdata;
  input M01_AXI_wready;
  output [3:0]M01_AXI_wstrb;
  output M01_AXI_wvalid;
  input M02_ACLK;
  input [0:0]M02_ARESETN;
  output [31:0]M02_AXI_araddr;
  input M02_AXI_arready;
  output M02_AXI_arvalid;
  output [31:0]M02_AXI_awaddr;
  input M02_AXI_awready;
  output M02_AXI_awvalid;
  output M02_AXI_bready;
  input [1:0]M02_AXI_bresp;
  input M02_AXI_bvalid;
  input [31:0]M02_AXI_rdata;
  output M02_AXI_rready;
  input [1:0]M02_AXI_rresp;
  input M02_AXI_rvalid;
  output [31:0]M02_AXI_wdata;
  input M02_AXI_wready;
  output [3:0]M02_AXI_wstrb;
  output M02_AXI_wvalid;
  input M03_ACLK;
  input [0:0]M03_ARESETN;
  output [31:0]M03_AXI_araddr;
  output [2:0]M03_AXI_arprot;
  input M03_AXI_arready;
  output M03_AXI_arvalid;
  output [31:0]M03_AXI_awaddr;
  output [2:0]M03_AXI_awprot;
  input M03_AXI_awready;
  output M03_AXI_awvalid;
  output M03_AXI_bready;
  input [1:0]M03_AXI_bresp;
  input M03_AXI_bvalid;
  input [31:0]M03_AXI_rdata;
  output M03_AXI_rready;
  input [1:0]M03_AXI_rresp;
  input M03_AXI_rvalid;
  output [31:0]M03_AXI_wdata;
  input M03_AXI_wready;
  output [3:0]M03_AXI_wstrb;
  output M03_AXI_wvalid;
  input M04_ACLK;
  input [0:0]M04_ARESETN;
  output [31:0]M04_AXI_araddr;
  input M04_AXI_arready;
  output M04_AXI_arvalid;
  output [31:0]M04_AXI_awaddr;
  input M04_AXI_awready;
  output M04_AXI_awvalid;
  output M04_AXI_bready;
  input [1:0]M04_AXI_bresp;
  input M04_AXI_bvalid;
  input [31:0]M04_AXI_rdata;
  output M04_AXI_rready;
  input [1:0]M04_AXI_rresp;
  input M04_AXI_rvalid;
  output [31:0]M04_AXI_wdata;
  input M04_AXI_wready;
  output [3:0]M04_AXI_wstrb;
  output M04_AXI_wvalid;
  input M05_ACLK;
  input [0:0]M05_ARESETN;
  output [31:0]M05_AXI_araddr;
  input M05_AXI_arready;
  output M05_AXI_arvalid;
  output [31:0]M05_AXI_awaddr;
  input M05_AXI_awready;
  output M05_AXI_awvalid;
  output M05_AXI_bready;
  input [1:0]M05_AXI_bresp;
  input M05_AXI_bvalid;
  input [31:0]M05_AXI_rdata;
  output M05_AXI_rready;
  input [1:0]M05_AXI_rresp;
  input M05_AXI_rvalid;
  output [31:0]M05_AXI_wdata;
  input M05_AXI_wready;
  output [3:0]M05_AXI_wstrb;
  output M05_AXI_wvalid;
  input M06_ACLK;
  input [0:0]M06_ARESETN;
  output [31:0]M06_AXI_araddr;
  input M06_AXI_arready;
  output M06_AXI_arvalid;
  output [31:0]M06_AXI_awaddr;
  input M06_AXI_awready;
  output M06_AXI_awvalid;
  output M06_AXI_bready;
  input [1:0]M06_AXI_bresp;
  input M06_AXI_bvalid;
  input [31:0]M06_AXI_rdata;
  output M06_AXI_rready;
  input [1:0]M06_AXI_rresp;
  input M06_AXI_rvalid;
  output [31:0]M06_AXI_wdata;
  input M06_AXI_wready;
  output [3:0]M06_AXI_wstrb;
  output M06_AXI_wvalid;
  input M07_ACLK;
  input [0:0]M07_ARESETN;
  output [31:0]M07_AXI_araddr;
  input M07_AXI_arready;
  output M07_AXI_arvalid;
  output [31:0]M07_AXI_awaddr;
  input M07_AXI_awready;
  output M07_AXI_awvalid;
  output M07_AXI_bready;
  input [1:0]M07_AXI_bresp;
  input M07_AXI_bvalid;
  input [31:0]M07_AXI_rdata;
  output M07_AXI_rready;
  input [1:0]M07_AXI_rresp;
  input M07_AXI_rvalid;
  output [31:0]M07_AXI_wdata;
  input M07_AXI_wready;
  output [3:0]M07_AXI_wstrb;
  output M07_AXI_wvalid;
  input M08_ACLK;
  input [0:0]M08_ARESETN;
  output [31:0]M08_AXI_araddr;
  output [2:0]M08_AXI_arprot;
  input M08_AXI_arready;
  output M08_AXI_arvalid;
  output [31:0]M08_AXI_awaddr;
  output [2:0]M08_AXI_awprot;
  input M08_AXI_awready;
  output M08_AXI_awvalid;
  output M08_AXI_bready;
  input [1:0]M08_AXI_bresp;
  input M08_AXI_bvalid;
  input [31:0]M08_AXI_rdata;
  output M08_AXI_rready;
  input [1:0]M08_AXI_rresp;
  input M08_AXI_rvalid;
  output [31:0]M08_AXI_wdata;
  input M08_AXI_wready;
  output [3:0]M08_AXI_wstrb;
  output M08_AXI_wvalid;
  input M09_ACLK;
  input [0:0]M09_ARESETN;
  output [31:0]M09_AXI_araddr;
  input M09_AXI_arready;
  output M09_AXI_arvalid;
  output [31:0]M09_AXI_awaddr;
  input M09_AXI_awready;
  output M09_AXI_awvalid;
  output M09_AXI_bready;
  input [1:0]M09_AXI_bresp;
  input M09_AXI_bvalid;
  input [31:0]M09_AXI_rdata;
  output M09_AXI_rready;
  input [1:0]M09_AXI_rresp;
  input M09_AXI_rvalid;
  output [31:0]M09_AXI_wdata;
  input M09_AXI_wready;
  output [3:0]M09_AXI_wstrb;
  output M09_AXI_wvalid;
  input M10_ACLK;
  input [0:0]M10_ARESETN;
  output [31:0]M10_AXI_araddr;
  input M10_AXI_arready;
  output M10_AXI_arvalid;
  output [31:0]M10_AXI_awaddr;
  input M10_AXI_awready;
  output M10_AXI_awvalid;
  output M10_AXI_bready;
  input [1:0]M10_AXI_bresp;
  input M10_AXI_bvalid;
  input [31:0]M10_AXI_rdata;
  output M10_AXI_rready;
  input [1:0]M10_AXI_rresp;
  input M10_AXI_rvalid;
  output [31:0]M10_AXI_wdata;
  input M10_AXI_wready;
  output M10_AXI_wvalid;
  input M11_ACLK;
  input [0:0]M11_ARESETN;
  output [31:0]M11_AXI_araddr;
  input M11_AXI_arready;
  output M11_AXI_arvalid;
  output [31:0]M11_AXI_awaddr;
  input M11_AXI_awready;
  output M11_AXI_awvalid;
  output M11_AXI_bready;
  input [1:0]M11_AXI_bresp;
  input M11_AXI_bvalid;
  input [31:0]M11_AXI_rdata;
  output M11_AXI_rready;
  input [1:0]M11_AXI_rresp;
  input M11_AXI_rvalid;
  output [31:0]M11_AXI_wdata;
  input M11_AXI_wready;
  output [3:0]M11_AXI_wstrb;
  output M11_AXI_wvalid;
  input M12_ACLK;
  input [0:0]M12_ARESETN;
  output [31:0]M12_AXI_araddr;
  input M12_AXI_arready;
  output M12_AXI_arvalid;
  output [31:0]M12_AXI_awaddr;
  input M12_AXI_awready;
  output M12_AXI_awvalid;
  output M12_AXI_bready;
  input [1:0]M12_AXI_bresp;
  input M12_AXI_bvalid;
  input [31:0]M12_AXI_rdata;
  output M12_AXI_rready;
  input [1:0]M12_AXI_rresp;
  input M12_AXI_rvalid;
  output [31:0]M12_AXI_wdata;
  input M12_AXI_wready;
  output [3:0]M12_AXI_wstrb;
  output M12_AXI_wvalid;
  input S00_ACLK;
  input [0:0]S00_ARESETN;
  input [31:0]S00_AXI_araddr;
  input [1:0]S00_AXI_arburst;
  input [3:0]S00_AXI_arcache;
  input [11:0]S00_AXI_arid;
  input [3:0]S00_AXI_arlen;
  input [1:0]S00_AXI_arlock;
  input [2:0]S00_AXI_arprot;
  input [3:0]S00_AXI_arqos;
  output S00_AXI_arready;
  input [2:0]S00_AXI_arsize;
  input S00_AXI_arvalid;
  input [31:0]S00_AXI_awaddr;
  input [1:0]S00_AXI_awburst;
  input [3:0]S00_AXI_awcache;
  input [11:0]S00_AXI_awid;
  input [3:0]S00_AXI_awlen;
  input [1:0]S00_AXI_awlock;
  input [2:0]S00_AXI_awprot;
  input [3:0]S00_AXI_awqos;
  output S00_AXI_awready;
  input [2:0]S00_AXI_awsize;
  input S00_AXI_awvalid;
  output [11:0]S00_AXI_bid;
  input S00_AXI_bready;
  output [1:0]S00_AXI_bresp;
  output S00_AXI_bvalid;
  output [31:0]S00_AXI_rdata;
  output [11:0]S00_AXI_rid;
  output S00_AXI_rlast;
  input S00_AXI_rready;
  output [1:0]S00_AXI_rresp;
  output S00_AXI_rvalid;
  input [31:0]S00_AXI_wdata;
  input [11:0]S00_AXI_wid;
  input S00_AXI_wlast;
  output S00_AXI_wready;
  input [3:0]S00_AXI_wstrb;
  input S00_AXI_wvalid;

  wire M00_ACLK_1;
  wire [0:0]M00_ARESETN_1;
  wire M01_ACLK_1;
  wire [0:0]M01_ARESETN_1;
  wire M02_ACLK_1;
  wire [0:0]M02_ARESETN_1;
  wire M03_ACLK_1;
  wire [0:0]M03_ARESETN_1;
  wire M04_ACLK_1;
  wire [0:0]M04_ARESETN_1;
  wire M05_ACLK_1;
  wire [0:0]M05_ARESETN_1;
  wire M06_ACLK_1;
  wire [0:0]M06_ARESETN_1;
  wire M07_ACLK_1;
  wire [0:0]M07_ARESETN_1;
  wire M08_ACLK_1;
  wire [0:0]M08_ARESETN_1;
  wire M09_ACLK_1;
  wire [0:0]M09_ARESETN_1;
  wire M10_ACLK_1;
  wire [0:0]M10_ARESETN_1;
  wire M11_ACLK_1;
  wire [0:0]M11_ARESETN_1;
  wire M12_ACLK_1;
  wire [0:0]M12_ARESETN_1;
  wire S00_ACLK_1;
  wire [0:0]S00_ARESETN_1;
  wire [31:0]m00_couplers_to_processing_system7_0_axi_periph_ARADDR;
  wire [0:0]m00_couplers_to_processing_system7_0_axi_periph_ARREADY;
  wire [0:0]m00_couplers_to_processing_system7_0_axi_periph_ARVALID;
  wire [31:0]m00_couplers_to_processing_system7_0_axi_periph_AWADDR;
  wire [0:0]m00_couplers_to_processing_system7_0_axi_periph_AWREADY;
  wire [0:0]m00_couplers_to_processing_system7_0_axi_periph_AWVALID;
  wire [0:0]m00_couplers_to_processing_system7_0_axi_periph_BREADY;
  wire [1:0]m00_couplers_to_processing_system7_0_axi_periph_BRESP;
  wire [0:0]m00_couplers_to_processing_system7_0_axi_periph_BVALID;
  wire [31:0]m00_couplers_to_processing_system7_0_axi_periph_RDATA;
  wire [0:0]m00_couplers_to_processing_system7_0_axi_periph_RREADY;
  wire [1:0]m00_couplers_to_processing_system7_0_axi_periph_RRESP;
  wire [0:0]m00_couplers_to_processing_system7_0_axi_periph_RVALID;
  wire [31:0]m00_couplers_to_processing_system7_0_axi_periph_WDATA;
  wire [0:0]m00_couplers_to_processing_system7_0_axi_periph_WREADY;
  wire [3:0]m00_couplers_to_processing_system7_0_axi_periph_WSTRB;
  wire [0:0]m00_couplers_to_processing_system7_0_axi_periph_WVALID;
  wire [31:0]m01_couplers_to_processing_system7_0_axi_periph_ARADDR;
  wire m01_couplers_to_processing_system7_0_axi_periph_ARREADY;
  wire m01_couplers_to_processing_system7_0_axi_periph_ARVALID;
  wire [31:0]m01_couplers_to_processing_system7_0_axi_periph_AWADDR;
  wire m01_couplers_to_processing_system7_0_axi_periph_AWREADY;
  wire m01_couplers_to_processing_system7_0_axi_periph_AWVALID;
  wire m01_couplers_to_processing_system7_0_axi_periph_BREADY;
  wire [1:0]m01_couplers_to_processing_system7_0_axi_periph_BRESP;
  wire m01_couplers_to_processing_system7_0_axi_periph_BVALID;
  wire [31:0]m01_couplers_to_processing_system7_0_axi_periph_RDATA;
  wire m01_couplers_to_processing_system7_0_axi_periph_RREADY;
  wire [1:0]m01_couplers_to_processing_system7_0_axi_periph_RRESP;
  wire m01_couplers_to_processing_system7_0_axi_periph_RVALID;
  wire [31:0]m01_couplers_to_processing_system7_0_axi_periph_WDATA;
  wire m01_couplers_to_processing_system7_0_axi_periph_WREADY;
  wire [3:0]m01_couplers_to_processing_system7_0_axi_periph_WSTRB;
  wire m01_couplers_to_processing_system7_0_axi_periph_WVALID;
  wire [31:0]m02_couplers_to_processing_system7_0_axi_periph_ARADDR;
  wire m02_couplers_to_processing_system7_0_axi_periph_ARREADY;
  wire m02_couplers_to_processing_system7_0_axi_periph_ARVALID;
  wire [31:0]m02_couplers_to_processing_system7_0_axi_periph_AWADDR;
  wire m02_couplers_to_processing_system7_0_axi_periph_AWREADY;
  wire m02_couplers_to_processing_system7_0_axi_periph_AWVALID;
  wire m02_couplers_to_processing_system7_0_axi_periph_BREADY;
  wire [1:0]m02_couplers_to_processing_system7_0_axi_periph_BRESP;
  wire m02_couplers_to_processing_system7_0_axi_periph_BVALID;
  wire [31:0]m02_couplers_to_processing_system7_0_axi_periph_RDATA;
  wire m02_couplers_to_processing_system7_0_axi_periph_RREADY;
  wire [1:0]m02_couplers_to_processing_system7_0_axi_periph_RRESP;
  wire m02_couplers_to_processing_system7_0_axi_periph_RVALID;
  wire [31:0]m02_couplers_to_processing_system7_0_axi_periph_WDATA;
  wire m02_couplers_to_processing_system7_0_axi_periph_WREADY;
  wire [3:0]m02_couplers_to_processing_system7_0_axi_periph_WSTRB;
  wire m02_couplers_to_processing_system7_0_axi_periph_WVALID;
  wire [31:0]m03_couplers_to_processing_system7_0_axi_periph_ARADDR;
  wire [2:0]m03_couplers_to_processing_system7_0_axi_periph_ARPROT;
  wire m03_couplers_to_processing_system7_0_axi_periph_ARREADY;
  wire m03_couplers_to_processing_system7_0_axi_periph_ARVALID;
  wire [31:0]m03_couplers_to_processing_system7_0_axi_periph_AWADDR;
  wire [2:0]m03_couplers_to_processing_system7_0_axi_periph_AWPROT;
  wire m03_couplers_to_processing_system7_0_axi_periph_AWREADY;
  wire m03_couplers_to_processing_system7_0_axi_periph_AWVALID;
  wire m03_couplers_to_processing_system7_0_axi_periph_BREADY;
  wire [1:0]m03_couplers_to_processing_system7_0_axi_periph_BRESP;
  wire m03_couplers_to_processing_system7_0_axi_periph_BVALID;
  wire [31:0]m03_couplers_to_processing_system7_0_axi_periph_RDATA;
  wire m03_couplers_to_processing_system7_0_axi_periph_RREADY;
  wire [1:0]m03_couplers_to_processing_system7_0_axi_periph_RRESP;
  wire m03_couplers_to_processing_system7_0_axi_periph_RVALID;
  wire [31:0]m03_couplers_to_processing_system7_0_axi_periph_WDATA;
  wire m03_couplers_to_processing_system7_0_axi_periph_WREADY;
  wire [3:0]m03_couplers_to_processing_system7_0_axi_periph_WSTRB;
  wire m03_couplers_to_processing_system7_0_axi_periph_WVALID;
  wire [31:0]m04_couplers_to_processing_system7_0_axi_periph_ARADDR;
  wire m04_couplers_to_processing_system7_0_axi_periph_ARREADY;
  wire m04_couplers_to_processing_system7_0_axi_periph_ARVALID;
  wire [31:0]m04_couplers_to_processing_system7_0_axi_periph_AWADDR;
  wire m04_couplers_to_processing_system7_0_axi_periph_AWREADY;
  wire m04_couplers_to_processing_system7_0_axi_periph_AWVALID;
  wire m04_couplers_to_processing_system7_0_axi_periph_BREADY;
  wire [1:0]m04_couplers_to_processing_system7_0_axi_periph_BRESP;
  wire m04_couplers_to_processing_system7_0_axi_periph_BVALID;
  wire [31:0]m04_couplers_to_processing_system7_0_axi_periph_RDATA;
  wire m04_couplers_to_processing_system7_0_axi_periph_RREADY;
  wire [1:0]m04_couplers_to_processing_system7_0_axi_periph_RRESP;
  wire m04_couplers_to_processing_system7_0_axi_periph_RVALID;
  wire [31:0]m04_couplers_to_processing_system7_0_axi_periph_WDATA;
  wire m04_couplers_to_processing_system7_0_axi_periph_WREADY;
  wire [3:0]m04_couplers_to_processing_system7_0_axi_periph_WSTRB;
  wire m04_couplers_to_processing_system7_0_axi_periph_WVALID;
  wire [31:0]m05_couplers_to_processing_system7_0_axi_periph_ARADDR;
  wire m05_couplers_to_processing_system7_0_axi_periph_ARREADY;
  wire m05_couplers_to_processing_system7_0_axi_periph_ARVALID;
  wire [31:0]m05_couplers_to_processing_system7_0_axi_periph_AWADDR;
  wire m05_couplers_to_processing_system7_0_axi_periph_AWREADY;
  wire m05_couplers_to_processing_system7_0_axi_periph_AWVALID;
  wire m05_couplers_to_processing_system7_0_axi_periph_BREADY;
  wire [1:0]m05_couplers_to_processing_system7_0_axi_periph_BRESP;
  wire m05_couplers_to_processing_system7_0_axi_periph_BVALID;
  wire [31:0]m05_couplers_to_processing_system7_0_axi_periph_RDATA;
  wire m05_couplers_to_processing_system7_0_axi_periph_RREADY;
  wire [1:0]m05_couplers_to_processing_system7_0_axi_periph_RRESP;
  wire m05_couplers_to_processing_system7_0_axi_periph_RVALID;
  wire [31:0]m05_couplers_to_processing_system7_0_axi_periph_WDATA;
  wire m05_couplers_to_processing_system7_0_axi_periph_WREADY;
  wire [3:0]m05_couplers_to_processing_system7_0_axi_periph_WSTRB;
  wire m05_couplers_to_processing_system7_0_axi_periph_WVALID;
  wire [31:0]m06_couplers_to_processing_system7_0_axi_periph_ARADDR;
  wire m06_couplers_to_processing_system7_0_axi_periph_ARREADY;
  wire m06_couplers_to_processing_system7_0_axi_periph_ARVALID;
  wire [31:0]m06_couplers_to_processing_system7_0_axi_periph_AWADDR;
  wire m06_couplers_to_processing_system7_0_axi_periph_AWREADY;
  wire m06_couplers_to_processing_system7_0_axi_periph_AWVALID;
  wire m06_couplers_to_processing_system7_0_axi_periph_BREADY;
  wire [1:0]m06_couplers_to_processing_system7_0_axi_periph_BRESP;
  wire m06_couplers_to_processing_system7_0_axi_periph_BVALID;
  wire [31:0]m06_couplers_to_processing_system7_0_axi_periph_RDATA;
  wire m06_couplers_to_processing_system7_0_axi_periph_RREADY;
  wire [1:0]m06_couplers_to_processing_system7_0_axi_periph_RRESP;
  wire m06_couplers_to_processing_system7_0_axi_periph_RVALID;
  wire [31:0]m06_couplers_to_processing_system7_0_axi_periph_WDATA;
  wire m06_couplers_to_processing_system7_0_axi_periph_WREADY;
  wire [3:0]m06_couplers_to_processing_system7_0_axi_periph_WSTRB;
  wire m06_couplers_to_processing_system7_0_axi_periph_WVALID;
  wire [31:0]m07_couplers_to_processing_system7_0_axi_periph_ARADDR;
  wire m07_couplers_to_processing_system7_0_axi_periph_ARREADY;
  wire m07_couplers_to_processing_system7_0_axi_periph_ARVALID;
  wire [31:0]m07_couplers_to_processing_system7_0_axi_periph_AWADDR;
  wire m07_couplers_to_processing_system7_0_axi_periph_AWREADY;
  wire m07_couplers_to_processing_system7_0_axi_periph_AWVALID;
  wire m07_couplers_to_processing_system7_0_axi_periph_BREADY;
  wire [1:0]m07_couplers_to_processing_system7_0_axi_periph_BRESP;
  wire m07_couplers_to_processing_system7_0_axi_periph_BVALID;
  wire [31:0]m07_couplers_to_processing_system7_0_axi_periph_RDATA;
  wire m07_couplers_to_processing_system7_0_axi_periph_RREADY;
  wire [1:0]m07_couplers_to_processing_system7_0_axi_periph_RRESP;
  wire m07_couplers_to_processing_system7_0_axi_periph_RVALID;
  wire [31:0]m07_couplers_to_processing_system7_0_axi_periph_WDATA;
  wire m07_couplers_to_processing_system7_0_axi_periph_WREADY;
  wire [3:0]m07_couplers_to_processing_system7_0_axi_periph_WSTRB;
  wire m07_couplers_to_processing_system7_0_axi_periph_WVALID;
  wire [31:0]m08_couplers_to_processing_system7_0_axi_periph_ARADDR;
  wire [2:0]m08_couplers_to_processing_system7_0_axi_periph_ARPROT;
  wire m08_couplers_to_processing_system7_0_axi_periph_ARREADY;
  wire m08_couplers_to_processing_system7_0_axi_periph_ARVALID;
  wire [31:0]m08_couplers_to_processing_system7_0_axi_periph_AWADDR;
  wire [2:0]m08_couplers_to_processing_system7_0_axi_periph_AWPROT;
  wire m08_couplers_to_processing_system7_0_axi_periph_AWREADY;
  wire m08_couplers_to_processing_system7_0_axi_periph_AWVALID;
  wire m08_couplers_to_processing_system7_0_axi_periph_BREADY;
  wire [1:0]m08_couplers_to_processing_system7_0_axi_periph_BRESP;
  wire m08_couplers_to_processing_system7_0_axi_periph_BVALID;
  wire [31:0]m08_couplers_to_processing_system7_0_axi_periph_RDATA;
  wire m08_couplers_to_processing_system7_0_axi_periph_RREADY;
  wire [1:0]m08_couplers_to_processing_system7_0_axi_periph_RRESP;
  wire m08_couplers_to_processing_system7_0_axi_periph_RVALID;
  wire [31:0]m08_couplers_to_processing_system7_0_axi_periph_WDATA;
  wire m08_couplers_to_processing_system7_0_axi_periph_WREADY;
  wire [3:0]m08_couplers_to_processing_system7_0_axi_periph_WSTRB;
  wire m08_couplers_to_processing_system7_0_axi_periph_WVALID;
  wire [31:0]m09_couplers_to_processing_system7_0_axi_periph_ARADDR;
  wire m09_couplers_to_processing_system7_0_axi_periph_ARREADY;
  wire m09_couplers_to_processing_system7_0_axi_periph_ARVALID;
  wire [31:0]m09_couplers_to_processing_system7_0_axi_periph_AWADDR;
  wire m09_couplers_to_processing_system7_0_axi_periph_AWREADY;
  wire m09_couplers_to_processing_system7_0_axi_periph_AWVALID;
  wire m09_couplers_to_processing_system7_0_axi_periph_BREADY;
  wire [1:0]m09_couplers_to_processing_system7_0_axi_periph_BRESP;
  wire m09_couplers_to_processing_system7_0_axi_periph_BVALID;
  wire [31:0]m09_couplers_to_processing_system7_0_axi_periph_RDATA;
  wire m09_couplers_to_processing_system7_0_axi_periph_RREADY;
  wire [1:0]m09_couplers_to_processing_system7_0_axi_periph_RRESP;
  wire m09_couplers_to_processing_system7_0_axi_periph_RVALID;
  wire [31:0]m09_couplers_to_processing_system7_0_axi_periph_WDATA;
  wire m09_couplers_to_processing_system7_0_axi_periph_WREADY;
  wire [3:0]m09_couplers_to_processing_system7_0_axi_periph_WSTRB;
  wire m09_couplers_to_processing_system7_0_axi_periph_WVALID;
  wire [31:0]m10_couplers_to_processing_system7_0_axi_periph_ARADDR;
  wire m10_couplers_to_processing_system7_0_axi_periph_ARREADY;
  wire m10_couplers_to_processing_system7_0_axi_periph_ARVALID;
  wire [31:0]m10_couplers_to_processing_system7_0_axi_periph_AWADDR;
  wire m10_couplers_to_processing_system7_0_axi_periph_AWREADY;
  wire m10_couplers_to_processing_system7_0_axi_periph_AWVALID;
  wire m10_couplers_to_processing_system7_0_axi_periph_BREADY;
  wire [1:0]m10_couplers_to_processing_system7_0_axi_periph_BRESP;
  wire m10_couplers_to_processing_system7_0_axi_periph_BVALID;
  wire [31:0]m10_couplers_to_processing_system7_0_axi_periph_RDATA;
  wire m10_couplers_to_processing_system7_0_axi_periph_RREADY;
  wire [1:0]m10_couplers_to_processing_system7_0_axi_periph_RRESP;
  wire m10_couplers_to_processing_system7_0_axi_periph_RVALID;
  wire [31:0]m10_couplers_to_processing_system7_0_axi_periph_WDATA;
  wire m10_couplers_to_processing_system7_0_axi_periph_WREADY;
  wire m10_couplers_to_processing_system7_0_axi_periph_WVALID;
  wire [31:0]m11_couplers_to_processing_system7_0_axi_periph_ARADDR;
  wire m11_couplers_to_processing_system7_0_axi_periph_ARREADY;
  wire m11_couplers_to_processing_system7_0_axi_periph_ARVALID;
  wire [31:0]m11_couplers_to_processing_system7_0_axi_periph_AWADDR;
  wire m11_couplers_to_processing_system7_0_axi_periph_AWREADY;
  wire m11_couplers_to_processing_system7_0_axi_periph_AWVALID;
  wire m11_couplers_to_processing_system7_0_axi_periph_BREADY;
  wire [1:0]m11_couplers_to_processing_system7_0_axi_periph_BRESP;
  wire m11_couplers_to_processing_system7_0_axi_periph_BVALID;
  wire [31:0]m11_couplers_to_processing_system7_0_axi_periph_RDATA;
  wire m11_couplers_to_processing_system7_0_axi_periph_RREADY;
  wire [1:0]m11_couplers_to_processing_system7_0_axi_periph_RRESP;
  wire m11_couplers_to_processing_system7_0_axi_periph_RVALID;
  wire [31:0]m11_couplers_to_processing_system7_0_axi_periph_WDATA;
  wire m11_couplers_to_processing_system7_0_axi_periph_WREADY;
  wire [3:0]m11_couplers_to_processing_system7_0_axi_periph_WSTRB;
  wire m11_couplers_to_processing_system7_0_axi_periph_WVALID;
  wire [31:0]m12_couplers_to_processing_system7_0_axi_periph_ARADDR;
  wire m12_couplers_to_processing_system7_0_axi_periph_ARREADY;
  wire m12_couplers_to_processing_system7_0_axi_periph_ARVALID;
  wire [31:0]m12_couplers_to_processing_system7_0_axi_periph_AWADDR;
  wire m12_couplers_to_processing_system7_0_axi_periph_AWREADY;
  wire m12_couplers_to_processing_system7_0_axi_periph_AWVALID;
  wire m12_couplers_to_processing_system7_0_axi_periph_BREADY;
  wire [1:0]m12_couplers_to_processing_system7_0_axi_periph_BRESP;
  wire m12_couplers_to_processing_system7_0_axi_periph_BVALID;
  wire [31:0]m12_couplers_to_processing_system7_0_axi_periph_RDATA;
  wire m12_couplers_to_processing_system7_0_axi_periph_RREADY;
  wire [1:0]m12_couplers_to_processing_system7_0_axi_periph_RRESP;
  wire m12_couplers_to_processing_system7_0_axi_periph_RVALID;
  wire [31:0]m12_couplers_to_processing_system7_0_axi_periph_WDATA;
  wire m12_couplers_to_processing_system7_0_axi_periph_WREADY;
  wire [3:0]m12_couplers_to_processing_system7_0_axi_periph_WSTRB;
  wire m12_couplers_to_processing_system7_0_axi_periph_WVALID;
  wire processing_system7_0_axi_periph_ACLK_net;
  wire [0:0]processing_system7_0_axi_periph_ARESETN_net;
  wire [31:0]processing_system7_0_axi_periph_to_s00_couplers_ARADDR;
  wire [1:0]processing_system7_0_axi_periph_to_s00_couplers_ARBURST;
  wire [3:0]processing_system7_0_axi_periph_to_s00_couplers_ARCACHE;
  wire [11:0]processing_system7_0_axi_periph_to_s00_couplers_ARID;
  wire [3:0]processing_system7_0_axi_periph_to_s00_couplers_ARLEN;
  wire [1:0]processing_system7_0_axi_periph_to_s00_couplers_ARLOCK;
  wire [2:0]processing_system7_0_axi_periph_to_s00_couplers_ARPROT;
  wire [3:0]processing_system7_0_axi_periph_to_s00_couplers_ARQOS;
  wire processing_system7_0_axi_periph_to_s00_couplers_ARREADY;
  wire [2:0]processing_system7_0_axi_periph_to_s00_couplers_ARSIZE;
  wire processing_system7_0_axi_periph_to_s00_couplers_ARVALID;
  wire [31:0]processing_system7_0_axi_periph_to_s00_couplers_AWADDR;
  wire [1:0]processing_system7_0_axi_periph_to_s00_couplers_AWBURST;
  wire [3:0]processing_system7_0_axi_periph_to_s00_couplers_AWCACHE;
  wire [11:0]processing_system7_0_axi_periph_to_s00_couplers_AWID;
  wire [3:0]processing_system7_0_axi_periph_to_s00_couplers_AWLEN;
  wire [1:0]processing_system7_0_axi_periph_to_s00_couplers_AWLOCK;
  wire [2:0]processing_system7_0_axi_periph_to_s00_couplers_AWPROT;
  wire [3:0]processing_system7_0_axi_periph_to_s00_couplers_AWQOS;
  wire processing_system7_0_axi_periph_to_s00_couplers_AWREADY;
  wire [2:0]processing_system7_0_axi_periph_to_s00_couplers_AWSIZE;
  wire processing_system7_0_axi_periph_to_s00_couplers_AWVALID;
  wire [11:0]processing_system7_0_axi_periph_to_s00_couplers_BID;
  wire processing_system7_0_axi_periph_to_s00_couplers_BREADY;
  wire [1:0]processing_system7_0_axi_periph_to_s00_couplers_BRESP;
  wire processing_system7_0_axi_periph_to_s00_couplers_BVALID;
  wire [31:0]processing_system7_0_axi_periph_to_s00_couplers_RDATA;
  wire [11:0]processing_system7_0_axi_periph_to_s00_couplers_RID;
  wire processing_system7_0_axi_periph_to_s00_couplers_RLAST;
  wire processing_system7_0_axi_periph_to_s00_couplers_RREADY;
  wire [1:0]processing_system7_0_axi_periph_to_s00_couplers_RRESP;
  wire processing_system7_0_axi_periph_to_s00_couplers_RVALID;
  wire [31:0]processing_system7_0_axi_periph_to_s00_couplers_WDATA;
  wire [11:0]processing_system7_0_axi_periph_to_s00_couplers_WID;
  wire processing_system7_0_axi_periph_to_s00_couplers_WLAST;
  wire processing_system7_0_axi_periph_to_s00_couplers_WREADY;
  wire [3:0]processing_system7_0_axi_periph_to_s00_couplers_WSTRB;
  wire processing_system7_0_axi_periph_to_s00_couplers_WVALID;
  wire [31:0]s00_couplers_to_xbar_ARADDR;
  wire [2:0]s00_couplers_to_xbar_ARPROT;
  wire [0:0]s00_couplers_to_xbar_ARREADY;
  wire s00_couplers_to_xbar_ARVALID;
  wire [31:0]s00_couplers_to_xbar_AWADDR;
  wire [2:0]s00_couplers_to_xbar_AWPROT;
  wire [0:0]s00_couplers_to_xbar_AWREADY;
  wire s00_couplers_to_xbar_AWVALID;
  wire s00_couplers_to_xbar_BREADY;
  wire [1:0]s00_couplers_to_xbar_BRESP;
  wire [0:0]s00_couplers_to_xbar_BVALID;
  wire [31:0]s00_couplers_to_xbar_RDATA;
  wire s00_couplers_to_xbar_RREADY;
  wire [1:0]s00_couplers_to_xbar_RRESP;
  wire [0:0]s00_couplers_to_xbar_RVALID;
  wire [31:0]s00_couplers_to_xbar_WDATA;
  wire [0:0]s00_couplers_to_xbar_WREADY;
  wire [3:0]s00_couplers_to_xbar_WSTRB;
  wire s00_couplers_to_xbar_WVALID;
  wire [31:0]xbar_to_m00_couplers_ARADDR;
  wire [0:0]xbar_to_m00_couplers_ARREADY;
  wire [0:0]xbar_to_m00_couplers_ARVALID;
  wire [31:0]xbar_to_m00_couplers_AWADDR;
  wire [0:0]xbar_to_m00_couplers_AWREADY;
  wire [0:0]xbar_to_m00_couplers_AWVALID;
  wire [0:0]xbar_to_m00_couplers_BREADY;
  wire [1:0]xbar_to_m00_couplers_BRESP;
  wire [0:0]xbar_to_m00_couplers_BVALID;
  wire [31:0]xbar_to_m00_couplers_RDATA;
  wire [0:0]xbar_to_m00_couplers_RREADY;
  wire [1:0]xbar_to_m00_couplers_RRESP;
  wire [0:0]xbar_to_m00_couplers_RVALID;
  wire [31:0]xbar_to_m00_couplers_WDATA;
  wire [0:0]xbar_to_m00_couplers_WREADY;
  wire [3:0]xbar_to_m00_couplers_WSTRB;
  wire [0:0]xbar_to_m00_couplers_WVALID;
  wire [63:32]xbar_to_m01_couplers_ARADDR;
  wire xbar_to_m01_couplers_ARREADY;
  wire [1:1]xbar_to_m01_couplers_ARVALID;
  wire [63:32]xbar_to_m01_couplers_AWADDR;
  wire xbar_to_m01_couplers_AWREADY;
  wire [1:1]xbar_to_m01_couplers_AWVALID;
  wire [1:1]xbar_to_m01_couplers_BREADY;
  wire [1:0]xbar_to_m01_couplers_BRESP;
  wire xbar_to_m01_couplers_BVALID;
  wire [31:0]xbar_to_m01_couplers_RDATA;
  wire [1:1]xbar_to_m01_couplers_RREADY;
  wire [1:0]xbar_to_m01_couplers_RRESP;
  wire xbar_to_m01_couplers_RVALID;
  wire [63:32]xbar_to_m01_couplers_WDATA;
  wire xbar_to_m01_couplers_WREADY;
  wire [7:4]xbar_to_m01_couplers_WSTRB;
  wire [1:1]xbar_to_m01_couplers_WVALID;
  wire [95:64]xbar_to_m02_couplers_ARADDR;
  wire xbar_to_m02_couplers_ARREADY;
  wire [2:2]xbar_to_m02_couplers_ARVALID;
  wire [95:64]xbar_to_m02_couplers_AWADDR;
  wire xbar_to_m02_couplers_AWREADY;
  wire [2:2]xbar_to_m02_couplers_AWVALID;
  wire [2:2]xbar_to_m02_couplers_BREADY;
  wire [1:0]xbar_to_m02_couplers_BRESP;
  wire xbar_to_m02_couplers_BVALID;
  wire [31:0]xbar_to_m02_couplers_RDATA;
  wire [2:2]xbar_to_m02_couplers_RREADY;
  wire [1:0]xbar_to_m02_couplers_RRESP;
  wire xbar_to_m02_couplers_RVALID;
  wire [95:64]xbar_to_m02_couplers_WDATA;
  wire xbar_to_m02_couplers_WREADY;
  wire [11:8]xbar_to_m02_couplers_WSTRB;
  wire [2:2]xbar_to_m02_couplers_WVALID;
  wire [127:96]xbar_to_m03_couplers_ARADDR;
  wire [11:9]xbar_to_m03_couplers_ARPROT;
  wire xbar_to_m03_couplers_ARREADY;
  wire [3:3]xbar_to_m03_couplers_ARVALID;
  wire [127:96]xbar_to_m03_couplers_AWADDR;
  wire [11:9]xbar_to_m03_couplers_AWPROT;
  wire xbar_to_m03_couplers_AWREADY;
  wire [3:3]xbar_to_m03_couplers_AWVALID;
  wire [3:3]xbar_to_m03_couplers_BREADY;
  wire [1:0]xbar_to_m03_couplers_BRESP;
  wire xbar_to_m03_couplers_BVALID;
  wire [31:0]xbar_to_m03_couplers_RDATA;
  wire [3:3]xbar_to_m03_couplers_RREADY;
  wire [1:0]xbar_to_m03_couplers_RRESP;
  wire xbar_to_m03_couplers_RVALID;
  wire [127:96]xbar_to_m03_couplers_WDATA;
  wire xbar_to_m03_couplers_WREADY;
  wire [15:12]xbar_to_m03_couplers_WSTRB;
  wire [3:3]xbar_to_m03_couplers_WVALID;
  wire [159:128]xbar_to_m04_couplers_ARADDR;
  wire xbar_to_m04_couplers_ARREADY;
  wire [4:4]xbar_to_m04_couplers_ARVALID;
  wire [159:128]xbar_to_m04_couplers_AWADDR;
  wire xbar_to_m04_couplers_AWREADY;
  wire [4:4]xbar_to_m04_couplers_AWVALID;
  wire [4:4]xbar_to_m04_couplers_BREADY;
  wire [1:0]xbar_to_m04_couplers_BRESP;
  wire xbar_to_m04_couplers_BVALID;
  wire [31:0]xbar_to_m04_couplers_RDATA;
  wire [4:4]xbar_to_m04_couplers_RREADY;
  wire [1:0]xbar_to_m04_couplers_RRESP;
  wire xbar_to_m04_couplers_RVALID;
  wire [159:128]xbar_to_m04_couplers_WDATA;
  wire xbar_to_m04_couplers_WREADY;
  wire [19:16]xbar_to_m04_couplers_WSTRB;
  wire [4:4]xbar_to_m04_couplers_WVALID;
  wire [191:160]xbar_to_m05_couplers_ARADDR;
  wire xbar_to_m05_couplers_ARREADY;
  wire [5:5]xbar_to_m05_couplers_ARVALID;
  wire [191:160]xbar_to_m05_couplers_AWADDR;
  wire xbar_to_m05_couplers_AWREADY;
  wire [5:5]xbar_to_m05_couplers_AWVALID;
  wire [5:5]xbar_to_m05_couplers_BREADY;
  wire [1:0]xbar_to_m05_couplers_BRESP;
  wire xbar_to_m05_couplers_BVALID;
  wire [31:0]xbar_to_m05_couplers_RDATA;
  wire [5:5]xbar_to_m05_couplers_RREADY;
  wire [1:0]xbar_to_m05_couplers_RRESP;
  wire xbar_to_m05_couplers_RVALID;
  wire [191:160]xbar_to_m05_couplers_WDATA;
  wire xbar_to_m05_couplers_WREADY;
  wire [23:20]xbar_to_m05_couplers_WSTRB;
  wire [5:5]xbar_to_m05_couplers_WVALID;
  wire [223:192]xbar_to_m06_couplers_ARADDR;
  wire xbar_to_m06_couplers_ARREADY;
  wire [6:6]xbar_to_m06_couplers_ARVALID;
  wire [223:192]xbar_to_m06_couplers_AWADDR;
  wire xbar_to_m06_couplers_AWREADY;
  wire [6:6]xbar_to_m06_couplers_AWVALID;
  wire [6:6]xbar_to_m06_couplers_BREADY;
  wire [1:0]xbar_to_m06_couplers_BRESP;
  wire xbar_to_m06_couplers_BVALID;
  wire [31:0]xbar_to_m06_couplers_RDATA;
  wire [6:6]xbar_to_m06_couplers_RREADY;
  wire [1:0]xbar_to_m06_couplers_RRESP;
  wire xbar_to_m06_couplers_RVALID;
  wire [223:192]xbar_to_m06_couplers_WDATA;
  wire xbar_to_m06_couplers_WREADY;
  wire [27:24]xbar_to_m06_couplers_WSTRB;
  wire [6:6]xbar_to_m06_couplers_WVALID;
  wire [255:224]xbar_to_m07_couplers_ARADDR;
  wire xbar_to_m07_couplers_ARREADY;
  wire [7:7]xbar_to_m07_couplers_ARVALID;
  wire [255:224]xbar_to_m07_couplers_AWADDR;
  wire xbar_to_m07_couplers_AWREADY;
  wire [7:7]xbar_to_m07_couplers_AWVALID;
  wire [7:7]xbar_to_m07_couplers_BREADY;
  wire [1:0]xbar_to_m07_couplers_BRESP;
  wire xbar_to_m07_couplers_BVALID;
  wire [31:0]xbar_to_m07_couplers_RDATA;
  wire [7:7]xbar_to_m07_couplers_RREADY;
  wire [1:0]xbar_to_m07_couplers_RRESP;
  wire xbar_to_m07_couplers_RVALID;
  wire [255:224]xbar_to_m07_couplers_WDATA;
  wire xbar_to_m07_couplers_WREADY;
  wire [31:28]xbar_to_m07_couplers_WSTRB;
  wire [7:7]xbar_to_m07_couplers_WVALID;
  wire [287:256]xbar_to_m08_couplers_ARADDR;
  wire [26:24]xbar_to_m08_couplers_ARPROT;
  wire xbar_to_m08_couplers_ARREADY;
  wire [8:8]xbar_to_m08_couplers_ARVALID;
  wire [287:256]xbar_to_m08_couplers_AWADDR;
  wire [26:24]xbar_to_m08_couplers_AWPROT;
  wire xbar_to_m08_couplers_AWREADY;
  wire [8:8]xbar_to_m08_couplers_AWVALID;
  wire [8:8]xbar_to_m08_couplers_BREADY;
  wire [1:0]xbar_to_m08_couplers_BRESP;
  wire xbar_to_m08_couplers_BVALID;
  wire [31:0]xbar_to_m08_couplers_RDATA;
  wire [8:8]xbar_to_m08_couplers_RREADY;
  wire [1:0]xbar_to_m08_couplers_RRESP;
  wire xbar_to_m08_couplers_RVALID;
  wire [287:256]xbar_to_m08_couplers_WDATA;
  wire xbar_to_m08_couplers_WREADY;
  wire [35:32]xbar_to_m08_couplers_WSTRB;
  wire [8:8]xbar_to_m08_couplers_WVALID;
  wire [319:288]xbar_to_m09_couplers_ARADDR;
  wire xbar_to_m09_couplers_ARREADY;
  wire [9:9]xbar_to_m09_couplers_ARVALID;
  wire [319:288]xbar_to_m09_couplers_AWADDR;
  wire xbar_to_m09_couplers_AWREADY;
  wire [9:9]xbar_to_m09_couplers_AWVALID;
  wire [9:9]xbar_to_m09_couplers_BREADY;
  wire [1:0]xbar_to_m09_couplers_BRESP;
  wire xbar_to_m09_couplers_BVALID;
  wire [31:0]xbar_to_m09_couplers_RDATA;
  wire [9:9]xbar_to_m09_couplers_RREADY;
  wire [1:0]xbar_to_m09_couplers_RRESP;
  wire xbar_to_m09_couplers_RVALID;
  wire [319:288]xbar_to_m09_couplers_WDATA;
  wire xbar_to_m09_couplers_WREADY;
  wire [39:36]xbar_to_m09_couplers_WSTRB;
  wire [9:9]xbar_to_m09_couplers_WVALID;
  wire [351:320]xbar_to_m10_couplers_ARADDR;
  wire xbar_to_m10_couplers_ARREADY;
  wire [10:10]xbar_to_m10_couplers_ARVALID;
  wire [351:320]xbar_to_m10_couplers_AWADDR;
  wire xbar_to_m10_couplers_AWREADY;
  wire [10:10]xbar_to_m10_couplers_AWVALID;
  wire [10:10]xbar_to_m10_couplers_BREADY;
  wire [1:0]xbar_to_m10_couplers_BRESP;
  wire xbar_to_m10_couplers_BVALID;
  wire [31:0]xbar_to_m10_couplers_RDATA;
  wire [10:10]xbar_to_m10_couplers_RREADY;
  wire [1:0]xbar_to_m10_couplers_RRESP;
  wire xbar_to_m10_couplers_RVALID;
  wire [351:320]xbar_to_m10_couplers_WDATA;
  wire xbar_to_m10_couplers_WREADY;
  wire [10:10]xbar_to_m10_couplers_WVALID;
  wire [383:352]xbar_to_m11_couplers_ARADDR;
  wire xbar_to_m11_couplers_ARREADY;
  wire [11:11]xbar_to_m11_couplers_ARVALID;
  wire [383:352]xbar_to_m11_couplers_AWADDR;
  wire xbar_to_m11_couplers_AWREADY;
  wire [11:11]xbar_to_m11_couplers_AWVALID;
  wire [11:11]xbar_to_m11_couplers_BREADY;
  wire [1:0]xbar_to_m11_couplers_BRESP;
  wire xbar_to_m11_couplers_BVALID;
  wire [31:0]xbar_to_m11_couplers_RDATA;
  wire [11:11]xbar_to_m11_couplers_RREADY;
  wire [1:0]xbar_to_m11_couplers_RRESP;
  wire xbar_to_m11_couplers_RVALID;
  wire [383:352]xbar_to_m11_couplers_WDATA;
  wire xbar_to_m11_couplers_WREADY;
  wire [47:44]xbar_to_m11_couplers_WSTRB;
  wire [11:11]xbar_to_m11_couplers_WVALID;
  wire [415:384]xbar_to_m12_couplers_ARADDR;
  wire xbar_to_m12_couplers_ARREADY;
  wire [12:12]xbar_to_m12_couplers_ARVALID;
  wire [415:384]xbar_to_m12_couplers_AWADDR;
  wire xbar_to_m12_couplers_AWREADY;
  wire [12:12]xbar_to_m12_couplers_AWVALID;
  wire [12:12]xbar_to_m12_couplers_BREADY;
  wire [1:0]xbar_to_m12_couplers_BRESP;
  wire xbar_to_m12_couplers_BVALID;
  wire [31:0]xbar_to_m12_couplers_RDATA;
  wire [12:12]xbar_to_m12_couplers_RREADY;
  wire [1:0]xbar_to_m12_couplers_RRESP;
  wire xbar_to_m12_couplers_RVALID;
  wire [415:384]xbar_to_m12_couplers_WDATA;
  wire xbar_to_m12_couplers_WREADY;
  wire [51:48]xbar_to_m12_couplers_WSTRB;
  wire [12:12]xbar_to_m12_couplers_WVALID;
  wire [38:0]NLW_xbar_m_axi_arprot_UNCONNECTED;
  wire [38:0]NLW_xbar_m_axi_awprot_UNCONNECTED;
  wire [51:0]NLW_xbar_m_axi_wstrb_UNCONNECTED;

  assign M00_ACLK_1 = M00_ACLK;
  assign M00_ARESETN_1 = M00_ARESETN[0];
  assign M00_AXI_araddr[31:0] = m00_couplers_to_processing_system7_0_axi_periph_ARADDR;
  assign M00_AXI_arvalid[0] = m00_couplers_to_processing_system7_0_axi_periph_ARVALID;
  assign M00_AXI_awaddr[31:0] = m00_couplers_to_processing_system7_0_axi_periph_AWADDR;
  assign M00_AXI_awvalid[0] = m00_couplers_to_processing_system7_0_axi_periph_AWVALID;
  assign M00_AXI_bready[0] = m00_couplers_to_processing_system7_0_axi_periph_BREADY;
  assign M00_AXI_rready[0] = m00_couplers_to_processing_system7_0_axi_periph_RREADY;
  assign M00_AXI_wdata[31:0] = m00_couplers_to_processing_system7_0_axi_periph_WDATA;
  assign M00_AXI_wstrb[3:0] = m00_couplers_to_processing_system7_0_axi_periph_WSTRB;
  assign M00_AXI_wvalid[0] = m00_couplers_to_processing_system7_0_axi_periph_WVALID;
  assign M01_ACLK_1 = M01_ACLK;
  assign M01_ARESETN_1 = M01_ARESETN[0];
  assign M01_AXI_araddr[31:0] = m01_couplers_to_processing_system7_0_axi_periph_ARADDR;
  assign M01_AXI_arvalid = m01_couplers_to_processing_system7_0_axi_periph_ARVALID;
  assign M01_AXI_awaddr[31:0] = m01_couplers_to_processing_system7_0_axi_periph_AWADDR;
  assign M01_AXI_awvalid = m01_couplers_to_processing_system7_0_axi_periph_AWVALID;
  assign M01_AXI_bready = m01_couplers_to_processing_system7_0_axi_periph_BREADY;
  assign M01_AXI_rready = m01_couplers_to_processing_system7_0_axi_periph_RREADY;
  assign M01_AXI_wdata[31:0] = m01_couplers_to_processing_system7_0_axi_periph_WDATA;
  assign M01_AXI_wstrb[3:0] = m01_couplers_to_processing_system7_0_axi_periph_WSTRB;
  assign M01_AXI_wvalid = m01_couplers_to_processing_system7_0_axi_periph_WVALID;
  assign M02_ACLK_1 = M02_ACLK;
  assign M02_ARESETN_1 = M02_ARESETN[0];
  assign M02_AXI_araddr[31:0] = m02_couplers_to_processing_system7_0_axi_periph_ARADDR;
  assign M02_AXI_arvalid = m02_couplers_to_processing_system7_0_axi_periph_ARVALID;
  assign M02_AXI_awaddr[31:0] = m02_couplers_to_processing_system7_0_axi_periph_AWADDR;
  assign M02_AXI_awvalid = m02_couplers_to_processing_system7_0_axi_periph_AWVALID;
  assign M02_AXI_bready = m02_couplers_to_processing_system7_0_axi_periph_BREADY;
  assign M02_AXI_rready = m02_couplers_to_processing_system7_0_axi_periph_RREADY;
  assign M02_AXI_wdata[31:0] = m02_couplers_to_processing_system7_0_axi_periph_WDATA;
  assign M02_AXI_wstrb[3:0] = m02_couplers_to_processing_system7_0_axi_periph_WSTRB;
  assign M02_AXI_wvalid = m02_couplers_to_processing_system7_0_axi_periph_WVALID;
  assign M03_ACLK_1 = M03_ACLK;
  assign M03_ARESETN_1 = M03_ARESETN[0];
  assign M03_AXI_araddr[31:0] = m03_couplers_to_processing_system7_0_axi_periph_ARADDR;
  assign M03_AXI_arprot[2:0] = m03_couplers_to_processing_system7_0_axi_periph_ARPROT;
  assign M03_AXI_arvalid = m03_couplers_to_processing_system7_0_axi_periph_ARVALID;
  assign M03_AXI_awaddr[31:0] = m03_couplers_to_processing_system7_0_axi_periph_AWADDR;
  assign M03_AXI_awprot[2:0] = m03_couplers_to_processing_system7_0_axi_periph_AWPROT;
  assign M03_AXI_awvalid = m03_couplers_to_processing_system7_0_axi_periph_AWVALID;
  assign M03_AXI_bready = m03_couplers_to_processing_system7_0_axi_periph_BREADY;
  assign M03_AXI_rready = m03_couplers_to_processing_system7_0_axi_periph_RREADY;
  assign M03_AXI_wdata[31:0] = m03_couplers_to_processing_system7_0_axi_periph_WDATA;
  assign M03_AXI_wstrb[3:0] = m03_couplers_to_processing_system7_0_axi_periph_WSTRB;
  assign M03_AXI_wvalid = m03_couplers_to_processing_system7_0_axi_periph_WVALID;
  assign M04_ACLK_1 = M04_ACLK;
  assign M04_ARESETN_1 = M04_ARESETN[0];
  assign M04_AXI_araddr[31:0] = m04_couplers_to_processing_system7_0_axi_periph_ARADDR;
  assign M04_AXI_arvalid = m04_couplers_to_processing_system7_0_axi_periph_ARVALID;
  assign M04_AXI_awaddr[31:0] = m04_couplers_to_processing_system7_0_axi_periph_AWADDR;
  assign M04_AXI_awvalid = m04_couplers_to_processing_system7_0_axi_periph_AWVALID;
  assign M04_AXI_bready = m04_couplers_to_processing_system7_0_axi_periph_BREADY;
  assign M04_AXI_rready = m04_couplers_to_processing_system7_0_axi_periph_RREADY;
  assign M04_AXI_wdata[31:0] = m04_couplers_to_processing_system7_0_axi_periph_WDATA;
  assign M04_AXI_wstrb[3:0] = m04_couplers_to_processing_system7_0_axi_periph_WSTRB;
  assign M04_AXI_wvalid = m04_couplers_to_processing_system7_0_axi_periph_WVALID;
  assign M05_ACLK_1 = M05_ACLK;
  assign M05_ARESETN_1 = M05_ARESETN[0];
  assign M05_AXI_araddr[31:0] = m05_couplers_to_processing_system7_0_axi_periph_ARADDR;
  assign M05_AXI_arvalid = m05_couplers_to_processing_system7_0_axi_periph_ARVALID;
  assign M05_AXI_awaddr[31:0] = m05_couplers_to_processing_system7_0_axi_periph_AWADDR;
  assign M05_AXI_awvalid = m05_couplers_to_processing_system7_0_axi_periph_AWVALID;
  assign M05_AXI_bready = m05_couplers_to_processing_system7_0_axi_periph_BREADY;
  assign M05_AXI_rready = m05_couplers_to_processing_system7_0_axi_periph_RREADY;
  assign M05_AXI_wdata[31:0] = m05_couplers_to_processing_system7_0_axi_periph_WDATA;
  assign M05_AXI_wstrb[3:0] = m05_couplers_to_processing_system7_0_axi_periph_WSTRB;
  assign M05_AXI_wvalid = m05_couplers_to_processing_system7_0_axi_periph_WVALID;
  assign M06_ACLK_1 = M06_ACLK;
  assign M06_ARESETN_1 = M06_ARESETN[0];
  assign M06_AXI_araddr[31:0] = m06_couplers_to_processing_system7_0_axi_periph_ARADDR;
  assign M06_AXI_arvalid = m06_couplers_to_processing_system7_0_axi_periph_ARVALID;
  assign M06_AXI_awaddr[31:0] = m06_couplers_to_processing_system7_0_axi_periph_AWADDR;
  assign M06_AXI_awvalid = m06_couplers_to_processing_system7_0_axi_periph_AWVALID;
  assign M06_AXI_bready = m06_couplers_to_processing_system7_0_axi_periph_BREADY;
  assign M06_AXI_rready = m06_couplers_to_processing_system7_0_axi_periph_RREADY;
  assign M06_AXI_wdata[31:0] = m06_couplers_to_processing_system7_0_axi_periph_WDATA;
  assign M06_AXI_wstrb[3:0] = m06_couplers_to_processing_system7_0_axi_periph_WSTRB;
  assign M06_AXI_wvalid = m06_couplers_to_processing_system7_0_axi_periph_WVALID;
  assign M07_ACLK_1 = M07_ACLK;
  assign M07_ARESETN_1 = M07_ARESETN[0];
  assign M07_AXI_araddr[31:0] = m07_couplers_to_processing_system7_0_axi_periph_ARADDR;
  assign M07_AXI_arvalid = m07_couplers_to_processing_system7_0_axi_periph_ARVALID;
  assign M07_AXI_awaddr[31:0] = m07_couplers_to_processing_system7_0_axi_periph_AWADDR;
  assign M07_AXI_awvalid = m07_couplers_to_processing_system7_0_axi_periph_AWVALID;
  assign M07_AXI_bready = m07_couplers_to_processing_system7_0_axi_periph_BREADY;
  assign M07_AXI_rready = m07_couplers_to_processing_system7_0_axi_periph_RREADY;
  assign M07_AXI_wdata[31:0] = m07_couplers_to_processing_system7_0_axi_periph_WDATA;
  assign M07_AXI_wstrb[3:0] = m07_couplers_to_processing_system7_0_axi_periph_WSTRB;
  assign M07_AXI_wvalid = m07_couplers_to_processing_system7_0_axi_periph_WVALID;
  assign M08_ACLK_1 = M08_ACLK;
  assign M08_ARESETN_1 = M08_ARESETN[0];
  assign M08_AXI_araddr[31:0] = m08_couplers_to_processing_system7_0_axi_periph_ARADDR;
  assign M08_AXI_arprot[2:0] = m08_couplers_to_processing_system7_0_axi_periph_ARPROT;
  assign M08_AXI_arvalid = m08_couplers_to_processing_system7_0_axi_periph_ARVALID;
  assign M08_AXI_awaddr[31:0] = m08_couplers_to_processing_system7_0_axi_periph_AWADDR;
  assign M08_AXI_awprot[2:0] = m08_couplers_to_processing_system7_0_axi_periph_AWPROT;
  assign M08_AXI_awvalid = m08_couplers_to_processing_system7_0_axi_periph_AWVALID;
  assign M08_AXI_bready = m08_couplers_to_processing_system7_0_axi_periph_BREADY;
  assign M08_AXI_rready = m08_couplers_to_processing_system7_0_axi_periph_RREADY;
  assign M08_AXI_wdata[31:0] = m08_couplers_to_processing_system7_0_axi_periph_WDATA;
  assign M08_AXI_wstrb[3:0] = m08_couplers_to_processing_system7_0_axi_periph_WSTRB;
  assign M08_AXI_wvalid = m08_couplers_to_processing_system7_0_axi_periph_WVALID;
  assign M09_ACLK_1 = M09_ACLK;
  assign M09_ARESETN_1 = M09_ARESETN[0];
  assign M09_AXI_araddr[31:0] = m09_couplers_to_processing_system7_0_axi_periph_ARADDR;
  assign M09_AXI_arvalid = m09_couplers_to_processing_system7_0_axi_periph_ARVALID;
  assign M09_AXI_awaddr[31:0] = m09_couplers_to_processing_system7_0_axi_periph_AWADDR;
  assign M09_AXI_awvalid = m09_couplers_to_processing_system7_0_axi_periph_AWVALID;
  assign M09_AXI_bready = m09_couplers_to_processing_system7_0_axi_periph_BREADY;
  assign M09_AXI_rready = m09_couplers_to_processing_system7_0_axi_periph_RREADY;
  assign M09_AXI_wdata[31:0] = m09_couplers_to_processing_system7_0_axi_periph_WDATA;
  assign M09_AXI_wstrb[3:0] = m09_couplers_to_processing_system7_0_axi_periph_WSTRB;
  assign M09_AXI_wvalid = m09_couplers_to_processing_system7_0_axi_periph_WVALID;
  assign M10_ACLK_1 = M10_ACLK;
  assign M10_ARESETN_1 = M10_ARESETN[0];
  assign M10_AXI_araddr[31:0] = m10_couplers_to_processing_system7_0_axi_periph_ARADDR;
  assign M10_AXI_arvalid = m10_couplers_to_processing_system7_0_axi_periph_ARVALID;
  assign M10_AXI_awaddr[31:0] = m10_couplers_to_processing_system7_0_axi_periph_AWADDR;
  assign M10_AXI_awvalid = m10_couplers_to_processing_system7_0_axi_periph_AWVALID;
  assign M10_AXI_bready = m10_couplers_to_processing_system7_0_axi_periph_BREADY;
  assign M10_AXI_rready = m10_couplers_to_processing_system7_0_axi_periph_RREADY;
  assign M10_AXI_wdata[31:0] = m10_couplers_to_processing_system7_0_axi_periph_WDATA;
  assign M10_AXI_wvalid = m10_couplers_to_processing_system7_0_axi_periph_WVALID;
  assign M11_ACLK_1 = M11_ACLK;
  assign M11_ARESETN_1 = M11_ARESETN[0];
  assign M11_AXI_araddr[31:0] = m11_couplers_to_processing_system7_0_axi_periph_ARADDR;
  assign M11_AXI_arvalid = m11_couplers_to_processing_system7_0_axi_periph_ARVALID;
  assign M11_AXI_awaddr[31:0] = m11_couplers_to_processing_system7_0_axi_periph_AWADDR;
  assign M11_AXI_awvalid = m11_couplers_to_processing_system7_0_axi_periph_AWVALID;
  assign M11_AXI_bready = m11_couplers_to_processing_system7_0_axi_periph_BREADY;
  assign M11_AXI_rready = m11_couplers_to_processing_system7_0_axi_periph_RREADY;
  assign M11_AXI_wdata[31:0] = m11_couplers_to_processing_system7_0_axi_periph_WDATA;
  assign M11_AXI_wstrb[3:0] = m11_couplers_to_processing_system7_0_axi_periph_WSTRB;
  assign M11_AXI_wvalid = m11_couplers_to_processing_system7_0_axi_periph_WVALID;
  assign M12_ACLK_1 = M12_ACLK;
  assign M12_ARESETN_1 = M12_ARESETN[0];
  assign M12_AXI_araddr[31:0] = m12_couplers_to_processing_system7_0_axi_periph_ARADDR;
  assign M12_AXI_arvalid = m12_couplers_to_processing_system7_0_axi_periph_ARVALID;
  assign M12_AXI_awaddr[31:0] = m12_couplers_to_processing_system7_0_axi_periph_AWADDR;
  assign M12_AXI_awvalid = m12_couplers_to_processing_system7_0_axi_periph_AWVALID;
  assign M12_AXI_bready = m12_couplers_to_processing_system7_0_axi_periph_BREADY;
  assign M12_AXI_rready = m12_couplers_to_processing_system7_0_axi_periph_RREADY;
  assign M12_AXI_wdata[31:0] = m12_couplers_to_processing_system7_0_axi_periph_WDATA;
  assign M12_AXI_wstrb[3:0] = m12_couplers_to_processing_system7_0_axi_periph_WSTRB;
  assign M12_AXI_wvalid = m12_couplers_to_processing_system7_0_axi_periph_WVALID;
  assign S00_ACLK_1 = S00_ACLK;
  assign S00_ARESETN_1 = S00_ARESETN[0];
  assign S00_AXI_arready = processing_system7_0_axi_periph_to_s00_couplers_ARREADY;
  assign S00_AXI_awready = processing_system7_0_axi_periph_to_s00_couplers_AWREADY;
  assign S00_AXI_bid[11:0] = processing_system7_0_axi_periph_to_s00_couplers_BID;
  assign S00_AXI_bresp[1:0] = processing_system7_0_axi_periph_to_s00_couplers_BRESP;
  assign S00_AXI_bvalid = processing_system7_0_axi_periph_to_s00_couplers_BVALID;
  assign S00_AXI_rdata[31:0] = processing_system7_0_axi_periph_to_s00_couplers_RDATA;
  assign S00_AXI_rid[11:0] = processing_system7_0_axi_periph_to_s00_couplers_RID;
  assign S00_AXI_rlast = processing_system7_0_axi_periph_to_s00_couplers_RLAST;
  assign S00_AXI_rresp[1:0] = processing_system7_0_axi_periph_to_s00_couplers_RRESP;
  assign S00_AXI_rvalid = processing_system7_0_axi_periph_to_s00_couplers_RVALID;
  assign S00_AXI_wready = processing_system7_0_axi_periph_to_s00_couplers_WREADY;
  assign m00_couplers_to_processing_system7_0_axi_periph_ARREADY = M00_AXI_arready[0];
  assign m00_couplers_to_processing_system7_0_axi_periph_AWREADY = M00_AXI_awready[0];
  assign m00_couplers_to_processing_system7_0_axi_periph_BRESP = M00_AXI_bresp[1:0];
  assign m00_couplers_to_processing_system7_0_axi_periph_BVALID = M00_AXI_bvalid[0];
  assign m00_couplers_to_processing_system7_0_axi_periph_RDATA = M00_AXI_rdata[31:0];
  assign m00_couplers_to_processing_system7_0_axi_periph_RRESP = M00_AXI_rresp[1:0];
  assign m00_couplers_to_processing_system7_0_axi_periph_RVALID = M00_AXI_rvalid[0];
  assign m00_couplers_to_processing_system7_0_axi_periph_WREADY = M00_AXI_wready[0];
  assign m01_couplers_to_processing_system7_0_axi_periph_ARREADY = M01_AXI_arready;
  assign m01_couplers_to_processing_system7_0_axi_periph_AWREADY = M01_AXI_awready;
  assign m01_couplers_to_processing_system7_0_axi_periph_BRESP = M01_AXI_bresp[1:0];
  assign m01_couplers_to_processing_system7_0_axi_periph_BVALID = M01_AXI_bvalid;
  assign m01_couplers_to_processing_system7_0_axi_periph_RDATA = M01_AXI_rdata[31:0];
  assign m01_couplers_to_processing_system7_0_axi_periph_RRESP = M01_AXI_rresp[1:0];
  assign m01_couplers_to_processing_system7_0_axi_periph_RVALID = M01_AXI_rvalid;
  assign m01_couplers_to_processing_system7_0_axi_periph_WREADY = M01_AXI_wready;
  assign m02_couplers_to_processing_system7_0_axi_periph_ARREADY = M02_AXI_arready;
  assign m02_couplers_to_processing_system7_0_axi_periph_AWREADY = M02_AXI_awready;
  assign m02_couplers_to_processing_system7_0_axi_periph_BRESP = M02_AXI_bresp[1:0];
  assign m02_couplers_to_processing_system7_0_axi_periph_BVALID = M02_AXI_bvalid;
  assign m02_couplers_to_processing_system7_0_axi_periph_RDATA = M02_AXI_rdata[31:0];
  assign m02_couplers_to_processing_system7_0_axi_periph_RRESP = M02_AXI_rresp[1:0];
  assign m02_couplers_to_processing_system7_0_axi_periph_RVALID = M02_AXI_rvalid;
  assign m02_couplers_to_processing_system7_0_axi_periph_WREADY = M02_AXI_wready;
  assign m03_couplers_to_processing_system7_0_axi_periph_ARREADY = M03_AXI_arready;
  assign m03_couplers_to_processing_system7_0_axi_periph_AWREADY = M03_AXI_awready;
  assign m03_couplers_to_processing_system7_0_axi_periph_BRESP = M03_AXI_bresp[1:0];
  assign m03_couplers_to_processing_system7_0_axi_periph_BVALID = M03_AXI_bvalid;
  assign m03_couplers_to_processing_system7_0_axi_periph_RDATA = M03_AXI_rdata[31:0];
  assign m03_couplers_to_processing_system7_0_axi_periph_RRESP = M03_AXI_rresp[1:0];
  assign m03_couplers_to_processing_system7_0_axi_periph_RVALID = M03_AXI_rvalid;
  assign m03_couplers_to_processing_system7_0_axi_periph_WREADY = M03_AXI_wready;
  assign m04_couplers_to_processing_system7_0_axi_periph_ARREADY = M04_AXI_arready;
  assign m04_couplers_to_processing_system7_0_axi_periph_AWREADY = M04_AXI_awready;
  assign m04_couplers_to_processing_system7_0_axi_periph_BRESP = M04_AXI_bresp[1:0];
  assign m04_couplers_to_processing_system7_0_axi_periph_BVALID = M04_AXI_bvalid;
  assign m04_couplers_to_processing_system7_0_axi_periph_RDATA = M04_AXI_rdata[31:0];
  assign m04_couplers_to_processing_system7_0_axi_periph_RRESP = M04_AXI_rresp[1:0];
  assign m04_couplers_to_processing_system7_0_axi_periph_RVALID = M04_AXI_rvalid;
  assign m04_couplers_to_processing_system7_0_axi_periph_WREADY = M04_AXI_wready;
  assign m05_couplers_to_processing_system7_0_axi_periph_ARREADY = M05_AXI_arready;
  assign m05_couplers_to_processing_system7_0_axi_periph_AWREADY = M05_AXI_awready;
  assign m05_couplers_to_processing_system7_0_axi_periph_BRESP = M05_AXI_bresp[1:0];
  assign m05_couplers_to_processing_system7_0_axi_periph_BVALID = M05_AXI_bvalid;
  assign m05_couplers_to_processing_system7_0_axi_periph_RDATA = M05_AXI_rdata[31:0];
  assign m05_couplers_to_processing_system7_0_axi_periph_RRESP = M05_AXI_rresp[1:0];
  assign m05_couplers_to_processing_system7_0_axi_periph_RVALID = M05_AXI_rvalid;
  assign m05_couplers_to_processing_system7_0_axi_periph_WREADY = M05_AXI_wready;
  assign m06_couplers_to_processing_system7_0_axi_periph_ARREADY = M06_AXI_arready;
  assign m06_couplers_to_processing_system7_0_axi_periph_AWREADY = M06_AXI_awready;
  assign m06_couplers_to_processing_system7_0_axi_periph_BRESP = M06_AXI_bresp[1:0];
  assign m06_couplers_to_processing_system7_0_axi_periph_BVALID = M06_AXI_bvalid;
  assign m06_couplers_to_processing_system7_0_axi_periph_RDATA = M06_AXI_rdata[31:0];
  assign m06_couplers_to_processing_system7_0_axi_periph_RRESP = M06_AXI_rresp[1:0];
  assign m06_couplers_to_processing_system7_0_axi_periph_RVALID = M06_AXI_rvalid;
  assign m06_couplers_to_processing_system7_0_axi_periph_WREADY = M06_AXI_wready;
  assign m07_couplers_to_processing_system7_0_axi_periph_ARREADY = M07_AXI_arready;
  assign m07_couplers_to_processing_system7_0_axi_periph_AWREADY = M07_AXI_awready;
  assign m07_couplers_to_processing_system7_0_axi_periph_BRESP = M07_AXI_bresp[1:0];
  assign m07_couplers_to_processing_system7_0_axi_periph_BVALID = M07_AXI_bvalid;
  assign m07_couplers_to_processing_system7_0_axi_periph_RDATA = M07_AXI_rdata[31:0];
  assign m07_couplers_to_processing_system7_0_axi_periph_RRESP = M07_AXI_rresp[1:0];
  assign m07_couplers_to_processing_system7_0_axi_periph_RVALID = M07_AXI_rvalid;
  assign m07_couplers_to_processing_system7_0_axi_periph_WREADY = M07_AXI_wready;
  assign m08_couplers_to_processing_system7_0_axi_periph_ARREADY = M08_AXI_arready;
  assign m08_couplers_to_processing_system7_0_axi_periph_AWREADY = M08_AXI_awready;
  assign m08_couplers_to_processing_system7_0_axi_periph_BRESP = M08_AXI_bresp[1:0];
  assign m08_couplers_to_processing_system7_0_axi_periph_BVALID = M08_AXI_bvalid;
  assign m08_couplers_to_processing_system7_0_axi_periph_RDATA = M08_AXI_rdata[31:0];
  assign m08_couplers_to_processing_system7_0_axi_periph_RRESP = M08_AXI_rresp[1:0];
  assign m08_couplers_to_processing_system7_0_axi_periph_RVALID = M08_AXI_rvalid;
  assign m08_couplers_to_processing_system7_0_axi_periph_WREADY = M08_AXI_wready;
  assign m09_couplers_to_processing_system7_0_axi_periph_ARREADY = M09_AXI_arready;
  assign m09_couplers_to_processing_system7_0_axi_periph_AWREADY = M09_AXI_awready;
  assign m09_couplers_to_processing_system7_0_axi_periph_BRESP = M09_AXI_bresp[1:0];
  assign m09_couplers_to_processing_system7_0_axi_periph_BVALID = M09_AXI_bvalid;
  assign m09_couplers_to_processing_system7_0_axi_periph_RDATA = M09_AXI_rdata[31:0];
  assign m09_couplers_to_processing_system7_0_axi_periph_RRESP = M09_AXI_rresp[1:0];
  assign m09_couplers_to_processing_system7_0_axi_periph_RVALID = M09_AXI_rvalid;
  assign m09_couplers_to_processing_system7_0_axi_periph_WREADY = M09_AXI_wready;
  assign m10_couplers_to_processing_system7_0_axi_periph_ARREADY = M10_AXI_arready;
  assign m10_couplers_to_processing_system7_0_axi_periph_AWREADY = M10_AXI_awready;
  assign m10_couplers_to_processing_system7_0_axi_periph_BRESP = M10_AXI_bresp[1:0];
  assign m10_couplers_to_processing_system7_0_axi_periph_BVALID = M10_AXI_bvalid;
  assign m10_couplers_to_processing_system7_0_axi_periph_RDATA = M10_AXI_rdata[31:0];
  assign m10_couplers_to_processing_system7_0_axi_periph_RRESP = M10_AXI_rresp[1:0];
  assign m10_couplers_to_processing_system7_0_axi_periph_RVALID = M10_AXI_rvalid;
  assign m10_couplers_to_processing_system7_0_axi_periph_WREADY = M10_AXI_wready;
  assign m11_couplers_to_processing_system7_0_axi_periph_ARREADY = M11_AXI_arready;
  assign m11_couplers_to_processing_system7_0_axi_periph_AWREADY = M11_AXI_awready;
  assign m11_couplers_to_processing_system7_0_axi_periph_BRESP = M11_AXI_bresp[1:0];
  assign m11_couplers_to_processing_system7_0_axi_periph_BVALID = M11_AXI_bvalid;
  assign m11_couplers_to_processing_system7_0_axi_periph_RDATA = M11_AXI_rdata[31:0];
  assign m11_couplers_to_processing_system7_0_axi_periph_RRESP = M11_AXI_rresp[1:0];
  assign m11_couplers_to_processing_system7_0_axi_periph_RVALID = M11_AXI_rvalid;
  assign m11_couplers_to_processing_system7_0_axi_periph_WREADY = M11_AXI_wready;
  assign m12_couplers_to_processing_system7_0_axi_periph_ARREADY = M12_AXI_arready;
  assign m12_couplers_to_processing_system7_0_axi_periph_AWREADY = M12_AXI_awready;
  assign m12_couplers_to_processing_system7_0_axi_periph_BRESP = M12_AXI_bresp[1:0];
  assign m12_couplers_to_processing_system7_0_axi_periph_BVALID = M12_AXI_bvalid;
  assign m12_couplers_to_processing_system7_0_axi_periph_RDATA = M12_AXI_rdata[31:0];
  assign m12_couplers_to_processing_system7_0_axi_periph_RRESP = M12_AXI_rresp[1:0];
  assign m12_couplers_to_processing_system7_0_axi_periph_RVALID = M12_AXI_rvalid;
  assign m12_couplers_to_processing_system7_0_axi_periph_WREADY = M12_AXI_wready;
  assign processing_system7_0_axi_periph_ACLK_net = ACLK;
  assign processing_system7_0_axi_periph_ARESETN_net = ARESETN[0];
  assign processing_system7_0_axi_periph_to_s00_couplers_ARADDR = S00_AXI_araddr[31:0];
  assign processing_system7_0_axi_periph_to_s00_couplers_ARBURST = S00_AXI_arburst[1:0];
  assign processing_system7_0_axi_periph_to_s00_couplers_ARCACHE = S00_AXI_arcache[3:0];
  assign processing_system7_0_axi_periph_to_s00_couplers_ARID = S00_AXI_arid[11:0];
  assign processing_system7_0_axi_periph_to_s00_couplers_ARLEN = S00_AXI_arlen[3:0];
  assign processing_system7_0_axi_periph_to_s00_couplers_ARLOCK = S00_AXI_arlock[1:0];
  assign processing_system7_0_axi_periph_to_s00_couplers_ARPROT = S00_AXI_arprot[2:0];
  assign processing_system7_0_axi_periph_to_s00_couplers_ARQOS = S00_AXI_arqos[3:0];
  assign processing_system7_0_axi_periph_to_s00_couplers_ARSIZE = S00_AXI_arsize[2:0];
  assign processing_system7_0_axi_periph_to_s00_couplers_ARVALID = S00_AXI_arvalid;
  assign processing_system7_0_axi_periph_to_s00_couplers_AWADDR = S00_AXI_awaddr[31:0];
  assign processing_system7_0_axi_periph_to_s00_couplers_AWBURST = S00_AXI_awburst[1:0];
  assign processing_system7_0_axi_periph_to_s00_couplers_AWCACHE = S00_AXI_awcache[3:0];
  assign processing_system7_0_axi_periph_to_s00_couplers_AWID = S00_AXI_awid[11:0];
  assign processing_system7_0_axi_periph_to_s00_couplers_AWLEN = S00_AXI_awlen[3:0];
  assign processing_system7_0_axi_periph_to_s00_couplers_AWLOCK = S00_AXI_awlock[1:0];
  assign processing_system7_0_axi_periph_to_s00_couplers_AWPROT = S00_AXI_awprot[2:0];
  assign processing_system7_0_axi_periph_to_s00_couplers_AWQOS = S00_AXI_awqos[3:0];
  assign processing_system7_0_axi_periph_to_s00_couplers_AWSIZE = S00_AXI_awsize[2:0];
  assign processing_system7_0_axi_periph_to_s00_couplers_AWVALID = S00_AXI_awvalid;
  assign processing_system7_0_axi_periph_to_s00_couplers_BREADY = S00_AXI_bready;
  assign processing_system7_0_axi_periph_to_s00_couplers_RREADY = S00_AXI_rready;
  assign processing_system7_0_axi_periph_to_s00_couplers_WDATA = S00_AXI_wdata[31:0];
  assign processing_system7_0_axi_periph_to_s00_couplers_WID = S00_AXI_wid[11:0];
  assign processing_system7_0_axi_periph_to_s00_couplers_WLAST = S00_AXI_wlast;
  assign processing_system7_0_axi_periph_to_s00_couplers_WSTRB = S00_AXI_wstrb[3:0];
  assign processing_system7_0_axi_periph_to_s00_couplers_WVALID = S00_AXI_wvalid;
  m00_couplers_imp_1R5MXF4 m00_couplers
       (.M_ACLK(M00_ACLK_1),
        .M_ARESETN(M00_ARESETN_1),
        .M_AXI_araddr(m00_couplers_to_processing_system7_0_axi_periph_ARADDR),
        .M_AXI_arready(m00_couplers_to_processing_system7_0_axi_periph_ARREADY),
        .M_AXI_arvalid(m00_couplers_to_processing_system7_0_axi_periph_ARVALID),
        .M_AXI_awaddr(m00_couplers_to_processing_system7_0_axi_periph_AWADDR),
        .M_AXI_awready(m00_couplers_to_processing_system7_0_axi_periph_AWREADY),
        .M_AXI_awvalid(m00_couplers_to_processing_system7_0_axi_periph_AWVALID),
        .M_AXI_bready(m00_couplers_to_processing_system7_0_axi_periph_BREADY),
        .M_AXI_bresp(m00_couplers_to_processing_system7_0_axi_periph_BRESP),
        .M_AXI_bvalid(m00_couplers_to_processing_system7_0_axi_periph_BVALID),
        .M_AXI_rdata(m00_couplers_to_processing_system7_0_axi_periph_RDATA),
        .M_AXI_rready(m00_couplers_to_processing_system7_0_axi_periph_RREADY),
        .M_AXI_rresp(m00_couplers_to_processing_system7_0_axi_periph_RRESP),
        .M_AXI_rvalid(m00_couplers_to_processing_system7_0_axi_periph_RVALID),
        .M_AXI_wdata(m00_couplers_to_processing_system7_0_axi_periph_WDATA),
        .M_AXI_wready(m00_couplers_to_processing_system7_0_axi_periph_WREADY),
        .M_AXI_wstrb(m00_couplers_to_processing_system7_0_axi_periph_WSTRB),
        .M_AXI_wvalid(m00_couplers_to_processing_system7_0_axi_periph_WVALID),
        .S_ACLK(processing_system7_0_axi_periph_ACLK_net),
        .S_ARESETN(processing_system7_0_axi_periph_ARESETN_net),
        .S_AXI_araddr(xbar_to_m00_couplers_ARADDR),
        .S_AXI_arready(xbar_to_m00_couplers_ARREADY),
        .S_AXI_arvalid(xbar_to_m00_couplers_ARVALID),
        .S_AXI_awaddr(xbar_to_m00_couplers_AWADDR),
        .S_AXI_awready(xbar_to_m00_couplers_AWREADY),
        .S_AXI_awvalid(xbar_to_m00_couplers_AWVALID),
        .S_AXI_bready(xbar_to_m00_couplers_BREADY),
        .S_AXI_bresp(xbar_to_m00_couplers_BRESP),
        .S_AXI_bvalid(xbar_to_m00_couplers_BVALID),
        .S_AXI_rdata(xbar_to_m00_couplers_RDATA),
        .S_AXI_rready(xbar_to_m00_couplers_RREADY),
        .S_AXI_rresp(xbar_to_m00_couplers_RRESP),
        .S_AXI_rvalid(xbar_to_m00_couplers_RVALID),
        .S_AXI_wdata(xbar_to_m00_couplers_WDATA),
        .S_AXI_wready(xbar_to_m00_couplers_WREADY),
        .S_AXI_wstrb(xbar_to_m00_couplers_WSTRB),
        .S_AXI_wvalid(xbar_to_m00_couplers_WVALID));
  m01_couplers_imp_19312F m01_couplers
       (.M_ACLK(M01_ACLK_1),
        .M_ARESETN(M01_ARESETN_1),
        .M_AXI_araddr(m01_couplers_to_processing_system7_0_axi_periph_ARADDR),
        .M_AXI_arready(m01_couplers_to_processing_system7_0_axi_periph_ARREADY),
        .M_AXI_arvalid(m01_couplers_to_processing_system7_0_axi_periph_ARVALID),
        .M_AXI_awaddr(m01_couplers_to_processing_system7_0_axi_periph_AWADDR),
        .M_AXI_awready(m01_couplers_to_processing_system7_0_axi_periph_AWREADY),
        .M_AXI_awvalid(m01_couplers_to_processing_system7_0_axi_periph_AWVALID),
        .M_AXI_bready(m01_couplers_to_processing_system7_0_axi_periph_BREADY),
        .M_AXI_bresp(m01_couplers_to_processing_system7_0_axi_periph_BRESP),
        .M_AXI_bvalid(m01_couplers_to_processing_system7_0_axi_periph_BVALID),
        .M_AXI_rdata(m01_couplers_to_processing_system7_0_axi_periph_RDATA),
        .M_AXI_rready(m01_couplers_to_processing_system7_0_axi_periph_RREADY),
        .M_AXI_rresp(m01_couplers_to_processing_system7_0_axi_periph_RRESP),
        .M_AXI_rvalid(m01_couplers_to_processing_system7_0_axi_periph_RVALID),
        .M_AXI_wdata(m01_couplers_to_processing_system7_0_axi_periph_WDATA),
        .M_AXI_wready(m01_couplers_to_processing_system7_0_axi_periph_WREADY),
        .M_AXI_wstrb(m01_couplers_to_processing_system7_0_axi_periph_WSTRB),
        .M_AXI_wvalid(m01_couplers_to_processing_system7_0_axi_periph_WVALID),
        .S_ACLK(processing_system7_0_axi_periph_ACLK_net),
        .S_ARESETN(processing_system7_0_axi_periph_ARESETN_net),
        .S_AXI_araddr(xbar_to_m01_couplers_ARADDR),
        .S_AXI_arready(xbar_to_m01_couplers_ARREADY),
        .S_AXI_arvalid(xbar_to_m01_couplers_ARVALID),
        .S_AXI_awaddr(xbar_to_m01_couplers_AWADDR),
        .S_AXI_awready(xbar_to_m01_couplers_AWREADY),
        .S_AXI_awvalid(xbar_to_m01_couplers_AWVALID),
        .S_AXI_bready(xbar_to_m01_couplers_BREADY),
        .S_AXI_bresp(xbar_to_m01_couplers_BRESP),
        .S_AXI_bvalid(xbar_to_m01_couplers_BVALID),
        .S_AXI_rdata(xbar_to_m01_couplers_RDATA),
        .S_AXI_rready(xbar_to_m01_couplers_RREADY),
        .S_AXI_rresp(xbar_to_m01_couplers_RRESP),
        .S_AXI_rvalid(xbar_to_m01_couplers_RVALID),
        .S_AXI_wdata(xbar_to_m01_couplers_WDATA),
        .S_AXI_wready(xbar_to_m01_couplers_WREADY),
        .S_AXI_wstrb(xbar_to_m01_couplers_WSTRB),
        .S_AXI_wvalid(xbar_to_m01_couplers_WVALID));
  m02_couplers_imp_1WHIQQN m02_couplers
       (.M_ACLK(M02_ACLK_1),
        .M_ARESETN(M02_ARESETN_1),
        .M_AXI_araddr(m02_couplers_to_processing_system7_0_axi_periph_ARADDR),
        .M_AXI_arready(m02_couplers_to_processing_system7_0_axi_periph_ARREADY),
        .M_AXI_arvalid(m02_couplers_to_processing_system7_0_axi_periph_ARVALID),
        .M_AXI_awaddr(m02_couplers_to_processing_system7_0_axi_periph_AWADDR),
        .M_AXI_awready(m02_couplers_to_processing_system7_0_axi_periph_AWREADY),
        .M_AXI_awvalid(m02_couplers_to_processing_system7_0_axi_periph_AWVALID),
        .M_AXI_bready(m02_couplers_to_processing_system7_0_axi_periph_BREADY),
        .M_AXI_bresp(m02_couplers_to_processing_system7_0_axi_periph_BRESP),
        .M_AXI_bvalid(m02_couplers_to_processing_system7_0_axi_periph_BVALID),
        .M_AXI_rdata(m02_couplers_to_processing_system7_0_axi_periph_RDATA),
        .M_AXI_rready(m02_couplers_to_processing_system7_0_axi_periph_RREADY),
        .M_AXI_rresp(m02_couplers_to_processing_system7_0_axi_periph_RRESP),
        .M_AXI_rvalid(m02_couplers_to_processing_system7_0_axi_periph_RVALID),
        .M_AXI_wdata(m02_couplers_to_processing_system7_0_axi_periph_WDATA),
        .M_AXI_wready(m02_couplers_to_processing_system7_0_axi_periph_WREADY),
        .M_AXI_wstrb(m02_couplers_to_processing_system7_0_axi_periph_WSTRB),
        .M_AXI_wvalid(m02_couplers_to_processing_system7_0_axi_periph_WVALID),
        .S_ACLK(processing_system7_0_axi_periph_ACLK_net),
        .S_ARESETN(processing_system7_0_axi_periph_ARESETN_net),
        .S_AXI_araddr(xbar_to_m02_couplers_ARADDR),
        .S_AXI_arready(xbar_to_m02_couplers_ARREADY),
        .S_AXI_arvalid(xbar_to_m02_couplers_ARVALID),
        .S_AXI_awaddr(xbar_to_m02_couplers_AWADDR),
        .S_AXI_awready(xbar_to_m02_couplers_AWREADY),
        .S_AXI_awvalid(xbar_to_m02_couplers_AWVALID),
        .S_AXI_bready(xbar_to_m02_couplers_BREADY),
        .S_AXI_bresp(xbar_to_m02_couplers_BRESP),
        .S_AXI_bvalid(xbar_to_m02_couplers_BVALID),
        .S_AXI_rdata(xbar_to_m02_couplers_RDATA),
        .S_AXI_rready(xbar_to_m02_couplers_RREADY),
        .S_AXI_rresp(xbar_to_m02_couplers_RRESP),
        .S_AXI_rvalid(xbar_to_m02_couplers_RVALID),
        .S_AXI_wdata(xbar_to_m02_couplers_WDATA),
        .S_AXI_wready(xbar_to_m02_couplers_WREADY),
        .S_AXI_wstrb(xbar_to_m02_couplers_WSTRB),
        .S_AXI_wvalid(xbar_to_m02_couplers_WVALID));
  m03_couplers_imp_4XNRYG m03_couplers
       (.M_ACLK(M03_ACLK_1),
        .M_ARESETN(M03_ARESETN_1),
        .M_AXI_araddr(m03_couplers_to_processing_system7_0_axi_periph_ARADDR),
        .M_AXI_arprot(m03_couplers_to_processing_system7_0_axi_periph_ARPROT),
        .M_AXI_arready(m03_couplers_to_processing_system7_0_axi_periph_ARREADY),
        .M_AXI_arvalid(m03_couplers_to_processing_system7_0_axi_periph_ARVALID),
        .M_AXI_awaddr(m03_couplers_to_processing_system7_0_axi_periph_AWADDR),
        .M_AXI_awprot(m03_couplers_to_processing_system7_0_axi_periph_AWPROT),
        .M_AXI_awready(m03_couplers_to_processing_system7_0_axi_periph_AWREADY),
        .M_AXI_awvalid(m03_couplers_to_processing_system7_0_axi_periph_AWVALID),
        .M_AXI_bready(m03_couplers_to_processing_system7_0_axi_periph_BREADY),
        .M_AXI_bresp(m03_couplers_to_processing_system7_0_axi_periph_BRESP),
        .M_AXI_bvalid(m03_couplers_to_processing_system7_0_axi_periph_BVALID),
        .M_AXI_rdata(m03_couplers_to_processing_system7_0_axi_periph_RDATA),
        .M_AXI_rready(m03_couplers_to_processing_system7_0_axi_periph_RREADY),
        .M_AXI_rresp(m03_couplers_to_processing_system7_0_axi_periph_RRESP),
        .M_AXI_rvalid(m03_couplers_to_processing_system7_0_axi_periph_RVALID),
        .M_AXI_wdata(m03_couplers_to_processing_system7_0_axi_periph_WDATA),
        .M_AXI_wready(m03_couplers_to_processing_system7_0_axi_periph_WREADY),
        .M_AXI_wstrb(m03_couplers_to_processing_system7_0_axi_periph_WSTRB),
        .M_AXI_wvalid(m03_couplers_to_processing_system7_0_axi_periph_WVALID),
        .S_ACLK(processing_system7_0_axi_periph_ACLK_net),
        .S_ARESETN(processing_system7_0_axi_periph_ARESETN_net),
        .S_AXI_araddr(xbar_to_m03_couplers_ARADDR),
        .S_AXI_arprot(xbar_to_m03_couplers_ARPROT),
        .S_AXI_arready(xbar_to_m03_couplers_ARREADY),
        .S_AXI_arvalid(xbar_to_m03_couplers_ARVALID),
        .S_AXI_awaddr(xbar_to_m03_couplers_AWADDR),
        .S_AXI_awprot(xbar_to_m03_couplers_AWPROT),
        .S_AXI_awready(xbar_to_m03_couplers_AWREADY),
        .S_AXI_awvalid(xbar_to_m03_couplers_AWVALID),
        .S_AXI_bready(xbar_to_m03_couplers_BREADY),
        .S_AXI_bresp(xbar_to_m03_couplers_BRESP),
        .S_AXI_bvalid(xbar_to_m03_couplers_BVALID),
        .S_AXI_rdata(xbar_to_m03_couplers_RDATA),
        .S_AXI_rready(xbar_to_m03_couplers_RREADY),
        .S_AXI_rresp(xbar_to_m03_couplers_RRESP),
        .S_AXI_rvalid(xbar_to_m03_couplers_RVALID),
        .S_AXI_wdata(xbar_to_m03_couplers_WDATA),
        .S_AXI_wready(xbar_to_m03_couplers_WREADY),
        .S_AXI_wstrb(xbar_to_m03_couplers_WSTRB),
        .S_AXI_wvalid(xbar_to_m03_couplers_WVALID));
  m04_couplers_imp_1JS4TBI m04_couplers
       (.M_ACLK(M04_ACLK_1),
        .M_ARESETN(M04_ARESETN_1),
        .M_AXI_araddr(m04_couplers_to_processing_system7_0_axi_periph_ARADDR),
        .M_AXI_arready(m04_couplers_to_processing_system7_0_axi_periph_ARREADY),
        .M_AXI_arvalid(m04_couplers_to_processing_system7_0_axi_periph_ARVALID),
        .M_AXI_awaddr(m04_couplers_to_processing_system7_0_axi_periph_AWADDR),
        .M_AXI_awready(m04_couplers_to_processing_system7_0_axi_periph_AWREADY),
        .M_AXI_awvalid(m04_couplers_to_processing_system7_0_axi_periph_AWVALID),
        .M_AXI_bready(m04_couplers_to_processing_system7_0_axi_periph_BREADY),
        .M_AXI_bresp(m04_couplers_to_processing_system7_0_axi_periph_BRESP),
        .M_AXI_bvalid(m04_couplers_to_processing_system7_0_axi_periph_BVALID),
        .M_AXI_rdata(m04_couplers_to_processing_system7_0_axi_periph_RDATA),
        .M_AXI_rready(m04_couplers_to_processing_system7_0_axi_periph_RREADY),
        .M_AXI_rresp(m04_couplers_to_processing_system7_0_axi_periph_RRESP),
        .M_AXI_rvalid(m04_couplers_to_processing_system7_0_axi_periph_RVALID),
        .M_AXI_wdata(m04_couplers_to_processing_system7_0_axi_periph_WDATA),
        .M_AXI_wready(m04_couplers_to_processing_system7_0_axi_periph_WREADY),
        .M_AXI_wstrb(m04_couplers_to_processing_system7_0_axi_periph_WSTRB),
        .M_AXI_wvalid(m04_couplers_to_processing_system7_0_axi_periph_WVALID),
        .S_ACLK(processing_system7_0_axi_periph_ACLK_net),
        .S_ARESETN(processing_system7_0_axi_periph_ARESETN_net),
        .S_AXI_araddr(xbar_to_m04_couplers_ARADDR),
        .S_AXI_arready(xbar_to_m04_couplers_ARREADY),
        .S_AXI_arvalid(xbar_to_m04_couplers_ARVALID),
        .S_AXI_awaddr(xbar_to_m04_couplers_AWADDR),
        .S_AXI_awready(xbar_to_m04_couplers_AWREADY),
        .S_AXI_awvalid(xbar_to_m04_couplers_AWVALID),
        .S_AXI_bready(xbar_to_m04_couplers_BREADY),
        .S_AXI_bresp(xbar_to_m04_couplers_BRESP),
        .S_AXI_bvalid(xbar_to_m04_couplers_BVALID),
        .S_AXI_rdata(xbar_to_m04_couplers_RDATA),
        .S_AXI_rready(xbar_to_m04_couplers_RREADY),
        .S_AXI_rresp(xbar_to_m04_couplers_RRESP),
        .S_AXI_rvalid(xbar_to_m04_couplers_RVALID),
        .S_AXI_wdata(xbar_to_m04_couplers_WDATA),
        .S_AXI_wready(xbar_to_m04_couplers_WREADY),
        .S_AXI_wstrb(xbar_to_m04_couplers_WSTRB),
        .S_AXI_wvalid(xbar_to_m04_couplers_WVALID));
  m05_couplers_imp_CSEVP5 m05_couplers
       (.M_ACLK(M05_ACLK_1),
        .M_ARESETN(M05_ARESETN_1),
        .M_AXI_araddr(m05_couplers_to_processing_system7_0_axi_periph_ARADDR),
        .M_AXI_arready(m05_couplers_to_processing_system7_0_axi_periph_ARREADY),
        .M_AXI_arvalid(m05_couplers_to_processing_system7_0_axi_periph_ARVALID),
        .M_AXI_awaddr(m05_couplers_to_processing_system7_0_axi_periph_AWADDR),
        .M_AXI_awready(m05_couplers_to_processing_system7_0_axi_periph_AWREADY),
        .M_AXI_awvalid(m05_couplers_to_processing_system7_0_axi_periph_AWVALID),
        .M_AXI_bready(m05_couplers_to_processing_system7_0_axi_periph_BREADY),
        .M_AXI_bresp(m05_couplers_to_processing_system7_0_axi_periph_BRESP),
        .M_AXI_bvalid(m05_couplers_to_processing_system7_0_axi_periph_BVALID),
        .M_AXI_rdata(m05_couplers_to_processing_system7_0_axi_periph_RDATA),
        .M_AXI_rready(m05_couplers_to_processing_system7_0_axi_periph_RREADY),
        .M_AXI_rresp(m05_couplers_to_processing_system7_0_axi_periph_RRESP),
        .M_AXI_rvalid(m05_couplers_to_processing_system7_0_axi_periph_RVALID),
        .M_AXI_wdata(m05_couplers_to_processing_system7_0_axi_periph_WDATA),
        .M_AXI_wready(m05_couplers_to_processing_system7_0_axi_periph_WREADY),
        .M_AXI_wstrb(m05_couplers_to_processing_system7_0_axi_periph_WSTRB),
        .M_AXI_wvalid(m05_couplers_to_processing_system7_0_axi_periph_WVALID),
        .S_ACLK(processing_system7_0_axi_periph_ACLK_net),
        .S_ARESETN(processing_system7_0_axi_periph_ARESETN_net),
        .S_AXI_araddr(xbar_to_m05_couplers_ARADDR),
        .S_AXI_arready(xbar_to_m05_couplers_ARREADY),
        .S_AXI_arvalid(xbar_to_m05_couplers_ARVALID),
        .S_AXI_awaddr(xbar_to_m05_couplers_AWADDR),
        .S_AXI_awready(xbar_to_m05_couplers_AWREADY),
        .S_AXI_awvalid(xbar_to_m05_couplers_AWVALID),
        .S_AXI_bready(xbar_to_m05_couplers_BREADY),
        .S_AXI_bresp(xbar_to_m05_couplers_BRESP),
        .S_AXI_bvalid(xbar_to_m05_couplers_BVALID),
        .S_AXI_rdata(xbar_to_m05_couplers_RDATA),
        .S_AXI_rready(xbar_to_m05_couplers_RREADY),
        .S_AXI_rresp(xbar_to_m05_couplers_RRESP),
        .S_AXI_rvalid(xbar_to_m05_couplers_RVALID),
        .S_AXI_wdata(xbar_to_m05_couplers_WDATA),
        .S_AXI_wready(xbar_to_m05_couplers_WREADY),
        .S_AXI_wstrb(xbar_to_m05_couplers_WSTRB),
        .S_AXI_wvalid(xbar_to_m05_couplers_WVALID));
  m06_couplers_imp_1P40NB5 m06_couplers
       (.M_ACLK(M06_ACLK_1),
        .M_ARESETN(M06_ARESETN_1),
        .M_AXI_araddr(m06_couplers_to_processing_system7_0_axi_periph_ARADDR),
        .M_AXI_arready(m06_couplers_to_processing_system7_0_axi_periph_ARREADY),
        .M_AXI_arvalid(m06_couplers_to_processing_system7_0_axi_periph_ARVALID),
        .M_AXI_awaddr(m06_couplers_to_processing_system7_0_axi_periph_AWADDR),
        .M_AXI_awready(m06_couplers_to_processing_system7_0_axi_periph_AWREADY),
        .M_AXI_awvalid(m06_couplers_to_processing_system7_0_axi_periph_AWVALID),
        .M_AXI_bready(m06_couplers_to_processing_system7_0_axi_periph_BREADY),
        .M_AXI_bresp(m06_couplers_to_processing_system7_0_axi_periph_BRESP),
        .M_AXI_bvalid(m06_couplers_to_processing_system7_0_axi_periph_BVALID),
        .M_AXI_rdata(m06_couplers_to_processing_system7_0_axi_periph_RDATA),
        .M_AXI_rready(m06_couplers_to_processing_system7_0_axi_periph_RREADY),
        .M_AXI_rresp(m06_couplers_to_processing_system7_0_axi_periph_RRESP),
        .M_AXI_rvalid(m06_couplers_to_processing_system7_0_axi_periph_RVALID),
        .M_AXI_wdata(m06_couplers_to_processing_system7_0_axi_periph_WDATA),
        .M_AXI_wready(m06_couplers_to_processing_system7_0_axi_periph_WREADY),
        .M_AXI_wstrb(m06_couplers_to_processing_system7_0_axi_periph_WSTRB),
        .M_AXI_wvalid(m06_couplers_to_processing_system7_0_axi_periph_WVALID),
        .S_ACLK(processing_system7_0_axi_periph_ACLK_net),
        .S_ARESETN(processing_system7_0_axi_periph_ARESETN_net),
        .S_AXI_araddr(xbar_to_m06_couplers_ARADDR),
        .S_AXI_arready(xbar_to_m06_couplers_ARREADY),
        .S_AXI_arvalid(xbar_to_m06_couplers_ARVALID),
        .S_AXI_awaddr(xbar_to_m06_couplers_AWADDR),
        .S_AXI_awready(xbar_to_m06_couplers_AWREADY),
        .S_AXI_awvalid(xbar_to_m06_couplers_AWVALID),
        .S_AXI_bready(xbar_to_m06_couplers_BREADY),
        .S_AXI_bresp(xbar_to_m06_couplers_BRESP),
        .S_AXI_bvalid(xbar_to_m06_couplers_BVALID),
        .S_AXI_rdata(xbar_to_m06_couplers_RDATA),
        .S_AXI_rready(xbar_to_m06_couplers_RREADY),
        .S_AXI_rresp(xbar_to_m06_couplers_RRESP),
        .S_AXI_rvalid(xbar_to_m06_couplers_RVALID),
        .S_AXI_wdata(xbar_to_m06_couplers_WDATA),
        .S_AXI_wready(xbar_to_m06_couplers_WREADY),
        .S_AXI_wstrb(xbar_to_m06_couplers_WSTRB),
        .S_AXI_wvalid(xbar_to_m06_couplers_WVALID));
  m07_couplers_imp_GGZLP2 m07_couplers
       (.M_ACLK(M07_ACLK_1),
        .M_ARESETN(M07_ARESETN_1),
        .M_AXI_araddr(m07_couplers_to_processing_system7_0_axi_periph_ARADDR),
        .M_AXI_arready(m07_couplers_to_processing_system7_0_axi_periph_ARREADY),
        .M_AXI_arvalid(m07_couplers_to_processing_system7_0_axi_periph_ARVALID),
        .M_AXI_awaddr(m07_couplers_to_processing_system7_0_axi_periph_AWADDR),
        .M_AXI_awready(m07_couplers_to_processing_system7_0_axi_periph_AWREADY),
        .M_AXI_awvalid(m07_couplers_to_processing_system7_0_axi_periph_AWVALID),
        .M_AXI_bready(m07_couplers_to_processing_system7_0_axi_periph_BREADY),
        .M_AXI_bresp(m07_couplers_to_processing_system7_0_axi_periph_BRESP),
        .M_AXI_bvalid(m07_couplers_to_processing_system7_0_axi_periph_BVALID),
        .M_AXI_rdata(m07_couplers_to_processing_system7_0_axi_periph_RDATA),
        .M_AXI_rready(m07_couplers_to_processing_system7_0_axi_periph_RREADY),
        .M_AXI_rresp(m07_couplers_to_processing_system7_0_axi_periph_RRESP),
        .M_AXI_rvalid(m07_couplers_to_processing_system7_0_axi_periph_RVALID),
        .M_AXI_wdata(m07_couplers_to_processing_system7_0_axi_periph_WDATA),
        .M_AXI_wready(m07_couplers_to_processing_system7_0_axi_periph_WREADY),
        .M_AXI_wstrb(m07_couplers_to_processing_system7_0_axi_periph_WSTRB),
        .M_AXI_wvalid(m07_couplers_to_processing_system7_0_axi_periph_WVALID),
        .S_ACLK(processing_system7_0_axi_periph_ACLK_net),
        .S_ARESETN(processing_system7_0_axi_periph_ARESETN_net),
        .S_AXI_araddr(xbar_to_m07_couplers_ARADDR),
        .S_AXI_arready(xbar_to_m07_couplers_ARREADY),
        .S_AXI_arvalid(xbar_to_m07_couplers_ARVALID),
        .S_AXI_awaddr(xbar_to_m07_couplers_AWADDR),
        .S_AXI_awready(xbar_to_m07_couplers_AWREADY),
        .S_AXI_awvalid(xbar_to_m07_couplers_AWVALID),
        .S_AXI_bready(xbar_to_m07_couplers_BREADY),
        .S_AXI_bresp(xbar_to_m07_couplers_BRESP),
        .S_AXI_bvalid(xbar_to_m07_couplers_BVALID),
        .S_AXI_rdata(xbar_to_m07_couplers_RDATA),
        .S_AXI_rready(xbar_to_m07_couplers_RREADY),
        .S_AXI_rresp(xbar_to_m07_couplers_RRESP),
        .S_AXI_rvalid(xbar_to_m07_couplers_RVALID),
        .S_AXI_wdata(xbar_to_m07_couplers_WDATA),
        .S_AXI_wready(xbar_to_m07_couplers_WREADY),
        .S_AXI_wstrb(xbar_to_m07_couplers_WSTRB),
        .S_AXI_wvalid(xbar_to_m07_couplers_WVALID));
  m08_couplers_imp_1EP4JRG m08_couplers
       (.M_ACLK(M08_ACLK_1),
        .M_ARESETN(M08_ARESETN_1),
        .M_AXI_araddr(m08_couplers_to_processing_system7_0_axi_periph_ARADDR),
        .M_AXI_arprot(m08_couplers_to_processing_system7_0_axi_periph_ARPROT),
        .M_AXI_arready(m08_couplers_to_processing_system7_0_axi_periph_ARREADY),
        .M_AXI_arvalid(m08_couplers_to_processing_system7_0_axi_periph_ARVALID),
        .M_AXI_awaddr(m08_couplers_to_processing_system7_0_axi_periph_AWADDR),
        .M_AXI_awprot(m08_couplers_to_processing_system7_0_axi_periph_AWPROT),
        .M_AXI_awready(m08_couplers_to_processing_system7_0_axi_periph_AWREADY),
        .M_AXI_awvalid(m08_couplers_to_processing_system7_0_axi_periph_AWVALID),
        .M_AXI_bready(m08_couplers_to_processing_system7_0_axi_periph_BREADY),
        .M_AXI_bresp(m08_couplers_to_processing_system7_0_axi_periph_BRESP),
        .M_AXI_bvalid(m08_couplers_to_processing_system7_0_axi_periph_BVALID),
        .M_AXI_rdata(m08_couplers_to_processing_system7_0_axi_periph_RDATA),
        .M_AXI_rready(m08_couplers_to_processing_system7_0_axi_periph_RREADY),
        .M_AXI_rresp(m08_couplers_to_processing_system7_0_axi_periph_RRESP),
        .M_AXI_rvalid(m08_couplers_to_processing_system7_0_axi_periph_RVALID),
        .M_AXI_wdata(m08_couplers_to_processing_system7_0_axi_periph_WDATA),
        .M_AXI_wready(m08_couplers_to_processing_system7_0_axi_periph_WREADY),
        .M_AXI_wstrb(m08_couplers_to_processing_system7_0_axi_periph_WSTRB),
        .M_AXI_wvalid(m08_couplers_to_processing_system7_0_axi_periph_WVALID),
        .S_ACLK(processing_system7_0_axi_periph_ACLK_net),
        .S_ARESETN(processing_system7_0_axi_periph_ARESETN_net),
        .S_AXI_araddr(xbar_to_m08_couplers_ARADDR),
        .S_AXI_arprot(xbar_to_m08_couplers_ARPROT),
        .S_AXI_arready(xbar_to_m08_couplers_ARREADY),
        .S_AXI_arvalid(xbar_to_m08_couplers_ARVALID),
        .S_AXI_awaddr(xbar_to_m08_couplers_AWADDR),
        .S_AXI_awprot(xbar_to_m08_couplers_AWPROT),
        .S_AXI_awready(xbar_to_m08_couplers_AWREADY),
        .S_AXI_awvalid(xbar_to_m08_couplers_AWVALID),
        .S_AXI_bready(xbar_to_m08_couplers_BREADY),
        .S_AXI_bresp(xbar_to_m08_couplers_BRESP),
        .S_AXI_bvalid(xbar_to_m08_couplers_BVALID),
        .S_AXI_rdata(xbar_to_m08_couplers_RDATA),
        .S_AXI_rready(xbar_to_m08_couplers_RREADY),
        .S_AXI_rresp(xbar_to_m08_couplers_RRESP),
        .S_AXI_rvalid(xbar_to_m08_couplers_RVALID),
        .S_AXI_wdata(xbar_to_m08_couplers_WDATA),
        .S_AXI_wready(xbar_to_m08_couplers_WREADY),
        .S_AXI_wstrb(xbar_to_m08_couplers_WSTRB),
        .S_AXI_wvalid(xbar_to_m08_couplers_WVALID));
  m09_couplers_imp_MNQE1N m09_couplers
       (.M_ACLK(M09_ACLK_1),
        .M_ARESETN(M09_ARESETN_1),
        .M_AXI_araddr(m09_couplers_to_processing_system7_0_axi_periph_ARADDR),
        .M_AXI_arready(m09_couplers_to_processing_system7_0_axi_periph_ARREADY),
        .M_AXI_arvalid(m09_couplers_to_processing_system7_0_axi_periph_ARVALID),
        .M_AXI_awaddr(m09_couplers_to_processing_system7_0_axi_periph_AWADDR),
        .M_AXI_awready(m09_couplers_to_processing_system7_0_axi_periph_AWREADY),
        .M_AXI_awvalid(m09_couplers_to_processing_system7_0_axi_periph_AWVALID),
        .M_AXI_bready(m09_couplers_to_processing_system7_0_axi_periph_BREADY),
        .M_AXI_bresp(m09_couplers_to_processing_system7_0_axi_periph_BRESP),
        .M_AXI_bvalid(m09_couplers_to_processing_system7_0_axi_periph_BVALID),
        .M_AXI_rdata(m09_couplers_to_processing_system7_0_axi_periph_RDATA),
        .M_AXI_rready(m09_couplers_to_processing_system7_0_axi_periph_RREADY),
        .M_AXI_rresp(m09_couplers_to_processing_system7_0_axi_periph_RRESP),
        .M_AXI_rvalid(m09_couplers_to_processing_system7_0_axi_periph_RVALID),
        .M_AXI_wdata(m09_couplers_to_processing_system7_0_axi_periph_WDATA),
        .M_AXI_wready(m09_couplers_to_processing_system7_0_axi_periph_WREADY),
        .M_AXI_wstrb(m09_couplers_to_processing_system7_0_axi_periph_WSTRB),
        .M_AXI_wvalid(m09_couplers_to_processing_system7_0_axi_periph_WVALID),
        .S_ACLK(processing_system7_0_axi_periph_ACLK_net),
        .S_ARESETN(processing_system7_0_axi_periph_ARESETN_net),
        .S_AXI_araddr(xbar_to_m09_couplers_ARADDR),
        .S_AXI_arready(xbar_to_m09_couplers_ARREADY),
        .S_AXI_arvalid(xbar_to_m09_couplers_ARVALID),
        .S_AXI_awaddr(xbar_to_m09_couplers_AWADDR),
        .S_AXI_awready(xbar_to_m09_couplers_AWREADY),
        .S_AXI_awvalid(xbar_to_m09_couplers_AWVALID),
        .S_AXI_bready(xbar_to_m09_couplers_BREADY),
        .S_AXI_bresp(xbar_to_m09_couplers_BRESP),
        .S_AXI_bvalid(xbar_to_m09_couplers_BVALID),
        .S_AXI_rdata(xbar_to_m09_couplers_RDATA),
        .S_AXI_rready(xbar_to_m09_couplers_RREADY),
        .S_AXI_rresp(xbar_to_m09_couplers_RRESP),
        .S_AXI_rvalid(xbar_to_m09_couplers_RVALID),
        .S_AXI_wdata(xbar_to_m09_couplers_WDATA),
        .S_AXI_wready(xbar_to_m09_couplers_WREADY),
        .S_AXI_wstrb(xbar_to_m09_couplers_WSTRB),
        .S_AXI_wvalid(xbar_to_m09_couplers_WVALID));
  m10_couplers_imp_13554VL m10_couplers
       (.M_ACLK(M10_ACLK_1),
        .M_ARESETN(M10_ARESETN_1),
        .M_AXI_araddr(m10_couplers_to_processing_system7_0_axi_periph_ARADDR),
        .M_AXI_arready(m10_couplers_to_processing_system7_0_axi_periph_ARREADY),
        .M_AXI_arvalid(m10_couplers_to_processing_system7_0_axi_periph_ARVALID),
        .M_AXI_awaddr(m10_couplers_to_processing_system7_0_axi_periph_AWADDR),
        .M_AXI_awready(m10_couplers_to_processing_system7_0_axi_periph_AWREADY),
        .M_AXI_awvalid(m10_couplers_to_processing_system7_0_axi_periph_AWVALID),
        .M_AXI_bready(m10_couplers_to_processing_system7_0_axi_periph_BREADY),
        .M_AXI_bresp(m10_couplers_to_processing_system7_0_axi_periph_BRESP),
        .M_AXI_bvalid(m10_couplers_to_processing_system7_0_axi_periph_BVALID),
        .M_AXI_rdata(m10_couplers_to_processing_system7_0_axi_periph_RDATA),
        .M_AXI_rready(m10_couplers_to_processing_system7_0_axi_periph_RREADY),
        .M_AXI_rresp(m10_couplers_to_processing_system7_0_axi_periph_RRESP),
        .M_AXI_rvalid(m10_couplers_to_processing_system7_0_axi_periph_RVALID),
        .M_AXI_wdata(m10_couplers_to_processing_system7_0_axi_periph_WDATA),
        .M_AXI_wready(m10_couplers_to_processing_system7_0_axi_periph_WREADY),
        .M_AXI_wvalid(m10_couplers_to_processing_system7_0_axi_periph_WVALID),
        .S_ACLK(processing_system7_0_axi_periph_ACLK_net),
        .S_ARESETN(processing_system7_0_axi_periph_ARESETN_net),
        .S_AXI_araddr(xbar_to_m10_couplers_ARADDR),
        .S_AXI_arready(xbar_to_m10_couplers_ARREADY),
        .S_AXI_arvalid(xbar_to_m10_couplers_ARVALID),
        .S_AXI_awaddr(xbar_to_m10_couplers_AWADDR),
        .S_AXI_awready(xbar_to_m10_couplers_AWREADY),
        .S_AXI_awvalid(xbar_to_m10_couplers_AWVALID),
        .S_AXI_bready(xbar_to_m10_couplers_BREADY),
        .S_AXI_bresp(xbar_to_m10_couplers_BRESP),
        .S_AXI_bvalid(xbar_to_m10_couplers_BVALID),
        .S_AXI_rdata(xbar_to_m10_couplers_RDATA),
        .S_AXI_rready(xbar_to_m10_couplers_RREADY),
        .S_AXI_rresp(xbar_to_m10_couplers_RRESP),
        .S_AXI_rvalid(xbar_to_m10_couplers_RVALID),
        .S_AXI_wdata(xbar_to_m10_couplers_WDATA),
        .S_AXI_wready(xbar_to_m10_couplers_WREADY),
        .S_AXI_wvalid(xbar_to_m10_couplers_WVALID));
  m11_couplers_imp_TFEMW6 m11_couplers
       (.M_ACLK(M11_ACLK_1),
        .M_ARESETN(M11_ARESETN_1),
        .M_AXI_araddr(m11_couplers_to_processing_system7_0_axi_periph_ARADDR),
        .M_AXI_arready(m11_couplers_to_processing_system7_0_axi_periph_ARREADY),
        .M_AXI_arvalid(m11_couplers_to_processing_system7_0_axi_periph_ARVALID),
        .M_AXI_awaddr(m11_couplers_to_processing_system7_0_axi_periph_AWADDR),
        .M_AXI_awready(m11_couplers_to_processing_system7_0_axi_periph_AWREADY),
        .M_AXI_awvalid(m11_couplers_to_processing_system7_0_axi_periph_AWVALID),
        .M_AXI_bready(m11_couplers_to_processing_system7_0_axi_periph_BREADY),
        .M_AXI_bresp(m11_couplers_to_processing_system7_0_axi_periph_BRESP),
        .M_AXI_bvalid(m11_couplers_to_processing_system7_0_axi_periph_BVALID),
        .M_AXI_rdata(m11_couplers_to_processing_system7_0_axi_periph_RDATA),
        .M_AXI_rready(m11_couplers_to_processing_system7_0_axi_periph_RREADY),
        .M_AXI_rresp(m11_couplers_to_processing_system7_0_axi_periph_RRESP),
        .M_AXI_rvalid(m11_couplers_to_processing_system7_0_axi_periph_RVALID),
        .M_AXI_wdata(m11_couplers_to_processing_system7_0_axi_periph_WDATA),
        .M_AXI_wready(m11_couplers_to_processing_system7_0_axi_periph_WREADY),
        .M_AXI_wstrb(m11_couplers_to_processing_system7_0_axi_periph_WSTRB),
        .M_AXI_wvalid(m11_couplers_to_processing_system7_0_axi_periph_WVALID),
        .S_ACLK(processing_system7_0_axi_periph_ACLK_net),
        .S_ARESETN(processing_system7_0_axi_periph_ARESETN_net),
        .S_AXI_araddr(xbar_to_m11_couplers_ARADDR),
        .S_AXI_arready(xbar_to_m11_couplers_ARREADY),
        .S_AXI_arvalid(xbar_to_m11_couplers_ARVALID),
        .S_AXI_awaddr(xbar_to_m11_couplers_AWADDR),
        .S_AXI_awready(xbar_to_m11_couplers_AWREADY),
        .S_AXI_awvalid(xbar_to_m11_couplers_AWVALID),
        .S_AXI_bready(xbar_to_m11_couplers_BREADY),
        .S_AXI_bresp(xbar_to_m11_couplers_BRESP),
        .S_AXI_bvalid(xbar_to_m11_couplers_BVALID),
        .S_AXI_rdata(xbar_to_m11_couplers_RDATA),
        .S_AXI_rready(xbar_to_m11_couplers_RREADY),
        .S_AXI_rresp(xbar_to_m11_couplers_RRESP),
        .S_AXI_rvalid(xbar_to_m11_couplers_RVALID),
        .S_AXI_wdata(xbar_to_m11_couplers_WDATA),
        .S_AXI_wready(xbar_to_m11_couplers_WREADY),
        .S_AXI_wstrb(xbar_to_m11_couplers_WSTRB),
        .S_AXI_wvalid(xbar_to_m11_couplers_WVALID));
  m12_couplers_imp_169QM5A m12_couplers
       (.M_ACLK(M12_ACLK_1),
        .M_ARESETN(M12_ARESETN_1),
        .M_AXI_araddr(m12_couplers_to_processing_system7_0_axi_periph_ARADDR),
        .M_AXI_arready(m12_couplers_to_processing_system7_0_axi_periph_ARREADY),
        .M_AXI_arvalid(m12_couplers_to_processing_system7_0_axi_periph_ARVALID),
        .M_AXI_awaddr(m12_couplers_to_processing_system7_0_axi_periph_AWADDR),
        .M_AXI_awready(m12_couplers_to_processing_system7_0_axi_periph_AWREADY),
        .M_AXI_awvalid(m12_couplers_to_processing_system7_0_axi_periph_AWVALID),
        .M_AXI_bready(m12_couplers_to_processing_system7_0_axi_periph_BREADY),
        .M_AXI_bresp(m12_couplers_to_processing_system7_0_axi_periph_BRESP),
        .M_AXI_bvalid(m12_couplers_to_processing_system7_0_axi_periph_BVALID),
        .M_AXI_rdata(m12_couplers_to_processing_system7_0_axi_periph_RDATA),
        .M_AXI_rready(m12_couplers_to_processing_system7_0_axi_periph_RREADY),
        .M_AXI_rresp(m12_couplers_to_processing_system7_0_axi_periph_RRESP),
        .M_AXI_rvalid(m12_couplers_to_processing_system7_0_axi_periph_RVALID),
        .M_AXI_wdata(m12_couplers_to_processing_system7_0_axi_periph_WDATA),
        .M_AXI_wready(m12_couplers_to_processing_system7_0_axi_periph_WREADY),
        .M_AXI_wstrb(m12_couplers_to_processing_system7_0_axi_periph_WSTRB),
        .M_AXI_wvalid(m12_couplers_to_processing_system7_0_axi_periph_WVALID),
        .S_ACLK(processing_system7_0_axi_periph_ACLK_net),
        .S_ARESETN(processing_system7_0_axi_periph_ARESETN_net),
        .S_AXI_araddr(xbar_to_m12_couplers_ARADDR),
        .S_AXI_arready(xbar_to_m12_couplers_ARREADY),
        .S_AXI_arvalid(xbar_to_m12_couplers_ARVALID),
        .S_AXI_awaddr(xbar_to_m12_couplers_AWADDR),
        .S_AXI_awready(xbar_to_m12_couplers_AWREADY),
        .S_AXI_awvalid(xbar_to_m12_couplers_AWVALID),
        .S_AXI_bready(xbar_to_m12_couplers_BREADY),
        .S_AXI_bresp(xbar_to_m12_couplers_BRESP),
        .S_AXI_bvalid(xbar_to_m12_couplers_BVALID),
        .S_AXI_rdata(xbar_to_m12_couplers_RDATA),
        .S_AXI_rready(xbar_to_m12_couplers_RREADY),
        .S_AXI_rresp(xbar_to_m12_couplers_RRESP),
        .S_AXI_rvalid(xbar_to_m12_couplers_RVALID),
        .S_AXI_wdata(xbar_to_m12_couplers_WDATA),
        .S_AXI_wready(xbar_to_m12_couplers_WREADY),
        .S_AXI_wstrb(xbar_to_m12_couplers_WSTRB),
        .S_AXI_wvalid(xbar_to_m12_couplers_WVALID));
  s00_couplers_imp_1UTISAU s00_couplers
       (.M_ACLK(processing_system7_0_axi_periph_ACLK_net),
        .M_ARESETN(processing_system7_0_axi_periph_ARESETN_net),
        .M_AXI_araddr(s00_couplers_to_xbar_ARADDR),
        .M_AXI_arprot(s00_couplers_to_xbar_ARPROT),
        .M_AXI_arready(s00_couplers_to_xbar_ARREADY),
        .M_AXI_arvalid(s00_couplers_to_xbar_ARVALID),
        .M_AXI_awaddr(s00_couplers_to_xbar_AWADDR),
        .M_AXI_awprot(s00_couplers_to_xbar_AWPROT),
        .M_AXI_awready(s00_couplers_to_xbar_AWREADY),
        .M_AXI_awvalid(s00_couplers_to_xbar_AWVALID),
        .M_AXI_bready(s00_couplers_to_xbar_BREADY),
        .M_AXI_bresp(s00_couplers_to_xbar_BRESP),
        .M_AXI_bvalid(s00_couplers_to_xbar_BVALID),
        .M_AXI_rdata(s00_couplers_to_xbar_RDATA),
        .M_AXI_rready(s00_couplers_to_xbar_RREADY),
        .M_AXI_rresp(s00_couplers_to_xbar_RRESP),
        .M_AXI_rvalid(s00_couplers_to_xbar_RVALID),
        .M_AXI_wdata(s00_couplers_to_xbar_WDATA),
        .M_AXI_wready(s00_couplers_to_xbar_WREADY),
        .M_AXI_wstrb(s00_couplers_to_xbar_WSTRB),
        .M_AXI_wvalid(s00_couplers_to_xbar_WVALID),
        .S_ACLK(S00_ACLK_1),
        .S_ARESETN(S00_ARESETN_1),
        .S_AXI_araddr(processing_system7_0_axi_periph_to_s00_couplers_ARADDR),
        .S_AXI_arburst(processing_system7_0_axi_periph_to_s00_couplers_ARBURST),
        .S_AXI_arcache(processing_system7_0_axi_periph_to_s00_couplers_ARCACHE),
        .S_AXI_arid(processing_system7_0_axi_periph_to_s00_couplers_ARID),
        .S_AXI_arlen(processing_system7_0_axi_periph_to_s00_couplers_ARLEN),
        .S_AXI_arlock(processing_system7_0_axi_periph_to_s00_couplers_ARLOCK),
        .S_AXI_arprot(processing_system7_0_axi_periph_to_s00_couplers_ARPROT),
        .S_AXI_arqos(processing_system7_0_axi_periph_to_s00_couplers_ARQOS),
        .S_AXI_arready(processing_system7_0_axi_periph_to_s00_couplers_ARREADY),
        .S_AXI_arsize(processing_system7_0_axi_periph_to_s00_couplers_ARSIZE),
        .S_AXI_arvalid(processing_system7_0_axi_periph_to_s00_couplers_ARVALID),
        .S_AXI_awaddr(processing_system7_0_axi_periph_to_s00_couplers_AWADDR),
        .S_AXI_awburst(processing_system7_0_axi_periph_to_s00_couplers_AWBURST),
        .S_AXI_awcache(processing_system7_0_axi_periph_to_s00_couplers_AWCACHE),
        .S_AXI_awid(processing_system7_0_axi_periph_to_s00_couplers_AWID),
        .S_AXI_awlen(processing_system7_0_axi_periph_to_s00_couplers_AWLEN),
        .S_AXI_awlock(processing_system7_0_axi_periph_to_s00_couplers_AWLOCK),
        .S_AXI_awprot(processing_system7_0_axi_periph_to_s00_couplers_AWPROT),
        .S_AXI_awqos(processing_system7_0_axi_periph_to_s00_couplers_AWQOS),
        .S_AXI_awready(processing_system7_0_axi_periph_to_s00_couplers_AWREADY),
        .S_AXI_awsize(processing_system7_0_axi_periph_to_s00_couplers_AWSIZE),
        .S_AXI_awvalid(processing_system7_0_axi_periph_to_s00_couplers_AWVALID),
        .S_AXI_bid(processing_system7_0_axi_periph_to_s00_couplers_BID),
        .S_AXI_bready(processing_system7_0_axi_periph_to_s00_couplers_BREADY),
        .S_AXI_bresp(processing_system7_0_axi_periph_to_s00_couplers_BRESP),
        .S_AXI_bvalid(processing_system7_0_axi_periph_to_s00_couplers_BVALID),
        .S_AXI_rdata(processing_system7_0_axi_periph_to_s00_couplers_RDATA),
        .S_AXI_rid(processing_system7_0_axi_periph_to_s00_couplers_RID),
        .S_AXI_rlast(processing_system7_0_axi_periph_to_s00_couplers_RLAST),
        .S_AXI_rready(processing_system7_0_axi_periph_to_s00_couplers_RREADY),
        .S_AXI_rresp(processing_system7_0_axi_periph_to_s00_couplers_RRESP),
        .S_AXI_rvalid(processing_system7_0_axi_periph_to_s00_couplers_RVALID),
        .S_AXI_wdata(processing_system7_0_axi_periph_to_s00_couplers_WDATA),
        .S_AXI_wid(processing_system7_0_axi_periph_to_s00_couplers_WID),
        .S_AXI_wlast(processing_system7_0_axi_periph_to_s00_couplers_WLAST),
        .S_AXI_wready(processing_system7_0_axi_periph_to_s00_couplers_WREADY),
        .S_AXI_wstrb(processing_system7_0_axi_periph_to_s00_couplers_WSTRB),
        .S_AXI_wvalid(processing_system7_0_axi_periph_to_s00_couplers_WVALID));
  base_zynq_design_xbar_0 xbar
       (.aclk(processing_system7_0_axi_periph_ACLK_net),
        .aresetn(processing_system7_0_axi_periph_ARESETN_net),
        .m_axi_araddr({xbar_to_m12_couplers_ARADDR,xbar_to_m11_couplers_ARADDR,xbar_to_m10_couplers_ARADDR,xbar_to_m09_couplers_ARADDR,xbar_to_m08_couplers_ARADDR,xbar_to_m07_couplers_ARADDR,xbar_to_m06_couplers_ARADDR,xbar_to_m05_couplers_ARADDR,xbar_to_m04_couplers_ARADDR,xbar_to_m03_couplers_ARADDR,xbar_to_m02_couplers_ARADDR,xbar_to_m01_couplers_ARADDR,xbar_to_m00_couplers_ARADDR}),
        .m_axi_arprot({xbar_to_m08_couplers_ARPROT,NLW_xbar_m_axi_arprot_UNCONNECTED[23:12],xbar_to_m03_couplers_ARPROT,NLW_xbar_m_axi_arprot_UNCONNECTED[8:0]}),
        .m_axi_arready({xbar_to_m12_couplers_ARREADY,xbar_to_m11_couplers_ARREADY,xbar_to_m10_couplers_ARREADY,xbar_to_m09_couplers_ARREADY,xbar_to_m08_couplers_ARREADY,xbar_to_m07_couplers_ARREADY,xbar_to_m06_couplers_ARREADY,xbar_to_m05_couplers_ARREADY,xbar_to_m04_couplers_ARREADY,xbar_to_m03_couplers_ARREADY,xbar_to_m02_couplers_ARREADY,xbar_to_m01_couplers_ARREADY,xbar_to_m00_couplers_ARREADY}),
        .m_axi_arvalid({xbar_to_m12_couplers_ARVALID,xbar_to_m11_couplers_ARVALID,xbar_to_m10_couplers_ARVALID,xbar_to_m09_couplers_ARVALID,xbar_to_m08_couplers_ARVALID,xbar_to_m07_couplers_ARVALID,xbar_to_m06_couplers_ARVALID,xbar_to_m05_couplers_ARVALID,xbar_to_m04_couplers_ARVALID,xbar_to_m03_couplers_ARVALID,xbar_to_m02_couplers_ARVALID,xbar_to_m01_couplers_ARVALID,xbar_to_m00_couplers_ARVALID}),
        .m_axi_awaddr({xbar_to_m12_couplers_AWADDR,xbar_to_m11_couplers_AWADDR,xbar_to_m10_couplers_AWADDR,xbar_to_m09_couplers_AWADDR,xbar_to_m08_couplers_AWADDR,xbar_to_m07_couplers_AWADDR,xbar_to_m06_couplers_AWADDR,xbar_to_m05_couplers_AWADDR,xbar_to_m04_couplers_AWADDR,xbar_to_m03_couplers_AWADDR,xbar_to_m02_couplers_AWADDR,xbar_to_m01_couplers_AWADDR,xbar_to_m00_couplers_AWADDR}),
        .m_axi_awprot({xbar_to_m08_couplers_AWPROT,NLW_xbar_m_axi_awprot_UNCONNECTED[23:12],xbar_to_m03_couplers_AWPROT,NLW_xbar_m_axi_awprot_UNCONNECTED[8:0]}),
        .m_axi_awready({xbar_to_m12_couplers_AWREADY,xbar_to_m11_couplers_AWREADY,xbar_to_m10_couplers_AWREADY,xbar_to_m09_couplers_AWREADY,xbar_to_m08_couplers_AWREADY,xbar_to_m07_couplers_AWREADY,xbar_to_m06_couplers_AWREADY,xbar_to_m05_couplers_AWREADY,xbar_to_m04_couplers_AWREADY,xbar_to_m03_couplers_AWREADY,xbar_to_m02_couplers_AWREADY,xbar_to_m01_couplers_AWREADY,xbar_to_m00_couplers_AWREADY}),
        .m_axi_awvalid({xbar_to_m12_couplers_AWVALID,xbar_to_m11_couplers_AWVALID,xbar_to_m10_couplers_AWVALID,xbar_to_m09_couplers_AWVALID,xbar_to_m08_couplers_AWVALID,xbar_to_m07_couplers_AWVALID,xbar_to_m06_couplers_AWVALID,xbar_to_m05_couplers_AWVALID,xbar_to_m04_couplers_AWVALID,xbar_to_m03_couplers_AWVALID,xbar_to_m02_couplers_AWVALID,xbar_to_m01_couplers_AWVALID,xbar_to_m00_couplers_AWVALID}),
        .m_axi_bready({xbar_to_m12_couplers_BREADY,xbar_to_m11_couplers_BREADY,xbar_to_m10_couplers_BREADY,xbar_to_m09_couplers_BREADY,xbar_to_m08_couplers_BREADY,xbar_to_m07_couplers_BREADY,xbar_to_m06_couplers_BREADY,xbar_to_m05_couplers_BREADY,xbar_to_m04_couplers_BREADY,xbar_to_m03_couplers_BREADY,xbar_to_m02_couplers_BREADY,xbar_to_m01_couplers_BREADY,xbar_to_m00_couplers_BREADY}),
        .m_axi_bresp({xbar_to_m12_couplers_BRESP,xbar_to_m11_couplers_BRESP,xbar_to_m10_couplers_BRESP,xbar_to_m09_couplers_BRESP,xbar_to_m08_couplers_BRESP,xbar_to_m07_couplers_BRESP,xbar_to_m06_couplers_BRESP,xbar_to_m05_couplers_BRESP,xbar_to_m04_couplers_BRESP,xbar_to_m03_couplers_BRESP,xbar_to_m02_couplers_BRESP,xbar_to_m01_couplers_BRESP,xbar_to_m00_couplers_BRESP}),
        .m_axi_bvalid({xbar_to_m12_couplers_BVALID,xbar_to_m11_couplers_BVALID,xbar_to_m10_couplers_BVALID,xbar_to_m09_couplers_BVALID,xbar_to_m08_couplers_BVALID,xbar_to_m07_couplers_BVALID,xbar_to_m06_couplers_BVALID,xbar_to_m05_couplers_BVALID,xbar_to_m04_couplers_BVALID,xbar_to_m03_couplers_BVALID,xbar_to_m02_couplers_BVALID,xbar_to_m01_couplers_BVALID,xbar_to_m00_couplers_BVALID}),
        .m_axi_rdata({xbar_to_m12_couplers_RDATA,xbar_to_m11_couplers_RDATA,xbar_to_m10_couplers_RDATA,xbar_to_m09_couplers_RDATA,xbar_to_m08_couplers_RDATA,xbar_to_m07_couplers_RDATA,xbar_to_m06_couplers_RDATA,xbar_to_m05_couplers_RDATA,xbar_to_m04_couplers_RDATA,xbar_to_m03_couplers_RDATA,xbar_to_m02_couplers_RDATA,xbar_to_m01_couplers_RDATA,xbar_to_m00_couplers_RDATA}),
        .m_axi_rready({xbar_to_m12_couplers_RREADY,xbar_to_m11_couplers_RREADY,xbar_to_m10_couplers_RREADY,xbar_to_m09_couplers_RREADY,xbar_to_m08_couplers_RREADY,xbar_to_m07_couplers_RREADY,xbar_to_m06_couplers_RREADY,xbar_to_m05_couplers_RREADY,xbar_to_m04_couplers_RREADY,xbar_to_m03_couplers_RREADY,xbar_to_m02_couplers_RREADY,xbar_to_m01_couplers_RREADY,xbar_to_m00_couplers_RREADY}),
        .m_axi_rresp({xbar_to_m12_couplers_RRESP,xbar_to_m11_couplers_RRESP,xbar_to_m10_couplers_RRESP,xbar_to_m09_couplers_RRESP,xbar_to_m08_couplers_RRESP,xbar_to_m07_couplers_RRESP,xbar_to_m06_couplers_RRESP,xbar_to_m05_couplers_RRESP,xbar_to_m04_couplers_RRESP,xbar_to_m03_couplers_RRESP,xbar_to_m02_couplers_RRESP,xbar_to_m01_couplers_RRESP,xbar_to_m00_couplers_RRESP}),
        .m_axi_rvalid({xbar_to_m12_couplers_RVALID,xbar_to_m11_couplers_RVALID,xbar_to_m10_couplers_RVALID,xbar_to_m09_couplers_RVALID,xbar_to_m08_couplers_RVALID,xbar_to_m07_couplers_RVALID,xbar_to_m06_couplers_RVALID,xbar_to_m05_couplers_RVALID,xbar_to_m04_couplers_RVALID,xbar_to_m03_couplers_RVALID,xbar_to_m02_couplers_RVALID,xbar_to_m01_couplers_RVALID,xbar_to_m00_couplers_RVALID}),
        .m_axi_wdata({xbar_to_m12_couplers_WDATA,xbar_to_m11_couplers_WDATA,xbar_to_m10_couplers_WDATA,xbar_to_m09_couplers_WDATA,xbar_to_m08_couplers_WDATA,xbar_to_m07_couplers_WDATA,xbar_to_m06_couplers_WDATA,xbar_to_m05_couplers_WDATA,xbar_to_m04_couplers_WDATA,xbar_to_m03_couplers_WDATA,xbar_to_m02_couplers_WDATA,xbar_to_m01_couplers_WDATA,xbar_to_m00_couplers_WDATA}),
        .m_axi_wready({xbar_to_m12_couplers_WREADY,xbar_to_m11_couplers_WREADY,xbar_to_m10_couplers_WREADY,xbar_to_m09_couplers_WREADY,xbar_to_m08_couplers_WREADY,xbar_to_m07_couplers_WREADY,xbar_to_m06_couplers_WREADY,xbar_to_m05_couplers_WREADY,xbar_to_m04_couplers_WREADY,xbar_to_m03_couplers_WREADY,xbar_to_m02_couplers_WREADY,xbar_to_m01_couplers_WREADY,xbar_to_m00_couplers_WREADY}),
        .m_axi_wstrb({xbar_to_m12_couplers_WSTRB,xbar_to_m11_couplers_WSTRB,NLW_xbar_m_axi_wstrb_UNCONNECTED[43:40],xbar_to_m09_couplers_WSTRB,xbar_to_m08_couplers_WSTRB,xbar_to_m07_couplers_WSTRB,xbar_to_m06_couplers_WSTRB,xbar_to_m05_couplers_WSTRB,xbar_to_m04_couplers_WSTRB,xbar_to_m03_couplers_WSTRB,xbar_to_m02_couplers_WSTRB,xbar_to_m01_couplers_WSTRB,xbar_to_m00_couplers_WSTRB}),
        .m_axi_wvalid({xbar_to_m12_couplers_WVALID,xbar_to_m11_couplers_WVALID,xbar_to_m10_couplers_WVALID,xbar_to_m09_couplers_WVALID,xbar_to_m08_couplers_WVALID,xbar_to_m07_couplers_WVALID,xbar_to_m06_couplers_WVALID,xbar_to_m05_couplers_WVALID,xbar_to_m04_couplers_WVALID,xbar_to_m03_couplers_WVALID,xbar_to_m02_couplers_WVALID,xbar_to_m01_couplers_WVALID,xbar_to_m00_couplers_WVALID}),
        .s_axi_araddr(s00_couplers_to_xbar_ARADDR),
        .s_axi_arprot(s00_couplers_to_xbar_ARPROT),
        .s_axi_arready(s00_couplers_to_xbar_ARREADY),
        .s_axi_arvalid(s00_couplers_to_xbar_ARVALID),
        .s_axi_awaddr(s00_couplers_to_xbar_AWADDR),
        .s_axi_awprot(s00_couplers_to_xbar_AWPROT),
        .s_axi_awready(s00_couplers_to_xbar_AWREADY),
        .s_axi_awvalid(s00_couplers_to_xbar_AWVALID),
        .s_axi_bready(s00_couplers_to_xbar_BREADY),
        .s_axi_bresp(s00_couplers_to_xbar_BRESP),
        .s_axi_bvalid(s00_couplers_to_xbar_BVALID),
        .s_axi_rdata(s00_couplers_to_xbar_RDATA),
        .s_axi_rready(s00_couplers_to_xbar_RREADY),
        .s_axi_rresp(s00_couplers_to_xbar_RRESP),
        .s_axi_rvalid(s00_couplers_to_xbar_RVALID),
        .s_axi_wdata(s00_couplers_to_xbar_WDATA),
        .s_axi_wready(s00_couplers_to_xbar_WREADY),
        .s_axi_wstrb(s00_couplers_to_xbar_WSTRB),
        .s_axi_wvalid(s00_couplers_to_xbar_WVALID));
endmodule

module m00_couplers_imp_1AOQ6J3
   (M_AXIS_ACLK,
    M_AXIS_ARESETN,
    M_AXIS_tdata,
    M_AXIS_tkeep,
    M_AXIS_tlast,
    M_AXIS_tready,
    M_AXIS_tvalid,
    S_AXIS_ACLK,
    S_AXIS_ARESETN,
    S_AXIS_tdata,
    S_AXIS_tlast,
    S_AXIS_tready,
    S_AXIS_tvalid);
  input M_AXIS_ACLK;
  input [0:0]M_AXIS_ARESETN;
  output [31:0]M_AXIS_tdata;
  output [3:0]M_AXIS_tkeep;
  output M_AXIS_tlast;
  input M_AXIS_tready;
  output M_AXIS_tvalid;
  input S_AXIS_ACLK;
  input [0:0]S_AXIS_ARESETN;
  input [31:0]S_AXIS_tdata;
  input S_AXIS_tlast;
  output S_AXIS_tready;
  input S_AXIS_tvalid;

  wire S_AXIS_ACLK_1;
  wire [0:0]S_AXIS_ARESETN_1;
  wire [31:0]auto_ss_k_to_m00_couplers_TDATA;
  wire [3:0]auto_ss_k_to_m00_couplers_TKEEP;
  wire auto_ss_k_to_m00_couplers_TLAST;
  wire auto_ss_k_to_m00_couplers_TREADY;
  wire auto_ss_k_to_m00_couplers_TVALID;
  wire [31:0]m00_couplers_to_auto_ss_k_TDATA;
  wire m00_couplers_to_auto_ss_k_TLAST;
  wire m00_couplers_to_auto_ss_k_TREADY;
  wire m00_couplers_to_auto_ss_k_TVALID;

  assign M_AXIS_tdata[31:0] = auto_ss_k_to_m00_couplers_TDATA;
  assign M_AXIS_tkeep[3:0] = auto_ss_k_to_m00_couplers_TKEEP;
  assign M_AXIS_tlast = auto_ss_k_to_m00_couplers_TLAST;
  assign M_AXIS_tvalid = auto_ss_k_to_m00_couplers_TVALID;
  assign S_AXIS_ACLK_1 = S_AXIS_ACLK;
  assign S_AXIS_ARESETN_1 = S_AXIS_ARESETN[0];
  assign S_AXIS_tready = m00_couplers_to_auto_ss_k_TREADY;
  assign auto_ss_k_to_m00_couplers_TREADY = M_AXIS_tready;
  assign m00_couplers_to_auto_ss_k_TDATA = S_AXIS_tdata[31:0];
  assign m00_couplers_to_auto_ss_k_TLAST = S_AXIS_tlast;
  assign m00_couplers_to_auto_ss_k_TVALID = S_AXIS_tvalid;
  base_zynq_design_auto_ss_k_0 auto_ss_k
       (.aclk(S_AXIS_ACLK_1),
        .aresetn(S_AXIS_ARESETN_1),
        .m_axis_tdata(auto_ss_k_to_m00_couplers_TDATA),
        .m_axis_tkeep(auto_ss_k_to_m00_couplers_TKEEP),
        .m_axis_tlast(auto_ss_k_to_m00_couplers_TLAST),
        .m_axis_tready(auto_ss_k_to_m00_couplers_TREADY),
        .m_axis_tvalid(auto_ss_k_to_m00_couplers_TVALID),
        .s_axis_tdata(m00_couplers_to_auto_ss_k_TDATA),
        .s_axis_tlast(m00_couplers_to_auto_ss_k_TLAST),
        .s_axis_tready(m00_couplers_to_auto_ss_k_TREADY),
        .s_axis_tvalid(m00_couplers_to_auto_ss_k_TVALID));
endmodule

module m00_couplers_imp_1R5MXF4
   (M_ACLK,
    M_ARESETN,
    M_AXI_araddr,
    M_AXI_arready,
    M_AXI_arvalid,
    M_AXI_awaddr,
    M_AXI_awready,
    M_AXI_awvalid,
    M_AXI_bready,
    M_AXI_bresp,
    M_AXI_bvalid,
    M_AXI_rdata,
    M_AXI_rready,
    M_AXI_rresp,
    M_AXI_rvalid,
    M_AXI_wdata,
    M_AXI_wready,
    M_AXI_wstrb,
    M_AXI_wvalid,
    S_ACLK,
    S_ARESETN,
    S_AXI_araddr,
    S_AXI_arready,
    S_AXI_arvalid,
    S_AXI_awaddr,
    S_AXI_awready,
    S_AXI_awvalid,
    S_AXI_bready,
    S_AXI_bresp,
    S_AXI_bvalid,
    S_AXI_rdata,
    S_AXI_rready,
    S_AXI_rresp,
    S_AXI_rvalid,
    S_AXI_wdata,
    S_AXI_wready,
    S_AXI_wstrb,
    S_AXI_wvalid);
  input M_ACLK;
  input [0:0]M_ARESETN;
  output [31:0]M_AXI_araddr;
  input [0:0]M_AXI_arready;
  output [0:0]M_AXI_arvalid;
  output [31:0]M_AXI_awaddr;
  input [0:0]M_AXI_awready;
  output [0:0]M_AXI_awvalid;
  output [0:0]M_AXI_bready;
  input [1:0]M_AXI_bresp;
  input [0:0]M_AXI_bvalid;
  input [31:0]M_AXI_rdata;
  output [0:0]M_AXI_rready;
  input [1:0]M_AXI_rresp;
  input [0:0]M_AXI_rvalid;
  output [31:0]M_AXI_wdata;
  input [0:0]M_AXI_wready;
  output [3:0]M_AXI_wstrb;
  output [0:0]M_AXI_wvalid;
  input S_ACLK;
  input [0:0]S_ARESETN;
  input [31:0]S_AXI_araddr;
  output [0:0]S_AXI_arready;
  input [0:0]S_AXI_arvalid;
  input [31:0]S_AXI_awaddr;
  output [0:0]S_AXI_awready;
  input [0:0]S_AXI_awvalid;
  input [0:0]S_AXI_bready;
  output [1:0]S_AXI_bresp;
  output [0:0]S_AXI_bvalid;
  output [31:0]S_AXI_rdata;
  input [0:0]S_AXI_rready;
  output [1:0]S_AXI_rresp;
  output [0:0]S_AXI_rvalid;
  input [31:0]S_AXI_wdata;
  output [0:0]S_AXI_wready;
  input [3:0]S_AXI_wstrb;
  input [0:0]S_AXI_wvalid;

  wire [31:0]m00_couplers_to_m00_couplers_ARADDR;
  wire [0:0]m00_couplers_to_m00_couplers_ARREADY;
  wire [0:0]m00_couplers_to_m00_couplers_ARVALID;
  wire [31:0]m00_couplers_to_m00_couplers_AWADDR;
  wire [0:0]m00_couplers_to_m00_couplers_AWREADY;
  wire [0:0]m00_couplers_to_m00_couplers_AWVALID;
  wire [0:0]m00_couplers_to_m00_couplers_BREADY;
  wire [1:0]m00_couplers_to_m00_couplers_BRESP;
  wire [0:0]m00_couplers_to_m00_couplers_BVALID;
  wire [31:0]m00_couplers_to_m00_couplers_RDATA;
  wire [0:0]m00_couplers_to_m00_couplers_RREADY;
  wire [1:0]m00_couplers_to_m00_couplers_RRESP;
  wire [0:0]m00_couplers_to_m00_couplers_RVALID;
  wire [31:0]m00_couplers_to_m00_couplers_WDATA;
  wire [0:0]m00_couplers_to_m00_couplers_WREADY;
  wire [3:0]m00_couplers_to_m00_couplers_WSTRB;
  wire [0:0]m00_couplers_to_m00_couplers_WVALID;

  assign M_AXI_araddr[31:0] = m00_couplers_to_m00_couplers_ARADDR;
  assign M_AXI_arvalid[0] = m00_couplers_to_m00_couplers_ARVALID;
  assign M_AXI_awaddr[31:0] = m00_couplers_to_m00_couplers_AWADDR;
  assign M_AXI_awvalid[0] = m00_couplers_to_m00_couplers_AWVALID;
  assign M_AXI_bready[0] = m00_couplers_to_m00_couplers_BREADY;
  assign M_AXI_rready[0] = m00_couplers_to_m00_couplers_RREADY;
  assign M_AXI_wdata[31:0] = m00_couplers_to_m00_couplers_WDATA;
  assign M_AXI_wstrb[3:0] = m00_couplers_to_m00_couplers_WSTRB;
  assign M_AXI_wvalid[0] = m00_couplers_to_m00_couplers_WVALID;
  assign S_AXI_arready[0] = m00_couplers_to_m00_couplers_ARREADY;
  assign S_AXI_awready[0] = m00_couplers_to_m00_couplers_AWREADY;
  assign S_AXI_bresp[1:0] = m00_couplers_to_m00_couplers_BRESP;
  assign S_AXI_bvalid[0] = m00_couplers_to_m00_couplers_BVALID;
  assign S_AXI_rdata[31:0] = m00_couplers_to_m00_couplers_RDATA;
  assign S_AXI_rresp[1:0] = m00_couplers_to_m00_couplers_RRESP;
  assign S_AXI_rvalid[0] = m00_couplers_to_m00_couplers_RVALID;
  assign S_AXI_wready[0] = m00_couplers_to_m00_couplers_WREADY;
  assign m00_couplers_to_m00_couplers_ARADDR = S_AXI_araddr[31:0];
  assign m00_couplers_to_m00_couplers_ARREADY = M_AXI_arready[0];
  assign m00_couplers_to_m00_couplers_ARVALID = S_AXI_arvalid[0];
  assign m00_couplers_to_m00_couplers_AWADDR = S_AXI_awaddr[31:0];
  assign m00_couplers_to_m00_couplers_AWREADY = M_AXI_awready[0];
  assign m00_couplers_to_m00_couplers_AWVALID = S_AXI_awvalid[0];
  assign m00_couplers_to_m00_couplers_BREADY = S_AXI_bready[0];
  assign m00_couplers_to_m00_couplers_BRESP = M_AXI_bresp[1:0];
  assign m00_couplers_to_m00_couplers_BVALID = M_AXI_bvalid[0];
  assign m00_couplers_to_m00_couplers_RDATA = M_AXI_rdata[31:0];
  assign m00_couplers_to_m00_couplers_RREADY = S_AXI_rready[0];
  assign m00_couplers_to_m00_couplers_RRESP = M_AXI_rresp[1:0];
  assign m00_couplers_to_m00_couplers_RVALID = M_AXI_rvalid[0];
  assign m00_couplers_to_m00_couplers_WDATA = S_AXI_wdata[31:0];
  assign m00_couplers_to_m00_couplers_WREADY = M_AXI_wready[0];
  assign m00_couplers_to_m00_couplers_WSTRB = S_AXI_wstrb[3:0];
  assign m00_couplers_to_m00_couplers_WVALID = S_AXI_wvalid[0];
endmodule

module m01_couplers_imp_19312F
   (M_ACLK,
    M_ARESETN,
    M_AXI_araddr,
    M_AXI_arready,
    M_AXI_arvalid,
    M_AXI_awaddr,
    M_AXI_awready,
    M_AXI_awvalid,
    M_AXI_bready,
    M_AXI_bresp,
    M_AXI_bvalid,
    M_AXI_rdata,
    M_AXI_rready,
    M_AXI_rresp,
    M_AXI_rvalid,
    M_AXI_wdata,
    M_AXI_wready,
    M_AXI_wstrb,
    M_AXI_wvalid,
    S_ACLK,
    S_ARESETN,
    S_AXI_araddr,
    S_AXI_arready,
    S_AXI_arvalid,
    S_AXI_awaddr,
    S_AXI_awready,
    S_AXI_awvalid,
    S_AXI_bready,
    S_AXI_bresp,
    S_AXI_bvalid,
    S_AXI_rdata,
    S_AXI_rready,
    S_AXI_rresp,
    S_AXI_rvalid,
    S_AXI_wdata,
    S_AXI_wready,
    S_AXI_wstrb,
    S_AXI_wvalid);
  input M_ACLK;
  input [0:0]M_ARESETN;
  output [31:0]M_AXI_araddr;
  input M_AXI_arready;
  output M_AXI_arvalid;
  output [31:0]M_AXI_awaddr;
  input M_AXI_awready;
  output M_AXI_awvalid;
  output M_AXI_bready;
  input [1:0]M_AXI_bresp;
  input M_AXI_bvalid;
  input [31:0]M_AXI_rdata;
  output M_AXI_rready;
  input [1:0]M_AXI_rresp;
  input M_AXI_rvalid;
  output [31:0]M_AXI_wdata;
  input M_AXI_wready;
  output [3:0]M_AXI_wstrb;
  output M_AXI_wvalid;
  input S_ACLK;
  input [0:0]S_ARESETN;
  input [31:0]S_AXI_araddr;
  output S_AXI_arready;
  input S_AXI_arvalid;
  input [31:0]S_AXI_awaddr;
  output S_AXI_awready;
  input S_AXI_awvalid;
  input S_AXI_bready;
  output [1:0]S_AXI_bresp;
  output S_AXI_bvalid;
  output [31:0]S_AXI_rdata;
  input S_AXI_rready;
  output [1:0]S_AXI_rresp;
  output S_AXI_rvalid;
  input [31:0]S_AXI_wdata;
  output S_AXI_wready;
  input [3:0]S_AXI_wstrb;
  input S_AXI_wvalid;

  wire [31:0]m01_couplers_to_m01_couplers_ARADDR;
  wire m01_couplers_to_m01_couplers_ARREADY;
  wire m01_couplers_to_m01_couplers_ARVALID;
  wire [31:0]m01_couplers_to_m01_couplers_AWADDR;
  wire m01_couplers_to_m01_couplers_AWREADY;
  wire m01_couplers_to_m01_couplers_AWVALID;
  wire m01_couplers_to_m01_couplers_BREADY;
  wire [1:0]m01_couplers_to_m01_couplers_BRESP;
  wire m01_couplers_to_m01_couplers_BVALID;
  wire [31:0]m01_couplers_to_m01_couplers_RDATA;
  wire m01_couplers_to_m01_couplers_RREADY;
  wire [1:0]m01_couplers_to_m01_couplers_RRESP;
  wire m01_couplers_to_m01_couplers_RVALID;
  wire [31:0]m01_couplers_to_m01_couplers_WDATA;
  wire m01_couplers_to_m01_couplers_WREADY;
  wire [3:0]m01_couplers_to_m01_couplers_WSTRB;
  wire m01_couplers_to_m01_couplers_WVALID;

  assign M_AXI_araddr[31:0] = m01_couplers_to_m01_couplers_ARADDR;
  assign M_AXI_arvalid = m01_couplers_to_m01_couplers_ARVALID;
  assign M_AXI_awaddr[31:0] = m01_couplers_to_m01_couplers_AWADDR;
  assign M_AXI_awvalid = m01_couplers_to_m01_couplers_AWVALID;
  assign M_AXI_bready = m01_couplers_to_m01_couplers_BREADY;
  assign M_AXI_rready = m01_couplers_to_m01_couplers_RREADY;
  assign M_AXI_wdata[31:0] = m01_couplers_to_m01_couplers_WDATA;
  assign M_AXI_wstrb[3:0] = m01_couplers_to_m01_couplers_WSTRB;
  assign M_AXI_wvalid = m01_couplers_to_m01_couplers_WVALID;
  assign S_AXI_arready = m01_couplers_to_m01_couplers_ARREADY;
  assign S_AXI_awready = m01_couplers_to_m01_couplers_AWREADY;
  assign S_AXI_bresp[1:0] = m01_couplers_to_m01_couplers_BRESP;
  assign S_AXI_bvalid = m01_couplers_to_m01_couplers_BVALID;
  assign S_AXI_rdata[31:0] = m01_couplers_to_m01_couplers_RDATA;
  assign S_AXI_rresp[1:0] = m01_couplers_to_m01_couplers_RRESP;
  assign S_AXI_rvalid = m01_couplers_to_m01_couplers_RVALID;
  assign S_AXI_wready = m01_couplers_to_m01_couplers_WREADY;
  assign m01_couplers_to_m01_couplers_ARADDR = S_AXI_araddr[31:0];
  assign m01_couplers_to_m01_couplers_ARREADY = M_AXI_arready;
  assign m01_couplers_to_m01_couplers_ARVALID = S_AXI_arvalid;
  assign m01_couplers_to_m01_couplers_AWADDR = S_AXI_awaddr[31:0];
  assign m01_couplers_to_m01_couplers_AWREADY = M_AXI_awready;
  assign m01_couplers_to_m01_couplers_AWVALID = S_AXI_awvalid;
  assign m01_couplers_to_m01_couplers_BREADY = S_AXI_bready;
  assign m01_couplers_to_m01_couplers_BRESP = M_AXI_bresp[1:0];
  assign m01_couplers_to_m01_couplers_BVALID = M_AXI_bvalid;
  assign m01_couplers_to_m01_couplers_RDATA = M_AXI_rdata[31:0];
  assign m01_couplers_to_m01_couplers_RREADY = S_AXI_rready;
  assign m01_couplers_to_m01_couplers_RRESP = M_AXI_rresp[1:0];
  assign m01_couplers_to_m01_couplers_RVALID = M_AXI_rvalid;
  assign m01_couplers_to_m01_couplers_WDATA = S_AXI_wdata[31:0];
  assign m01_couplers_to_m01_couplers_WREADY = M_AXI_wready;
  assign m01_couplers_to_m01_couplers_WSTRB = S_AXI_wstrb[3:0];
  assign m01_couplers_to_m01_couplers_WVALID = S_AXI_wvalid;
endmodule

module m02_couplers_imp_1WHIQQN
   (M_ACLK,
    M_ARESETN,
    M_AXI_araddr,
    M_AXI_arready,
    M_AXI_arvalid,
    M_AXI_awaddr,
    M_AXI_awready,
    M_AXI_awvalid,
    M_AXI_bready,
    M_AXI_bresp,
    M_AXI_bvalid,
    M_AXI_rdata,
    M_AXI_rready,
    M_AXI_rresp,
    M_AXI_rvalid,
    M_AXI_wdata,
    M_AXI_wready,
    M_AXI_wstrb,
    M_AXI_wvalid,
    S_ACLK,
    S_ARESETN,
    S_AXI_araddr,
    S_AXI_arready,
    S_AXI_arvalid,
    S_AXI_awaddr,
    S_AXI_awready,
    S_AXI_awvalid,
    S_AXI_bready,
    S_AXI_bresp,
    S_AXI_bvalid,
    S_AXI_rdata,
    S_AXI_rready,
    S_AXI_rresp,
    S_AXI_rvalid,
    S_AXI_wdata,
    S_AXI_wready,
    S_AXI_wstrb,
    S_AXI_wvalid);
  input M_ACLK;
  input [0:0]M_ARESETN;
  output [31:0]M_AXI_araddr;
  input M_AXI_arready;
  output M_AXI_arvalid;
  output [31:0]M_AXI_awaddr;
  input M_AXI_awready;
  output M_AXI_awvalid;
  output M_AXI_bready;
  input [1:0]M_AXI_bresp;
  input M_AXI_bvalid;
  input [31:0]M_AXI_rdata;
  output M_AXI_rready;
  input [1:0]M_AXI_rresp;
  input M_AXI_rvalid;
  output [31:0]M_AXI_wdata;
  input M_AXI_wready;
  output [3:0]M_AXI_wstrb;
  output M_AXI_wvalid;
  input S_ACLK;
  input [0:0]S_ARESETN;
  input [31:0]S_AXI_araddr;
  output S_AXI_arready;
  input S_AXI_arvalid;
  input [31:0]S_AXI_awaddr;
  output S_AXI_awready;
  input S_AXI_awvalid;
  input S_AXI_bready;
  output [1:0]S_AXI_bresp;
  output S_AXI_bvalid;
  output [31:0]S_AXI_rdata;
  input S_AXI_rready;
  output [1:0]S_AXI_rresp;
  output S_AXI_rvalid;
  input [31:0]S_AXI_wdata;
  output S_AXI_wready;
  input [3:0]S_AXI_wstrb;
  input S_AXI_wvalid;

  wire [31:0]m02_couplers_to_m02_couplers_ARADDR;
  wire m02_couplers_to_m02_couplers_ARREADY;
  wire m02_couplers_to_m02_couplers_ARVALID;
  wire [31:0]m02_couplers_to_m02_couplers_AWADDR;
  wire m02_couplers_to_m02_couplers_AWREADY;
  wire m02_couplers_to_m02_couplers_AWVALID;
  wire m02_couplers_to_m02_couplers_BREADY;
  wire [1:0]m02_couplers_to_m02_couplers_BRESP;
  wire m02_couplers_to_m02_couplers_BVALID;
  wire [31:0]m02_couplers_to_m02_couplers_RDATA;
  wire m02_couplers_to_m02_couplers_RREADY;
  wire [1:0]m02_couplers_to_m02_couplers_RRESP;
  wire m02_couplers_to_m02_couplers_RVALID;
  wire [31:0]m02_couplers_to_m02_couplers_WDATA;
  wire m02_couplers_to_m02_couplers_WREADY;
  wire [3:0]m02_couplers_to_m02_couplers_WSTRB;
  wire m02_couplers_to_m02_couplers_WVALID;

  assign M_AXI_araddr[31:0] = m02_couplers_to_m02_couplers_ARADDR;
  assign M_AXI_arvalid = m02_couplers_to_m02_couplers_ARVALID;
  assign M_AXI_awaddr[31:0] = m02_couplers_to_m02_couplers_AWADDR;
  assign M_AXI_awvalid = m02_couplers_to_m02_couplers_AWVALID;
  assign M_AXI_bready = m02_couplers_to_m02_couplers_BREADY;
  assign M_AXI_rready = m02_couplers_to_m02_couplers_RREADY;
  assign M_AXI_wdata[31:0] = m02_couplers_to_m02_couplers_WDATA;
  assign M_AXI_wstrb[3:0] = m02_couplers_to_m02_couplers_WSTRB;
  assign M_AXI_wvalid = m02_couplers_to_m02_couplers_WVALID;
  assign S_AXI_arready = m02_couplers_to_m02_couplers_ARREADY;
  assign S_AXI_awready = m02_couplers_to_m02_couplers_AWREADY;
  assign S_AXI_bresp[1:0] = m02_couplers_to_m02_couplers_BRESP;
  assign S_AXI_bvalid = m02_couplers_to_m02_couplers_BVALID;
  assign S_AXI_rdata[31:0] = m02_couplers_to_m02_couplers_RDATA;
  assign S_AXI_rresp[1:0] = m02_couplers_to_m02_couplers_RRESP;
  assign S_AXI_rvalid = m02_couplers_to_m02_couplers_RVALID;
  assign S_AXI_wready = m02_couplers_to_m02_couplers_WREADY;
  assign m02_couplers_to_m02_couplers_ARADDR = S_AXI_araddr[31:0];
  assign m02_couplers_to_m02_couplers_ARREADY = M_AXI_arready;
  assign m02_couplers_to_m02_couplers_ARVALID = S_AXI_arvalid;
  assign m02_couplers_to_m02_couplers_AWADDR = S_AXI_awaddr[31:0];
  assign m02_couplers_to_m02_couplers_AWREADY = M_AXI_awready;
  assign m02_couplers_to_m02_couplers_AWVALID = S_AXI_awvalid;
  assign m02_couplers_to_m02_couplers_BREADY = S_AXI_bready;
  assign m02_couplers_to_m02_couplers_BRESP = M_AXI_bresp[1:0];
  assign m02_couplers_to_m02_couplers_BVALID = M_AXI_bvalid;
  assign m02_couplers_to_m02_couplers_RDATA = M_AXI_rdata[31:0];
  assign m02_couplers_to_m02_couplers_RREADY = S_AXI_rready;
  assign m02_couplers_to_m02_couplers_RRESP = M_AXI_rresp[1:0];
  assign m02_couplers_to_m02_couplers_RVALID = M_AXI_rvalid;
  assign m02_couplers_to_m02_couplers_WDATA = S_AXI_wdata[31:0];
  assign m02_couplers_to_m02_couplers_WREADY = M_AXI_wready;
  assign m02_couplers_to_m02_couplers_WSTRB = S_AXI_wstrb[3:0];
  assign m02_couplers_to_m02_couplers_WVALID = S_AXI_wvalid;
endmodule

module m03_couplers_imp_4XNRYG
   (M_ACLK,
    M_ARESETN,
    M_AXI_araddr,
    M_AXI_arprot,
    M_AXI_arready,
    M_AXI_arvalid,
    M_AXI_awaddr,
    M_AXI_awprot,
    M_AXI_awready,
    M_AXI_awvalid,
    M_AXI_bready,
    M_AXI_bresp,
    M_AXI_bvalid,
    M_AXI_rdata,
    M_AXI_rready,
    M_AXI_rresp,
    M_AXI_rvalid,
    M_AXI_wdata,
    M_AXI_wready,
    M_AXI_wstrb,
    M_AXI_wvalid,
    S_ACLK,
    S_ARESETN,
    S_AXI_araddr,
    S_AXI_arprot,
    S_AXI_arready,
    S_AXI_arvalid,
    S_AXI_awaddr,
    S_AXI_awprot,
    S_AXI_awready,
    S_AXI_awvalid,
    S_AXI_bready,
    S_AXI_bresp,
    S_AXI_bvalid,
    S_AXI_rdata,
    S_AXI_rready,
    S_AXI_rresp,
    S_AXI_rvalid,
    S_AXI_wdata,
    S_AXI_wready,
    S_AXI_wstrb,
    S_AXI_wvalid);
  input M_ACLK;
  input [0:0]M_ARESETN;
  output [31:0]M_AXI_araddr;
  output [2:0]M_AXI_arprot;
  input M_AXI_arready;
  output M_AXI_arvalid;
  output [31:0]M_AXI_awaddr;
  output [2:0]M_AXI_awprot;
  input M_AXI_awready;
  output M_AXI_awvalid;
  output M_AXI_bready;
  input [1:0]M_AXI_bresp;
  input M_AXI_bvalid;
  input [31:0]M_AXI_rdata;
  output M_AXI_rready;
  input [1:0]M_AXI_rresp;
  input M_AXI_rvalid;
  output [31:0]M_AXI_wdata;
  input M_AXI_wready;
  output [3:0]M_AXI_wstrb;
  output M_AXI_wvalid;
  input S_ACLK;
  input [0:0]S_ARESETN;
  input [31:0]S_AXI_araddr;
  input [2:0]S_AXI_arprot;
  output S_AXI_arready;
  input S_AXI_arvalid;
  input [31:0]S_AXI_awaddr;
  input [2:0]S_AXI_awprot;
  output S_AXI_awready;
  input S_AXI_awvalid;
  input S_AXI_bready;
  output [1:0]S_AXI_bresp;
  output S_AXI_bvalid;
  output [31:0]S_AXI_rdata;
  input S_AXI_rready;
  output [1:0]S_AXI_rresp;
  output S_AXI_rvalid;
  input [31:0]S_AXI_wdata;
  output S_AXI_wready;
  input [3:0]S_AXI_wstrb;
  input S_AXI_wvalid;

  wire [31:0]m03_couplers_to_m03_couplers_ARADDR;
  wire [2:0]m03_couplers_to_m03_couplers_ARPROT;
  wire m03_couplers_to_m03_couplers_ARREADY;
  wire m03_couplers_to_m03_couplers_ARVALID;
  wire [31:0]m03_couplers_to_m03_couplers_AWADDR;
  wire [2:0]m03_couplers_to_m03_couplers_AWPROT;
  wire m03_couplers_to_m03_couplers_AWREADY;
  wire m03_couplers_to_m03_couplers_AWVALID;
  wire m03_couplers_to_m03_couplers_BREADY;
  wire [1:0]m03_couplers_to_m03_couplers_BRESP;
  wire m03_couplers_to_m03_couplers_BVALID;
  wire [31:0]m03_couplers_to_m03_couplers_RDATA;
  wire m03_couplers_to_m03_couplers_RREADY;
  wire [1:0]m03_couplers_to_m03_couplers_RRESP;
  wire m03_couplers_to_m03_couplers_RVALID;
  wire [31:0]m03_couplers_to_m03_couplers_WDATA;
  wire m03_couplers_to_m03_couplers_WREADY;
  wire [3:0]m03_couplers_to_m03_couplers_WSTRB;
  wire m03_couplers_to_m03_couplers_WVALID;

  assign M_AXI_araddr[31:0] = m03_couplers_to_m03_couplers_ARADDR;
  assign M_AXI_arprot[2:0] = m03_couplers_to_m03_couplers_ARPROT;
  assign M_AXI_arvalid = m03_couplers_to_m03_couplers_ARVALID;
  assign M_AXI_awaddr[31:0] = m03_couplers_to_m03_couplers_AWADDR;
  assign M_AXI_awprot[2:0] = m03_couplers_to_m03_couplers_AWPROT;
  assign M_AXI_awvalid = m03_couplers_to_m03_couplers_AWVALID;
  assign M_AXI_bready = m03_couplers_to_m03_couplers_BREADY;
  assign M_AXI_rready = m03_couplers_to_m03_couplers_RREADY;
  assign M_AXI_wdata[31:0] = m03_couplers_to_m03_couplers_WDATA;
  assign M_AXI_wstrb[3:0] = m03_couplers_to_m03_couplers_WSTRB;
  assign M_AXI_wvalid = m03_couplers_to_m03_couplers_WVALID;
  assign S_AXI_arready = m03_couplers_to_m03_couplers_ARREADY;
  assign S_AXI_awready = m03_couplers_to_m03_couplers_AWREADY;
  assign S_AXI_bresp[1:0] = m03_couplers_to_m03_couplers_BRESP;
  assign S_AXI_bvalid = m03_couplers_to_m03_couplers_BVALID;
  assign S_AXI_rdata[31:0] = m03_couplers_to_m03_couplers_RDATA;
  assign S_AXI_rresp[1:0] = m03_couplers_to_m03_couplers_RRESP;
  assign S_AXI_rvalid = m03_couplers_to_m03_couplers_RVALID;
  assign S_AXI_wready = m03_couplers_to_m03_couplers_WREADY;
  assign m03_couplers_to_m03_couplers_ARADDR = S_AXI_araddr[31:0];
  assign m03_couplers_to_m03_couplers_ARPROT = S_AXI_arprot[2:0];
  assign m03_couplers_to_m03_couplers_ARREADY = M_AXI_arready;
  assign m03_couplers_to_m03_couplers_ARVALID = S_AXI_arvalid;
  assign m03_couplers_to_m03_couplers_AWADDR = S_AXI_awaddr[31:0];
  assign m03_couplers_to_m03_couplers_AWPROT = S_AXI_awprot[2:0];
  assign m03_couplers_to_m03_couplers_AWREADY = M_AXI_awready;
  assign m03_couplers_to_m03_couplers_AWVALID = S_AXI_awvalid;
  assign m03_couplers_to_m03_couplers_BREADY = S_AXI_bready;
  assign m03_couplers_to_m03_couplers_BRESP = M_AXI_bresp[1:0];
  assign m03_couplers_to_m03_couplers_BVALID = M_AXI_bvalid;
  assign m03_couplers_to_m03_couplers_RDATA = M_AXI_rdata[31:0];
  assign m03_couplers_to_m03_couplers_RREADY = S_AXI_rready;
  assign m03_couplers_to_m03_couplers_RRESP = M_AXI_rresp[1:0];
  assign m03_couplers_to_m03_couplers_RVALID = M_AXI_rvalid;
  assign m03_couplers_to_m03_couplers_WDATA = S_AXI_wdata[31:0];
  assign m03_couplers_to_m03_couplers_WREADY = M_AXI_wready;
  assign m03_couplers_to_m03_couplers_WSTRB = S_AXI_wstrb[3:0];
  assign m03_couplers_to_m03_couplers_WVALID = S_AXI_wvalid;
endmodule

module m04_couplers_imp_1JS4TBI
   (M_ACLK,
    M_ARESETN,
    M_AXI_araddr,
    M_AXI_arready,
    M_AXI_arvalid,
    M_AXI_awaddr,
    M_AXI_awready,
    M_AXI_awvalid,
    M_AXI_bready,
    M_AXI_bresp,
    M_AXI_bvalid,
    M_AXI_rdata,
    M_AXI_rready,
    M_AXI_rresp,
    M_AXI_rvalid,
    M_AXI_wdata,
    M_AXI_wready,
    M_AXI_wstrb,
    M_AXI_wvalid,
    S_ACLK,
    S_ARESETN,
    S_AXI_araddr,
    S_AXI_arready,
    S_AXI_arvalid,
    S_AXI_awaddr,
    S_AXI_awready,
    S_AXI_awvalid,
    S_AXI_bready,
    S_AXI_bresp,
    S_AXI_bvalid,
    S_AXI_rdata,
    S_AXI_rready,
    S_AXI_rresp,
    S_AXI_rvalid,
    S_AXI_wdata,
    S_AXI_wready,
    S_AXI_wstrb,
    S_AXI_wvalid);
  input M_ACLK;
  input [0:0]M_ARESETN;
  output [31:0]M_AXI_araddr;
  input M_AXI_arready;
  output M_AXI_arvalid;
  output [31:0]M_AXI_awaddr;
  input M_AXI_awready;
  output M_AXI_awvalid;
  output M_AXI_bready;
  input [1:0]M_AXI_bresp;
  input M_AXI_bvalid;
  input [31:0]M_AXI_rdata;
  output M_AXI_rready;
  input [1:0]M_AXI_rresp;
  input M_AXI_rvalid;
  output [31:0]M_AXI_wdata;
  input M_AXI_wready;
  output [3:0]M_AXI_wstrb;
  output M_AXI_wvalid;
  input S_ACLK;
  input [0:0]S_ARESETN;
  input [31:0]S_AXI_araddr;
  output S_AXI_arready;
  input S_AXI_arvalid;
  input [31:0]S_AXI_awaddr;
  output S_AXI_awready;
  input S_AXI_awvalid;
  input S_AXI_bready;
  output [1:0]S_AXI_bresp;
  output S_AXI_bvalid;
  output [31:0]S_AXI_rdata;
  input S_AXI_rready;
  output [1:0]S_AXI_rresp;
  output S_AXI_rvalid;
  input [31:0]S_AXI_wdata;
  output S_AXI_wready;
  input [3:0]S_AXI_wstrb;
  input S_AXI_wvalid;

  wire [31:0]m04_couplers_to_m04_couplers_ARADDR;
  wire m04_couplers_to_m04_couplers_ARREADY;
  wire m04_couplers_to_m04_couplers_ARVALID;
  wire [31:0]m04_couplers_to_m04_couplers_AWADDR;
  wire m04_couplers_to_m04_couplers_AWREADY;
  wire m04_couplers_to_m04_couplers_AWVALID;
  wire m04_couplers_to_m04_couplers_BREADY;
  wire [1:0]m04_couplers_to_m04_couplers_BRESP;
  wire m04_couplers_to_m04_couplers_BVALID;
  wire [31:0]m04_couplers_to_m04_couplers_RDATA;
  wire m04_couplers_to_m04_couplers_RREADY;
  wire [1:0]m04_couplers_to_m04_couplers_RRESP;
  wire m04_couplers_to_m04_couplers_RVALID;
  wire [31:0]m04_couplers_to_m04_couplers_WDATA;
  wire m04_couplers_to_m04_couplers_WREADY;
  wire [3:0]m04_couplers_to_m04_couplers_WSTRB;
  wire m04_couplers_to_m04_couplers_WVALID;

  assign M_AXI_araddr[31:0] = m04_couplers_to_m04_couplers_ARADDR;
  assign M_AXI_arvalid = m04_couplers_to_m04_couplers_ARVALID;
  assign M_AXI_awaddr[31:0] = m04_couplers_to_m04_couplers_AWADDR;
  assign M_AXI_awvalid = m04_couplers_to_m04_couplers_AWVALID;
  assign M_AXI_bready = m04_couplers_to_m04_couplers_BREADY;
  assign M_AXI_rready = m04_couplers_to_m04_couplers_RREADY;
  assign M_AXI_wdata[31:0] = m04_couplers_to_m04_couplers_WDATA;
  assign M_AXI_wstrb[3:0] = m04_couplers_to_m04_couplers_WSTRB;
  assign M_AXI_wvalid = m04_couplers_to_m04_couplers_WVALID;
  assign S_AXI_arready = m04_couplers_to_m04_couplers_ARREADY;
  assign S_AXI_awready = m04_couplers_to_m04_couplers_AWREADY;
  assign S_AXI_bresp[1:0] = m04_couplers_to_m04_couplers_BRESP;
  assign S_AXI_bvalid = m04_couplers_to_m04_couplers_BVALID;
  assign S_AXI_rdata[31:0] = m04_couplers_to_m04_couplers_RDATA;
  assign S_AXI_rresp[1:0] = m04_couplers_to_m04_couplers_RRESP;
  assign S_AXI_rvalid = m04_couplers_to_m04_couplers_RVALID;
  assign S_AXI_wready = m04_couplers_to_m04_couplers_WREADY;
  assign m04_couplers_to_m04_couplers_ARADDR = S_AXI_araddr[31:0];
  assign m04_couplers_to_m04_couplers_ARREADY = M_AXI_arready;
  assign m04_couplers_to_m04_couplers_ARVALID = S_AXI_arvalid;
  assign m04_couplers_to_m04_couplers_AWADDR = S_AXI_awaddr[31:0];
  assign m04_couplers_to_m04_couplers_AWREADY = M_AXI_awready;
  assign m04_couplers_to_m04_couplers_AWVALID = S_AXI_awvalid;
  assign m04_couplers_to_m04_couplers_BREADY = S_AXI_bready;
  assign m04_couplers_to_m04_couplers_BRESP = M_AXI_bresp[1:0];
  assign m04_couplers_to_m04_couplers_BVALID = M_AXI_bvalid;
  assign m04_couplers_to_m04_couplers_RDATA = M_AXI_rdata[31:0];
  assign m04_couplers_to_m04_couplers_RREADY = S_AXI_rready;
  assign m04_couplers_to_m04_couplers_RRESP = M_AXI_rresp[1:0];
  assign m04_couplers_to_m04_couplers_RVALID = M_AXI_rvalid;
  assign m04_couplers_to_m04_couplers_WDATA = S_AXI_wdata[31:0];
  assign m04_couplers_to_m04_couplers_WREADY = M_AXI_wready;
  assign m04_couplers_to_m04_couplers_WSTRB = S_AXI_wstrb[3:0];
  assign m04_couplers_to_m04_couplers_WVALID = S_AXI_wvalid;
endmodule

module m05_couplers_imp_CSEVP5
   (M_ACLK,
    M_ARESETN,
    M_AXI_araddr,
    M_AXI_arready,
    M_AXI_arvalid,
    M_AXI_awaddr,
    M_AXI_awready,
    M_AXI_awvalid,
    M_AXI_bready,
    M_AXI_bresp,
    M_AXI_bvalid,
    M_AXI_rdata,
    M_AXI_rready,
    M_AXI_rresp,
    M_AXI_rvalid,
    M_AXI_wdata,
    M_AXI_wready,
    M_AXI_wstrb,
    M_AXI_wvalid,
    S_ACLK,
    S_ARESETN,
    S_AXI_araddr,
    S_AXI_arready,
    S_AXI_arvalid,
    S_AXI_awaddr,
    S_AXI_awready,
    S_AXI_awvalid,
    S_AXI_bready,
    S_AXI_bresp,
    S_AXI_bvalid,
    S_AXI_rdata,
    S_AXI_rready,
    S_AXI_rresp,
    S_AXI_rvalid,
    S_AXI_wdata,
    S_AXI_wready,
    S_AXI_wstrb,
    S_AXI_wvalid);
  input M_ACLK;
  input [0:0]M_ARESETN;
  output [31:0]M_AXI_araddr;
  input M_AXI_arready;
  output M_AXI_arvalid;
  output [31:0]M_AXI_awaddr;
  input M_AXI_awready;
  output M_AXI_awvalid;
  output M_AXI_bready;
  input [1:0]M_AXI_bresp;
  input M_AXI_bvalid;
  input [31:0]M_AXI_rdata;
  output M_AXI_rready;
  input [1:0]M_AXI_rresp;
  input M_AXI_rvalid;
  output [31:0]M_AXI_wdata;
  input M_AXI_wready;
  output [3:0]M_AXI_wstrb;
  output M_AXI_wvalid;
  input S_ACLK;
  input [0:0]S_ARESETN;
  input [31:0]S_AXI_araddr;
  output S_AXI_arready;
  input S_AXI_arvalid;
  input [31:0]S_AXI_awaddr;
  output S_AXI_awready;
  input S_AXI_awvalid;
  input S_AXI_bready;
  output [1:0]S_AXI_bresp;
  output S_AXI_bvalid;
  output [31:0]S_AXI_rdata;
  input S_AXI_rready;
  output [1:0]S_AXI_rresp;
  output S_AXI_rvalid;
  input [31:0]S_AXI_wdata;
  output S_AXI_wready;
  input [3:0]S_AXI_wstrb;
  input S_AXI_wvalid;

  wire [31:0]m05_couplers_to_m05_couplers_ARADDR;
  wire m05_couplers_to_m05_couplers_ARREADY;
  wire m05_couplers_to_m05_couplers_ARVALID;
  wire [31:0]m05_couplers_to_m05_couplers_AWADDR;
  wire m05_couplers_to_m05_couplers_AWREADY;
  wire m05_couplers_to_m05_couplers_AWVALID;
  wire m05_couplers_to_m05_couplers_BREADY;
  wire [1:0]m05_couplers_to_m05_couplers_BRESP;
  wire m05_couplers_to_m05_couplers_BVALID;
  wire [31:0]m05_couplers_to_m05_couplers_RDATA;
  wire m05_couplers_to_m05_couplers_RREADY;
  wire [1:0]m05_couplers_to_m05_couplers_RRESP;
  wire m05_couplers_to_m05_couplers_RVALID;
  wire [31:0]m05_couplers_to_m05_couplers_WDATA;
  wire m05_couplers_to_m05_couplers_WREADY;
  wire [3:0]m05_couplers_to_m05_couplers_WSTRB;
  wire m05_couplers_to_m05_couplers_WVALID;

  assign M_AXI_araddr[31:0] = m05_couplers_to_m05_couplers_ARADDR;
  assign M_AXI_arvalid = m05_couplers_to_m05_couplers_ARVALID;
  assign M_AXI_awaddr[31:0] = m05_couplers_to_m05_couplers_AWADDR;
  assign M_AXI_awvalid = m05_couplers_to_m05_couplers_AWVALID;
  assign M_AXI_bready = m05_couplers_to_m05_couplers_BREADY;
  assign M_AXI_rready = m05_couplers_to_m05_couplers_RREADY;
  assign M_AXI_wdata[31:0] = m05_couplers_to_m05_couplers_WDATA;
  assign M_AXI_wstrb[3:0] = m05_couplers_to_m05_couplers_WSTRB;
  assign M_AXI_wvalid = m05_couplers_to_m05_couplers_WVALID;
  assign S_AXI_arready = m05_couplers_to_m05_couplers_ARREADY;
  assign S_AXI_awready = m05_couplers_to_m05_couplers_AWREADY;
  assign S_AXI_bresp[1:0] = m05_couplers_to_m05_couplers_BRESP;
  assign S_AXI_bvalid = m05_couplers_to_m05_couplers_BVALID;
  assign S_AXI_rdata[31:0] = m05_couplers_to_m05_couplers_RDATA;
  assign S_AXI_rresp[1:0] = m05_couplers_to_m05_couplers_RRESP;
  assign S_AXI_rvalid = m05_couplers_to_m05_couplers_RVALID;
  assign S_AXI_wready = m05_couplers_to_m05_couplers_WREADY;
  assign m05_couplers_to_m05_couplers_ARADDR = S_AXI_araddr[31:0];
  assign m05_couplers_to_m05_couplers_ARREADY = M_AXI_arready;
  assign m05_couplers_to_m05_couplers_ARVALID = S_AXI_arvalid;
  assign m05_couplers_to_m05_couplers_AWADDR = S_AXI_awaddr[31:0];
  assign m05_couplers_to_m05_couplers_AWREADY = M_AXI_awready;
  assign m05_couplers_to_m05_couplers_AWVALID = S_AXI_awvalid;
  assign m05_couplers_to_m05_couplers_BREADY = S_AXI_bready;
  assign m05_couplers_to_m05_couplers_BRESP = M_AXI_bresp[1:0];
  assign m05_couplers_to_m05_couplers_BVALID = M_AXI_bvalid;
  assign m05_couplers_to_m05_couplers_RDATA = M_AXI_rdata[31:0];
  assign m05_couplers_to_m05_couplers_RREADY = S_AXI_rready;
  assign m05_couplers_to_m05_couplers_RRESP = M_AXI_rresp[1:0];
  assign m05_couplers_to_m05_couplers_RVALID = M_AXI_rvalid;
  assign m05_couplers_to_m05_couplers_WDATA = S_AXI_wdata[31:0];
  assign m05_couplers_to_m05_couplers_WREADY = M_AXI_wready;
  assign m05_couplers_to_m05_couplers_WSTRB = S_AXI_wstrb[3:0];
  assign m05_couplers_to_m05_couplers_WVALID = S_AXI_wvalid;
endmodule

module m06_couplers_imp_1P40NB5
   (M_ACLK,
    M_ARESETN,
    M_AXI_araddr,
    M_AXI_arready,
    M_AXI_arvalid,
    M_AXI_awaddr,
    M_AXI_awready,
    M_AXI_awvalid,
    M_AXI_bready,
    M_AXI_bresp,
    M_AXI_bvalid,
    M_AXI_rdata,
    M_AXI_rready,
    M_AXI_rresp,
    M_AXI_rvalid,
    M_AXI_wdata,
    M_AXI_wready,
    M_AXI_wstrb,
    M_AXI_wvalid,
    S_ACLK,
    S_ARESETN,
    S_AXI_araddr,
    S_AXI_arready,
    S_AXI_arvalid,
    S_AXI_awaddr,
    S_AXI_awready,
    S_AXI_awvalid,
    S_AXI_bready,
    S_AXI_bresp,
    S_AXI_bvalid,
    S_AXI_rdata,
    S_AXI_rready,
    S_AXI_rresp,
    S_AXI_rvalid,
    S_AXI_wdata,
    S_AXI_wready,
    S_AXI_wstrb,
    S_AXI_wvalid);
  input M_ACLK;
  input [0:0]M_ARESETN;
  output [31:0]M_AXI_araddr;
  input M_AXI_arready;
  output M_AXI_arvalid;
  output [31:0]M_AXI_awaddr;
  input M_AXI_awready;
  output M_AXI_awvalid;
  output M_AXI_bready;
  input [1:0]M_AXI_bresp;
  input M_AXI_bvalid;
  input [31:0]M_AXI_rdata;
  output M_AXI_rready;
  input [1:0]M_AXI_rresp;
  input M_AXI_rvalid;
  output [31:0]M_AXI_wdata;
  input M_AXI_wready;
  output [3:0]M_AXI_wstrb;
  output M_AXI_wvalid;
  input S_ACLK;
  input [0:0]S_ARESETN;
  input [31:0]S_AXI_araddr;
  output S_AXI_arready;
  input S_AXI_arvalid;
  input [31:0]S_AXI_awaddr;
  output S_AXI_awready;
  input S_AXI_awvalid;
  input S_AXI_bready;
  output [1:0]S_AXI_bresp;
  output S_AXI_bvalid;
  output [31:0]S_AXI_rdata;
  input S_AXI_rready;
  output [1:0]S_AXI_rresp;
  output S_AXI_rvalid;
  input [31:0]S_AXI_wdata;
  output S_AXI_wready;
  input [3:0]S_AXI_wstrb;
  input S_AXI_wvalid;

  wire [31:0]m06_couplers_to_m06_couplers_ARADDR;
  wire m06_couplers_to_m06_couplers_ARREADY;
  wire m06_couplers_to_m06_couplers_ARVALID;
  wire [31:0]m06_couplers_to_m06_couplers_AWADDR;
  wire m06_couplers_to_m06_couplers_AWREADY;
  wire m06_couplers_to_m06_couplers_AWVALID;
  wire m06_couplers_to_m06_couplers_BREADY;
  wire [1:0]m06_couplers_to_m06_couplers_BRESP;
  wire m06_couplers_to_m06_couplers_BVALID;
  wire [31:0]m06_couplers_to_m06_couplers_RDATA;
  wire m06_couplers_to_m06_couplers_RREADY;
  wire [1:0]m06_couplers_to_m06_couplers_RRESP;
  wire m06_couplers_to_m06_couplers_RVALID;
  wire [31:0]m06_couplers_to_m06_couplers_WDATA;
  wire m06_couplers_to_m06_couplers_WREADY;
  wire [3:0]m06_couplers_to_m06_couplers_WSTRB;
  wire m06_couplers_to_m06_couplers_WVALID;

  assign M_AXI_araddr[31:0] = m06_couplers_to_m06_couplers_ARADDR;
  assign M_AXI_arvalid = m06_couplers_to_m06_couplers_ARVALID;
  assign M_AXI_awaddr[31:0] = m06_couplers_to_m06_couplers_AWADDR;
  assign M_AXI_awvalid = m06_couplers_to_m06_couplers_AWVALID;
  assign M_AXI_bready = m06_couplers_to_m06_couplers_BREADY;
  assign M_AXI_rready = m06_couplers_to_m06_couplers_RREADY;
  assign M_AXI_wdata[31:0] = m06_couplers_to_m06_couplers_WDATA;
  assign M_AXI_wstrb[3:0] = m06_couplers_to_m06_couplers_WSTRB;
  assign M_AXI_wvalid = m06_couplers_to_m06_couplers_WVALID;
  assign S_AXI_arready = m06_couplers_to_m06_couplers_ARREADY;
  assign S_AXI_awready = m06_couplers_to_m06_couplers_AWREADY;
  assign S_AXI_bresp[1:0] = m06_couplers_to_m06_couplers_BRESP;
  assign S_AXI_bvalid = m06_couplers_to_m06_couplers_BVALID;
  assign S_AXI_rdata[31:0] = m06_couplers_to_m06_couplers_RDATA;
  assign S_AXI_rresp[1:0] = m06_couplers_to_m06_couplers_RRESP;
  assign S_AXI_rvalid = m06_couplers_to_m06_couplers_RVALID;
  assign S_AXI_wready = m06_couplers_to_m06_couplers_WREADY;
  assign m06_couplers_to_m06_couplers_ARADDR = S_AXI_araddr[31:0];
  assign m06_couplers_to_m06_couplers_ARREADY = M_AXI_arready;
  assign m06_couplers_to_m06_couplers_ARVALID = S_AXI_arvalid;
  assign m06_couplers_to_m06_couplers_AWADDR = S_AXI_awaddr[31:0];
  assign m06_couplers_to_m06_couplers_AWREADY = M_AXI_awready;
  assign m06_couplers_to_m06_couplers_AWVALID = S_AXI_awvalid;
  assign m06_couplers_to_m06_couplers_BREADY = S_AXI_bready;
  assign m06_couplers_to_m06_couplers_BRESP = M_AXI_bresp[1:0];
  assign m06_couplers_to_m06_couplers_BVALID = M_AXI_bvalid;
  assign m06_couplers_to_m06_couplers_RDATA = M_AXI_rdata[31:0];
  assign m06_couplers_to_m06_couplers_RREADY = S_AXI_rready;
  assign m06_couplers_to_m06_couplers_RRESP = M_AXI_rresp[1:0];
  assign m06_couplers_to_m06_couplers_RVALID = M_AXI_rvalid;
  assign m06_couplers_to_m06_couplers_WDATA = S_AXI_wdata[31:0];
  assign m06_couplers_to_m06_couplers_WREADY = M_AXI_wready;
  assign m06_couplers_to_m06_couplers_WSTRB = S_AXI_wstrb[3:0];
  assign m06_couplers_to_m06_couplers_WVALID = S_AXI_wvalid;
endmodule

module m07_couplers_imp_GGZLP2
   (M_ACLK,
    M_ARESETN,
    M_AXI_araddr,
    M_AXI_arready,
    M_AXI_arvalid,
    M_AXI_awaddr,
    M_AXI_awready,
    M_AXI_awvalid,
    M_AXI_bready,
    M_AXI_bresp,
    M_AXI_bvalid,
    M_AXI_rdata,
    M_AXI_rready,
    M_AXI_rresp,
    M_AXI_rvalid,
    M_AXI_wdata,
    M_AXI_wready,
    M_AXI_wstrb,
    M_AXI_wvalid,
    S_ACLK,
    S_ARESETN,
    S_AXI_araddr,
    S_AXI_arready,
    S_AXI_arvalid,
    S_AXI_awaddr,
    S_AXI_awready,
    S_AXI_awvalid,
    S_AXI_bready,
    S_AXI_bresp,
    S_AXI_bvalid,
    S_AXI_rdata,
    S_AXI_rready,
    S_AXI_rresp,
    S_AXI_rvalid,
    S_AXI_wdata,
    S_AXI_wready,
    S_AXI_wstrb,
    S_AXI_wvalid);
  input M_ACLK;
  input [0:0]M_ARESETN;
  output [31:0]M_AXI_araddr;
  input M_AXI_arready;
  output M_AXI_arvalid;
  output [31:0]M_AXI_awaddr;
  input M_AXI_awready;
  output M_AXI_awvalid;
  output M_AXI_bready;
  input [1:0]M_AXI_bresp;
  input M_AXI_bvalid;
  input [31:0]M_AXI_rdata;
  output M_AXI_rready;
  input [1:0]M_AXI_rresp;
  input M_AXI_rvalid;
  output [31:0]M_AXI_wdata;
  input M_AXI_wready;
  output [3:0]M_AXI_wstrb;
  output M_AXI_wvalid;
  input S_ACLK;
  input [0:0]S_ARESETN;
  input [31:0]S_AXI_araddr;
  output S_AXI_arready;
  input S_AXI_arvalid;
  input [31:0]S_AXI_awaddr;
  output S_AXI_awready;
  input S_AXI_awvalid;
  input S_AXI_bready;
  output [1:0]S_AXI_bresp;
  output S_AXI_bvalid;
  output [31:0]S_AXI_rdata;
  input S_AXI_rready;
  output [1:0]S_AXI_rresp;
  output S_AXI_rvalid;
  input [31:0]S_AXI_wdata;
  output S_AXI_wready;
  input [3:0]S_AXI_wstrb;
  input S_AXI_wvalid;

  wire [31:0]m07_couplers_to_m07_couplers_ARADDR;
  wire m07_couplers_to_m07_couplers_ARREADY;
  wire m07_couplers_to_m07_couplers_ARVALID;
  wire [31:0]m07_couplers_to_m07_couplers_AWADDR;
  wire m07_couplers_to_m07_couplers_AWREADY;
  wire m07_couplers_to_m07_couplers_AWVALID;
  wire m07_couplers_to_m07_couplers_BREADY;
  wire [1:0]m07_couplers_to_m07_couplers_BRESP;
  wire m07_couplers_to_m07_couplers_BVALID;
  wire [31:0]m07_couplers_to_m07_couplers_RDATA;
  wire m07_couplers_to_m07_couplers_RREADY;
  wire [1:0]m07_couplers_to_m07_couplers_RRESP;
  wire m07_couplers_to_m07_couplers_RVALID;
  wire [31:0]m07_couplers_to_m07_couplers_WDATA;
  wire m07_couplers_to_m07_couplers_WREADY;
  wire [3:0]m07_couplers_to_m07_couplers_WSTRB;
  wire m07_couplers_to_m07_couplers_WVALID;

  assign M_AXI_araddr[31:0] = m07_couplers_to_m07_couplers_ARADDR;
  assign M_AXI_arvalid = m07_couplers_to_m07_couplers_ARVALID;
  assign M_AXI_awaddr[31:0] = m07_couplers_to_m07_couplers_AWADDR;
  assign M_AXI_awvalid = m07_couplers_to_m07_couplers_AWVALID;
  assign M_AXI_bready = m07_couplers_to_m07_couplers_BREADY;
  assign M_AXI_rready = m07_couplers_to_m07_couplers_RREADY;
  assign M_AXI_wdata[31:0] = m07_couplers_to_m07_couplers_WDATA;
  assign M_AXI_wstrb[3:0] = m07_couplers_to_m07_couplers_WSTRB;
  assign M_AXI_wvalid = m07_couplers_to_m07_couplers_WVALID;
  assign S_AXI_arready = m07_couplers_to_m07_couplers_ARREADY;
  assign S_AXI_awready = m07_couplers_to_m07_couplers_AWREADY;
  assign S_AXI_bresp[1:0] = m07_couplers_to_m07_couplers_BRESP;
  assign S_AXI_bvalid = m07_couplers_to_m07_couplers_BVALID;
  assign S_AXI_rdata[31:0] = m07_couplers_to_m07_couplers_RDATA;
  assign S_AXI_rresp[1:0] = m07_couplers_to_m07_couplers_RRESP;
  assign S_AXI_rvalid = m07_couplers_to_m07_couplers_RVALID;
  assign S_AXI_wready = m07_couplers_to_m07_couplers_WREADY;
  assign m07_couplers_to_m07_couplers_ARADDR = S_AXI_araddr[31:0];
  assign m07_couplers_to_m07_couplers_ARREADY = M_AXI_arready;
  assign m07_couplers_to_m07_couplers_ARVALID = S_AXI_arvalid;
  assign m07_couplers_to_m07_couplers_AWADDR = S_AXI_awaddr[31:0];
  assign m07_couplers_to_m07_couplers_AWREADY = M_AXI_awready;
  assign m07_couplers_to_m07_couplers_AWVALID = S_AXI_awvalid;
  assign m07_couplers_to_m07_couplers_BREADY = S_AXI_bready;
  assign m07_couplers_to_m07_couplers_BRESP = M_AXI_bresp[1:0];
  assign m07_couplers_to_m07_couplers_BVALID = M_AXI_bvalid;
  assign m07_couplers_to_m07_couplers_RDATA = M_AXI_rdata[31:0];
  assign m07_couplers_to_m07_couplers_RREADY = S_AXI_rready;
  assign m07_couplers_to_m07_couplers_RRESP = M_AXI_rresp[1:0];
  assign m07_couplers_to_m07_couplers_RVALID = M_AXI_rvalid;
  assign m07_couplers_to_m07_couplers_WDATA = S_AXI_wdata[31:0];
  assign m07_couplers_to_m07_couplers_WREADY = M_AXI_wready;
  assign m07_couplers_to_m07_couplers_WSTRB = S_AXI_wstrb[3:0];
  assign m07_couplers_to_m07_couplers_WVALID = S_AXI_wvalid;
endmodule

module m08_couplers_imp_1EP4JRG
   (M_ACLK,
    M_ARESETN,
    M_AXI_araddr,
    M_AXI_arprot,
    M_AXI_arready,
    M_AXI_arvalid,
    M_AXI_awaddr,
    M_AXI_awprot,
    M_AXI_awready,
    M_AXI_awvalid,
    M_AXI_bready,
    M_AXI_bresp,
    M_AXI_bvalid,
    M_AXI_rdata,
    M_AXI_rready,
    M_AXI_rresp,
    M_AXI_rvalid,
    M_AXI_wdata,
    M_AXI_wready,
    M_AXI_wstrb,
    M_AXI_wvalid,
    S_ACLK,
    S_ARESETN,
    S_AXI_araddr,
    S_AXI_arprot,
    S_AXI_arready,
    S_AXI_arvalid,
    S_AXI_awaddr,
    S_AXI_awprot,
    S_AXI_awready,
    S_AXI_awvalid,
    S_AXI_bready,
    S_AXI_bresp,
    S_AXI_bvalid,
    S_AXI_rdata,
    S_AXI_rready,
    S_AXI_rresp,
    S_AXI_rvalid,
    S_AXI_wdata,
    S_AXI_wready,
    S_AXI_wstrb,
    S_AXI_wvalid);
  input M_ACLK;
  input [0:0]M_ARESETN;
  output [31:0]M_AXI_araddr;
  output [2:0]M_AXI_arprot;
  input M_AXI_arready;
  output M_AXI_arvalid;
  output [31:0]M_AXI_awaddr;
  output [2:0]M_AXI_awprot;
  input M_AXI_awready;
  output M_AXI_awvalid;
  output M_AXI_bready;
  input [1:0]M_AXI_bresp;
  input M_AXI_bvalid;
  input [31:0]M_AXI_rdata;
  output M_AXI_rready;
  input [1:0]M_AXI_rresp;
  input M_AXI_rvalid;
  output [31:0]M_AXI_wdata;
  input M_AXI_wready;
  output [3:0]M_AXI_wstrb;
  output M_AXI_wvalid;
  input S_ACLK;
  input [0:0]S_ARESETN;
  input [31:0]S_AXI_araddr;
  input [2:0]S_AXI_arprot;
  output S_AXI_arready;
  input S_AXI_arvalid;
  input [31:0]S_AXI_awaddr;
  input [2:0]S_AXI_awprot;
  output S_AXI_awready;
  input S_AXI_awvalid;
  input S_AXI_bready;
  output [1:0]S_AXI_bresp;
  output S_AXI_bvalid;
  output [31:0]S_AXI_rdata;
  input S_AXI_rready;
  output [1:0]S_AXI_rresp;
  output S_AXI_rvalid;
  input [31:0]S_AXI_wdata;
  output S_AXI_wready;
  input [3:0]S_AXI_wstrb;
  input S_AXI_wvalid;

  wire [31:0]m08_couplers_to_m08_couplers_ARADDR;
  wire [2:0]m08_couplers_to_m08_couplers_ARPROT;
  wire m08_couplers_to_m08_couplers_ARREADY;
  wire m08_couplers_to_m08_couplers_ARVALID;
  wire [31:0]m08_couplers_to_m08_couplers_AWADDR;
  wire [2:0]m08_couplers_to_m08_couplers_AWPROT;
  wire m08_couplers_to_m08_couplers_AWREADY;
  wire m08_couplers_to_m08_couplers_AWVALID;
  wire m08_couplers_to_m08_couplers_BREADY;
  wire [1:0]m08_couplers_to_m08_couplers_BRESP;
  wire m08_couplers_to_m08_couplers_BVALID;
  wire [31:0]m08_couplers_to_m08_couplers_RDATA;
  wire m08_couplers_to_m08_couplers_RREADY;
  wire [1:0]m08_couplers_to_m08_couplers_RRESP;
  wire m08_couplers_to_m08_couplers_RVALID;
  wire [31:0]m08_couplers_to_m08_couplers_WDATA;
  wire m08_couplers_to_m08_couplers_WREADY;
  wire [3:0]m08_couplers_to_m08_couplers_WSTRB;
  wire m08_couplers_to_m08_couplers_WVALID;

  assign M_AXI_araddr[31:0] = m08_couplers_to_m08_couplers_ARADDR;
  assign M_AXI_arprot[2:0] = m08_couplers_to_m08_couplers_ARPROT;
  assign M_AXI_arvalid = m08_couplers_to_m08_couplers_ARVALID;
  assign M_AXI_awaddr[31:0] = m08_couplers_to_m08_couplers_AWADDR;
  assign M_AXI_awprot[2:0] = m08_couplers_to_m08_couplers_AWPROT;
  assign M_AXI_awvalid = m08_couplers_to_m08_couplers_AWVALID;
  assign M_AXI_bready = m08_couplers_to_m08_couplers_BREADY;
  assign M_AXI_rready = m08_couplers_to_m08_couplers_RREADY;
  assign M_AXI_wdata[31:0] = m08_couplers_to_m08_couplers_WDATA;
  assign M_AXI_wstrb[3:0] = m08_couplers_to_m08_couplers_WSTRB;
  assign M_AXI_wvalid = m08_couplers_to_m08_couplers_WVALID;
  assign S_AXI_arready = m08_couplers_to_m08_couplers_ARREADY;
  assign S_AXI_awready = m08_couplers_to_m08_couplers_AWREADY;
  assign S_AXI_bresp[1:0] = m08_couplers_to_m08_couplers_BRESP;
  assign S_AXI_bvalid = m08_couplers_to_m08_couplers_BVALID;
  assign S_AXI_rdata[31:0] = m08_couplers_to_m08_couplers_RDATA;
  assign S_AXI_rresp[1:0] = m08_couplers_to_m08_couplers_RRESP;
  assign S_AXI_rvalid = m08_couplers_to_m08_couplers_RVALID;
  assign S_AXI_wready = m08_couplers_to_m08_couplers_WREADY;
  assign m08_couplers_to_m08_couplers_ARADDR = S_AXI_araddr[31:0];
  assign m08_couplers_to_m08_couplers_ARPROT = S_AXI_arprot[2:0];
  assign m08_couplers_to_m08_couplers_ARREADY = M_AXI_arready;
  assign m08_couplers_to_m08_couplers_ARVALID = S_AXI_arvalid;
  assign m08_couplers_to_m08_couplers_AWADDR = S_AXI_awaddr[31:0];
  assign m08_couplers_to_m08_couplers_AWPROT = S_AXI_awprot[2:0];
  assign m08_couplers_to_m08_couplers_AWREADY = M_AXI_awready;
  assign m08_couplers_to_m08_couplers_AWVALID = S_AXI_awvalid;
  assign m08_couplers_to_m08_couplers_BREADY = S_AXI_bready;
  assign m08_couplers_to_m08_couplers_BRESP = M_AXI_bresp[1:0];
  assign m08_couplers_to_m08_couplers_BVALID = M_AXI_bvalid;
  assign m08_couplers_to_m08_couplers_RDATA = M_AXI_rdata[31:0];
  assign m08_couplers_to_m08_couplers_RREADY = S_AXI_rready;
  assign m08_couplers_to_m08_couplers_RRESP = M_AXI_rresp[1:0];
  assign m08_couplers_to_m08_couplers_RVALID = M_AXI_rvalid;
  assign m08_couplers_to_m08_couplers_WDATA = S_AXI_wdata[31:0];
  assign m08_couplers_to_m08_couplers_WREADY = M_AXI_wready;
  assign m08_couplers_to_m08_couplers_WSTRB = S_AXI_wstrb[3:0];
  assign m08_couplers_to_m08_couplers_WVALID = S_AXI_wvalid;
endmodule

module m09_couplers_imp_MNQE1N
   (M_ACLK,
    M_ARESETN,
    M_AXI_araddr,
    M_AXI_arready,
    M_AXI_arvalid,
    M_AXI_awaddr,
    M_AXI_awready,
    M_AXI_awvalid,
    M_AXI_bready,
    M_AXI_bresp,
    M_AXI_bvalid,
    M_AXI_rdata,
    M_AXI_rready,
    M_AXI_rresp,
    M_AXI_rvalid,
    M_AXI_wdata,
    M_AXI_wready,
    M_AXI_wstrb,
    M_AXI_wvalid,
    S_ACLK,
    S_ARESETN,
    S_AXI_araddr,
    S_AXI_arready,
    S_AXI_arvalid,
    S_AXI_awaddr,
    S_AXI_awready,
    S_AXI_awvalid,
    S_AXI_bready,
    S_AXI_bresp,
    S_AXI_bvalid,
    S_AXI_rdata,
    S_AXI_rready,
    S_AXI_rresp,
    S_AXI_rvalid,
    S_AXI_wdata,
    S_AXI_wready,
    S_AXI_wstrb,
    S_AXI_wvalid);
  input M_ACLK;
  input [0:0]M_ARESETN;
  output [31:0]M_AXI_araddr;
  input M_AXI_arready;
  output M_AXI_arvalid;
  output [31:0]M_AXI_awaddr;
  input M_AXI_awready;
  output M_AXI_awvalid;
  output M_AXI_bready;
  input [1:0]M_AXI_bresp;
  input M_AXI_bvalid;
  input [31:0]M_AXI_rdata;
  output M_AXI_rready;
  input [1:0]M_AXI_rresp;
  input M_AXI_rvalid;
  output [31:0]M_AXI_wdata;
  input M_AXI_wready;
  output [3:0]M_AXI_wstrb;
  output M_AXI_wvalid;
  input S_ACLK;
  input [0:0]S_ARESETN;
  input [31:0]S_AXI_araddr;
  output S_AXI_arready;
  input S_AXI_arvalid;
  input [31:0]S_AXI_awaddr;
  output S_AXI_awready;
  input S_AXI_awvalid;
  input S_AXI_bready;
  output [1:0]S_AXI_bresp;
  output S_AXI_bvalid;
  output [31:0]S_AXI_rdata;
  input S_AXI_rready;
  output [1:0]S_AXI_rresp;
  output S_AXI_rvalid;
  input [31:0]S_AXI_wdata;
  output S_AXI_wready;
  input [3:0]S_AXI_wstrb;
  input S_AXI_wvalid;

  wire [31:0]m09_couplers_to_m09_couplers_ARADDR;
  wire m09_couplers_to_m09_couplers_ARREADY;
  wire m09_couplers_to_m09_couplers_ARVALID;
  wire [31:0]m09_couplers_to_m09_couplers_AWADDR;
  wire m09_couplers_to_m09_couplers_AWREADY;
  wire m09_couplers_to_m09_couplers_AWVALID;
  wire m09_couplers_to_m09_couplers_BREADY;
  wire [1:0]m09_couplers_to_m09_couplers_BRESP;
  wire m09_couplers_to_m09_couplers_BVALID;
  wire [31:0]m09_couplers_to_m09_couplers_RDATA;
  wire m09_couplers_to_m09_couplers_RREADY;
  wire [1:0]m09_couplers_to_m09_couplers_RRESP;
  wire m09_couplers_to_m09_couplers_RVALID;
  wire [31:0]m09_couplers_to_m09_couplers_WDATA;
  wire m09_couplers_to_m09_couplers_WREADY;
  wire [3:0]m09_couplers_to_m09_couplers_WSTRB;
  wire m09_couplers_to_m09_couplers_WVALID;

  assign M_AXI_araddr[31:0] = m09_couplers_to_m09_couplers_ARADDR;
  assign M_AXI_arvalid = m09_couplers_to_m09_couplers_ARVALID;
  assign M_AXI_awaddr[31:0] = m09_couplers_to_m09_couplers_AWADDR;
  assign M_AXI_awvalid = m09_couplers_to_m09_couplers_AWVALID;
  assign M_AXI_bready = m09_couplers_to_m09_couplers_BREADY;
  assign M_AXI_rready = m09_couplers_to_m09_couplers_RREADY;
  assign M_AXI_wdata[31:0] = m09_couplers_to_m09_couplers_WDATA;
  assign M_AXI_wstrb[3:0] = m09_couplers_to_m09_couplers_WSTRB;
  assign M_AXI_wvalid = m09_couplers_to_m09_couplers_WVALID;
  assign S_AXI_arready = m09_couplers_to_m09_couplers_ARREADY;
  assign S_AXI_awready = m09_couplers_to_m09_couplers_AWREADY;
  assign S_AXI_bresp[1:0] = m09_couplers_to_m09_couplers_BRESP;
  assign S_AXI_bvalid = m09_couplers_to_m09_couplers_BVALID;
  assign S_AXI_rdata[31:0] = m09_couplers_to_m09_couplers_RDATA;
  assign S_AXI_rresp[1:0] = m09_couplers_to_m09_couplers_RRESP;
  assign S_AXI_rvalid = m09_couplers_to_m09_couplers_RVALID;
  assign S_AXI_wready = m09_couplers_to_m09_couplers_WREADY;
  assign m09_couplers_to_m09_couplers_ARADDR = S_AXI_araddr[31:0];
  assign m09_couplers_to_m09_couplers_ARREADY = M_AXI_arready;
  assign m09_couplers_to_m09_couplers_ARVALID = S_AXI_arvalid;
  assign m09_couplers_to_m09_couplers_AWADDR = S_AXI_awaddr[31:0];
  assign m09_couplers_to_m09_couplers_AWREADY = M_AXI_awready;
  assign m09_couplers_to_m09_couplers_AWVALID = S_AXI_awvalid;
  assign m09_couplers_to_m09_couplers_BREADY = S_AXI_bready;
  assign m09_couplers_to_m09_couplers_BRESP = M_AXI_bresp[1:0];
  assign m09_couplers_to_m09_couplers_BVALID = M_AXI_bvalid;
  assign m09_couplers_to_m09_couplers_RDATA = M_AXI_rdata[31:0];
  assign m09_couplers_to_m09_couplers_RREADY = S_AXI_rready;
  assign m09_couplers_to_m09_couplers_RRESP = M_AXI_rresp[1:0];
  assign m09_couplers_to_m09_couplers_RVALID = M_AXI_rvalid;
  assign m09_couplers_to_m09_couplers_WDATA = S_AXI_wdata[31:0];
  assign m09_couplers_to_m09_couplers_WREADY = M_AXI_wready;
  assign m09_couplers_to_m09_couplers_WSTRB = S_AXI_wstrb[3:0];
  assign m09_couplers_to_m09_couplers_WVALID = S_AXI_wvalid;
endmodule

module m10_couplers_imp_13554VL
   (M_ACLK,
    M_ARESETN,
    M_AXI_araddr,
    M_AXI_arready,
    M_AXI_arvalid,
    M_AXI_awaddr,
    M_AXI_awready,
    M_AXI_awvalid,
    M_AXI_bready,
    M_AXI_bresp,
    M_AXI_bvalid,
    M_AXI_rdata,
    M_AXI_rready,
    M_AXI_rresp,
    M_AXI_rvalid,
    M_AXI_wdata,
    M_AXI_wready,
    M_AXI_wvalid,
    S_ACLK,
    S_ARESETN,
    S_AXI_araddr,
    S_AXI_arready,
    S_AXI_arvalid,
    S_AXI_awaddr,
    S_AXI_awready,
    S_AXI_awvalid,
    S_AXI_bready,
    S_AXI_bresp,
    S_AXI_bvalid,
    S_AXI_rdata,
    S_AXI_rready,
    S_AXI_rresp,
    S_AXI_rvalid,
    S_AXI_wdata,
    S_AXI_wready,
    S_AXI_wvalid);
  input M_ACLK;
  input [0:0]M_ARESETN;
  output [31:0]M_AXI_araddr;
  input M_AXI_arready;
  output M_AXI_arvalid;
  output [31:0]M_AXI_awaddr;
  input M_AXI_awready;
  output M_AXI_awvalid;
  output M_AXI_bready;
  input [1:0]M_AXI_bresp;
  input M_AXI_bvalid;
  input [31:0]M_AXI_rdata;
  output M_AXI_rready;
  input [1:0]M_AXI_rresp;
  input M_AXI_rvalid;
  output [31:0]M_AXI_wdata;
  input M_AXI_wready;
  output M_AXI_wvalid;
  input S_ACLK;
  input [0:0]S_ARESETN;
  input [31:0]S_AXI_araddr;
  output S_AXI_arready;
  input S_AXI_arvalid;
  input [31:0]S_AXI_awaddr;
  output S_AXI_awready;
  input S_AXI_awvalid;
  input S_AXI_bready;
  output [1:0]S_AXI_bresp;
  output S_AXI_bvalid;
  output [31:0]S_AXI_rdata;
  input S_AXI_rready;
  output [1:0]S_AXI_rresp;
  output S_AXI_rvalid;
  input [31:0]S_AXI_wdata;
  output S_AXI_wready;
  input S_AXI_wvalid;

  wire [31:0]m10_couplers_to_m10_couplers_ARADDR;
  wire m10_couplers_to_m10_couplers_ARREADY;
  wire m10_couplers_to_m10_couplers_ARVALID;
  wire [31:0]m10_couplers_to_m10_couplers_AWADDR;
  wire m10_couplers_to_m10_couplers_AWREADY;
  wire m10_couplers_to_m10_couplers_AWVALID;
  wire m10_couplers_to_m10_couplers_BREADY;
  wire [1:0]m10_couplers_to_m10_couplers_BRESP;
  wire m10_couplers_to_m10_couplers_BVALID;
  wire [31:0]m10_couplers_to_m10_couplers_RDATA;
  wire m10_couplers_to_m10_couplers_RREADY;
  wire [1:0]m10_couplers_to_m10_couplers_RRESP;
  wire m10_couplers_to_m10_couplers_RVALID;
  wire [31:0]m10_couplers_to_m10_couplers_WDATA;
  wire m10_couplers_to_m10_couplers_WREADY;
  wire m10_couplers_to_m10_couplers_WVALID;

  assign M_AXI_araddr[31:0] = m10_couplers_to_m10_couplers_ARADDR;
  assign M_AXI_arvalid = m10_couplers_to_m10_couplers_ARVALID;
  assign M_AXI_awaddr[31:0] = m10_couplers_to_m10_couplers_AWADDR;
  assign M_AXI_awvalid = m10_couplers_to_m10_couplers_AWVALID;
  assign M_AXI_bready = m10_couplers_to_m10_couplers_BREADY;
  assign M_AXI_rready = m10_couplers_to_m10_couplers_RREADY;
  assign M_AXI_wdata[31:0] = m10_couplers_to_m10_couplers_WDATA;
  assign M_AXI_wvalid = m10_couplers_to_m10_couplers_WVALID;
  assign S_AXI_arready = m10_couplers_to_m10_couplers_ARREADY;
  assign S_AXI_awready = m10_couplers_to_m10_couplers_AWREADY;
  assign S_AXI_bresp[1:0] = m10_couplers_to_m10_couplers_BRESP;
  assign S_AXI_bvalid = m10_couplers_to_m10_couplers_BVALID;
  assign S_AXI_rdata[31:0] = m10_couplers_to_m10_couplers_RDATA;
  assign S_AXI_rresp[1:0] = m10_couplers_to_m10_couplers_RRESP;
  assign S_AXI_rvalid = m10_couplers_to_m10_couplers_RVALID;
  assign S_AXI_wready = m10_couplers_to_m10_couplers_WREADY;
  assign m10_couplers_to_m10_couplers_ARADDR = S_AXI_araddr[31:0];
  assign m10_couplers_to_m10_couplers_ARREADY = M_AXI_arready;
  assign m10_couplers_to_m10_couplers_ARVALID = S_AXI_arvalid;
  assign m10_couplers_to_m10_couplers_AWADDR = S_AXI_awaddr[31:0];
  assign m10_couplers_to_m10_couplers_AWREADY = M_AXI_awready;
  assign m10_couplers_to_m10_couplers_AWVALID = S_AXI_awvalid;
  assign m10_couplers_to_m10_couplers_BREADY = S_AXI_bready;
  assign m10_couplers_to_m10_couplers_BRESP = M_AXI_bresp[1:0];
  assign m10_couplers_to_m10_couplers_BVALID = M_AXI_bvalid;
  assign m10_couplers_to_m10_couplers_RDATA = M_AXI_rdata[31:0];
  assign m10_couplers_to_m10_couplers_RREADY = S_AXI_rready;
  assign m10_couplers_to_m10_couplers_RRESP = M_AXI_rresp[1:0];
  assign m10_couplers_to_m10_couplers_RVALID = M_AXI_rvalid;
  assign m10_couplers_to_m10_couplers_WDATA = S_AXI_wdata[31:0];
  assign m10_couplers_to_m10_couplers_WREADY = M_AXI_wready;
  assign m10_couplers_to_m10_couplers_WVALID = S_AXI_wvalid;
endmodule

module m11_couplers_imp_TFEMW6
   (M_ACLK,
    M_ARESETN,
    M_AXI_araddr,
    M_AXI_arready,
    M_AXI_arvalid,
    M_AXI_awaddr,
    M_AXI_awready,
    M_AXI_awvalid,
    M_AXI_bready,
    M_AXI_bresp,
    M_AXI_bvalid,
    M_AXI_rdata,
    M_AXI_rready,
    M_AXI_rresp,
    M_AXI_rvalid,
    M_AXI_wdata,
    M_AXI_wready,
    M_AXI_wstrb,
    M_AXI_wvalid,
    S_ACLK,
    S_ARESETN,
    S_AXI_araddr,
    S_AXI_arready,
    S_AXI_arvalid,
    S_AXI_awaddr,
    S_AXI_awready,
    S_AXI_awvalid,
    S_AXI_bready,
    S_AXI_bresp,
    S_AXI_bvalid,
    S_AXI_rdata,
    S_AXI_rready,
    S_AXI_rresp,
    S_AXI_rvalid,
    S_AXI_wdata,
    S_AXI_wready,
    S_AXI_wstrb,
    S_AXI_wvalid);
  input M_ACLK;
  input [0:0]M_ARESETN;
  output [31:0]M_AXI_araddr;
  input M_AXI_arready;
  output M_AXI_arvalid;
  output [31:0]M_AXI_awaddr;
  input M_AXI_awready;
  output M_AXI_awvalid;
  output M_AXI_bready;
  input [1:0]M_AXI_bresp;
  input M_AXI_bvalid;
  input [31:0]M_AXI_rdata;
  output M_AXI_rready;
  input [1:0]M_AXI_rresp;
  input M_AXI_rvalid;
  output [31:0]M_AXI_wdata;
  input M_AXI_wready;
  output [3:0]M_AXI_wstrb;
  output M_AXI_wvalid;
  input S_ACLK;
  input [0:0]S_ARESETN;
  input [31:0]S_AXI_araddr;
  output S_AXI_arready;
  input S_AXI_arvalid;
  input [31:0]S_AXI_awaddr;
  output S_AXI_awready;
  input S_AXI_awvalid;
  input S_AXI_bready;
  output [1:0]S_AXI_bresp;
  output S_AXI_bvalid;
  output [31:0]S_AXI_rdata;
  input S_AXI_rready;
  output [1:0]S_AXI_rresp;
  output S_AXI_rvalid;
  input [31:0]S_AXI_wdata;
  output S_AXI_wready;
  input [3:0]S_AXI_wstrb;
  input S_AXI_wvalid;

  wire [31:0]m11_couplers_to_m11_couplers_ARADDR;
  wire m11_couplers_to_m11_couplers_ARREADY;
  wire m11_couplers_to_m11_couplers_ARVALID;
  wire [31:0]m11_couplers_to_m11_couplers_AWADDR;
  wire m11_couplers_to_m11_couplers_AWREADY;
  wire m11_couplers_to_m11_couplers_AWVALID;
  wire m11_couplers_to_m11_couplers_BREADY;
  wire [1:0]m11_couplers_to_m11_couplers_BRESP;
  wire m11_couplers_to_m11_couplers_BVALID;
  wire [31:0]m11_couplers_to_m11_couplers_RDATA;
  wire m11_couplers_to_m11_couplers_RREADY;
  wire [1:0]m11_couplers_to_m11_couplers_RRESP;
  wire m11_couplers_to_m11_couplers_RVALID;
  wire [31:0]m11_couplers_to_m11_couplers_WDATA;
  wire m11_couplers_to_m11_couplers_WREADY;
  wire [3:0]m11_couplers_to_m11_couplers_WSTRB;
  wire m11_couplers_to_m11_couplers_WVALID;

  assign M_AXI_araddr[31:0] = m11_couplers_to_m11_couplers_ARADDR;
  assign M_AXI_arvalid = m11_couplers_to_m11_couplers_ARVALID;
  assign M_AXI_awaddr[31:0] = m11_couplers_to_m11_couplers_AWADDR;
  assign M_AXI_awvalid = m11_couplers_to_m11_couplers_AWVALID;
  assign M_AXI_bready = m11_couplers_to_m11_couplers_BREADY;
  assign M_AXI_rready = m11_couplers_to_m11_couplers_RREADY;
  assign M_AXI_wdata[31:0] = m11_couplers_to_m11_couplers_WDATA;
  assign M_AXI_wstrb[3:0] = m11_couplers_to_m11_couplers_WSTRB;
  assign M_AXI_wvalid = m11_couplers_to_m11_couplers_WVALID;
  assign S_AXI_arready = m11_couplers_to_m11_couplers_ARREADY;
  assign S_AXI_awready = m11_couplers_to_m11_couplers_AWREADY;
  assign S_AXI_bresp[1:0] = m11_couplers_to_m11_couplers_BRESP;
  assign S_AXI_bvalid = m11_couplers_to_m11_couplers_BVALID;
  assign S_AXI_rdata[31:0] = m11_couplers_to_m11_couplers_RDATA;
  assign S_AXI_rresp[1:0] = m11_couplers_to_m11_couplers_RRESP;
  assign S_AXI_rvalid = m11_couplers_to_m11_couplers_RVALID;
  assign S_AXI_wready = m11_couplers_to_m11_couplers_WREADY;
  assign m11_couplers_to_m11_couplers_ARADDR = S_AXI_araddr[31:0];
  assign m11_couplers_to_m11_couplers_ARREADY = M_AXI_arready;
  assign m11_couplers_to_m11_couplers_ARVALID = S_AXI_arvalid;
  assign m11_couplers_to_m11_couplers_AWADDR = S_AXI_awaddr[31:0];
  assign m11_couplers_to_m11_couplers_AWREADY = M_AXI_awready;
  assign m11_couplers_to_m11_couplers_AWVALID = S_AXI_awvalid;
  assign m11_couplers_to_m11_couplers_BREADY = S_AXI_bready;
  assign m11_couplers_to_m11_couplers_BRESP = M_AXI_bresp[1:0];
  assign m11_couplers_to_m11_couplers_BVALID = M_AXI_bvalid;
  assign m11_couplers_to_m11_couplers_RDATA = M_AXI_rdata[31:0];
  assign m11_couplers_to_m11_couplers_RREADY = S_AXI_rready;
  assign m11_couplers_to_m11_couplers_RRESP = M_AXI_rresp[1:0];
  assign m11_couplers_to_m11_couplers_RVALID = M_AXI_rvalid;
  assign m11_couplers_to_m11_couplers_WDATA = S_AXI_wdata[31:0];
  assign m11_couplers_to_m11_couplers_WREADY = M_AXI_wready;
  assign m11_couplers_to_m11_couplers_WSTRB = S_AXI_wstrb[3:0];
  assign m11_couplers_to_m11_couplers_WVALID = S_AXI_wvalid;
endmodule

module m12_couplers_imp_169QM5A
   (M_ACLK,
    M_ARESETN,
    M_AXI_araddr,
    M_AXI_arready,
    M_AXI_arvalid,
    M_AXI_awaddr,
    M_AXI_awready,
    M_AXI_awvalid,
    M_AXI_bready,
    M_AXI_bresp,
    M_AXI_bvalid,
    M_AXI_rdata,
    M_AXI_rready,
    M_AXI_rresp,
    M_AXI_rvalid,
    M_AXI_wdata,
    M_AXI_wready,
    M_AXI_wstrb,
    M_AXI_wvalid,
    S_ACLK,
    S_ARESETN,
    S_AXI_araddr,
    S_AXI_arready,
    S_AXI_arvalid,
    S_AXI_awaddr,
    S_AXI_awready,
    S_AXI_awvalid,
    S_AXI_bready,
    S_AXI_bresp,
    S_AXI_bvalid,
    S_AXI_rdata,
    S_AXI_rready,
    S_AXI_rresp,
    S_AXI_rvalid,
    S_AXI_wdata,
    S_AXI_wready,
    S_AXI_wstrb,
    S_AXI_wvalid);
  input M_ACLK;
  input [0:0]M_ARESETN;
  output [31:0]M_AXI_araddr;
  input M_AXI_arready;
  output M_AXI_arvalid;
  output [31:0]M_AXI_awaddr;
  input M_AXI_awready;
  output M_AXI_awvalid;
  output M_AXI_bready;
  input [1:0]M_AXI_bresp;
  input M_AXI_bvalid;
  input [31:0]M_AXI_rdata;
  output M_AXI_rready;
  input [1:0]M_AXI_rresp;
  input M_AXI_rvalid;
  output [31:0]M_AXI_wdata;
  input M_AXI_wready;
  output [3:0]M_AXI_wstrb;
  output M_AXI_wvalid;
  input S_ACLK;
  input [0:0]S_ARESETN;
  input [31:0]S_AXI_araddr;
  output S_AXI_arready;
  input S_AXI_arvalid;
  input [31:0]S_AXI_awaddr;
  output S_AXI_awready;
  input S_AXI_awvalid;
  input S_AXI_bready;
  output [1:0]S_AXI_bresp;
  output S_AXI_bvalid;
  output [31:0]S_AXI_rdata;
  input S_AXI_rready;
  output [1:0]S_AXI_rresp;
  output S_AXI_rvalid;
  input [31:0]S_AXI_wdata;
  output S_AXI_wready;
  input [3:0]S_AXI_wstrb;
  input S_AXI_wvalid;

  wire [31:0]m12_couplers_to_m12_couplers_ARADDR;
  wire m12_couplers_to_m12_couplers_ARREADY;
  wire m12_couplers_to_m12_couplers_ARVALID;
  wire [31:0]m12_couplers_to_m12_couplers_AWADDR;
  wire m12_couplers_to_m12_couplers_AWREADY;
  wire m12_couplers_to_m12_couplers_AWVALID;
  wire m12_couplers_to_m12_couplers_BREADY;
  wire [1:0]m12_couplers_to_m12_couplers_BRESP;
  wire m12_couplers_to_m12_couplers_BVALID;
  wire [31:0]m12_couplers_to_m12_couplers_RDATA;
  wire m12_couplers_to_m12_couplers_RREADY;
  wire [1:0]m12_couplers_to_m12_couplers_RRESP;
  wire m12_couplers_to_m12_couplers_RVALID;
  wire [31:0]m12_couplers_to_m12_couplers_WDATA;
  wire m12_couplers_to_m12_couplers_WREADY;
  wire [3:0]m12_couplers_to_m12_couplers_WSTRB;
  wire m12_couplers_to_m12_couplers_WVALID;

  assign M_AXI_araddr[31:0] = m12_couplers_to_m12_couplers_ARADDR;
  assign M_AXI_arvalid = m12_couplers_to_m12_couplers_ARVALID;
  assign M_AXI_awaddr[31:0] = m12_couplers_to_m12_couplers_AWADDR;
  assign M_AXI_awvalid = m12_couplers_to_m12_couplers_AWVALID;
  assign M_AXI_bready = m12_couplers_to_m12_couplers_BREADY;
  assign M_AXI_rready = m12_couplers_to_m12_couplers_RREADY;
  assign M_AXI_wdata[31:0] = m12_couplers_to_m12_couplers_WDATA;
  assign M_AXI_wstrb[3:0] = m12_couplers_to_m12_couplers_WSTRB;
  assign M_AXI_wvalid = m12_couplers_to_m12_couplers_WVALID;
  assign S_AXI_arready = m12_couplers_to_m12_couplers_ARREADY;
  assign S_AXI_awready = m12_couplers_to_m12_couplers_AWREADY;
  assign S_AXI_bresp[1:0] = m12_couplers_to_m12_couplers_BRESP;
  assign S_AXI_bvalid = m12_couplers_to_m12_couplers_BVALID;
  assign S_AXI_rdata[31:0] = m12_couplers_to_m12_couplers_RDATA;
  assign S_AXI_rresp[1:0] = m12_couplers_to_m12_couplers_RRESP;
  assign S_AXI_rvalid = m12_couplers_to_m12_couplers_RVALID;
  assign S_AXI_wready = m12_couplers_to_m12_couplers_WREADY;
  assign m12_couplers_to_m12_couplers_ARADDR = S_AXI_araddr[31:0];
  assign m12_couplers_to_m12_couplers_ARREADY = M_AXI_arready;
  assign m12_couplers_to_m12_couplers_ARVALID = S_AXI_arvalid;
  assign m12_couplers_to_m12_couplers_AWADDR = S_AXI_awaddr[31:0];
  assign m12_couplers_to_m12_couplers_AWREADY = M_AXI_awready;
  assign m12_couplers_to_m12_couplers_AWVALID = S_AXI_awvalid;
  assign m12_couplers_to_m12_couplers_BREADY = S_AXI_bready;
  assign m12_couplers_to_m12_couplers_BRESP = M_AXI_bresp[1:0];
  assign m12_couplers_to_m12_couplers_BVALID = M_AXI_bvalid;
  assign m12_couplers_to_m12_couplers_RDATA = M_AXI_rdata[31:0];
  assign m12_couplers_to_m12_couplers_RREADY = S_AXI_rready;
  assign m12_couplers_to_m12_couplers_RRESP = M_AXI_rresp[1:0];
  assign m12_couplers_to_m12_couplers_RVALID = M_AXI_rvalid;
  assign m12_couplers_to_m12_couplers_WDATA = S_AXI_wdata[31:0];
  assign m12_couplers_to_m12_couplers_WREADY = M_AXI_wready;
  assign m12_couplers_to_m12_couplers_WSTRB = S_AXI_wstrb[3:0];
  assign m12_couplers_to_m12_couplers_WVALID = S_AXI_wvalid;
endmodule

module s00_couplers_imp_13JUP3T
   (M_AXIS_ACLK,
    M_AXIS_ARESETN,
    M_AXIS_tdata,
    M_AXIS_tkeep,
    M_AXIS_tlast,
    M_AXIS_tready,
    M_AXIS_tvalid,
    S_AXIS_ACLK,
    S_AXIS_ARESETN,
    S_AXIS_tdata,
    S_AXIS_tkeep,
    S_AXIS_tlast,
    S_AXIS_tvalid);
  input M_AXIS_ACLK;
  input [0:0]M_AXIS_ARESETN;
  output [15:0]M_AXIS_tdata;
  output [1:0]M_AXIS_tkeep;
  output M_AXIS_tlast;
  input M_AXIS_tready;
  output M_AXIS_tvalid;
  input S_AXIS_ACLK;
  input [0:0]S_AXIS_ARESETN;
  input [0:15]S_AXIS_tdata;
  input [0:1]S_AXIS_tkeep;
  input S_AXIS_tlast;
  input S_AXIS_tvalid;

  wire [31:0]AXIS_DATA_COUNT_to_S_AXIS_DATA_COUNT;
  wire [31:0]AXIS_RD_DATA_COUNT_to_S_AXIS_RD_DATA_COUNT;
  wire [31:0]AXIS_WR_DATA_COUNT_to_S_AXIS_WR_DATA_COUNT;
  wire M_AXIS_ACLK;
  wire [0:0]M_AXIS_ARESETN;
  wire S_AXIS_ACLK_1;
  wire [0:0]S_AXIS_ARESETN_1;
  wire [15:0]auto_cc_to_s00_data_fifo_TDATA;
  wire [1:0]auto_cc_to_s00_data_fifo_TKEEP;
  wire auto_cc_to_s00_data_fifo_TLAST;
  wire auto_cc_to_s00_data_fifo_TREADY;
  wire auto_cc_to_s00_data_fifo_TVALID;
  wire [15:0]auto_ss_si_r_to_auto_cc_TDATA;
  wire [1:0]auto_ss_si_r_to_auto_cc_TKEEP;
  wire auto_ss_si_r_to_auto_cc_TLAST;
  wire auto_ss_si_r_to_auto_cc_TREADY;
  wire auto_ss_si_r_to_auto_cc_TVALID;
  wire [0:15]s00_couplers_to_auto_ss_si_r_TDATA;
  wire [0:1]s00_couplers_to_auto_ss_si_r_TKEEP;
  wire s00_couplers_to_auto_ss_si_r_TLAST;
  wire s00_couplers_to_auto_ss_si_r_TVALID;
  wire [15:0]s00_data_fifo_to_s00_couplers_TDATA;
  wire [1:0]s00_data_fifo_to_s00_couplers_TKEEP;
  wire s00_data_fifo_to_s00_couplers_TLAST;
  wire s00_data_fifo_to_s00_couplers_TREADY;
  wire s00_data_fifo_to_s00_couplers_TVALID;

  assign M_AXIS_tdata[15:0] = s00_data_fifo_to_s00_couplers_TDATA;
  assign M_AXIS_tkeep[1:0] = s00_data_fifo_to_s00_couplers_TKEEP;
  assign M_AXIS_tlast = s00_data_fifo_to_s00_couplers_TLAST;
  assign M_AXIS_tvalid = s00_data_fifo_to_s00_couplers_TVALID;
  assign S_AXIS_ACLK_1 = S_AXIS_ACLK;
  assign S_AXIS_ARESETN_1 = S_AXIS_ARESETN[0];
  assign s00_couplers_to_auto_ss_si_r_TDATA = S_AXIS_tdata[0:15];
  assign s00_couplers_to_auto_ss_si_r_TKEEP = S_AXIS_tkeep[0:1];
  assign s00_couplers_to_auto_ss_si_r_TLAST = S_AXIS_tlast;
  assign s00_couplers_to_auto_ss_si_r_TVALID = S_AXIS_tvalid;
  assign s00_data_fifo_to_s00_couplers_TREADY = M_AXIS_tready;
  base_zynq_design_auto_cc_0 auto_cc
       (.m_axis_aclk(M_AXIS_ACLK),
        .m_axis_aresetn(M_AXIS_ARESETN),
        .m_axis_tdata(auto_cc_to_s00_data_fifo_TDATA),
        .m_axis_tkeep(auto_cc_to_s00_data_fifo_TKEEP),
        .m_axis_tlast(auto_cc_to_s00_data_fifo_TLAST),
        .m_axis_tready(auto_cc_to_s00_data_fifo_TREADY),
        .m_axis_tvalid(auto_cc_to_s00_data_fifo_TVALID),
        .s_axis_aclk(S_AXIS_ACLK_1),
        .s_axis_aresetn(S_AXIS_ARESETN_1),
        .s_axis_tdata(auto_ss_si_r_to_auto_cc_TDATA),
        .s_axis_tkeep(auto_ss_si_r_to_auto_cc_TKEEP),
        .s_axis_tlast(auto_ss_si_r_to_auto_cc_TLAST),
        .s_axis_tready(auto_ss_si_r_to_auto_cc_TREADY),
        .s_axis_tvalid(auto_ss_si_r_to_auto_cc_TVALID));
  base_zynq_design_auto_ss_si_r_0 auto_ss_si_r
       (.aclk(S_AXIS_ACLK_1),
        .aresetn(S_AXIS_ARESETN_1),
        .m_axis_tdata(auto_ss_si_r_to_auto_cc_TDATA),
        .m_axis_tkeep(auto_ss_si_r_to_auto_cc_TKEEP),
        .m_axis_tlast(auto_ss_si_r_to_auto_cc_TLAST),
        .m_axis_tready(auto_ss_si_r_to_auto_cc_TREADY),
        .m_axis_tvalid(auto_ss_si_r_to_auto_cc_TVALID),
        .s_axis_tdata({s00_couplers_to_auto_ss_si_r_TDATA[0],s00_couplers_to_auto_ss_si_r_TDATA[1],s00_couplers_to_auto_ss_si_r_TDATA[2],s00_couplers_to_auto_ss_si_r_TDATA[3],s00_couplers_to_auto_ss_si_r_TDATA[4],s00_couplers_to_auto_ss_si_r_TDATA[5],s00_couplers_to_auto_ss_si_r_TDATA[6],s00_couplers_to_auto_ss_si_r_TDATA[7],s00_couplers_to_auto_ss_si_r_TDATA[8],s00_couplers_to_auto_ss_si_r_TDATA[9],s00_couplers_to_auto_ss_si_r_TDATA[10],s00_couplers_to_auto_ss_si_r_TDATA[11],s00_couplers_to_auto_ss_si_r_TDATA[12],s00_couplers_to_auto_ss_si_r_TDATA[13],s00_couplers_to_auto_ss_si_r_TDATA[14],s00_couplers_to_auto_ss_si_r_TDATA[15]}),
        .s_axis_tkeep({s00_couplers_to_auto_ss_si_r_TKEEP[0],s00_couplers_to_auto_ss_si_r_TKEEP[1]}),
        .s_axis_tlast(s00_couplers_to_auto_ss_si_r_TLAST),
        .s_axis_tvalid(s00_couplers_to_auto_ss_si_r_TVALID));
  base_zynq_design_s00_data_fifo_1 s00_data_fifo
       (.axis_data_count(AXIS_DATA_COUNT_to_S_AXIS_DATA_COUNT),
        .axis_rd_data_count(AXIS_RD_DATA_COUNT_to_S_AXIS_RD_DATA_COUNT),
        .axis_wr_data_count(AXIS_WR_DATA_COUNT_to_S_AXIS_WR_DATA_COUNT),
        .m_axis_tdata(s00_data_fifo_to_s00_couplers_TDATA),
        .m_axis_tkeep(s00_data_fifo_to_s00_couplers_TKEEP),
        .m_axis_tlast(s00_data_fifo_to_s00_couplers_TLAST),
        .m_axis_tready(s00_data_fifo_to_s00_couplers_TREADY),
        .m_axis_tvalid(s00_data_fifo_to_s00_couplers_TVALID),
        .s_axis_aclk(M_AXIS_ACLK),
        .s_axis_aresetn(M_AXIS_ARESETN),
        .s_axis_tdata(auto_cc_to_s00_data_fifo_TDATA),
        .s_axis_tkeep(auto_cc_to_s00_data_fifo_TKEEP),
        .s_axis_tlast(auto_cc_to_s00_data_fifo_TLAST),
        .s_axis_tready(auto_cc_to_s00_data_fifo_TREADY),
        .s_axis_tvalid(auto_cc_to_s00_data_fifo_TVALID));
endmodule

module s00_couplers_imp_1DN61ZL
   (M_AXIS_ACLK,
    M_AXIS_ARESETN,
    M_AXIS_tdata,
    M_AXIS_tkeep,
    M_AXIS_tlast,
    M_AXIS_tready,
    M_AXIS_tvalid,
    S_AXIS_ACLK,
    S_AXIS_ARESETN,
    S_AXIS_tdata,
    S_AXIS_tkeep,
    S_AXIS_tlast,
    S_AXIS_tvalid);
  input M_AXIS_ACLK;
  input [0:0]M_AXIS_ARESETN;
  output [15:0]M_AXIS_tdata;
  output [1:0]M_AXIS_tkeep;
  output M_AXIS_tlast;
  input M_AXIS_tready;
  output M_AXIS_tvalid;
  input S_AXIS_ACLK;
  input [0:0]S_AXIS_ARESETN;
  input [0:15]S_AXIS_tdata;
  input [0:1]S_AXIS_tkeep;
  input S_AXIS_tlast;
  input S_AXIS_tvalid;

  wire [31:0]AXIS_DATA_COUNT_to_S_AXIS_DATA_COUNT;
  wire [31:0]AXIS_RD_DATA_COUNT_to_S_AXIS_RD_DATA_COUNT;
  wire [31:0]AXIS_WR_DATA_COUNT_to_S_AXIS_WR_DATA_COUNT;
  wire M_AXIS_ACLK;
  wire [0:0]M_AXIS_ARESETN;
  wire S_AXIS_ACLK_1;
  wire [0:0]S_AXIS_ARESETN_1;
  wire [15:0]auto_cc_to_s00_data_fifo_TDATA;
  wire [1:0]auto_cc_to_s00_data_fifo_TKEEP;
  wire auto_cc_to_s00_data_fifo_TLAST;
  wire auto_cc_to_s00_data_fifo_TREADY;
  wire auto_cc_to_s00_data_fifo_TVALID;
  wire [15:0]auto_ss_si_r_to_auto_cc_TDATA;
  wire [1:0]auto_ss_si_r_to_auto_cc_TKEEP;
  wire auto_ss_si_r_to_auto_cc_TLAST;
  wire auto_ss_si_r_to_auto_cc_TREADY;
  wire auto_ss_si_r_to_auto_cc_TVALID;
  wire [0:15]s00_couplers_to_auto_ss_si_r_TDATA;
  wire [0:1]s00_couplers_to_auto_ss_si_r_TKEEP;
  wire s00_couplers_to_auto_ss_si_r_TLAST;
  wire s00_couplers_to_auto_ss_si_r_TVALID;
  wire [15:0]s00_data_fifo_to_s00_couplers_TDATA;
  wire [1:0]s00_data_fifo_to_s00_couplers_TKEEP;
  wire s00_data_fifo_to_s00_couplers_TLAST;
  wire s00_data_fifo_to_s00_couplers_TREADY;
  wire s00_data_fifo_to_s00_couplers_TVALID;

  assign M_AXIS_tdata[15:0] = s00_data_fifo_to_s00_couplers_TDATA;
  assign M_AXIS_tkeep[1:0] = s00_data_fifo_to_s00_couplers_TKEEP;
  assign M_AXIS_tlast = s00_data_fifo_to_s00_couplers_TLAST;
  assign M_AXIS_tvalid = s00_data_fifo_to_s00_couplers_TVALID;
  assign S_AXIS_ACLK_1 = S_AXIS_ACLK;
  assign S_AXIS_ARESETN_1 = S_AXIS_ARESETN[0];
  assign s00_couplers_to_auto_ss_si_r_TDATA = S_AXIS_tdata[0:15];
  assign s00_couplers_to_auto_ss_si_r_TKEEP = S_AXIS_tkeep[0:1];
  assign s00_couplers_to_auto_ss_si_r_TLAST = S_AXIS_tlast;
  assign s00_couplers_to_auto_ss_si_r_TVALID = S_AXIS_tvalid;
  assign s00_data_fifo_to_s00_couplers_TREADY = M_AXIS_tready;
  base_zynq_design_auto_cc_1 auto_cc
       (.m_axis_aclk(M_AXIS_ACLK),
        .m_axis_aresetn(M_AXIS_ARESETN),
        .m_axis_tdata(auto_cc_to_s00_data_fifo_TDATA),
        .m_axis_tkeep(auto_cc_to_s00_data_fifo_TKEEP),
        .m_axis_tlast(auto_cc_to_s00_data_fifo_TLAST),
        .m_axis_tready(auto_cc_to_s00_data_fifo_TREADY),
        .m_axis_tvalid(auto_cc_to_s00_data_fifo_TVALID),
        .s_axis_aclk(S_AXIS_ACLK_1),
        .s_axis_aresetn(S_AXIS_ARESETN_1),
        .s_axis_tdata(auto_ss_si_r_to_auto_cc_TDATA),
        .s_axis_tkeep(auto_ss_si_r_to_auto_cc_TKEEP),
        .s_axis_tlast(auto_ss_si_r_to_auto_cc_TLAST),
        .s_axis_tready(auto_ss_si_r_to_auto_cc_TREADY),
        .s_axis_tvalid(auto_ss_si_r_to_auto_cc_TVALID));
  base_zynq_design_auto_ss_si_r_1 auto_ss_si_r
       (.aclk(S_AXIS_ACLK_1),
        .aresetn(S_AXIS_ARESETN_1),
        .m_axis_tdata(auto_ss_si_r_to_auto_cc_TDATA),
        .m_axis_tkeep(auto_ss_si_r_to_auto_cc_TKEEP),
        .m_axis_tlast(auto_ss_si_r_to_auto_cc_TLAST),
        .m_axis_tready(auto_ss_si_r_to_auto_cc_TREADY),
        .m_axis_tvalid(auto_ss_si_r_to_auto_cc_TVALID),
        .s_axis_tdata({s00_couplers_to_auto_ss_si_r_TDATA[0],s00_couplers_to_auto_ss_si_r_TDATA[1],s00_couplers_to_auto_ss_si_r_TDATA[2],s00_couplers_to_auto_ss_si_r_TDATA[3],s00_couplers_to_auto_ss_si_r_TDATA[4],s00_couplers_to_auto_ss_si_r_TDATA[5],s00_couplers_to_auto_ss_si_r_TDATA[6],s00_couplers_to_auto_ss_si_r_TDATA[7],s00_couplers_to_auto_ss_si_r_TDATA[8],s00_couplers_to_auto_ss_si_r_TDATA[9],s00_couplers_to_auto_ss_si_r_TDATA[10],s00_couplers_to_auto_ss_si_r_TDATA[11],s00_couplers_to_auto_ss_si_r_TDATA[12],s00_couplers_to_auto_ss_si_r_TDATA[13],s00_couplers_to_auto_ss_si_r_TDATA[14],s00_couplers_to_auto_ss_si_r_TDATA[15]}),
        .s_axis_tkeep({s00_couplers_to_auto_ss_si_r_TKEEP[0],s00_couplers_to_auto_ss_si_r_TKEEP[1]}),
        .s_axis_tlast(s00_couplers_to_auto_ss_si_r_TLAST),
        .s_axis_tvalid(s00_couplers_to_auto_ss_si_r_TVALID));
  base_zynq_design_s00_data_fifo_3 s00_data_fifo
       (.axis_data_count(AXIS_DATA_COUNT_to_S_AXIS_DATA_COUNT),
        .axis_rd_data_count(AXIS_RD_DATA_COUNT_to_S_AXIS_RD_DATA_COUNT),
        .axis_wr_data_count(AXIS_WR_DATA_COUNT_to_S_AXIS_WR_DATA_COUNT),
        .m_axis_tdata(s00_data_fifo_to_s00_couplers_TDATA),
        .m_axis_tkeep(s00_data_fifo_to_s00_couplers_TKEEP),
        .m_axis_tlast(s00_data_fifo_to_s00_couplers_TLAST),
        .m_axis_tready(s00_data_fifo_to_s00_couplers_TREADY),
        .m_axis_tvalid(s00_data_fifo_to_s00_couplers_TVALID),
        .s_axis_aclk(M_AXIS_ACLK),
        .s_axis_aresetn(M_AXIS_ARESETN),
        .s_axis_tdata(auto_cc_to_s00_data_fifo_TDATA),
        .s_axis_tkeep(auto_cc_to_s00_data_fifo_TKEEP),
        .s_axis_tlast(auto_cc_to_s00_data_fifo_TLAST),
        .s_axis_tready(auto_cc_to_s00_data_fifo_TREADY),
        .s_axis_tvalid(auto_cc_to_s00_data_fifo_TVALID));
endmodule

module s00_couplers_imp_1FWH62H
   (M_AXIS_ACLK,
    M_AXIS_ARESETN,
    M_AXIS_tdata,
    M_AXIS_tlast,
    M_AXIS_tready,
    M_AXIS_tvalid,
    S_AXIS_ACLK,
    S_AXIS_ARESETN,
    S_AXIS_tdata,
    S_AXIS_tlast,
    S_AXIS_tready,
    S_AXIS_tvalid);
  input M_AXIS_ACLK;
  input [0:0]M_AXIS_ARESETN;
  output [31:0]M_AXIS_tdata;
  output M_AXIS_tlast;
  input M_AXIS_tready;
  output M_AXIS_tvalid;
  input S_AXIS_ACLK;
  input [0:0]S_AXIS_ARESETN;
  input [31:0]S_AXIS_tdata;
  input S_AXIS_tlast;
  output S_AXIS_tready;
  input S_AXIS_tvalid;

  wire [31:0]AXIS_DATA_COUNT_to_S_AXIS_DATA_COUNT;
  wire [31:0]AXIS_RD_DATA_COUNT_to_S_AXIS_RD_DATA_COUNT;
  wire [31:0]AXIS_WR_DATA_COUNT_to_S_AXIS_WR_DATA_COUNT;
  wire M_AXIS_ACLK;
  wire [0:0]M_AXIS_ARESETN;
  wire [31:0]s00_couplers_to_s00_data_fifo_TDATA;
  wire s00_couplers_to_s00_data_fifo_TLAST;
  wire s00_couplers_to_s00_data_fifo_TREADY;
  wire s00_couplers_to_s00_data_fifo_TVALID;
  wire [31:0]s00_data_fifo_to_s00_couplers_TDATA;
  wire s00_data_fifo_to_s00_couplers_TLAST;
  wire s00_data_fifo_to_s00_couplers_TREADY;
  wire s00_data_fifo_to_s00_couplers_TVALID;

  assign M_AXIS_tdata[31:0] = s00_data_fifo_to_s00_couplers_TDATA;
  assign M_AXIS_tlast = s00_data_fifo_to_s00_couplers_TLAST;
  assign M_AXIS_tvalid = s00_data_fifo_to_s00_couplers_TVALID;
  assign S_AXIS_tready = s00_couplers_to_s00_data_fifo_TREADY;
  assign s00_couplers_to_s00_data_fifo_TDATA = S_AXIS_tdata[31:0];
  assign s00_couplers_to_s00_data_fifo_TLAST = S_AXIS_tlast;
  assign s00_couplers_to_s00_data_fifo_TVALID = S_AXIS_tvalid;
  assign s00_data_fifo_to_s00_couplers_TREADY = M_AXIS_tready;
  base_zynq_design_s00_data_fifo_0 s00_data_fifo
       (.axis_data_count(AXIS_DATA_COUNT_to_S_AXIS_DATA_COUNT),
        .axis_rd_data_count(AXIS_RD_DATA_COUNT_to_S_AXIS_RD_DATA_COUNT),
        .axis_wr_data_count(AXIS_WR_DATA_COUNT_to_S_AXIS_WR_DATA_COUNT),
        .m_axis_tdata(s00_data_fifo_to_s00_couplers_TDATA),
        .m_axis_tlast(s00_data_fifo_to_s00_couplers_TLAST),
        .m_axis_tready(s00_data_fifo_to_s00_couplers_TREADY),
        .m_axis_tvalid(s00_data_fifo_to_s00_couplers_TVALID),
        .s_axis_aclk(M_AXIS_ACLK),
        .s_axis_aresetn(M_AXIS_ARESETN),
        .s_axis_tdata(s00_couplers_to_s00_data_fifo_TDATA),
        .s_axis_tlast(s00_couplers_to_s00_data_fifo_TLAST),
        .s_axis_tready(s00_couplers_to_s00_data_fifo_TREADY),
        .s_axis_tvalid(s00_couplers_to_s00_data_fifo_TVALID));
endmodule

module s00_couplers_imp_1K3E9VL
   (M_AXIS_ACLK,
    M_AXIS_ARESETN,
    M_AXIS_tdata,
    M_AXIS_tkeep,
    M_AXIS_tlast,
    M_AXIS_tready,
    M_AXIS_tvalid,
    S_AXIS_ACLK,
    S_AXIS_ARESETN,
    S_AXIS_tdata,
    S_AXIS_tkeep,
    S_AXIS_tlast,
    S_AXIS_tvalid);
  input M_AXIS_ACLK;
  input [0:0]M_AXIS_ARESETN;
  output [15:0]M_AXIS_tdata;
  output [1:0]M_AXIS_tkeep;
  output M_AXIS_tlast;
  input M_AXIS_tready;
  output M_AXIS_tvalid;
  input S_AXIS_ACLK;
  input [0:0]S_AXIS_ARESETN;
  input [0:15]S_AXIS_tdata;
  input [0:1]S_AXIS_tkeep;
  input S_AXIS_tlast;
  input S_AXIS_tvalid;

  wire [31:0]AXIS_DATA_COUNT_to_S_AXIS_DATA_COUNT;
  wire [31:0]AXIS_RD_DATA_COUNT_to_S_AXIS_RD_DATA_COUNT;
  wire [31:0]AXIS_WR_DATA_COUNT_to_S_AXIS_WR_DATA_COUNT;
  wire M_AXIS_ACLK;
  wire [0:0]M_AXIS_ARESETN;
  wire S_AXIS_ACLK_1;
  wire [0:0]S_AXIS_ARESETN_1;
  wire [15:0]auto_cc_to_s00_data_fifo_TDATA;
  wire [1:0]auto_cc_to_s00_data_fifo_TKEEP;
  wire auto_cc_to_s00_data_fifo_TLAST;
  wire auto_cc_to_s00_data_fifo_TREADY;
  wire auto_cc_to_s00_data_fifo_TVALID;
  wire [15:0]auto_ss_si_r_to_auto_cc_TDATA;
  wire [1:0]auto_ss_si_r_to_auto_cc_TKEEP;
  wire auto_ss_si_r_to_auto_cc_TLAST;
  wire auto_ss_si_r_to_auto_cc_TREADY;
  wire auto_ss_si_r_to_auto_cc_TVALID;
  wire [0:15]s00_couplers_to_auto_ss_si_r_TDATA;
  wire [0:1]s00_couplers_to_auto_ss_si_r_TKEEP;
  wire s00_couplers_to_auto_ss_si_r_TLAST;
  wire s00_couplers_to_auto_ss_si_r_TVALID;
  wire [15:0]s00_data_fifo_to_s00_couplers_TDATA;
  wire [1:0]s00_data_fifo_to_s00_couplers_TKEEP;
  wire s00_data_fifo_to_s00_couplers_TLAST;
  wire s00_data_fifo_to_s00_couplers_TREADY;
  wire s00_data_fifo_to_s00_couplers_TVALID;

  assign M_AXIS_tdata[15:0] = s00_data_fifo_to_s00_couplers_TDATA;
  assign M_AXIS_tkeep[1:0] = s00_data_fifo_to_s00_couplers_TKEEP;
  assign M_AXIS_tlast = s00_data_fifo_to_s00_couplers_TLAST;
  assign M_AXIS_tvalid = s00_data_fifo_to_s00_couplers_TVALID;
  assign S_AXIS_ACLK_1 = S_AXIS_ACLK;
  assign S_AXIS_ARESETN_1 = S_AXIS_ARESETN[0];
  assign s00_couplers_to_auto_ss_si_r_TDATA = S_AXIS_tdata[0:15];
  assign s00_couplers_to_auto_ss_si_r_TKEEP = S_AXIS_tkeep[0:1];
  assign s00_couplers_to_auto_ss_si_r_TLAST = S_AXIS_tlast;
  assign s00_couplers_to_auto_ss_si_r_TVALID = S_AXIS_tvalid;
  assign s00_data_fifo_to_s00_couplers_TREADY = M_AXIS_tready;
  base_zynq_design_auto_cc_3 auto_cc
       (.m_axis_aclk(M_AXIS_ACLK),
        .m_axis_aresetn(M_AXIS_ARESETN),
        .m_axis_tdata(auto_cc_to_s00_data_fifo_TDATA),
        .m_axis_tkeep(auto_cc_to_s00_data_fifo_TKEEP),
        .m_axis_tlast(auto_cc_to_s00_data_fifo_TLAST),
        .m_axis_tready(auto_cc_to_s00_data_fifo_TREADY),
        .m_axis_tvalid(auto_cc_to_s00_data_fifo_TVALID),
        .s_axis_aclk(S_AXIS_ACLK_1),
        .s_axis_aresetn(S_AXIS_ARESETN_1),
        .s_axis_tdata(auto_ss_si_r_to_auto_cc_TDATA),
        .s_axis_tkeep(auto_ss_si_r_to_auto_cc_TKEEP),
        .s_axis_tlast(auto_ss_si_r_to_auto_cc_TLAST),
        .s_axis_tready(auto_ss_si_r_to_auto_cc_TREADY),
        .s_axis_tvalid(auto_ss_si_r_to_auto_cc_TVALID));
  base_zynq_design_auto_ss_si_r_3 auto_ss_si_r
       (.aclk(S_AXIS_ACLK_1),
        .aresetn(S_AXIS_ARESETN_1),
        .m_axis_tdata(auto_ss_si_r_to_auto_cc_TDATA),
        .m_axis_tkeep(auto_ss_si_r_to_auto_cc_TKEEP),
        .m_axis_tlast(auto_ss_si_r_to_auto_cc_TLAST),
        .m_axis_tready(auto_ss_si_r_to_auto_cc_TREADY),
        .m_axis_tvalid(auto_ss_si_r_to_auto_cc_TVALID),
        .s_axis_tdata({s00_couplers_to_auto_ss_si_r_TDATA[0],s00_couplers_to_auto_ss_si_r_TDATA[1],s00_couplers_to_auto_ss_si_r_TDATA[2],s00_couplers_to_auto_ss_si_r_TDATA[3],s00_couplers_to_auto_ss_si_r_TDATA[4],s00_couplers_to_auto_ss_si_r_TDATA[5],s00_couplers_to_auto_ss_si_r_TDATA[6],s00_couplers_to_auto_ss_si_r_TDATA[7],s00_couplers_to_auto_ss_si_r_TDATA[8],s00_couplers_to_auto_ss_si_r_TDATA[9],s00_couplers_to_auto_ss_si_r_TDATA[10],s00_couplers_to_auto_ss_si_r_TDATA[11],s00_couplers_to_auto_ss_si_r_TDATA[12],s00_couplers_to_auto_ss_si_r_TDATA[13],s00_couplers_to_auto_ss_si_r_TDATA[14],s00_couplers_to_auto_ss_si_r_TDATA[15]}),
        .s_axis_tkeep({s00_couplers_to_auto_ss_si_r_TKEEP[0],s00_couplers_to_auto_ss_si_r_TKEEP[1]}),
        .s_axis_tlast(s00_couplers_to_auto_ss_si_r_TLAST),
        .s_axis_tvalid(s00_couplers_to_auto_ss_si_r_TVALID));
  base_zynq_design_s00_data_fifo_4 s00_data_fifo
       (.axis_data_count(AXIS_DATA_COUNT_to_S_AXIS_DATA_COUNT),
        .axis_rd_data_count(AXIS_RD_DATA_COUNT_to_S_AXIS_RD_DATA_COUNT),
        .axis_wr_data_count(AXIS_WR_DATA_COUNT_to_S_AXIS_WR_DATA_COUNT),
        .m_axis_tdata(s00_data_fifo_to_s00_couplers_TDATA),
        .m_axis_tkeep(s00_data_fifo_to_s00_couplers_TKEEP),
        .m_axis_tlast(s00_data_fifo_to_s00_couplers_TLAST),
        .m_axis_tready(s00_data_fifo_to_s00_couplers_TREADY),
        .m_axis_tvalid(s00_data_fifo_to_s00_couplers_TVALID),
        .s_axis_aclk(M_AXIS_ACLK),
        .s_axis_aresetn(M_AXIS_ARESETN),
        .s_axis_tdata(auto_cc_to_s00_data_fifo_TDATA),
        .s_axis_tkeep(auto_cc_to_s00_data_fifo_TKEEP),
        .s_axis_tlast(auto_cc_to_s00_data_fifo_TLAST),
        .s_axis_tready(auto_cc_to_s00_data_fifo_TREADY),
        .s_axis_tvalid(auto_cc_to_s00_data_fifo_TVALID));
endmodule

module s00_couplers_imp_1UTISAU
   (M_ACLK,
    M_ARESETN,
    M_AXI_araddr,
    M_AXI_arprot,
    M_AXI_arready,
    M_AXI_arvalid,
    M_AXI_awaddr,
    M_AXI_awprot,
    M_AXI_awready,
    M_AXI_awvalid,
    M_AXI_bready,
    M_AXI_bresp,
    M_AXI_bvalid,
    M_AXI_rdata,
    M_AXI_rready,
    M_AXI_rresp,
    M_AXI_rvalid,
    M_AXI_wdata,
    M_AXI_wready,
    M_AXI_wstrb,
    M_AXI_wvalid,
    S_ACLK,
    S_ARESETN,
    S_AXI_araddr,
    S_AXI_arburst,
    S_AXI_arcache,
    S_AXI_arid,
    S_AXI_arlen,
    S_AXI_arlock,
    S_AXI_arprot,
    S_AXI_arqos,
    S_AXI_arready,
    S_AXI_arsize,
    S_AXI_arvalid,
    S_AXI_awaddr,
    S_AXI_awburst,
    S_AXI_awcache,
    S_AXI_awid,
    S_AXI_awlen,
    S_AXI_awlock,
    S_AXI_awprot,
    S_AXI_awqos,
    S_AXI_awready,
    S_AXI_awsize,
    S_AXI_awvalid,
    S_AXI_bid,
    S_AXI_bready,
    S_AXI_bresp,
    S_AXI_bvalid,
    S_AXI_rdata,
    S_AXI_rid,
    S_AXI_rlast,
    S_AXI_rready,
    S_AXI_rresp,
    S_AXI_rvalid,
    S_AXI_wdata,
    S_AXI_wid,
    S_AXI_wlast,
    S_AXI_wready,
    S_AXI_wstrb,
    S_AXI_wvalid);
  input M_ACLK;
  input [0:0]M_ARESETN;
  output [31:0]M_AXI_araddr;
  output [2:0]M_AXI_arprot;
  input M_AXI_arready;
  output M_AXI_arvalid;
  output [31:0]M_AXI_awaddr;
  output [2:0]M_AXI_awprot;
  input M_AXI_awready;
  output M_AXI_awvalid;
  output M_AXI_bready;
  input [1:0]M_AXI_bresp;
  input M_AXI_bvalid;
  input [31:0]M_AXI_rdata;
  output M_AXI_rready;
  input [1:0]M_AXI_rresp;
  input M_AXI_rvalid;
  output [31:0]M_AXI_wdata;
  input M_AXI_wready;
  output [3:0]M_AXI_wstrb;
  output M_AXI_wvalid;
  input S_ACLK;
  input [0:0]S_ARESETN;
  input [31:0]S_AXI_araddr;
  input [1:0]S_AXI_arburst;
  input [3:0]S_AXI_arcache;
  input [11:0]S_AXI_arid;
  input [3:0]S_AXI_arlen;
  input [1:0]S_AXI_arlock;
  input [2:0]S_AXI_arprot;
  input [3:0]S_AXI_arqos;
  output S_AXI_arready;
  input [2:0]S_AXI_arsize;
  input S_AXI_arvalid;
  input [31:0]S_AXI_awaddr;
  input [1:0]S_AXI_awburst;
  input [3:0]S_AXI_awcache;
  input [11:0]S_AXI_awid;
  input [3:0]S_AXI_awlen;
  input [1:0]S_AXI_awlock;
  input [2:0]S_AXI_awprot;
  input [3:0]S_AXI_awqos;
  output S_AXI_awready;
  input [2:0]S_AXI_awsize;
  input S_AXI_awvalid;
  output [11:0]S_AXI_bid;
  input S_AXI_bready;
  output [1:0]S_AXI_bresp;
  output S_AXI_bvalid;
  output [31:0]S_AXI_rdata;
  output [11:0]S_AXI_rid;
  output S_AXI_rlast;
  input S_AXI_rready;
  output [1:0]S_AXI_rresp;
  output S_AXI_rvalid;
  input [31:0]S_AXI_wdata;
  input [11:0]S_AXI_wid;
  input S_AXI_wlast;
  output S_AXI_wready;
  input [3:0]S_AXI_wstrb;
  input S_AXI_wvalid;

  wire S_ACLK_1;
  wire [0:0]S_ARESETN_1;
  wire [31:0]auto_pc_to_s00_couplers_ARADDR;
  wire [2:0]auto_pc_to_s00_couplers_ARPROT;
  wire auto_pc_to_s00_couplers_ARREADY;
  wire auto_pc_to_s00_couplers_ARVALID;
  wire [31:0]auto_pc_to_s00_couplers_AWADDR;
  wire [2:0]auto_pc_to_s00_couplers_AWPROT;
  wire auto_pc_to_s00_couplers_AWREADY;
  wire auto_pc_to_s00_couplers_AWVALID;
  wire auto_pc_to_s00_couplers_BREADY;
  wire [1:0]auto_pc_to_s00_couplers_BRESP;
  wire auto_pc_to_s00_couplers_BVALID;
  wire [31:0]auto_pc_to_s00_couplers_RDATA;
  wire auto_pc_to_s00_couplers_RREADY;
  wire [1:0]auto_pc_to_s00_couplers_RRESP;
  wire auto_pc_to_s00_couplers_RVALID;
  wire [31:0]auto_pc_to_s00_couplers_WDATA;
  wire auto_pc_to_s00_couplers_WREADY;
  wire [3:0]auto_pc_to_s00_couplers_WSTRB;
  wire auto_pc_to_s00_couplers_WVALID;
  wire [31:0]s00_couplers_to_auto_pc_ARADDR;
  wire [1:0]s00_couplers_to_auto_pc_ARBURST;
  wire [3:0]s00_couplers_to_auto_pc_ARCACHE;
  wire [11:0]s00_couplers_to_auto_pc_ARID;
  wire [3:0]s00_couplers_to_auto_pc_ARLEN;
  wire [1:0]s00_couplers_to_auto_pc_ARLOCK;
  wire [2:0]s00_couplers_to_auto_pc_ARPROT;
  wire [3:0]s00_couplers_to_auto_pc_ARQOS;
  wire s00_couplers_to_auto_pc_ARREADY;
  wire [2:0]s00_couplers_to_auto_pc_ARSIZE;
  wire s00_couplers_to_auto_pc_ARVALID;
  wire [31:0]s00_couplers_to_auto_pc_AWADDR;
  wire [1:0]s00_couplers_to_auto_pc_AWBURST;
  wire [3:0]s00_couplers_to_auto_pc_AWCACHE;
  wire [11:0]s00_couplers_to_auto_pc_AWID;
  wire [3:0]s00_couplers_to_auto_pc_AWLEN;
  wire [1:0]s00_couplers_to_auto_pc_AWLOCK;
  wire [2:0]s00_couplers_to_auto_pc_AWPROT;
  wire [3:0]s00_couplers_to_auto_pc_AWQOS;
  wire s00_couplers_to_auto_pc_AWREADY;
  wire [2:0]s00_couplers_to_auto_pc_AWSIZE;
  wire s00_couplers_to_auto_pc_AWVALID;
  wire [11:0]s00_couplers_to_auto_pc_BID;
  wire s00_couplers_to_auto_pc_BREADY;
  wire [1:0]s00_couplers_to_auto_pc_BRESP;
  wire s00_couplers_to_auto_pc_BVALID;
  wire [31:0]s00_couplers_to_auto_pc_RDATA;
  wire [11:0]s00_couplers_to_auto_pc_RID;
  wire s00_couplers_to_auto_pc_RLAST;
  wire s00_couplers_to_auto_pc_RREADY;
  wire [1:0]s00_couplers_to_auto_pc_RRESP;
  wire s00_couplers_to_auto_pc_RVALID;
  wire [31:0]s00_couplers_to_auto_pc_WDATA;
  wire [11:0]s00_couplers_to_auto_pc_WID;
  wire s00_couplers_to_auto_pc_WLAST;
  wire s00_couplers_to_auto_pc_WREADY;
  wire [3:0]s00_couplers_to_auto_pc_WSTRB;
  wire s00_couplers_to_auto_pc_WVALID;

  assign M_AXI_araddr[31:0] = auto_pc_to_s00_couplers_ARADDR;
  assign M_AXI_arprot[2:0] = auto_pc_to_s00_couplers_ARPROT;
  assign M_AXI_arvalid = auto_pc_to_s00_couplers_ARVALID;
  assign M_AXI_awaddr[31:0] = auto_pc_to_s00_couplers_AWADDR;
  assign M_AXI_awprot[2:0] = auto_pc_to_s00_couplers_AWPROT;
  assign M_AXI_awvalid = auto_pc_to_s00_couplers_AWVALID;
  assign M_AXI_bready = auto_pc_to_s00_couplers_BREADY;
  assign M_AXI_rready = auto_pc_to_s00_couplers_RREADY;
  assign M_AXI_wdata[31:0] = auto_pc_to_s00_couplers_WDATA;
  assign M_AXI_wstrb[3:0] = auto_pc_to_s00_couplers_WSTRB;
  assign M_AXI_wvalid = auto_pc_to_s00_couplers_WVALID;
  assign S_ACLK_1 = S_ACLK;
  assign S_ARESETN_1 = S_ARESETN[0];
  assign S_AXI_arready = s00_couplers_to_auto_pc_ARREADY;
  assign S_AXI_awready = s00_couplers_to_auto_pc_AWREADY;
  assign S_AXI_bid[11:0] = s00_couplers_to_auto_pc_BID;
  assign S_AXI_bresp[1:0] = s00_couplers_to_auto_pc_BRESP;
  assign S_AXI_bvalid = s00_couplers_to_auto_pc_BVALID;
  assign S_AXI_rdata[31:0] = s00_couplers_to_auto_pc_RDATA;
  assign S_AXI_rid[11:0] = s00_couplers_to_auto_pc_RID;
  assign S_AXI_rlast = s00_couplers_to_auto_pc_RLAST;
  assign S_AXI_rresp[1:0] = s00_couplers_to_auto_pc_RRESP;
  assign S_AXI_rvalid = s00_couplers_to_auto_pc_RVALID;
  assign S_AXI_wready = s00_couplers_to_auto_pc_WREADY;
  assign auto_pc_to_s00_couplers_ARREADY = M_AXI_arready;
  assign auto_pc_to_s00_couplers_AWREADY = M_AXI_awready;
  assign auto_pc_to_s00_couplers_BRESP = M_AXI_bresp[1:0];
  assign auto_pc_to_s00_couplers_BVALID = M_AXI_bvalid;
  assign auto_pc_to_s00_couplers_RDATA = M_AXI_rdata[31:0];
  assign auto_pc_to_s00_couplers_RRESP = M_AXI_rresp[1:0];
  assign auto_pc_to_s00_couplers_RVALID = M_AXI_rvalid;
  assign auto_pc_to_s00_couplers_WREADY = M_AXI_wready;
  assign s00_couplers_to_auto_pc_ARADDR = S_AXI_araddr[31:0];
  assign s00_couplers_to_auto_pc_ARBURST = S_AXI_arburst[1:0];
  assign s00_couplers_to_auto_pc_ARCACHE = S_AXI_arcache[3:0];
  assign s00_couplers_to_auto_pc_ARID = S_AXI_arid[11:0];
  assign s00_couplers_to_auto_pc_ARLEN = S_AXI_arlen[3:0];
  assign s00_couplers_to_auto_pc_ARLOCK = S_AXI_arlock[1:0];
  assign s00_couplers_to_auto_pc_ARPROT = S_AXI_arprot[2:0];
  assign s00_couplers_to_auto_pc_ARQOS = S_AXI_arqos[3:0];
  assign s00_couplers_to_auto_pc_ARSIZE = S_AXI_arsize[2:0];
  assign s00_couplers_to_auto_pc_ARVALID = S_AXI_arvalid;
  assign s00_couplers_to_auto_pc_AWADDR = S_AXI_awaddr[31:0];
  assign s00_couplers_to_auto_pc_AWBURST = S_AXI_awburst[1:0];
  assign s00_couplers_to_auto_pc_AWCACHE = S_AXI_awcache[3:0];
  assign s00_couplers_to_auto_pc_AWID = S_AXI_awid[11:0];
  assign s00_couplers_to_auto_pc_AWLEN = S_AXI_awlen[3:0];
  assign s00_couplers_to_auto_pc_AWLOCK = S_AXI_awlock[1:0];
  assign s00_couplers_to_auto_pc_AWPROT = S_AXI_awprot[2:0];
  assign s00_couplers_to_auto_pc_AWQOS = S_AXI_awqos[3:0];
  assign s00_couplers_to_auto_pc_AWSIZE = S_AXI_awsize[2:0];
  assign s00_couplers_to_auto_pc_AWVALID = S_AXI_awvalid;
  assign s00_couplers_to_auto_pc_BREADY = S_AXI_bready;
  assign s00_couplers_to_auto_pc_RREADY = S_AXI_rready;
  assign s00_couplers_to_auto_pc_WDATA = S_AXI_wdata[31:0];
  assign s00_couplers_to_auto_pc_WID = S_AXI_wid[11:0];
  assign s00_couplers_to_auto_pc_WLAST = S_AXI_wlast;
  assign s00_couplers_to_auto_pc_WSTRB = S_AXI_wstrb[3:0];
  assign s00_couplers_to_auto_pc_WVALID = S_AXI_wvalid;
  base_zynq_design_auto_pc_0 auto_pc
       (.aclk(S_ACLK_1),
        .aresetn(S_ARESETN_1),
        .m_axi_araddr(auto_pc_to_s00_couplers_ARADDR),
        .m_axi_arprot(auto_pc_to_s00_couplers_ARPROT),
        .m_axi_arready(auto_pc_to_s00_couplers_ARREADY),
        .m_axi_arvalid(auto_pc_to_s00_couplers_ARVALID),
        .m_axi_awaddr(auto_pc_to_s00_couplers_AWADDR),
        .m_axi_awprot(auto_pc_to_s00_couplers_AWPROT),
        .m_axi_awready(auto_pc_to_s00_couplers_AWREADY),
        .m_axi_awvalid(auto_pc_to_s00_couplers_AWVALID),
        .m_axi_bready(auto_pc_to_s00_couplers_BREADY),
        .m_axi_bresp(auto_pc_to_s00_couplers_BRESP),
        .m_axi_bvalid(auto_pc_to_s00_couplers_BVALID),
        .m_axi_rdata(auto_pc_to_s00_couplers_RDATA),
        .m_axi_rready(auto_pc_to_s00_couplers_RREADY),
        .m_axi_rresp(auto_pc_to_s00_couplers_RRESP),
        .m_axi_rvalid(auto_pc_to_s00_couplers_RVALID),
        .m_axi_wdata(auto_pc_to_s00_couplers_WDATA),
        .m_axi_wready(auto_pc_to_s00_couplers_WREADY),
        .m_axi_wstrb(auto_pc_to_s00_couplers_WSTRB),
        .m_axi_wvalid(auto_pc_to_s00_couplers_WVALID),
        .s_axi_araddr(s00_couplers_to_auto_pc_ARADDR),
        .s_axi_arburst(s00_couplers_to_auto_pc_ARBURST),
        .s_axi_arcache(s00_couplers_to_auto_pc_ARCACHE),
        .s_axi_arid(s00_couplers_to_auto_pc_ARID),
        .s_axi_arlen(s00_couplers_to_auto_pc_ARLEN),
        .s_axi_arlock(s00_couplers_to_auto_pc_ARLOCK),
        .s_axi_arprot(s00_couplers_to_auto_pc_ARPROT),
        .s_axi_arqos(s00_couplers_to_auto_pc_ARQOS),
        .s_axi_arready(s00_couplers_to_auto_pc_ARREADY),
        .s_axi_arsize(s00_couplers_to_auto_pc_ARSIZE),
        .s_axi_arvalid(s00_couplers_to_auto_pc_ARVALID),
        .s_axi_awaddr(s00_couplers_to_auto_pc_AWADDR),
        .s_axi_awburst(s00_couplers_to_auto_pc_AWBURST),
        .s_axi_awcache(s00_couplers_to_auto_pc_AWCACHE),
        .s_axi_awid(s00_couplers_to_auto_pc_AWID),
        .s_axi_awlen(s00_couplers_to_auto_pc_AWLEN),
        .s_axi_awlock(s00_couplers_to_auto_pc_AWLOCK),
        .s_axi_awprot(s00_couplers_to_auto_pc_AWPROT),
        .s_axi_awqos(s00_couplers_to_auto_pc_AWQOS),
        .s_axi_awready(s00_couplers_to_auto_pc_AWREADY),
        .s_axi_awsize(s00_couplers_to_auto_pc_AWSIZE),
        .s_axi_awvalid(s00_couplers_to_auto_pc_AWVALID),
        .s_axi_bid(s00_couplers_to_auto_pc_BID),
        .s_axi_bready(s00_couplers_to_auto_pc_BREADY),
        .s_axi_bresp(s00_couplers_to_auto_pc_BRESP),
        .s_axi_bvalid(s00_couplers_to_auto_pc_BVALID),
        .s_axi_rdata(s00_couplers_to_auto_pc_RDATA),
        .s_axi_rid(s00_couplers_to_auto_pc_RID),
        .s_axi_rlast(s00_couplers_to_auto_pc_RLAST),
        .s_axi_rready(s00_couplers_to_auto_pc_RREADY),
        .s_axi_rresp(s00_couplers_to_auto_pc_RRESP),
        .s_axi_rvalid(s00_couplers_to_auto_pc_RVALID),
        .s_axi_wdata(s00_couplers_to_auto_pc_WDATA),
        .s_axi_wid(s00_couplers_to_auto_pc_WID),
        .s_axi_wlast(s00_couplers_to_auto_pc_WLAST),
        .s_axi_wready(s00_couplers_to_auto_pc_WREADY),
        .s_axi_wstrb(s00_couplers_to_auto_pc_WSTRB),
        .s_axi_wvalid(s00_couplers_to_auto_pc_WVALID));
endmodule

module s00_couplers_imp_1WC458P
   (M_AXIS_ACLK,
    M_AXIS_ARESETN,
    M_AXIS_tdata,
    M_AXIS_tkeep,
    M_AXIS_tlast,
    M_AXIS_tready,
    M_AXIS_tvalid,
    S_AXIS_ACLK,
    S_AXIS_ARESETN,
    S_AXIS_tdata,
    S_AXIS_tkeep,
    S_AXIS_tlast,
    S_AXIS_tvalid);
  input M_AXIS_ACLK;
  input [0:0]M_AXIS_ARESETN;
  output [15:0]M_AXIS_tdata;
  output [1:0]M_AXIS_tkeep;
  output M_AXIS_tlast;
  input M_AXIS_tready;
  output M_AXIS_tvalid;
  input S_AXIS_ACLK;
  input [0:0]S_AXIS_ARESETN;
  input [0:15]S_AXIS_tdata;
  input [0:1]S_AXIS_tkeep;
  input S_AXIS_tlast;
  input S_AXIS_tvalid;

  wire [31:0]AXIS_DATA_COUNT_to_S_AXIS_DATA_COUNT;
  wire [31:0]AXIS_RD_DATA_COUNT_to_S_AXIS_RD_DATA_COUNT;
  wire [31:0]AXIS_WR_DATA_COUNT_to_S_AXIS_WR_DATA_COUNT;
  wire M_AXIS_ACLK;
  wire [0:0]M_AXIS_ARESETN;
  wire S_AXIS_ACLK_1;
  wire [0:0]S_AXIS_ARESETN_1;
  wire [15:0]auto_cc_to_s00_data_fifo_TDATA;
  wire [1:0]auto_cc_to_s00_data_fifo_TKEEP;
  wire auto_cc_to_s00_data_fifo_TLAST;
  wire auto_cc_to_s00_data_fifo_TREADY;
  wire auto_cc_to_s00_data_fifo_TVALID;
  wire [15:0]auto_ss_si_r_to_auto_cc_TDATA;
  wire [1:0]auto_ss_si_r_to_auto_cc_TKEEP;
  wire auto_ss_si_r_to_auto_cc_TLAST;
  wire auto_ss_si_r_to_auto_cc_TREADY;
  wire auto_ss_si_r_to_auto_cc_TVALID;
  wire [0:15]s00_couplers_to_auto_ss_si_r_TDATA;
  wire [0:1]s00_couplers_to_auto_ss_si_r_TKEEP;
  wire s00_couplers_to_auto_ss_si_r_TLAST;
  wire s00_couplers_to_auto_ss_si_r_TVALID;
  wire [15:0]s00_data_fifo_to_s00_couplers_TDATA;
  wire [1:0]s00_data_fifo_to_s00_couplers_TKEEP;
  wire s00_data_fifo_to_s00_couplers_TLAST;
  wire s00_data_fifo_to_s00_couplers_TREADY;
  wire s00_data_fifo_to_s00_couplers_TVALID;

  assign M_AXIS_tdata[15:0] = s00_data_fifo_to_s00_couplers_TDATA;
  assign M_AXIS_tkeep[1:0] = s00_data_fifo_to_s00_couplers_TKEEP;
  assign M_AXIS_tlast = s00_data_fifo_to_s00_couplers_TLAST;
  assign M_AXIS_tvalid = s00_data_fifo_to_s00_couplers_TVALID;
  assign S_AXIS_ACLK_1 = S_AXIS_ACLK;
  assign S_AXIS_ARESETN_1 = S_AXIS_ARESETN[0];
  assign s00_couplers_to_auto_ss_si_r_TDATA = S_AXIS_tdata[0:15];
  assign s00_couplers_to_auto_ss_si_r_TKEEP = S_AXIS_tkeep[0:1];
  assign s00_couplers_to_auto_ss_si_r_TLAST = S_AXIS_tlast;
  assign s00_couplers_to_auto_ss_si_r_TVALID = S_AXIS_tvalid;
  assign s00_data_fifo_to_s00_couplers_TREADY = M_AXIS_tready;
  base_zynq_design_auto_cc_2 auto_cc
       (.m_axis_aclk(M_AXIS_ACLK),
        .m_axis_aresetn(M_AXIS_ARESETN),
        .m_axis_tdata(auto_cc_to_s00_data_fifo_TDATA),
        .m_axis_tkeep(auto_cc_to_s00_data_fifo_TKEEP),
        .m_axis_tlast(auto_cc_to_s00_data_fifo_TLAST),
        .m_axis_tready(auto_cc_to_s00_data_fifo_TREADY),
        .m_axis_tvalid(auto_cc_to_s00_data_fifo_TVALID),
        .s_axis_aclk(S_AXIS_ACLK_1),
        .s_axis_aresetn(S_AXIS_ARESETN_1),
        .s_axis_tdata(auto_ss_si_r_to_auto_cc_TDATA),
        .s_axis_tkeep(auto_ss_si_r_to_auto_cc_TKEEP),
        .s_axis_tlast(auto_ss_si_r_to_auto_cc_TLAST),
        .s_axis_tready(auto_ss_si_r_to_auto_cc_TREADY),
        .s_axis_tvalid(auto_ss_si_r_to_auto_cc_TVALID));
  base_zynq_design_auto_ss_si_r_2 auto_ss_si_r
       (.aclk(S_AXIS_ACLK_1),
        .aresetn(S_AXIS_ARESETN_1),
        .m_axis_tdata(auto_ss_si_r_to_auto_cc_TDATA),
        .m_axis_tkeep(auto_ss_si_r_to_auto_cc_TKEEP),
        .m_axis_tlast(auto_ss_si_r_to_auto_cc_TLAST),
        .m_axis_tready(auto_ss_si_r_to_auto_cc_TREADY),
        .m_axis_tvalid(auto_ss_si_r_to_auto_cc_TVALID),
        .s_axis_tdata({s00_couplers_to_auto_ss_si_r_TDATA[0],s00_couplers_to_auto_ss_si_r_TDATA[1],s00_couplers_to_auto_ss_si_r_TDATA[2],s00_couplers_to_auto_ss_si_r_TDATA[3],s00_couplers_to_auto_ss_si_r_TDATA[4],s00_couplers_to_auto_ss_si_r_TDATA[5],s00_couplers_to_auto_ss_si_r_TDATA[6],s00_couplers_to_auto_ss_si_r_TDATA[7],s00_couplers_to_auto_ss_si_r_TDATA[8],s00_couplers_to_auto_ss_si_r_TDATA[9],s00_couplers_to_auto_ss_si_r_TDATA[10],s00_couplers_to_auto_ss_si_r_TDATA[11],s00_couplers_to_auto_ss_si_r_TDATA[12],s00_couplers_to_auto_ss_si_r_TDATA[13],s00_couplers_to_auto_ss_si_r_TDATA[14],s00_couplers_to_auto_ss_si_r_TDATA[15]}),
        .s_axis_tkeep({s00_couplers_to_auto_ss_si_r_TKEEP[0],s00_couplers_to_auto_ss_si_r_TKEEP[1]}),
        .s_axis_tlast(s00_couplers_to_auto_ss_si_r_TLAST),
        .s_axis_tvalid(s00_couplers_to_auto_ss_si_r_TVALID));
  base_zynq_design_s00_data_fifo_2 s00_data_fifo
       (.axis_data_count(AXIS_DATA_COUNT_to_S_AXIS_DATA_COUNT),
        .axis_rd_data_count(AXIS_RD_DATA_COUNT_to_S_AXIS_RD_DATA_COUNT),
        .axis_wr_data_count(AXIS_WR_DATA_COUNT_to_S_AXIS_WR_DATA_COUNT),
        .m_axis_tdata(s00_data_fifo_to_s00_couplers_TDATA),
        .m_axis_tkeep(s00_data_fifo_to_s00_couplers_TKEEP),
        .m_axis_tlast(s00_data_fifo_to_s00_couplers_TLAST),
        .m_axis_tready(s00_data_fifo_to_s00_couplers_TREADY),
        .m_axis_tvalid(s00_data_fifo_to_s00_couplers_TVALID),
        .s_axis_aclk(M_AXIS_ACLK),
        .s_axis_aresetn(M_AXIS_ARESETN),
        .s_axis_tdata(auto_cc_to_s00_data_fifo_TDATA),
        .s_axis_tkeep(auto_cc_to_s00_data_fifo_TKEEP),
        .s_axis_tlast(auto_cc_to_s00_data_fifo_TLAST),
        .s_axis_tready(auto_cc_to_s00_data_fifo_TREADY),
        .s_axis_tvalid(auto_cc_to_s00_data_fifo_TVALID));
endmodule

module s00_couplers_imp_26EUIW
   (M_ACLK,
    M_ARESETN,
    M_AXI_araddr,
    M_AXI_arburst,
    M_AXI_arcache,
    M_AXI_arlen,
    M_AXI_arlock,
    M_AXI_arprot,
    M_AXI_arqos,
    M_AXI_arready,
    M_AXI_arsize,
    M_AXI_arvalid,
    M_AXI_awaddr,
    M_AXI_awburst,
    M_AXI_awcache,
    M_AXI_awlen,
    M_AXI_awlock,
    M_AXI_awprot,
    M_AXI_awqos,
    M_AXI_awready,
    M_AXI_awsize,
    M_AXI_awvalid,
    M_AXI_bready,
    M_AXI_bresp,
    M_AXI_bvalid,
    M_AXI_rdata,
    M_AXI_rlast,
    M_AXI_rready,
    M_AXI_rresp,
    M_AXI_rvalid,
    M_AXI_wdata,
    M_AXI_wlast,
    M_AXI_wready,
    M_AXI_wstrb,
    M_AXI_wvalid,
    S_ACLK,
    S_ARESETN,
    S_AXI_araddr,
    S_AXI_arburst,
    S_AXI_arcache,
    S_AXI_arlen,
    S_AXI_arprot,
    S_AXI_arready,
    S_AXI_arsize,
    S_AXI_arvalid,
    S_AXI_awaddr,
    S_AXI_awburst,
    S_AXI_awcache,
    S_AXI_awlen,
    S_AXI_awprot,
    S_AXI_awready,
    S_AXI_awsize,
    S_AXI_awvalid,
    S_AXI_bready,
    S_AXI_bresp,
    S_AXI_bvalid,
    S_AXI_rdata,
    S_AXI_rlast,
    S_AXI_rready,
    S_AXI_rresp,
    S_AXI_rvalid,
    S_AXI_wdata,
    S_AXI_wlast,
    S_AXI_wready,
    S_AXI_wstrb,
    S_AXI_wvalid);
  input M_ACLK;
  input [0:0]M_ARESETN;
  output [31:0]M_AXI_araddr;
  output [1:0]M_AXI_arburst;
  output [3:0]M_AXI_arcache;
  output [3:0]M_AXI_arlen;
  output [1:0]M_AXI_arlock;
  output [2:0]M_AXI_arprot;
  output [3:0]M_AXI_arqos;
  input M_AXI_arready;
  output [2:0]M_AXI_arsize;
  output M_AXI_arvalid;
  output [31:0]M_AXI_awaddr;
  output [1:0]M_AXI_awburst;
  output [3:0]M_AXI_awcache;
  output [3:0]M_AXI_awlen;
  output [1:0]M_AXI_awlock;
  output [2:0]M_AXI_awprot;
  output [3:0]M_AXI_awqos;
  input M_AXI_awready;
  output [2:0]M_AXI_awsize;
  output M_AXI_awvalid;
  output M_AXI_bready;
  input [1:0]M_AXI_bresp;
  input M_AXI_bvalid;
  input [63:0]M_AXI_rdata;
  input M_AXI_rlast;
  output M_AXI_rready;
  input [1:0]M_AXI_rresp;
  input M_AXI_rvalid;
  output [63:0]M_AXI_wdata;
  output M_AXI_wlast;
  input M_AXI_wready;
  output [7:0]M_AXI_wstrb;
  output M_AXI_wvalid;
  input S_ACLK;
  input [0:0]S_ARESETN;
  input [31:0]S_AXI_araddr;
  input [1:0]S_AXI_arburst;
  input [3:0]S_AXI_arcache;
  input [7:0]S_AXI_arlen;
  input [2:0]S_AXI_arprot;
  output S_AXI_arready;
  input [2:0]S_AXI_arsize;
  input S_AXI_arvalid;
  input [31:0]S_AXI_awaddr;
  input [1:0]S_AXI_awburst;
  input [3:0]S_AXI_awcache;
  input [7:0]S_AXI_awlen;
  input [2:0]S_AXI_awprot;
  output S_AXI_awready;
  input [2:0]S_AXI_awsize;
  input S_AXI_awvalid;
  input S_AXI_bready;
  output [1:0]S_AXI_bresp;
  output S_AXI_bvalid;
  output [31:0]S_AXI_rdata;
  output S_AXI_rlast;
  input S_AXI_rready;
  output [1:0]S_AXI_rresp;
  output S_AXI_rvalid;
  input [31:0]S_AXI_wdata;
  input S_AXI_wlast;
  output S_AXI_wready;
  input [3:0]S_AXI_wstrb;
  input S_AXI_wvalid;

  wire S_ACLK_1;
  wire [0:0]S_ARESETN_1;
  wire [31:0]auto_pc_to_auto_us_ARADDR;
  wire [1:0]auto_pc_to_auto_us_ARBURST;
  wire [3:0]auto_pc_to_auto_us_ARCACHE;
  wire [3:0]auto_pc_to_auto_us_ARLEN;
  wire [1:0]auto_pc_to_auto_us_ARLOCK;
  wire [2:0]auto_pc_to_auto_us_ARPROT;
  wire [3:0]auto_pc_to_auto_us_ARQOS;
  wire auto_pc_to_auto_us_ARREADY;
  wire [2:0]auto_pc_to_auto_us_ARSIZE;
  wire auto_pc_to_auto_us_ARVALID;
  wire [31:0]auto_pc_to_auto_us_AWADDR;
  wire [1:0]auto_pc_to_auto_us_AWBURST;
  wire [3:0]auto_pc_to_auto_us_AWCACHE;
  wire [3:0]auto_pc_to_auto_us_AWLEN;
  wire [1:0]auto_pc_to_auto_us_AWLOCK;
  wire [2:0]auto_pc_to_auto_us_AWPROT;
  wire [3:0]auto_pc_to_auto_us_AWQOS;
  wire auto_pc_to_auto_us_AWREADY;
  wire [2:0]auto_pc_to_auto_us_AWSIZE;
  wire auto_pc_to_auto_us_AWVALID;
  wire auto_pc_to_auto_us_BREADY;
  wire [1:0]auto_pc_to_auto_us_BRESP;
  wire auto_pc_to_auto_us_BVALID;
  wire [31:0]auto_pc_to_auto_us_RDATA;
  wire auto_pc_to_auto_us_RLAST;
  wire auto_pc_to_auto_us_RREADY;
  wire [1:0]auto_pc_to_auto_us_RRESP;
  wire auto_pc_to_auto_us_RVALID;
  wire [31:0]auto_pc_to_auto_us_WDATA;
  wire auto_pc_to_auto_us_WLAST;
  wire auto_pc_to_auto_us_WREADY;
  wire [3:0]auto_pc_to_auto_us_WSTRB;
  wire auto_pc_to_auto_us_WVALID;
  wire [31:0]auto_us_to_s00_couplers_ARADDR;
  wire [1:0]auto_us_to_s00_couplers_ARBURST;
  wire [3:0]auto_us_to_s00_couplers_ARCACHE;
  wire [3:0]auto_us_to_s00_couplers_ARLEN;
  wire [1:0]auto_us_to_s00_couplers_ARLOCK;
  wire [2:0]auto_us_to_s00_couplers_ARPROT;
  wire [3:0]auto_us_to_s00_couplers_ARQOS;
  wire auto_us_to_s00_couplers_ARREADY;
  wire [2:0]auto_us_to_s00_couplers_ARSIZE;
  wire auto_us_to_s00_couplers_ARVALID;
  wire [31:0]auto_us_to_s00_couplers_AWADDR;
  wire [1:0]auto_us_to_s00_couplers_AWBURST;
  wire [3:0]auto_us_to_s00_couplers_AWCACHE;
  wire [3:0]auto_us_to_s00_couplers_AWLEN;
  wire [1:0]auto_us_to_s00_couplers_AWLOCK;
  wire [2:0]auto_us_to_s00_couplers_AWPROT;
  wire [3:0]auto_us_to_s00_couplers_AWQOS;
  wire auto_us_to_s00_couplers_AWREADY;
  wire [2:0]auto_us_to_s00_couplers_AWSIZE;
  wire auto_us_to_s00_couplers_AWVALID;
  wire auto_us_to_s00_couplers_BREADY;
  wire [1:0]auto_us_to_s00_couplers_BRESP;
  wire auto_us_to_s00_couplers_BVALID;
  wire [63:0]auto_us_to_s00_couplers_RDATA;
  wire auto_us_to_s00_couplers_RLAST;
  wire auto_us_to_s00_couplers_RREADY;
  wire [1:0]auto_us_to_s00_couplers_RRESP;
  wire auto_us_to_s00_couplers_RVALID;
  wire [63:0]auto_us_to_s00_couplers_WDATA;
  wire auto_us_to_s00_couplers_WLAST;
  wire auto_us_to_s00_couplers_WREADY;
  wire [7:0]auto_us_to_s00_couplers_WSTRB;
  wire auto_us_to_s00_couplers_WVALID;
  wire [31:0]s00_couplers_to_auto_pc_ARADDR;
  wire [1:0]s00_couplers_to_auto_pc_ARBURST;
  wire [3:0]s00_couplers_to_auto_pc_ARCACHE;
  wire [7:0]s00_couplers_to_auto_pc_ARLEN;
  wire [2:0]s00_couplers_to_auto_pc_ARPROT;
  wire s00_couplers_to_auto_pc_ARREADY;
  wire [2:0]s00_couplers_to_auto_pc_ARSIZE;
  wire s00_couplers_to_auto_pc_ARVALID;
  wire [31:0]s00_couplers_to_auto_pc_AWADDR;
  wire [1:0]s00_couplers_to_auto_pc_AWBURST;
  wire [3:0]s00_couplers_to_auto_pc_AWCACHE;
  wire [7:0]s00_couplers_to_auto_pc_AWLEN;
  wire [2:0]s00_couplers_to_auto_pc_AWPROT;
  wire s00_couplers_to_auto_pc_AWREADY;
  wire [2:0]s00_couplers_to_auto_pc_AWSIZE;
  wire s00_couplers_to_auto_pc_AWVALID;
  wire s00_couplers_to_auto_pc_BREADY;
  wire [1:0]s00_couplers_to_auto_pc_BRESP;
  wire s00_couplers_to_auto_pc_BVALID;
  wire [31:0]s00_couplers_to_auto_pc_RDATA;
  wire s00_couplers_to_auto_pc_RLAST;
  wire s00_couplers_to_auto_pc_RREADY;
  wire [1:0]s00_couplers_to_auto_pc_RRESP;
  wire s00_couplers_to_auto_pc_RVALID;
  wire [31:0]s00_couplers_to_auto_pc_WDATA;
  wire s00_couplers_to_auto_pc_WLAST;
  wire s00_couplers_to_auto_pc_WREADY;
  wire [3:0]s00_couplers_to_auto_pc_WSTRB;
  wire s00_couplers_to_auto_pc_WVALID;

  assign M_AXI_araddr[31:0] = auto_us_to_s00_couplers_ARADDR;
  assign M_AXI_arburst[1:0] = auto_us_to_s00_couplers_ARBURST;
  assign M_AXI_arcache[3:0] = auto_us_to_s00_couplers_ARCACHE;
  assign M_AXI_arlen[3:0] = auto_us_to_s00_couplers_ARLEN;
  assign M_AXI_arlock[1:0] = auto_us_to_s00_couplers_ARLOCK;
  assign M_AXI_arprot[2:0] = auto_us_to_s00_couplers_ARPROT;
  assign M_AXI_arqos[3:0] = auto_us_to_s00_couplers_ARQOS;
  assign M_AXI_arsize[2:0] = auto_us_to_s00_couplers_ARSIZE;
  assign M_AXI_arvalid = auto_us_to_s00_couplers_ARVALID;
  assign M_AXI_awaddr[31:0] = auto_us_to_s00_couplers_AWADDR;
  assign M_AXI_awburst[1:0] = auto_us_to_s00_couplers_AWBURST;
  assign M_AXI_awcache[3:0] = auto_us_to_s00_couplers_AWCACHE;
  assign M_AXI_awlen[3:0] = auto_us_to_s00_couplers_AWLEN;
  assign M_AXI_awlock[1:0] = auto_us_to_s00_couplers_AWLOCK;
  assign M_AXI_awprot[2:0] = auto_us_to_s00_couplers_AWPROT;
  assign M_AXI_awqos[3:0] = auto_us_to_s00_couplers_AWQOS;
  assign M_AXI_awsize[2:0] = auto_us_to_s00_couplers_AWSIZE;
  assign M_AXI_awvalid = auto_us_to_s00_couplers_AWVALID;
  assign M_AXI_bready = auto_us_to_s00_couplers_BREADY;
  assign M_AXI_rready = auto_us_to_s00_couplers_RREADY;
  assign M_AXI_wdata[63:0] = auto_us_to_s00_couplers_WDATA;
  assign M_AXI_wlast = auto_us_to_s00_couplers_WLAST;
  assign M_AXI_wstrb[7:0] = auto_us_to_s00_couplers_WSTRB;
  assign M_AXI_wvalid = auto_us_to_s00_couplers_WVALID;
  assign S_ACLK_1 = S_ACLK;
  assign S_ARESETN_1 = S_ARESETN[0];
  assign S_AXI_arready = s00_couplers_to_auto_pc_ARREADY;
  assign S_AXI_awready = s00_couplers_to_auto_pc_AWREADY;
  assign S_AXI_bresp[1:0] = s00_couplers_to_auto_pc_BRESP;
  assign S_AXI_bvalid = s00_couplers_to_auto_pc_BVALID;
  assign S_AXI_rdata[31:0] = s00_couplers_to_auto_pc_RDATA;
  assign S_AXI_rlast = s00_couplers_to_auto_pc_RLAST;
  assign S_AXI_rresp[1:0] = s00_couplers_to_auto_pc_RRESP;
  assign S_AXI_rvalid = s00_couplers_to_auto_pc_RVALID;
  assign S_AXI_wready = s00_couplers_to_auto_pc_WREADY;
  assign auto_us_to_s00_couplers_ARREADY = M_AXI_arready;
  assign auto_us_to_s00_couplers_AWREADY = M_AXI_awready;
  assign auto_us_to_s00_couplers_BRESP = M_AXI_bresp[1:0];
  assign auto_us_to_s00_couplers_BVALID = M_AXI_bvalid;
  assign auto_us_to_s00_couplers_RDATA = M_AXI_rdata[63:0];
  assign auto_us_to_s00_couplers_RLAST = M_AXI_rlast;
  assign auto_us_to_s00_couplers_RRESP = M_AXI_rresp[1:0];
  assign auto_us_to_s00_couplers_RVALID = M_AXI_rvalid;
  assign auto_us_to_s00_couplers_WREADY = M_AXI_wready;
  assign s00_couplers_to_auto_pc_ARADDR = S_AXI_araddr[31:0];
  assign s00_couplers_to_auto_pc_ARBURST = S_AXI_arburst[1:0];
  assign s00_couplers_to_auto_pc_ARCACHE = S_AXI_arcache[3:0];
  assign s00_couplers_to_auto_pc_ARLEN = S_AXI_arlen[7:0];
  assign s00_couplers_to_auto_pc_ARPROT = S_AXI_arprot[2:0];
  assign s00_couplers_to_auto_pc_ARSIZE = S_AXI_arsize[2:0];
  assign s00_couplers_to_auto_pc_ARVALID = S_AXI_arvalid;
  assign s00_couplers_to_auto_pc_AWADDR = S_AXI_awaddr[31:0];
  assign s00_couplers_to_auto_pc_AWBURST = S_AXI_awburst[1:0];
  assign s00_couplers_to_auto_pc_AWCACHE = S_AXI_awcache[3:0];
  assign s00_couplers_to_auto_pc_AWLEN = S_AXI_awlen[7:0];
  assign s00_couplers_to_auto_pc_AWPROT = S_AXI_awprot[2:0];
  assign s00_couplers_to_auto_pc_AWSIZE = S_AXI_awsize[2:0];
  assign s00_couplers_to_auto_pc_AWVALID = S_AXI_awvalid;
  assign s00_couplers_to_auto_pc_BREADY = S_AXI_bready;
  assign s00_couplers_to_auto_pc_RREADY = S_AXI_rready;
  assign s00_couplers_to_auto_pc_WDATA = S_AXI_wdata[31:0];
  assign s00_couplers_to_auto_pc_WLAST = S_AXI_wlast;
  assign s00_couplers_to_auto_pc_WSTRB = S_AXI_wstrb[3:0];
  assign s00_couplers_to_auto_pc_WVALID = S_AXI_wvalid;
  base_zynq_design_auto_pc_1 auto_pc
       (.aclk(S_ACLK_1),
        .aresetn(S_ARESETN_1),
        .m_axi_araddr(auto_pc_to_auto_us_ARADDR),
        .m_axi_arburst(auto_pc_to_auto_us_ARBURST),
        .m_axi_arcache(auto_pc_to_auto_us_ARCACHE),
        .m_axi_arlen(auto_pc_to_auto_us_ARLEN),
        .m_axi_arlock(auto_pc_to_auto_us_ARLOCK),
        .m_axi_arprot(auto_pc_to_auto_us_ARPROT),
        .m_axi_arqos(auto_pc_to_auto_us_ARQOS),
        .m_axi_arready(auto_pc_to_auto_us_ARREADY),
        .m_axi_arsize(auto_pc_to_auto_us_ARSIZE),
        .m_axi_arvalid(auto_pc_to_auto_us_ARVALID),
        .m_axi_awaddr(auto_pc_to_auto_us_AWADDR),
        .m_axi_awburst(auto_pc_to_auto_us_AWBURST),
        .m_axi_awcache(auto_pc_to_auto_us_AWCACHE),
        .m_axi_awlen(auto_pc_to_auto_us_AWLEN),
        .m_axi_awlock(auto_pc_to_auto_us_AWLOCK),
        .m_axi_awprot(auto_pc_to_auto_us_AWPROT),
        .m_axi_awqos(auto_pc_to_auto_us_AWQOS),
        .m_axi_awready(auto_pc_to_auto_us_AWREADY),
        .m_axi_awsize(auto_pc_to_auto_us_AWSIZE),
        .m_axi_awvalid(auto_pc_to_auto_us_AWVALID),
        .m_axi_bready(auto_pc_to_auto_us_BREADY),
        .m_axi_bresp(auto_pc_to_auto_us_BRESP),
        .m_axi_bvalid(auto_pc_to_auto_us_BVALID),
        .m_axi_rdata(auto_pc_to_auto_us_RDATA),
        .m_axi_rlast(auto_pc_to_auto_us_RLAST),
        .m_axi_rready(auto_pc_to_auto_us_RREADY),
        .m_axi_rresp(auto_pc_to_auto_us_RRESP),
        .m_axi_rvalid(auto_pc_to_auto_us_RVALID),
        .m_axi_wdata(auto_pc_to_auto_us_WDATA),
        .m_axi_wlast(auto_pc_to_auto_us_WLAST),
        .m_axi_wready(auto_pc_to_auto_us_WREADY),
        .m_axi_wstrb(auto_pc_to_auto_us_WSTRB),
        .m_axi_wvalid(auto_pc_to_auto_us_WVALID),
        .s_axi_araddr(s00_couplers_to_auto_pc_ARADDR),
        .s_axi_arburst(s00_couplers_to_auto_pc_ARBURST),
        .s_axi_arcache(s00_couplers_to_auto_pc_ARCACHE),
        .s_axi_arlen(s00_couplers_to_auto_pc_ARLEN),
        .s_axi_arlock(1'b0),
        .s_axi_arprot(s00_couplers_to_auto_pc_ARPROT),
        .s_axi_arqos({1'b0,1'b0,1'b0,1'b0}),
        .s_axi_arready(s00_couplers_to_auto_pc_ARREADY),
        .s_axi_arregion({1'b0,1'b0,1'b0,1'b0}),
        .s_axi_arsize(s00_couplers_to_auto_pc_ARSIZE),
        .s_axi_arvalid(s00_couplers_to_auto_pc_ARVALID),
        .s_axi_awaddr(s00_couplers_to_auto_pc_AWADDR),
        .s_axi_awburst(s00_couplers_to_auto_pc_AWBURST),
        .s_axi_awcache(s00_couplers_to_auto_pc_AWCACHE),
        .s_axi_awlen(s00_couplers_to_auto_pc_AWLEN),
        .s_axi_awlock(1'b0),
        .s_axi_awprot(s00_couplers_to_auto_pc_AWPROT),
        .s_axi_awqos({1'b0,1'b0,1'b0,1'b0}),
        .s_axi_awready(s00_couplers_to_auto_pc_AWREADY),
        .s_axi_awregion({1'b0,1'b0,1'b0,1'b0}),
        .s_axi_awsize(s00_couplers_to_auto_pc_AWSIZE),
        .s_axi_awvalid(s00_couplers_to_auto_pc_AWVALID),
        .s_axi_bready(s00_couplers_to_auto_pc_BREADY),
        .s_axi_bresp(s00_couplers_to_auto_pc_BRESP),
        .s_axi_bvalid(s00_couplers_to_auto_pc_BVALID),
        .s_axi_rdata(s00_couplers_to_auto_pc_RDATA),
        .s_axi_rlast(s00_couplers_to_auto_pc_RLAST),
        .s_axi_rready(s00_couplers_to_auto_pc_RREADY),
        .s_axi_rresp(s00_couplers_to_auto_pc_RRESP),
        .s_axi_rvalid(s00_couplers_to_auto_pc_RVALID),
        .s_axi_wdata(s00_couplers_to_auto_pc_WDATA),
        .s_axi_wlast(s00_couplers_to_auto_pc_WLAST),
        .s_axi_wready(s00_couplers_to_auto_pc_WREADY),
        .s_axi_wstrb(s00_couplers_to_auto_pc_WSTRB),
        .s_axi_wvalid(s00_couplers_to_auto_pc_WVALID));
  base_zynq_design_auto_us_0 auto_us
       (.m_axi_araddr(auto_us_to_s00_couplers_ARADDR),
        .m_axi_arburst(auto_us_to_s00_couplers_ARBURST),
        .m_axi_arcache(auto_us_to_s00_couplers_ARCACHE),
        .m_axi_arlen(auto_us_to_s00_couplers_ARLEN),
        .m_axi_arlock(auto_us_to_s00_couplers_ARLOCK),
        .m_axi_arprot(auto_us_to_s00_couplers_ARPROT),
        .m_axi_arqos(auto_us_to_s00_couplers_ARQOS),
        .m_axi_arready(auto_us_to_s00_couplers_ARREADY),
        .m_axi_arsize(auto_us_to_s00_couplers_ARSIZE),
        .m_axi_arvalid(auto_us_to_s00_couplers_ARVALID),
        .m_axi_awaddr(auto_us_to_s00_couplers_AWADDR),
        .m_axi_awburst(auto_us_to_s00_couplers_AWBURST),
        .m_axi_awcache(auto_us_to_s00_couplers_AWCACHE),
        .m_axi_awlen(auto_us_to_s00_couplers_AWLEN),
        .m_axi_awlock(auto_us_to_s00_couplers_AWLOCK),
        .m_axi_awprot(auto_us_to_s00_couplers_AWPROT),
        .m_axi_awqos(auto_us_to_s00_couplers_AWQOS),
        .m_axi_awready(auto_us_to_s00_couplers_AWREADY),
        .m_axi_awsize(auto_us_to_s00_couplers_AWSIZE),
        .m_axi_awvalid(auto_us_to_s00_couplers_AWVALID),
        .m_axi_bready(auto_us_to_s00_couplers_BREADY),
        .m_axi_bresp(auto_us_to_s00_couplers_BRESP),
        .m_axi_bvalid(auto_us_to_s00_couplers_BVALID),
        .m_axi_rdata(auto_us_to_s00_couplers_RDATA),
        .m_axi_rlast(auto_us_to_s00_couplers_RLAST),
        .m_axi_rready(auto_us_to_s00_couplers_RREADY),
        .m_axi_rresp(auto_us_to_s00_couplers_RRESP),
        .m_axi_rvalid(auto_us_to_s00_couplers_RVALID),
        .m_axi_wdata(auto_us_to_s00_couplers_WDATA),
        .m_axi_wlast(auto_us_to_s00_couplers_WLAST),
        .m_axi_wready(auto_us_to_s00_couplers_WREADY),
        .m_axi_wstrb(auto_us_to_s00_couplers_WSTRB),
        .m_axi_wvalid(auto_us_to_s00_couplers_WVALID),
        .s_axi_aclk(S_ACLK_1),
        .s_axi_araddr(auto_pc_to_auto_us_ARADDR),
        .s_axi_arburst(auto_pc_to_auto_us_ARBURST),
        .s_axi_arcache(auto_pc_to_auto_us_ARCACHE),
        .s_axi_aresetn(S_ARESETN_1),
        .s_axi_arlen(auto_pc_to_auto_us_ARLEN),
        .s_axi_arlock(auto_pc_to_auto_us_ARLOCK),
        .s_axi_arprot(auto_pc_to_auto_us_ARPROT),
        .s_axi_arqos(auto_pc_to_auto_us_ARQOS),
        .s_axi_arready(auto_pc_to_auto_us_ARREADY),
        .s_axi_arsize(auto_pc_to_auto_us_ARSIZE),
        .s_axi_arvalid(auto_pc_to_auto_us_ARVALID),
        .s_axi_awaddr(auto_pc_to_auto_us_AWADDR),
        .s_axi_awburst(auto_pc_to_auto_us_AWBURST),
        .s_axi_awcache(auto_pc_to_auto_us_AWCACHE),
        .s_axi_awlen(auto_pc_to_auto_us_AWLEN),
        .s_axi_awlock(auto_pc_to_auto_us_AWLOCK),
        .s_axi_awprot(auto_pc_to_auto_us_AWPROT),
        .s_axi_awqos(auto_pc_to_auto_us_AWQOS),
        .s_axi_awready(auto_pc_to_auto_us_AWREADY),
        .s_axi_awsize(auto_pc_to_auto_us_AWSIZE),
        .s_axi_awvalid(auto_pc_to_auto_us_AWVALID),
        .s_axi_bready(auto_pc_to_auto_us_BREADY),
        .s_axi_bresp(auto_pc_to_auto_us_BRESP),
        .s_axi_bvalid(auto_pc_to_auto_us_BVALID),
        .s_axi_rdata(auto_pc_to_auto_us_RDATA),
        .s_axi_rlast(auto_pc_to_auto_us_RLAST),
        .s_axi_rready(auto_pc_to_auto_us_RREADY),
        .s_axi_rresp(auto_pc_to_auto_us_RRESP),
        .s_axi_rvalid(auto_pc_to_auto_us_RVALID),
        .s_axi_wdata(auto_pc_to_auto_us_WDATA),
        .s_axi_wlast(auto_pc_to_auto_us_WLAST),
        .s_axi_wready(auto_pc_to_auto_us_WREADY),
        .s_axi_wstrb(auto_pc_to_auto_us_WSTRB),
        .s_axi_wvalid(auto_pc_to_auto_us_WVALID));
endmodule

module s00_couplers_imp_GKK9VK
   (M_ACLK,
    M_ARESETN,
    M_AXI_awaddr,
    M_AXI_awburst,
    M_AXI_awcache,
    M_AXI_awlen,
    M_AXI_awlock,
    M_AXI_awprot,
    M_AXI_awqos,
    M_AXI_awready,
    M_AXI_awsize,
    M_AXI_awvalid,
    M_AXI_bready,
    M_AXI_bresp,
    M_AXI_bvalid,
    M_AXI_wdata,
    M_AXI_wlast,
    M_AXI_wready,
    M_AXI_wstrb,
    M_AXI_wvalid,
    S_ACLK,
    S_ARESETN,
    S_AXI_awaddr,
    S_AXI_awburst,
    S_AXI_awcache,
    S_AXI_awlen,
    S_AXI_awprot,
    S_AXI_awready,
    S_AXI_awsize,
    S_AXI_awvalid,
    S_AXI_bready,
    S_AXI_bresp,
    S_AXI_bvalid,
    S_AXI_wdata,
    S_AXI_wlast,
    S_AXI_wready,
    S_AXI_wstrb,
    S_AXI_wvalid);
  input M_ACLK;
  input [0:0]M_ARESETN;
  output [31:0]M_AXI_awaddr;
  output [1:0]M_AXI_awburst;
  output [3:0]M_AXI_awcache;
  output [3:0]M_AXI_awlen;
  output [1:0]M_AXI_awlock;
  output [2:0]M_AXI_awprot;
  output [3:0]M_AXI_awqos;
  input M_AXI_awready;
  output [2:0]M_AXI_awsize;
  output M_AXI_awvalid;
  output M_AXI_bready;
  input [1:0]M_AXI_bresp;
  input M_AXI_bvalid;
  output [63:0]M_AXI_wdata;
  output M_AXI_wlast;
  input M_AXI_wready;
  output [7:0]M_AXI_wstrb;
  output M_AXI_wvalid;
  input S_ACLK;
  input [0:0]S_ARESETN;
  input [31:0]S_AXI_awaddr;
  input [1:0]S_AXI_awburst;
  input [3:0]S_AXI_awcache;
  input [7:0]S_AXI_awlen;
  input [2:0]S_AXI_awprot;
  output S_AXI_awready;
  input [2:0]S_AXI_awsize;
  input S_AXI_awvalid;
  input S_AXI_bready;
  output [1:0]S_AXI_bresp;
  output S_AXI_bvalid;
  input [31:0]S_AXI_wdata;
  input S_AXI_wlast;
  output S_AXI_wready;
  input [3:0]S_AXI_wstrb;
  input S_AXI_wvalid;

  wire S_ACLK_1;
  wire [0:0]S_ARESETN_1;
  wire [31:0]auto_pc_to_auto_us_AWADDR;
  wire [1:0]auto_pc_to_auto_us_AWBURST;
  wire [3:0]auto_pc_to_auto_us_AWCACHE;
  wire [3:0]auto_pc_to_auto_us_AWLEN;
  wire [1:0]auto_pc_to_auto_us_AWLOCK;
  wire [2:0]auto_pc_to_auto_us_AWPROT;
  wire [3:0]auto_pc_to_auto_us_AWQOS;
  wire auto_pc_to_auto_us_AWREADY;
  wire [2:0]auto_pc_to_auto_us_AWSIZE;
  wire auto_pc_to_auto_us_AWVALID;
  wire auto_pc_to_auto_us_BREADY;
  wire [1:0]auto_pc_to_auto_us_BRESP;
  wire auto_pc_to_auto_us_BVALID;
  wire [31:0]auto_pc_to_auto_us_WDATA;
  wire auto_pc_to_auto_us_WLAST;
  wire auto_pc_to_auto_us_WREADY;
  wire [3:0]auto_pc_to_auto_us_WSTRB;
  wire auto_pc_to_auto_us_WVALID;
  wire [31:0]auto_us_to_s00_couplers_AWADDR;
  wire [1:0]auto_us_to_s00_couplers_AWBURST;
  wire [3:0]auto_us_to_s00_couplers_AWCACHE;
  wire [3:0]auto_us_to_s00_couplers_AWLEN;
  wire [1:0]auto_us_to_s00_couplers_AWLOCK;
  wire [2:0]auto_us_to_s00_couplers_AWPROT;
  wire [3:0]auto_us_to_s00_couplers_AWQOS;
  wire auto_us_to_s00_couplers_AWREADY;
  wire [2:0]auto_us_to_s00_couplers_AWSIZE;
  wire auto_us_to_s00_couplers_AWVALID;
  wire auto_us_to_s00_couplers_BREADY;
  wire [1:0]auto_us_to_s00_couplers_BRESP;
  wire auto_us_to_s00_couplers_BVALID;
  wire [63:0]auto_us_to_s00_couplers_WDATA;
  wire auto_us_to_s00_couplers_WLAST;
  wire auto_us_to_s00_couplers_WREADY;
  wire [7:0]auto_us_to_s00_couplers_WSTRB;
  wire auto_us_to_s00_couplers_WVALID;
  wire [31:0]s00_couplers_to_auto_pc_AWADDR;
  wire [1:0]s00_couplers_to_auto_pc_AWBURST;
  wire [3:0]s00_couplers_to_auto_pc_AWCACHE;
  wire [7:0]s00_couplers_to_auto_pc_AWLEN;
  wire [2:0]s00_couplers_to_auto_pc_AWPROT;
  wire s00_couplers_to_auto_pc_AWREADY;
  wire [2:0]s00_couplers_to_auto_pc_AWSIZE;
  wire s00_couplers_to_auto_pc_AWVALID;
  wire s00_couplers_to_auto_pc_BREADY;
  wire [1:0]s00_couplers_to_auto_pc_BRESP;
  wire s00_couplers_to_auto_pc_BVALID;
  wire [31:0]s00_couplers_to_auto_pc_WDATA;
  wire s00_couplers_to_auto_pc_WLAST;
  wire s00_couplers_to_auto_pc_WREADY;
  wire [3:0]s00_couplers_to_auto_pc_WSTRB;
  wire s00_couplers_to_auto_pc_WVALID;

  assign M_AXI_awaddr[31:0] = auto_us_to_s00_couplers_AWADDR;
  assign M_AXI_awburst[1:0] = auto_us_to_s00_couplers_AWBURST;
  assign M_AXI_awcache[3:0] = auto_us_to_s00_couplers_AWCACHE;
  assign M_AXI_awlen[3:0] = auto_us_to_s00_couplers_AWLEN;
  assign M_AXI_awlock[1:0] = auto_us_to_s00_couplers_AWLOCK;
  assign M_AXI_awprot[2:0] = auto_us_to_s00_couplers_AWPROT;
  assign M_AXI_awqos[3:0] = auto_us_to_s00_couplers_AWQOS;
  assign M_AXI_awsize[2:0] = auto_us_to_s00_couplers_AWSIZE;
  assign M_AXI_awvalid = auto_us_to_s00_couplers_AWVALID;
  assign M_AXI_bready = auto_us_to_s00_couplers_BREADY;
  assign M_AXI_wdata[63:0] = auto_us_to_s00_couplers_WDATA;
  assign M_AXI_wlast = auto_us_to_s00_couplers_WLAST;
  assign M_AXI_wstrb[7:0] = auto_us_to_s00_couplers_WSTRB;
  assign M_AXI_wvalid = auto_us_to_s00_couplers_WVALID;
  assign S_ACLK_1 = S_ACLK;
  assign S_ARESETN_1 = S_ARESETN[0];
  assign S_AXI_awready = s00_couplers_to_auto_pc_AWREADY;
  assign S_AXI_bresp[1:0] = s00_couplers_to_auto_pc_BRESP;
  assign S_AXI_bvalid = s00_couplers_to_auto_pc_BVALID;
  assign S_AXI_wready = s00_couplers_to_auto_pc_WREADY;
  assign auto_us_to_s00_couplers_AWREADY = M_AXI_awready;
  assign auto_us_to_s00_couplers_BRESP = M_AXI_bresp[1:0];
  assign auto_us_to_s00_couplers_BVALID = M_AXI_bvalid;
  assign auto_us_to_s00_couplers_WREADY = M_AXI_wready;
  assign s00_couplers_to_auto_pc_AWADDR = S_AXI_awaddr[31:0];
  assign s00_couplers_to_auto_pc_AWBURST = S_AXI_awburst[1:0];
  assign s00_couplers_to_auto_pc_AWCACHE = S_AXI_awcache[3:0];
  assign s00_couplers_to_auto_pc_AWLEN = S_AXI_awlen[7:0];
  assign s00_couplers_to_auto_pc_AWPROT = S_AXI_awprot[2:0];
  assign s00_couplers_to_auto_pc_AWSIZE = S_AXI_awsize[2:0];
  assign s00_couplers_to_auto_pc_AWVALID = S_AXI_awvalid;
  assign s00_couplers_to_auto_pc_BREADY = S_AXI_bready;
  assign s00_couplers_to_auto_pc_WDATA = S_AXI_wdata[31:0];
  assign s00_couplers_to_auto_pc_WLAST = S_AXI_wlast;
  assign s00_couplers_to_auto_pc_WSTRB = S_AXI_wstrb[3:0];
  assign s00_couplers_to_auto_pc_WVALID = S_AXI_wvalid;
  base_zynq_design_auto_pc_2 auto_pc
       (.aclk(S_ACLK_1),
        .aresetn(S_ARESETN_1),
        .m_axi_awaddr(auto_pc_to_auto_us_AWADDR),
        .m_axi_awburst(auto_pc_to_auto_us_AWBURST),
        .m_axi_awcache(auto_pc_to_auto_us_AWCACHE),
        .m_axi_awlen(auto_pc_to_auto_us_AWLEN),
        .m_axi_awlock(auto_pc_to_auto_us_AWLOCK),
        .m_axi_awprot(auto_pc_to_auto_us_AWPROT),
        .m_axi_awqos(auto_pc_to_auto_us_AWQOS),
        .m_axi_awready(auto_pc_to_auto_us_AWREADY),
        .m_axi_awsize(auto_pc_to_auto_us_AWSIZE),
        .m_axi_awvalid(auto_pc_to_auto_us_AWVALID),
        .m_axi_bready(auto_pc_to_auto_us_BREADY),
        .m_axi_bresp(auto_pc_to_auto_us_BRESP),
        .m_axi_bvalid(auto_pc_to_auto_us_BVALID),
        .m_axi_wdata(auto_pc_to_auto_us_WDATA),
        .m_axi_wlast(auto_pc_to_auto_us_WLAST),
        .m_axi_wready(auto_pc_to_auto_us_WREADY),
        .m_axi_wstrb(auto_pc_to_auto_us_WSTRB),
        .m_axi_wvalid(auto_pc_to_auto_us_WVALID),
        .s_axi_awaddr(s00_couplers_to_auto_pc_AWADDR),
        .s_axi_awburst(s00_couplers_to_auto_pc_AWBURST),
        .s_axi_awcache(s00_couplers_to_auto_pc_AWCACHE),
        .s_axi_awlen(s00_couplers_to_auto_pc_AWLEN),
        .s_axi_awlock(1'b0),
        .s_axi_awprot(s00_couplers_to_auto_pc_AWPROT),
        .s_axi_awqos({1'b0,1'b0,1'b0,1'b0}),
        .s_axi_awready(s00_couplers_to_auto_pc_AWREADY),
        .s_axi_awregion({1'b0,1'b0,1'b0,1'b0}),
        .s_axi_awsize(s00_couplers_to_auto_pc_AWSIZE),
        .s_axi_awvalid(s00_couplers_to_auto_pc_AWVALID),
        .s_axi_bready(s00_couplers_to_auto_pc_BREADY),
        .s_axi_bresp(s00_couplers_to_auto_pc_BRESP),
        .s_axi_bvalid(s00_couplers_to_auto_pc_BVALID),
        .s_axi_wdata(s00_couplers_to_auto_pc_WDATA),
        .s_axi_wlast(s00_couplers_to_auto_pc_WLAST),
        .s_axi_wready(s00_couplers_to_auto_pc_WREADY),
        .s_axi_wstrb(s00_couplers_to_auto_pc_WSTRB),
        .s_axi_wvalid(s00_couplers_to_auto_pc_WVALID));
  base_zynq_design_auto_us_1 auto_us
       (.m_axi_awaddr(auto_us_to_s00_couplers_AWADDR),
        .m_axi_awburst(auto_us_to_s00_couplers_AWBURST),
        .m_axi_awcache(auto_us_to_s00_couplers_AWCACHE),
        .m_axi_awlen(auto_us_to_s00_couplers_AWLEN),
        .m_axi_awlock(auto_us_to_s00_couplers_AWLOCK),
        .m_axi_awprot(auto_us_to_s00_couplers_AWPROT),
        .m_axi_awqos(auto_us_to_s00_couplers_AWQOS),
        .m_axi_awready(auto_us_to_s00_couplers_AWREADY),
        .m_axi_awsize(auto_us_to_s00_couplers_AWSIZE),
        .m_axi_awvalid(auto_us_to_s00_couplers_AWVALID),
        .m_axi_bready(auto_us_to_s00_couplers_BREADY),
        .m_axi_bresp(auto_us_to_s00_couplers_BRESP),
        .m_axi_bvalid(auto_us_to_s00_couplers_BVALID),
        .m_axi_wdata(auto_us_to_s00_couplers_WDATA),
        .m_axi_wlast(auto_us_to_s00_couplers_WLAST),
        .m_axi_wready(auto_us_to_s00_couplers_WREADY),
        .m_axi_wstrb(auto_us_to_s00_couplers_WSTRB),
        .m_axi_wvalid(auto_us_to_s00_couplers_WVALID),
        .s_axi_aclk(S_ACLK_1),
        .s_axi_aresetn(S_ARESETN_1),
        .s_axi_awaddr(auto_pc_to_auto_us_AWADDR),
        .s_axi_awburst(auto_pc_to_auto_us_AWBURST),
        .s_axi_awcache(auto_pc_to_auto_us_AWCACHE),
        .s_axi_awlen(auto_pc_to_auto_us_AWLEN),
        .s_axi_awlock(auto_pc_to_auto_us_AWLOCK),
        .s_axi_awprot(auto_pc_to_auto_us_AWPROT),
        .s_axi_awqos(auto_pc_to_auto_us_AWQOS),
        .s_axi_awready(auto_pc_to_auto_us_AWREADY),
        .s_axi_awsize(auto_pc_to_auto_us_AWSIZE),
        .s_axi_awvalid(auto_pc_to_auto_us_AWVALID),
        .s_axi_bready(auto_pc_to_auto_us_BREADY),
        .s_axi_bresp(auto_pc_to_auto_us_BRESP),
        .s_axi_bvalid(auto_pc_to_auto_us_BVALID),
        .s_axi_wdata(auto_pc_to_auto_us_WDATA),
        .s_axi_wlast(auto_pc_to_auto_us_WLAST),
        .s_axi_wready(auto_pc_to_auto_us_WREADY),
        .s_axi_wstrb(auto_pc_to_auto_us_WSTRB),
        .s_axi_wvalid(auto_pc_to_auto_us_WVALID));
endmodule

module s01_couplers_imp_PJOTIM
   (M_AXIS_ACLK,
    M_AXIS_ARESETN,
    M_AXIS_tdata,
    M_AXIS_tlast,
    M_AXIS_tready,
    M_AXIS_tvalid,
    S_AXIS_ACLK,
    S_AXIS_ARESETN,
    S_AXIS_tdata,
    S_AXIS_tlast,
    S_AXIS_tready,
    S_AXIS_tvalid);
  input M_AXIS_ACLK;
  input [0:0]M_AXIS_ARESETN;
  output [31:0]M_AXIS_tdata;
  output M_AXIS_tlast;
  input M_AXIS_tready;
  output M_AXIS_tvalid;
  input S_AXIS_ACLK;
  input [0:0]S_AXIS_ARESETN;
  input [31:0]S_AXIS_tdata;
  input S_AXIS_tlast;
  output S_AXIS_tready;
  input S_AXIS_tvalid;

  wire [31:0]AXIS_DATA_COUNT_to_S_AXIS_DATA_COUNT;
  wire [31:0]AXIS_RD_DATA_COUNT_to_S_AXIS_RD_DATA_COUNT;
  wire [31:0]AXIS_WR_DATA_COUNT_to_S_AXIS_WR_DATA_COUNT;
  wire M_AXIS_ACLK;
  wire [0:0]M_AXIS_ARESETN;
  wire [31:0]s01_couplers_to_s01_data_fifo_TDATA;
  wire s01_couplers_to_s01_data_fifo_TLAST;
  wire s01_couplers_to_s01_data_fifo_TREADY;
  wire s01_couplers_to_s01_data_fifo_TVALID;
  wire [31:0]s01_data_fifo_to_s01_couplers_TDATA;
  wire s01_data_fifo_to_s01_couplers_TLAST;
  wire s01_data_fifo_to_s01_couplers_TREADY;
  wire s01_data_fifo_to_s01_couplers_TVALID;

  assign M_AXIS_tdata[31:0] = s01_data_fifo_to_s01_couplers_TDATA;
  assign M_AXIS_tlast = s01_data_fifo_to_s01_couplers_TLAST;
  assign M_AXIS_tvalid = s01_data_fifo_to_s01_couplers_TVALID;
  assign S_AXIS_tready = s01_couplers_to_s01_data_fifo_TREADY;
  assign s01_couplers_to_s01_data_fifo_TDATA = S_AXIS_tdata[31:0];
  assign s01_couplers_to_s01_data_fifo_TLAST = S_AXIS_tlast;
  assign s01_couplers_to_s01_data_fifo_TVALID = S_AXIS_tvalid;
  assign s01_data_fifo_to_s01_couplers_TREADY = M_AXIS_tready;
  base_zynq_design_s01_data_fifo_0 s01_data_fifo
       (.axis_data_count(AXIS_DATA_COUNT_to_S_AXIS_DATA_COUNT),
        .axis_rd_data_count(AXIS_RD_DATA_COUNT_to_S_AXIS_RD_DATA_COUNT),
        .axis_wr_data_count(AXIS_WR_DATA_COUNT_to_S_AXIS_WR_DATA_COUNT),
        .m_axis_tdata(s01_data_fifo_to_s01_couplers_TDATA),
        .m_axis_tlast(s01_data_fifo_to_s01_couplers_TLAST),
        .m_axis_tready(s01_data_fifo_to_s01_couplers_TREADY),
        .m_axis_tvalid(s01_data_fifo_to_s01_couplers_TVALID),
        .s_axis_aclk(M_AXIS_ACLK),
        .s_axis_aresetn(M_AXIS_ARESETN),
        .s_axis_tdata(s01_couplers_to_s01_data_fifo_TDATA),
        .s_axis_tlast(s01_couplers_to_s01_data_fifo_TLAST),
        .s_axis_tready(s01_couplers_to_s01_data_fifo_TREADY),
        .s_axis_tvalid(s01_couplers_to_s01_data_fifo_TVALID));
endmodule
