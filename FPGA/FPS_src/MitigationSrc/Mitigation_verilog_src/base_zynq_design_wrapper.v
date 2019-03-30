//Copyright 1986-2016 Xilinx, Inc. All Rights Reserved.
//--------------------------------------------------------------------------------
//Tool Version: Vivado v.2016.1 (lin64) Build 1538259 Fri Apr  8 15:45:23 MDT 2016
//Date        : Wed Nov 16 10:35:44 2016
//Host        : localhost.localdomain running 64-bit CentOS Linux release 7.2.1511 (Core)
//Command     : generate_target base_zynq_design_wrapper.bd
//Design      : base_zynq_design_wrapper
//Purpose     : IP block netlist
//--------------------------------------------------------------------------------
`timescale 1 ps / 1 ps

module base_zynq_design_wrapper
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
    auroraUserClk,
    gpio_aurora_a_tri_io,
    gpio_aurora_b_tri_i,
    gpio_evr_gtx_tri_io,
    gpio_fan_speed_tri_i,
    gpio_firmware_date_tri_i,
    gpio_mitigation_control_tri_o,
    gpio_sfp_status_tri_i,
    gpio_stale_nodes_hi_tri_i,
    gpio_stale_nodes_lo_tri_i,
    gpio_sysmon_readout_tri_io,
    gpio_timestamp_seconds_tri_i,
    gpio_timestamp_ticks_tri_i,
    mitigationBRAMctrl_addr,
    mitigationBRAMctrl_clk,
    mitigationBRAMctrl_din,
    mitigationBRAMctrl_dout,
    mitigationBRAMctrl_en,
    mitigationBRAMctrl_rst,
    mitigationBRAMctrl_we,
    mitigationClk,
    sfp_iic_scl_io,
    sfp_iic_sda_io,
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
  output auroraUserClk;
  inout [31:0]gpio_aurora_a_tri_io;
  input [31:0]gpio_aurora_b_tri_i;
  inout [3:0]gpio_evr_gtx_tri_io;
  input [31:0]gpio_fan_speed_tri_i;
  input [31:0]gpio_firmware_date_tri_i;
  output [31:0]gpio_mitigation_control_tri_o;
  input [31:0]gpio_sfp_status_tri_i;
  input [31:0]gpio_stale_nodes_hi_tri_i;
  input [31:0]gpio_stale_nodes_lo_tri_i;
  inout [15:0]gpio_sysmon_readout_tri_io;
  input [31:0]gpio_timestamp_seconds_tri_i;
  input [31:0]gpio_timestamp_ticks_tri_i;
  output [15:0]mitigationBRAMctrl_addr;
  output mitigationBRAMctrl_clk;
  output [31:0]mitigationBRAMctrl_din;
  input [31:0]mitigationBRAMctrl_dout;
  output mitigationBRAMctrl_en;
  output mitigationBRAMctrl_rst;
  output [3:0]mitigationBRAMctrl_we;
  output mitigationClk;
  inout sfp_iic_scl_io;
  inout sfp_iic_sda_io;
  output sysClk;
  output [0:0]sysReset_n;

  wire Aurora0_CORE_STATUS_channel_up;
  wire Aurora0_CORE_STATUS_frame_err;
  wire Aurora0_CORE_STATUS_hard_err;
  wire [0:0]Aurora0_CORE_STATUS_lane_up;
  wire Aurora0_CORE_STATUS_rx_resetdone_out;
  wire Aurora0_CORE_STATUS_soft_err;
  wire Aurora0_CORE_STATUS_tx_lock;
  wire Aurora0_CORE_STATUS_tx_resetdone_out;
  wire [0:0]Aurora0_RX_rxn;
  wire [0:0]Aurora0_RX_rxp;
  wire [0:0]Aurora0_TX_txn;
  wire [0:0]Aurora0_TX_txp;
  wire [0:15]Aurora0_USER_DATA_M_AXI_RX_tdata;
  wire [0:1]Aurora0_USER_DATA_M_AXI_RX_tkeep;
  wire Aurora0_USER_DATA_M_AXI_RX_tlast;
  wire Aurora0_USER_DATA_M_AXI_RX_tvalid;
  wire [0:15]Aurora0_USER_DATA_S_AXI_TX_tdata;
  wire [0:1]Aurora0_USER_DATA_S_AXI_TX_tkeep;
  wire Aurora0_USER_DATA_S_AXI_TX_tlast;
  wire Aurora0_USER_DATA_S_AXI_TX_tready;
  wire Aurora0_USER_DATA_S_AXI_TX_tvalid;
  wire Aurora1_CORE_STATUS_channel_up;
  wire Aurora1_CORE_STATUS_frame_err;
  wire Aurora1_CORE_STATUS_hard_err;
  wire [0:0]Aurora1_CORE_STATUS_lane_up;
  wire Aurora1_CORE_STATUS_rx_resetdone_out;
  wire Aurora1_CORE_STATUS_soft_err;
  wire Aurora1_CORE_STATUS_tx_lock;
  wire Aurora1_CORE_STATUS_tx_resetdone_out;
  wire [0:0]Aurora1_RX_rxn;
  wire [0:0]Aurora1_RX_rxp;
  wire [0:0]Aurora1_TX_txn;
  wire [0:0]Aurora1_TX_txp;
  wire [0:15]Aurora1_USER_DATA_M_AXI_RX_tdata;
  wire [0:1]Aurora1_USER_DATA_M_AXI_RX_tkeep;
  wire Aurora1_USER_DATA_M_AXI_RX_tlast;
  wire Aurora1_USER_DATA_M_AXI_RX_tvalid;
  wire [0:15]Aurora1_USER_DATA_S_AXI_TX_tdata;
  wire [0:1]Aurora1_USER_DATA_S_AXI_TX_tkeep;
  wire Aurora1_USER_DATA_S_AXI_TX_tlast;
  wire Aurora1_USER_DATA_S_AXI_TX_tready;
  wire Aurora1_USER_DATA_S_AXI_TX_tvalid;
  wire Aurora2_CORE_STATUS_channel_up;
  wire Aurora2_CORE_STATUS_frame_err;
  wire Aurora2_CORE_STATUS_hard_err;
  wire [0:0]Aurora2_CORE_STATUS_lane_up;
  wire Aurora2_CORE_STATUS_rx_resetdone_out;
  wire Aurora2_CORE_STATUS_soft_err;
  wire Aurora2_CORE_STATUS_tx_lock;
  wire Aurora2_CORE_STATUS_tx_resetdone_out;
  wire [0:0]Aurora2_RX_rxn;
  wire [0:0]Aurora2_RX_rxp;
  wire [0:0]Aurora2_TX_txn;
  wire [0:0]Aurora2_TX_txp;
  wire [0:15]Aurora2_USER_DATA_M_AXI_RX_tdata;
  wire [0:1]Aurora2_USER_DATA_M_AXI_RX_tkeep;
  wire Aurora2_USER_DATA_M_AXI_RX_tlast;
  wire Aurora2_USER_DATA_M_AXI_RX_tvalid;
  wire [0:15]Aurora2_USER_DATA_S_AXI_TX_tdata;
  wire [0:1]Aurora2_USER_DATA_S_AXI_TX_tkeep;
  wire Aurora2_USER_DATA_S_AXI_TX_tlast;
  wire Aurora2_USER_DATA_S_AXI_TX_tready;
  wire Aurora2_USER_DATA_S_AXI_TX_tvalid;
  wire Aurora3_CORE_STATUS_channel_up;
  wire Aurora3_CORE_STATUS_frame_err;
  wire Aurora3_CORE_STATUS_hard_err;
  wire [0:0]Aurora3_CORE_STATUS_lane_up;
  wire Aurora3_CORE_STATUS_rx_resetdone_out;
  wire Aurora3_CORE_STATUS_soft_err;
  wire Aurora3_CORE_STATUS_tx_lock;
  wire Aurora3_CORE_STATUS_tx_resetdone_out;
  wire [0:0]Aurora3_RX_rxn;
  wire [0:0]Aurora3_RX_rxp;
  wire [0:0]Aurora3_TX_txn;
  wire [0:0]Aurora3_TX_txp;
  wire [0:15]Aurora3_USER_DATA_M_AXI_RX_tdata;
  wire [0:1]Aurora3_USER_DATA_M_AXI_RX_tkeep;
  wire Aurora3_USER_DATA_M_AXI_RX_tlast;
  wire Aurora3_USER_DATA_M_AXI_RX_tvalid;
  wire [0:15]Aurora3_USER_DATA_S_AXI_TX_tdata;
  wire [0:1]Aurora3_USER_DATA_S_AXI_TX_tkeep;
  wire Aurora3_USER_DATA_S_AXI_TX_tlast;
  wire Aurora3_USER_DATA_S_AXI_TX_tready;
  wire Aurora3_USER_DATA_S_AXI_TX_tvalid;
  wire Aurora4_CORE_STATUS_channel_up;
  wire Aurora4_CORE_STATUS_frame_err;
  wire Aurora4_CORE_STATUS_hard_err;
  wire [0:0]Aurora4_CORE_STATUS_lane_up;
  wire Aurora4_CORE_STATUS_rx_resetdone_out;
  wire Aurora4_CORE_STATUS_soft_err;
  wire Aurora4_CORE_STATUS_tx_lock;
  wire Aurora4_CORE_STATUS_tx_resetdone_out;
  wire [0:0]Aurora4_RX_rxn;
  wire [0:0]Aurora4_RX_rxp;
  wire [0:0]Aurora4_TX_txn;
  wire [0:0]Aurora4_TX_txp;
  wire [15:0]Aurora4_USER_DATA_M_AXI_RX_tdata;
  wire [1:0]Aurora4_USER_DATA_M_AXI_RX_tkeep;
  wire Aurora4_USER_DATA_M_AXI_RX_tlast;
  wire Aurora4_USER_DATA_M_AXI_RX_tready;
  wire Aurora4_USER_DATA_M_AXI_RX_tvalid;
  wire [0:15]Aurora4_USER_DATA_S_AXI_TX_tdata;
  wire [0:1]Aurora4_USER_DATA_S_AXI_TX_tkeep;
  wire Aurora4_USER_DATA_S_AXI_TX_tlast;
  wire Aurora4_USER_DATA_S_AXI_TX_tready;
  wire Aurora4_USER_DATA_S_AXI_TX_tvalid;
  wire Aurora5_CORE_STATUS_channel_up;
  wire Aurora5_CORE_STATUS_frame_err;
  wire Aurora5_CORE_STATUS_hard_err;
  wire [0:0]Aurora5_CORE_STATUS_lane_up;
  wire Aurora5_CORE_STATUS_rx_resetdone_out;
  wire Aurora5_CORE_STATUS_soft_err;
  wire Aurora5_CORE_STATUS_tx_lock;
  wire Aurora5_CORE_STATUS_tx_resetdone_out;
  wire [0:0]Aurora5_RX_rxn;
  wire [0:0]Aurora5_RX_rxp;
  wire [0:0]Aurora5_TX_txn;
  wire [0:0]Aurora5_TX_txp;
  wire [15:0]Aurora5_USER_DATA_M_AXI_RX_tdata;
  wire [1:0]Aurora5_USER_DATA_M_AXI_RX_tkeep;
  wire Aurora5_USER_DATA_M_AXI_RX_tlast;
  wire Aurora5_USER_DATA_M_AXI_RX_tready;
  wire Aurora5_USER_DATA_M_AXI_RX_tvalid;
  wire [0:15]Aurora5_USER_DATA_S_AXI_TX_tdata;
  wire [0:1]Aurora5_USER_DATA_S_AXI_TX_tkeep;
  wire Aurora5_USER_DATA_S_AXI_TX_tlast;
  wire Aurora5_USER_DATA_S_AXI_TX_tready;
  wire Aurora5_USER_DATA_S_AXI_TX_tvalid;
  wire Aurora6_CORE_STATUS_channel_up;
  wire Aurora6_CORE_STATUS_frame_err;
  wire Aurora6_CORE_STATUS_hard_err;
  wire [0:0]Aurora6_CORE_STATUS_lane_up;
  wire Aurora6_CORE_STATUS_rx_resetdone_out;
  wire Aurora6_CORE_STATUS_soft_err;
  wire Aurora6_CORE_STATUS_tx_lock;
  wire Aurora6_CORE_STATUS_tx_resetdone_out;
  wire [0:0]Aurora6_RX_rxn;
  wire [0:0]Aurora6_RX_rxp;
  wire [0:0]Aurora6_TX_txn;
  wire [0:0]Aurora6_TX_txp;
  wire [15:0]Aurora6_USER_DATA_M_AXI_RX_tdata;
  wire [1:0]Aurora6_USER_DATA_M_AXI_RX_tkeep;
  wire Aurora6_USER_DATA_M_AXI_RX_tlast;
  wire Aurora6_USER_DATA_M_AXI_RX_tready;
  wire Aurora6_USER_DATA_M_AXI_RX_tvalid;
  wire [0:15]Aurora6_USER_DATA_S_AXI_TX_tdata;
  wire [0:1]Aurora6_USER_DATA_S_AXI_TX_tkeep;
  wire Aurora6_USER_DATA_S_AXI_TX_tlast;
  wire Aurora6_USER_DATA_S_AXI_TX_tready;
  wire Aurora6_USER_DATA_S_AXI_TX_tvalid;
  wire Aurora7_CORE_STATUS_channel_up;
  wire Aurora7_CORE_STATUS_frame_err;
  wire Aurora7_CORE_STATUS_hard_err;
  wire [0:0]Aurora7_CORE_STATUS_lane_up;
  wire Aurora7_CORE_STATUS_rx_resetdone_out;
  wire Aurora7_CORE_STATUS_soft_err;
  wire Aurora7_CORE_STATUS_tx_lock;
  wire Aurora7_CORE_STATUS_tx_resetdone_out;
  wire [0:0]Aurora7_RX_rxn;
  wire [0:0]Aurora7_RX_rxp;
  wire [0:0]Aurora7_TX_txn;
  wire [0:0]Aurora7_TX_txp;
  wire [15:0]Aurora7_USER_DATA_M_AXI_RX_tdata;
  wire [1:0]Aurora7_USER_DATA_M_AXI_RX_tkeep;
  wire Aurora7_USER_DATA_M_AXI_RX_tlast;
  wire Aurora7_USER_DATA_M_AXI_RX_tready;
  wire Aurora7_USER_DATA_M_AXI_RX_tvalid;
  wire [0:15]Aurora7_USER_DATA_S_AXI_TX_tdata;
  wire [0:1]Aurora7_USER_DATA_S_AXI_TX_tkeep;
  wire Aurora7_USER_DATA_S_AXI_TX_tlast;
  wire Aurora7_USER_DATA_S_AXI_TX_tready;
  wire Aurora7_USER_DATA_S_AXI_TX_tvalid;
  wire AuroraArefclk_clk_n;
  wire AuroraArefclk_clk_p;
  wire AuroraBrefclk_clk_n;
  wire AuroraBrefclk_clk_p;
  wire AuroraReset;
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
  wire [8:0]DRP0_daddr;
  wire DRP0_den;
  wire [15:0]DRP0_di;
  wire [15:0]DRP0_do;
  wire DRP0_drdy;
  wire DRP0_dwe;
  wire FIXED_IO_ddr_vrn;
  wire FIXED_IO_ddr_vrp;
  wire [53:0]FIXED_IO_mio;
  wire FIXED_IO_ps_clk;
  wire FIXED_IO_ps_porb;
  wire FIXED_IO_ps_srstb;
  wire GTXreset;
  wire [31:0]Log_AXIS_tdata;
  wire Log_AXIS_tlast;
  wire Log_AXIS_tready;
  wire Log_AXIS_tvalid;
  wire [31:0]Marker_AXIS_tdata;
  wire Marker_AXIS_tlast;
  wire Marker_AXIS_tready;
  wire Marker_AXIS_tvalid;
  wire [2:0]SFP_IIC_SELECT;
  wire auroraUserClk;
  wire [0:0]gpio_aurora_a_tri_i_0;
  wire [1:1]gpio_aurora_a_tri_i_1;
  wire [10:10]gpio_aurora_a_tri_i_10;
  wire [11:11]gpio_aurora_a_tri_i_11;
  wire [12:12]gpio_aurora_a_tri_i_12;
  wire [13:13]gpio_aurora_a_tri_i_13;
  wire [14:14]gpio_aurora_a_tri_i_14;
  wire [15:15]gpio_aurora_a_tri_i_15;
  wire [16:16]gpio_aurora_a_tri_i_16;
  wire [17:17]gpio_aurora_a_tri_i_17;
  wire [18:18]gpio_aurora_a_tri_i_18;
  wire [19:19]gpio_aurora_a_tri_i_19;
  wire [2:2]gpio_aurora_a_tri_i_2;
  wire [20:20]gpio_aurora_a_tri_i_20;
  wire [21:21]gpio_aurora_a_tri_i_21;
  wire [22:22]gpio_aurora_a_tri_i_22;
  wire [23:23]gpio_aurora_a_tri_i_23;
  wire [24:24]gpio_aurora_a_tri_i_24;
  wire [25:25]gpio_aurora_a_tri_i_25;
  wire [26:26]gpio_aurora_a_tri_i_26;
  wire [27:27]gpio_aurora_a_tri_i_27;
  wire [28:28]gpio_aurora_a_tri_i_28;
  wire [29:29]gpio_aurora_a_tri_i_29;
  wire [3:3]gpio_aurora_a_tri_i_3;
  wire [30:30]gpio_aurora_a_tri_i_30;
  wire [31:31]gpio_aurora_a_tri_i_31;
  wire [4:4]gpio_aurora_a_tri_i_4;
  wire [5:5]gpio_aurora_a_tri_i_5;
  wire [6:6]gpio_aurora_a_tri_i_6;
  wire [7:7]gpio_aurora_a_tri_i_7;
  wire [8:8]gpio_aurora_a_tri_i_8;
  wire [9:9]gpio_aurora_a_tri_i_9;
  wire [0:0]gpio_aurora_a_tri_io_0;
  wire [1:1]gpio_aurora_a_tri_io_1;
  wire [10:10]gpio_aurora_a_tri_io_10;
  wire [11:11]gpio_aurora_a_tri_io_11;
  wire [12:12]gpio_aurora_a_tri_io_12;
  wire [13:13]gpio_aurora_a_tri_io_13;
  wire [14:14]gpio_aurora_a_tri_io_14;
  wire [15:15]gpio_aurora_a_tri_io_15;
  wire [16:16]gpio_aurora_a_tri_io_16;
  wire [17:17]gpio_aurora_a_tri_io_17;
  wire [18:18]gpio_aurora_a_tri_io_18;
  wire [19:19]gpio_aurora_a_tri_io_19;
  wire [2:2]gpio_aurora_a_tri_io_2;
  wire [20:20]gpio_aurora_a_tri_io_20;
  wire [21:21]gpio_aurora_a_tri_io_21;
  wire [22:22]gpio_aurora_a_tri_io_22;
  wire [23:23]gpio_aurora_a_tri_io_23;
  wire [24:24]gpio_aurora_a_tri_io_24;
  wire [25:25]gpio_aurora_a_tri_io_25;
  wire [26:26]gpio_aurora_a_tri_io_26;
  wire [27:27]gpio_aurora_a_tri_io_27;
  wire [28:28]gpio_aurora_a_tri_io_28;
  wire [29:29]gpio_aurora_a_tri_io_29;
  wire [3:3]gpio_aurora_a_tri_io_3;
  wire [30:30]gpio_aurora_a_tri_io_30;
  wire [31:31]gpio_aurora_a_tri_io_31;
  wire [4:4]gpio_aurora_a_tri_io_4;
  wire [5:5]gpio_aurora_a_tri_io_5;
  wire [6:6]gpio_aurora_a_tri_io_6;
  wire [7:7]gpio_aurora_a_tri_io_7;
  wire [8:8]gpio_aurora_a_tri_io_8;
  wire [9:9]gpio_aurora_a_tri_io_9;
  wire [0:0]gpio_aurora_a_tri_o_0;
  wire [1:1]gpio_aurora_a_tri_o_1;
  wire [10:10]gpio_aurora_a_tri_o_10;
  wire [11:11]gpio_aurora_a_tri_o_11;
  wire [12:12]gpio_aurora_a_tri_o_12;
  wire [13:13]gpio_aurora_a_tri_o_13;
  wire [14:14]gpio_aurora_a_tri_o_14;
  wire [15:15]gpio_aurora_a_tri_o_15;
  wire [16:16]gpio_aurora_a_tri_o_16;
  wire [17:17]gpio_aurora_a_tri_o_17;
  wire [18:18]gpio_aurora_a_tri_o_18;
  wire [19:19]gpio_aurora_a_tri_o_19;
  wire [2:2]gpio_aurora_a_tri_o_2;
  wire [20:20]gpio_aurora_a_tri_o_20;
  wire [21:21]gpio_aurora_a_tri_o_21;
  wire [22:22]gpio_aurora_a_tri_o_22;
  wire [23:23]gpio_aurora_a_tri_o_23;
  wire [24:24]gpio_aurora_a_tri_o_24;
  wire [25:25]gpio_aurora_a_tri_o_25;
  wire [26:26]gpio_aurora_a_tri_o_26;
  wire [27:27]gpio_aurora_a_tri_o_27;
  wire [28:28]gpio_aurora_a_tri_o_28;
  wire [29:29]gpio_aurora_a_tri_o_29;
  wire [3:3]gpio_aurora_a_tri_o_3;
  wire [30:30]gpio_aurora_a_tri_o_30;
  wire [31:31]gpio_aurora_a_tri_o_31;
  wire [4:4]gpio_aurora_a_tri_o_4;
  wire [5:5]gpio_aurora_a_tri_o_5;
  wire [6:6]gpio_aurora_a_tri_o_6;
  wire [7:7]gpio_aurora_a_tri_o_7;
  wire [8:8]gpio_aurora_a_tri_o_8;
  wire [9:9]gpio_aurora_a_tri_o_9;
  wire [0:0]gpio_aurora_a_tri_t_0;
  wire [1:1]gpio_aurora_a_tri_t_1;
  wire [10:10]gpio_aurora_a_tri_t_10;
  wire [11:11]gpio_aurora_a_tri_t_11;
  wire [12:12]gpio_aurora_a_tri_t_12;
  wire [13:13]gpio_aurora_a_tri_t_13;
  wire [14:14]gpio_aurora_a_tri_t_14;
  wire [15:15]gpio_aurora_a_tri_t_15;
  wire [16:16]gpio_aurora_a_tri_t_16;
  wire [17:17]gpio_aurora_a_tri_t_17;
  wire [18:18]gpio_aurora_a_tri_t_18;
  wire [19:19]gpio_aurora_a_tri_t_19;
  wire [2:2]gpio_aurora_a_tri_t_2;
  wire [20:20]gpio_aurora_a_tri_t_20;
  wire [21:21]gpio_aurora_a_tri_t_21;
  wire [22:22]gpio_aurora_a_tri_t_22;
  wire [23:23]gpio_aurora_a_tri_t_23;
  wire [24:24]gpio_aurora_a_tri_t_24;
  wire [25:25]gpio_aurora_a_tri_t_25;
  wire [26:26]gpio_aurora_a_tri_t_26;
  wire [27:27]gpio_aurora_a_tri_t_27;
  wire [28:28]gpio_aurora_a_tri_t_28;
  wire [29:29]gpio_aurora_a_tri_t_29;
  wire [3:3]gpio_aurora_a_tri_t_3;
  wire [30:30]gpio_aurora_a_tri_t_30;
  wire [31:31]gpio_aurora_a_tri_t_31;
  wire [4:4]gpio_aurora_a_tri_t_4;
  wire [5:5]gpio_aurora_a_tri_t_5;
  wire [6:6]gpio_aurora_a_tri_t_6;
  wire [7:7]gpio_aurora_a_tri_t_7;
  wire [8:8]gpio_aurora_a_tri_t_8;
  wire [9:9]gpio_aurora_a_tri_t_9;
  wire [31:0]gpio_aurora_b_tri_i;
  wire [0:0]gpio_evr_gtx_tri_i_0;
  wire [1:1]gpio_evr_gtx_tri_i_1;
  wire [2:2]gpio_evr_gtx_tri_i_2;
  wire [3:3]gpio_evr_gtx_tri_i_3;
  wire [0:0]gpio_evr_gtx_tri_io_0;
  wire [1:1]gpio_evr_gtx_tri_io_1;
  wire [2:2]gpio_evr_gtx_tri_io_2;
  wire [3:3]gpio_evr_gtx_tri_io_3;
  wire [0:0]gpio_evr_gtx_tri_o_0;
  wire [1:1]gpio_evr_gtx_tri_o_1;
  wire [2:2]gpio_evr_gtx_tri_o_2;
  wire [3:3]gpio_evr_gtx_tri_o_3;
  wire [0:0]gpio_evr_gtx_tri_t_0;
  wire [1:1]gpio_evr_gtx_tri_t_1;
  wire [2:2]gpio_evr_gtx_tri_t_2;
  wire [3:3]gpio_evr_gtx_tri_t_3;
  wire [31:0]gpio_fan_speed_tri_i;
  wire [31:0]gpio_firmware_date_tri_i;
  wire [31:0]gpio_mitigation_control_tri_o;
  wire [31:0]gpio_sfp_status_tri_i;
  wire [31:0]gpio_stale_nodes_hi_tri_i;
  wire [31:0]gpio_stale_nodes_lo_tri_i;
  wire [0:0]gpio_sysmon_readout_tri_i_0;
  wire [1:1]gpio_sysmon_readout_tri_i_1;
  wire [10:10]gpio_sysmon_readout_tri_i_10;
  wire [11:11]gpio_sysmon_readout_tri_i_11;
  wire [12:12]gpio_sysmon_readout_tri_i_12;
  wire [13:13]gpio_sysmon_readout_tri_i_13;
  wire [14:14]gpio_sysmon_readout_tri_i_14;
  wire [15:15]gpio_sysmon_readout_tri_i_15;
  wire [2:2]gpio_sysmon_readout_tri_i_2;
  wire [3:3]gpio_sysmon_readout_tri_i_3;
  wire [4:4]gpio_sysmon_readout_tri_i_4;
  wire [5:5]gpio_sysmon_readout_tri_i_5;
  wire [6:6]gpio_sysmon_readout_tri_i_6;
  wire [7:7]gpio_sysmon_readout_tri_i_7;
  wire [8:8]gpio_sysmon_readout_tri_i_8;
  wire [9:9]gpio_sysmon_readout_tri_i_9;
  wire [0:0]gpio_sysmon_readout_tri_io_0;
  wire [1:1]gpio_sysmon_readout_tri_io_1;
  wire [10:10]gpio_sysmon_readout_tri_io_10;
  wire [11:11]gpio_sysmon_readout_tri_io_11;
  wire [12:12]gpio_sysmon_readout_tri_io_12;
  wire [13:13]gpio_sysmon_readout_tri_io_13;
  wire [14:14]gpio_sysmon_readout_tri_io_14;
  wire [15:15]gpio_sysmon_readout_tri_io_15;
  wire [2:2]gpio_sysmon_readout_tri_io_2;
  wire [3:3]gpio_sysmon_readout_tri_io_3;
  wire [4:4]gpio_sysmon_readout_tri_io_4;
  wire [5:5]gpio_sysmon_readout_tri_io_5;
  wire [6:6]gpio_sysmon_readout_tri_io_6;
  wire [7:7]gpio_sysmon_readout_tri_io_7;
  wire [8:8]gpio_sysmon_readout_tri_io_8;
  wire [9:9]gpio_sysmon_readout_tri_io_9;
  wire [0:0]gpio_sysmon_readout_tri_o_0;
  wire [1:1]gpio_sysmon_readout_tri_o_1;
  wire [10:10]gpio_sysmon_readout_tri_o_10;
  wire [11:11]gpio_sysmon_readout_tri_o_11;
  wire [12:12]gpio_sysmon_readout_tri_o_12;
  wire [13:13]gpio_sysmon_readout_tri_o_13;
  wire [14:14]gpio_sysmon_readout_tri_o_14;
  wire [15:15]gpio_sysmon_readout_tri_o_15;
  wire [2:2]gpio_sysmon_readout_tri_o_2;
  wire [3:3]gpio_sysmon_readout_tri_o_3;
  wire [4:4]gpio_sysmon_readout_tri_o_4;
  wire [5:5]gpio_sysmon_readout_tri_o_5;
  wire [6:6]gpio_sysmon_readout_tri_o_6;
  wire [7:7]gpio_sysmon_readout_tri_o_7;
  wire [8:8]gpio_sysmon_readout_tri_o_8;
  wire [9:9]gpio_sysmon_readout_tri_o_9;
  wire [0:0]gpio_sysmon_readout_tri_t_0;
  wire [1:1]gpio_sysmon_readout_tri_t_1;
  wire [10:10]gpio_sysmon_readout_tri_t_10;
  wire [11:11]gpio_sysmon_readout_tri_t_11;
  wire [12:12]gpio_sysmon_readout_tri_t_12;
  wire [13:13]gpio_sysmon_readout_tri_t_13;
  wire [14:14]gpio_sysmon_readout_tri_t_14;
  wire [15:15]gpio_sysmon_readout_tri_t_15;
  wire [2:2]gpio_sysmon_readout_tri_t_2;
  wire [3:3]gpio_sysmon_readout_tri_t_3;
  wire [4:4]gpio_sysmon_readout_tri_t_4;
  wire [5:5]gpio_sysmon_readout_tri_t_5;
  wire [6:6]gpio_sysmon_readout_tri_t_6;
  wire [7:7]gpio_sysmon_readout_tri_t_7;
  wire [8:8]gpio_sysmon_readout_tri_t_8;
  wire [9:9]gpio_sysmon_readout_tri_t_9;
  wire [31:0]gpio_timestamp_seconds_tri_i;
  wire [31:0]gpio_timestamp_ticks_tri_i;
  wire [15:0]mitigationBRAMctrl_addr;
  wire mitigationBRAMctrl_clk;
  wire [31:0]mitigationBRAMctrl_din;
  wire [31:0]mitigationBRAMctrl_dout;
  wire mitigationBRAMctrl_en;
  wire mitigationBRAMctrl_rst;
  wire [3:0]mitigationBRAMctrl_we;
  wire mitigationClk;
  wire sfp_iic_scl_i;
  wire sfp_iic_scl_io;
  wire sfp_iic_scl_o;
  wire sfp_iic_scl_t;
  wire sfp_iic_sda_i;
  wire sfp_iic_sda_io;
  wire sfp_iic_sda_o;
  wire sfp_iic_sda_t;
  wire sysClk;
  wire [0:0]sysReset_n;

  base_zynq_design base_zynq_design_i
       (.Aurora0_CORE_STATUS_channel_up(Aurora0_CORE_STATUS_channel_up),
        .Aurora0_CORE_STATUS_frame_err(Aurora0_CORE_STATUS_frame_err),
        .Aurora0_CORE_STATUS_hard_err(Aurora0_CORE_STATUS_hard_err),
        .Aurora0_CORE_STATUS_lane_up(Aurora0_CORE_STATUS_lane_up),
        .Aurora0_CORE_STATUS_rx_resetdone_out(Aurora0_CORE_STATUS_rx_resetdone_out),
        .Aurora0_CORE_STATUS_soft_err(Aurora0_CORE_STATUS_soft_err),
        .Aurora0_CORE_STATUS_tx_lock(Aurora0_CORE_STATUS_tx_lock),
        .Aurora0_CORE_STATUS_tx_resetdone_out(Aurora0_CORE_STATUS_tx_resetdone_out),
        .Aurora0_RX_rxn(Aurora0_RX_rxn),
        .Aurora0_RX_rxp(Aurora0_RX_rxp),
        .Aurora0_TX_txn(Aurora0_TX_txn),
        .Aurora0_TX_txp(Aurora0_TX_txp),
        .Aurora0_USER_DATA_M_AXI_RX_tdata(Aurora0_USER_DATA_M_AXI_RX_tdata),
        .Aurora0_USER_DATA_M_AXI_RX_tkeep(Aurora0_USER_DATA_M_AXI_RX_tkeep),
        .Aurora0_USER_DATA_M_AXI_RX_tlast(Aurora0_USER_DATA_M_AXI_RX_tlast),
        .Aurora0_USER_DATA_M_AXI_RX_tvalid(Aurora0_USER_DATA_M_AXI_RX_tvalid),
        .Aurora0_USER_DATA_S_AXI_TX_tdata(Aurora0_USER_DATA_S_AXI_TX_tdata),
        .Aurora0_USER_DATA_S_AXI_TX_tkeep(Aurora0_USER_DATA_S_AXI_TX_tkeep),
        .Aurora0_USER_DATA_S_AXI_TX_tlast(Aurora0_USER_DATA_S_AXI_TX_tlast),
        .Aurora0_USER_DATA_S_AXI_TX_tready(Aurora0_USER_DATA_S_AXI_TX_tready),
        .Aurora0_USER_DATA_S_AXI_TX_tvalid(Aurora0_USER_DATA_S_AXI_TX_tvalid),
        .Aurora1_CORE_STATUS_channel_up(Aurora1_CORE_STATUS_channel_up),
        .Aurora1_CORE_STATUS_frame_err(Aurora1_CORE_STATUS_frame_err),
        .Aurora1_CORE_STATUS_hard_err(Aurora1_CORE_STATUS_hard_err),
        .Aurora1_CORE_STATUS_lane_up(Aurora1_CORE_STATUS_lane_up),
        .Aurora1_CORE_STATUS_rx_resetdone_out(Aurora1_CORE_STATUS_rx_resetdone_out),
        .Aurora1_CORE_STATUS_soft_err(Aurora1_CORE_STATUS_soft_err),
        .Aurora1_CORE_STATUS_tx_lock(Aurora1_CORE_STATUS_tx_lock),
        .Aurora1_CORE_STATUS_tx_resetdone_out(Aurora1_CORE_STATUS_tx_resetdone_out),
        .Aurora1_RX_rxn(Aurora1_RX_rxn),
        .Aurora1_RX_rxp(Aurora1_RX_rxp),
        .Aurora1_TX_txn(Aurora1_TX_txn),
        .Aurora1_TX_txp(Aurora1_TX_txp),
        .Aurora1_USER_DATA_M_AXI_RX_tdata(Aurora1_USER_DATA_M_AXI_RX_tdata),
        .Aurora1_USER_DATA_M_AXI_RX_tkeep(Aurora1_USER_DATA_M_AXI_RX_tkeep),
        .Aurora1_USER_DATA_M_AXI_RX_tlast(Aurora1_USER_DATA_M_AXI_RX_tlast),
        .Aurora1_USER_DATA_M_AXI_RX_tvalid(Aurora1_USER_DATA_M_AXI_RX_tvalid),
        .Aurora1_USER_DATA_S_AXI_TX_tdata(Aurora1_USER_DATA_S_AXI_TX_tdata),
        .Aurora1_USER_DATA_S_AXI_TX_tkeep(Aurora1_USER_DATA_S_AXI_TX_tkeep),
        .Aurora1_USER_DATA_S_AXI_TX_tlast(Aurora1_USER_DATA_S_AXI_TX_tlast),
        .Aurora1_USER_DATA_S_AXI_TX_tready(Aurora1_USER_DATA_S_AXI_TX_tready),
        .Aurora1_USER_DATA_S_AXI_TX_tvalid(Aurora1_USER_DATA_S_AXI_TX_tvalid),
        .Aurora2_CORE_STATUS_channel_up(Aurora2_CORE_STATUS_channel_up),
        .Aurora2_CORE_STATUS_frame_err(Aurora2_CORE_STATUS_frame_err),
        .Aurora2_CORE_STATUS_hard_err(Aurora2_CORE_STATUS_hard_err),
        .Aurora2_CORE_STATUS_lane_up(Aurora2_CORE_STATUS_lane_up),
        .Aurora2_CORE_STATUS_rx_resetdone_out(Aurora2_CORE_STATUS_rx_resetdone_out),
        .Aurora2_CORE_STATUS_soft_err(Aurora2_CORE_STATUS_soft_err),
        .Aurora2_CORE_STATUS_tx_lock(Aurora2_CORE_STATUS_tx_lock),
        .Aurora2_CORE_STATUS_tx_resetdone_out(Aurora2_CORE_STATUS_tx_resetdone_out),
        .Aurora2_RX_rxn(Aurora2_RX_rxn),
        .Aurora2_RX_rxp(Aurora2_RX_rxp),
        .Aurora2_TX_txn(Aurora2_TX_txn),
        .Aurora2_TX_txp(Aurora2_TX_txp),
        .Aurora2_USER_DATA_M_AXI_RX_tdata(Aurora2_USER_DATA_M_AXI_RX_tdata),
        .Aurora2_USER_DATA_M_AXI_RX_tkeep(Aurora2_USER_DATA_M_AXI_RX_tkeep),
        .Aurora2_USER_DATA_M_AXI_RX_tlast(Aurora2_USER_DATA_M_AXI_RX_tlast),
        .Aurora2_USER_DATA_M_AXI_RX_tvalid(Aurora2_USER_DATA_M_AXI_RX_tvalid),
        .Aurora2_USER_DATA_S_AXI_TX_tdata(Aurora2_USER_DATA_S_AXI_TX_tdata),
        .Aurora2_USER_DATA_S_AXI_TX_tkeep(Aurora2_USER_DATA_S_AXI_TX_tkeep),
        .Aurora2_USER_DATA_S_AXI_TX_tlast(Aurora2_USER_DATA_S_AXI_TX_tlast),
        .Aurora2_USER_DATA_S_AXI_TX_tready(Aurora2_USER_DATA_S_AXI_TX_tready),
        .Aurora2_USER_DATA_S_AXI_TX_tvalid(Aurora2_USER_DATA_S_AXI_TX_tvalid),
        .Aurora3_CORE_STATUS_channel_up(Aurora3_CORE_STATUS_channel_up),
        .Aurora3_CORE_STATUS_frame_err(Aurora3_CORE_STATUS_frame_err),
        .Aurora3_CORE_STATUS_hard_err(Aurora3_CORE_STATUS_hard_err),
        .Aurora3_CORE_STATUS_lane_up(Aurora3_CORE_STATUS_lane_up),
        .Aurora3_CORE_STATUS_rx_resetdone_out(Aurora3_CORE_STATUS_rx_resetdone_out),
        .Aurora3_CORE_STATUS_soft_err(Aurora3_CORE_STATUS_soft_err),
        .Aurora3_CORE_STATUS_tx_lock(Aurora3_CORE_STATUS_tx_lock),
        .Aurora3_CORE_STATUS_tx_resetdone_out(Aurora3_CORE_STATUS_tx_resetdone_out),
        .Aurora3_RX_rxn(Aurora3_RX_rxn),
        .Aurora3_RX_rxp(Aurora3_RX_rxp),
        .Aurora3_TX_txn(Aurora3_TX_txn),
        .Aurora3_TX_txp(Aurora3_TX_txp),
        .Aurora3_USER_DATA_M_AXI_RX_tdata(Aurora3_USER_DATA_M_AXI_RX_tdata),
        .Aurora3_USER_DATA_M_AXI_RX_tkeep(Aurora3_USER_DATA_M_AXI_RX_tkeep),
        .Aurora3_USER_DATA_M_AXI_RX_tlast(Aurora3_USER_DATA_M_AXI_RX_tlast),
        .Aurora3_USER_DATA_M_AXI_RX_tvalid(Aurora3_USER_DATA_M_AXI_RX_tvalid),
        .Aurora3_USER_DATA_S_AXI_TX_tdata(Aurora3_USER_DATA_S_AXI_TX_tdata),
        .Aurora3_USER_DATA_S_AXI_TX_tkeep(Aurora3_USER_DATA_S_AXI_TX_tkeep),
        .Aurora3_USER_DATA_S_AXI_TX_tlast(Aurora3_USER_DATA_S_AXI_TX_tlast),
        .Aurora3_USER_DATA_S_AXI_TX_tready(Aurora3_USER_DATA_S_AXI_TX_tready),
        .Aurora3_USER_DATA_S_AXI_TX_tvalid(Aurora3_USER_DATA_S_AXI_TX_tvalid),
        .Aurora4_CORE_STATUS_channel_up(Aurora4_CORE_STATUS_channel_up),
        .Aurora4_CORE_STATUS_frame_err(Aurora4_CORE_STATUS_frame_err),
        .Aurora4_CORE_STATUS_hard_err(Aurora4_CORE_STATUS_hard_err),
        .Aurora4_CORE_STATUS_lane_up(Aurora4_CORE_STATUS_lane_up),
        .Aurora4_CORE_STATUS_rx_resetdone_out(Aurora4_CORE_STATUS_rx_resetdone_out),
        .Aurora4_CORE_STATUS_soft_err(Aurora4_CORE_STATUS_soft_err),
        .Aurora4_CORE_STATUS_tx_lock(Aurora4_CORE_STATUS_tx_lock),
        .Aurora4_CORE_STATUS_tx_resetdone_out(Aurora4_CORE_STATUS_tx_resetdone_out),
        .Aurora4_RX_rxn(Aurora4_RX_rxn),
        .Aurora4_RX_rxp(Aurora4_RX_rxp),
        .Aurora4_TX_txn(Aurora4_TX_txn),
        .Aurora4_TX_txp(Aurora4_TX_txp),
        .Aurora4_USER_DATA_M_AXI_RX_tdata(Aurora4_USER_DATA_M_AXI_RX_tdata),
        .Aurora4_USER_DATA_M_AXI_RX_tkeep(Aurora4_USER_DATA_M_AXI_RX_tkeep),
        .Aurora4_USER_DATA_M_AXI_RX_tlast(Aurora4_USER_DATA_M_AXI_RX_tlast),
        .Aurora4_USER_DATA_M_AXI_RX_tready(Aurora4_USER_DATA_M_AXI_RX_tready),
        .Aurora4_USER_DATA_M_AXI_RX_tvalid(Aurora4_USER_DATA_M_AXI_RX_tvalid),
        .Aurora4_USER_DATA_S_AXI_TX_tdata(Aurora4_USER_DATA_S_AXI_TX_tdata),
        .Aurora4_USER_DATA_S_AXI_TX_tkeep(Aurora4_USER_DATA_S_AXI_TX_tkeep),
        .Aurora4_USER_DATA_S_AXI_TX_tlast(Aurora4_USER_DATA_S_AXI_TX_tlast),
        .Aurora4_USER_DATA_S_AXI_TX_tready(Aurora4_USER_DATA_S_AXI_TX_tready),
        .Aurora4_USER_DATA_S_AXI_TX_tvalid(Aurora4_USER_DATA_S_AXI_TX_tvalid),
        .Aurora5_CORE_STATUS_channel_up(Aurora5_CORE_STATUS_channel_up),
        .Aurora5_CORE_STATUS_frame_err(Aurora5_CORE_STATUS_frame_err),
        .Aurora5_CORE_STATUS_hard_err(Aurora5_CORE_STATUS_hard_err),
        .Aurora5_CORE_STATUS_lane_up(Aurora5_CORE_STATUS_lane_up),
        .Aurora5_CORE_STATUS_rx_resetdone_out(Aurora5_CORE_STATUS_rx_resetdone_out),
        .Aurora5_CORE_STATUS_soft_err(Aurora5_CORE_STATUS_soft_err),
        .Aurora5_CORE_STATUS_tx_lock(Aurora5_CORE_STATUS_tx_lock),
        .Aurora5_CORE_STATUS_tx_resetdone_out(Aurora5_CORE_STATUS_tx_resetdone_out),
        .Aurora5_RX_rxn(Aurora5_RX_rxn),
        .Aurora5_RX_rxp(Aurora5_RX_rxp),
        .Aurora5_TX_txn(Aurora5_TX_txn),
        .Aurora5_TX_txp(Aurora5_TX_txp),
        .Aurora5_USER_DATA_M_AXI_RX_tdata(Aurora5_USER_DATA_M_AXI_RX_tdata),
        .Aurora5_USER_DATA_M_AXI_RX_tkeep(Aurora5_USER_DATA_M_AXI_RX_tkeep),
        .Aurora5_USER_DATA_M_AXI_RX_tlast(Aurora5_USER_DATA_M_AXI_RX_tlast),
        .Aurora5_USER_DATA_M_AXI_RX_tready(Aurora5_USER_DATA_M_AXI_RX_tready),
        .Aurora5_USER_DATA_M_AXI_RX_tvalid(Aurora5_USER_DATA_M_AXI_RX_tvalid),
        .Aurora5_USER_DATA_S_AXI_TX_tdata(Aurora5_USER_DATA_S_AXI_TX_tdata),
        .Aurora5_USER_DATA_S_AXI_TX_tkeep(Aurora5_USER_DATA_S_AXI_TX_tkeep),
        .Aurora5_USER_DATA_S_AXI_TX_tlast(Aurora5_USER_DATA_S_AXI_TX_tlast),
        .Aurora5_USER_DATA_S_AXI_TX_tready(Aurora5_USER_DATA_S_AXI_TX_tready),
        .Aurora5_USER_DATA_S_AXI_TX_tvalid(Aurora5_USER_DATA_S_AXI_TX_tvalid),
        .Aurora6_CORE_STATUS_channel_up(Aurora6_CORE_STATUS_channel_up),
        .Aurora6_CORE_STATUS_frame_err(Aurora6_CORE_STATUS_frame_err),
        .Aurora6_CORE_STATUS_hard_err(Aurora6_CORE_STATUS_hard_err),
        .Aurora6_CORE_STATUS_lane_up(Aurora6_CORE_STATUS_lane_up),
        .Aurora6_CORE_STATUS_rx_resetdone_out(Aurora6_CORE_STATUS_rx_resetdone_out),
        .Aurora6_CORE_STATUS_soft_err(Aurora6_CORE_STATUS_soft_err),
        .Aurora6_CORE_STATUS_tx_lock(Aurora6_CORE_STATUS_tx_lock),
        .Aurora6_CORE_STATUS_tx_resetdone_out(Aurora6_CORE_STATUS_tx_resetdone_out),
        .Aurora6_RX_rxn(Aurora6_RX_rxn),
        .Aurora6_RX_rxp(Aurora6_RX_rxp),
        .Aurora6_TX_txn(Aurora6_TX_txn),
        .Aurora6_TX_txp(Aurora6_TX_txp),
        .Aurora6_USER_DATA_M_AXI_RX_tdata(Aurora6_USER_DATA_M_AXI_RX_tdata),
        .Aurora6_USER_DATA_M_AXI_RX_tkeep(Aurora6_USER_DATA_M_AXI_RX_tkeep),
        .Aurora6_USER_DATA_M_AXI_RX_tlast(Aurora6_USER_DATA_M_AXI_RX_tlast),
        .Aurora6_USER_DATA_M_AXI_RX_tready(Aurora6_USER_DATA_M_AXI_RX_tready),
        .Aurora6_USER_DATA_M_AXI_RX_tvalid(Aurora6_USER_DATA_M_AXI_RX_tvalid),
        .Aurora6_USER_DATA_S_AXI_TX_tdata(Aurora6_USER_DATA_S_AXI_TX_tdata),
        .Aurora6_USER_DATA_S_AXI_TX_tkeep(Aurora6_USER_DATA_S_AXI_TX_tkeep),
        .Aurora6_USER_DATA_S_AXI_TX_tlast(Aurora6_USER_DATA_S_AXI_TX_tlast),
        .Aurora6_USER_DATA_S_AXI_TX_tready(Aurora6_USER_DATA_S_AXI_TX_tready),
        .Aurora6_USER_DATA_S_AXI_TX_tvalid(Aurora6_USER_DATA_S_AXI_TX_tvalid),
        .Aurora7_CORE_STATUS_channel_up(Aurora7_CORE_STATUS_channel_up),
        .Aurora7_CORE_STATUS_frame_err(Aurora7_CORE_STATUS_frame_err),
        .Aurora7_CORE_STATUS_hard_err(Aurora7_CORE_STATUS_hard_err),
        .Aurora7_CORE_STATUS_lane_up(Aurora7_CORE_STATUS_lane_up),
        .Aurora7_CORE_STATUS_rx_resetdone_out(Aurora7_CORE_STATUS_rx_resetdone_out),
        .Aurora7_CORE_STATUS_soft_err(Aurora7_CORE_STATUS_soft_err),
        .Aurora7_CORE_STATUS_tx_lock(Aurora7_CORE_STATUS_tx_lock),
        .Aurora7_CORE_STATUS_tx_resetdone_out(Aurora7_CORE_STATUS_tx_resetdone_out),
        .Aurora7_RX_rxn(Aurora7_RX_rxn),
        .Aurora7_RX_rxp(Aurora7_RX_rxp),
        .Aurora7_TX_txn(Aurora7_TX_txn),
        .Aurora7_TX_txp(Aurora7_TX_txp),
        .Aurora7_USER_DATA_M_AXI_RX_tdata(Aurora7_USER_DATA_M_AXI_RX_tdata),
        .Aurora7_USER_DATA_M_AXI_RX_tkeep(Aurora7_USER_DATA_M_AXI_RX_tkeep),
        .Aurora7_USER_DATA_M_AXI_RX_tlast(Aurora7_USER_DATA_M_AXI_RX_tlast),
        .Aurora7_USER_DATA_M_AXI_RX_tready(Aurora7_USER_DATA_M_AXI_RX_tready),
        .Aurora7_USER_DATA_M_AXI_RX_tvalid(Aurora7_USER_DATA_M_AXI_RX_tvalid),
        .Aurora7_USER_DATA_S_AXI_TX_tdata(Aurora7_USER_DATA_S_AXI_TX_tdata),
        .Aurora7_USER_DATA_S_AXI_TX_tkeep(Aurora7_USER_DATA_S_AXI_TX_tkeep),
        .Aurora7_USER_DATA_S_AXI_TX_tlast(Aurora7_USER_DATA_S_AXI_TX_tlast),
        .Aurora7_USER_DATA_S_AXI_TX_tready(Aurora7_USER_DATA_S_AXI_TX_tready),
        .Aurora7_USER_DATA_S_AXI_TX_tvalid(Aurora7_USER_DATA_S_AXI_TX_tvalid),
        .AuroraArefclk_clk_n(AuroraArefclk_clk_n),
        .AuroraArefclk_clk_p(AuroraArefclk_clk_p),
        .AuroraBrefclk_clk_n(AuroraBrefclk_clk_n),
        .AuroraBrefclk_clk_p(AuroraBrefclk_clk_p),
        .AuroraReset(AuroraReset),
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
        .DRP0_daddr(DRP0_daddr),
        .DRP0_den(DRP0_den),
        .DRP0_di(DRP0_di),
        .DRP0_do(DRP0_do),
        .DRP0_drdy(DRP0_drdy),
        .DRP0_dwe(DRP0_dwe),
        .FIXED_IO_ddr_vrn(FIXED_IO_ddr_vrn),
        .FIXED_IO_ddr_vrp(FIXED_IO_ddr_vrp),
        .FIXED_IO_mio(FIXED_IO_mio),
        .FIXED_IO_ps_clk(FIXED_IO_ps_clk),
        .FIXED_IO_ps_porb(FIXED_IO_ps_porb),
        .FIXED_IO_ps_srstb(FIXED_IO_ps_srstb),
        .GPIO_AURORA_A_tri_i({gpio_aurora_a_tri_i_31,gpio_aurora_a_tri_i_30,gpio_aurora_a_tri_i_29,gpio_aurora_a_tri_i_28,gpio_aurora_a_tri_i_27,gpio_aurora_a_tri_i_26,gpio_aurora_a_tri_i_25,gpio_aurora_a_tri_i_24,gpio_aurora_a_tri_i_23,gpio_aurora_a_tri_i_22,gpio_aurora_a_tri_i_21,gpio_aurora_a_tri_i_20,gpio_aurora_a_tri_i_19,gpio_aurora_a_tri_i_18,gpio_aurora_a_tri_i_17,gpio_aurora_a_tri_i_16,gpio_aurora_a_tri_i_15,gpio_aurora_a_tri_i_14,gpio_aurora_a_tri_i_13,gpio_aurora_a_tri_i_12,gpio_aurora_a_tri_i_11,gpio_aurora_a_tri_i_10,gpio_aurora_a_tri_i_9,gpio_aurora_a_tri_i_8,gpio_aurora_a_tri_i_7,gpio_aurora_a_tri_i_6,gpio_aurora_a_tri_i_5,gpio_aurora_a_tri_i_4,gpio_aurora_a_tri_i_3,gpio_aurora_a_tri_i_2,gpio_aurora_a_tri_i_1,gpio_aurora_a_tri_i_0}),
        .GPIO_AURORA_A_tri_o({gpio_aurora_a_tri_o_31,gpio_aurora_a_tri_o_30,gpio_aurora_a_tri_o_29,gpio_aurora_a_tri_o_28,gpio_aurora_a_tri_o_27,gpio_aurora_a_tri_o_26,gpio_aurora_a_tri_o_25,gpio_aurora_a_tri_o_24,gpio_aurora_a_tri_o_23,gpio_aurora_a_tri_o_22,gpio_aurora_a_tri_o_21,gpio_aurora_a_tri_o_20,gpio_aurora_a_tri_o_19,gpio_aurora_a_tri_o_18,gpio_aurora_a_tri_o_17,gpio_aurora_a_tri_o_16,gpio_aurora_a_tri_o_15,gpio_aurora_a_tri_o_14,gpio_aurora_a_tri_o_13,gpio_aurora_a_tri_o_12,gpio_aurora_a_tri_o_11,gpio_aurora_a_tri_o_10,gpio_aurora_a_tri_o_9,gpio_aurora_a_tri_o_8,gpio_aurora_a_tri_o_7,gpio_aurora_a_tri_o_6,gpio_aurora_a_tri_o_5,gpio_aurora_a_tri_o_4,gpio_aurora_a_tri_o_3,gpio_aurora_a_tri_o_2,gpio_aurora_a_tri_o_1,gpio_aurora_a_tri_o_0}),
        .GPIO_AURORA_A_tri_t({gpio_aurora_a_tri_t_31,gpio_aurora_a_tri_t_30,gpio_aurora_a_tri_t_29,gpio_aurora_a_tri_t_28,gpio_aurora_a_tri_t_27,gpio_aurora_a_tri_t_26,gpio_aurora_a_tri_t_25,gpio_aurora_a_tri_t_24,gpio_aurora_a_tri_t_23,gpio_aurora_a_tri_t_22,gpio_aurora_a_tri_t_21,gpio_aurora_a_tri_t_20,gpio_aurora_a_tri_t_19,gpio_aurora_a_tri_t_18,gpio_aurora_a_tri_t_17,gpio_aurora_a_tri_t_16,gpio_aurora_a_tri_t_15,gpio_aurora_a_tri_t_14,gpio_aurora_a_tri_t_13,gpio_aurora_a_tri_t_12,gpio_aurora_a_tri_t_11,gpio_aurora_a_tri_t_10,gpio_aurora_a_tri_t_9,gpio_aurora_a_tri_t_8,gpio_aurora_a_tri_t_7,gpio_aurora_a_tri_t_6,gpio_aurora_a_tri_t_5,gpio_aurora_a_tri_t_4,gpio_aurora_a_tri_t_3,gpio_aurora_a_tri_t_2,gpio_aurora_a_tri_t_1,gpio_aurora_a_tri_t_0}),
        .GPIO_AURORA_B_tri_i(gpio_aurora_b_tri_i),
        .GPIO_EVR_GTX_tri_i({gpio_evr_gtx_tri_i_3,gpio_evr_gtx_tri_i_2,gpio_evr_gtx_tri_i_1,gpio_evr_gtx_tri_i_0}),
        .GPIO_EVR_GTX_tri_o({gpio_evr_gtx_tri_o_3,gpio_evr_gtx_tri_o_2,gpio_evr_gtx_tri_o_1,gpio_evr_gtx_tri_o_0}),
        .GPIO_EVR_GTX_tri_t({gpio_evr_gtx_tri_t_3,gpio_evr_gtx_tri_t_2,gpio_evr_gtx_tri_t_1,gpio_evr_gtx_tri_t_0}),
        .GPIO_FAN_SPEED_tri_i(gpio_fan_speed_tri_i),
        .GPIO_FIRMWARE_DATE_tri_i(gpio_firmware_date_tri_i),
        .GPIO_MITIGATION_CONTROL_tri_o(gpio_mitigation_control_tri_o),
        .GPIO_SFP_STATUS_tri_i(gpio_sfp_status_tri_i),
        .GPIO_STALE_NODES_HI_tri_i(gpio_stale_nodes_hi_tri_i),
        .GPIO_STALE_NODES_LO_tri_i(gpio_stale_nodes_lo_tri_i),
        .GPIO_SYSMON_READOUT_tri_i({gpio_sysmon_readout_tri_i_15,gpio_sysmon_readout_tri_i_14,gpio_sysmon_readout_tri_i_13,gpio_sysmon_readout_tri_i_12,gpio_sysmon_readout_tri_i_11,gpio_sysmon_readout_tri_i_10,gpio_sysmon_readout_tri_i_9,gpio_sysmon_readout_tri_i_8,gpio_sysmon_readout_tri_i_7,gpio_sysmon_readout_tri_i_6,gpio_sysmon_readout_tri_i_5,gpio_sysmon_readout_tri_i_4,gpio_sysmon_readout_tri_i_3,gpio_sysmon_readout_tri_i_2,gpio_sysmon_readout_tri_i_1,gpio_sysmon_readout_tri_i_0}),
        .GPIO_SYSMON_READOUT_tri_o({gpio_sysmon_readout_tri_o_15,gpio_sysmon_readout_tri_o_14,gpio_sysmon_readout_tri_o_13,gpio_sysmon_readout_tri_o_12,gpio_sysmon_readout_tri_o_11,gpio_sysmon_readout_tri_o_10,gpio_sysmon_readout_tri_o_9,gpio_sysmon_readout_tri_o_8,gpio_sysmon_readout_tri_o_7,gpio_sysmon_readout_tri_o_6,gpio_sysmon_readout_tri_o_5,gpio_sysmon_readout_tri_o_4,gpio_sysmon_readout_tri_o_3,gpio_sysmon_readout_tri_o_2,gpio_sysmon_readout_tri_o_1,gpio_sysmon_readout_tri_o_0}),
        .GPIO_SYSMON_READOUT_tri_t({gpio_sysmon_readout_tri_t_15,gpio_sysmon_readout_tri_t_14,gpio_sysmon_readout_tri_t_13,gpio_sysmon_readout_tri_t_12,gpio_sysmon_readout_tri_t_11,gpio_sysmon_readout_tri_t_10,gpio_sysmon_readout_tri_t_9,gpio_sysmon_readout_tri_t_8,gpio_sysmon_readout_tri_t_7,gpio_sysmon_readout_tri_t_6,gpio_sysmon_readout_tri_t_5,gpio_sysmon_readout_tri_t_4,gpio_sysmon_readout_tri_t_3,gpio_sysmon_readout_tri_t_2,gpio_sysmon_readout_tri_t_1,gpio_sysmon_readout_tri_t_0}),
        .GPIO_TIMESTAMP_SECONDS_tri_i(gpio_timestamp_seconds_tri_i),
        .GPIO_TIMESTAMP_TICKS_tri_i(gpio_timestamp_ticks_tri_i),
        .GTXreset(GTXreset),
        .Log_AXIS_tdata(Log_AXIS_tdata),
        .Log_AXIS_tlast(Log_AXIS_tlast),
        .Log_AXIS_tready(Log_AXIS_tready),
        .Log_AXIS_tvalid(Log_AXIS_tvalid),
        .Marker_AXIS_tdata(Marker_AXIS_tdata),
        .Marker_AXIS_tlast(Marker_AXIS_tlast),
        .Marker_AXIS_tready(Marker_AXIS_tready),
        .Marker_AXIS_tvalid(Marker_AXIS_tvalid),
        .SFP_IIC_SELECT(SFP_IIC_SELECT),
        .SFP_IIC_scl_i(sfp_iic_scl_i),
        .SFP_IIC_scl_o(sfp_iic_scl_o),
        .SFP_IIC_scl_t(sfp_iic_scl_t),
        .SFP_IIC_sda_i(sfp_iic_sda_i),
        .SFP_IIC_sda_o(sfp_iic_sda_o),
        .SFP_IIC_sda_t(sfp_iic_sda_t),
        .auroraUserClk(auroraUserClk),
        .mitigationBRAMctrl_addr(mitigationBRAMctrl_addr),
        .mitigationBRAMctrl_clk(mitigationBRAMctrl_clk),
        .mitigationBRAMctrl_din(mitigationBRAMctrl_din),
        .mitigationBRAMctrl_dout(mitigationBRAMctrl_dout),
        .mitigationBRAMctrl_en(mitigationBRAMctrl_en),
        .mitigationBRAMctrl_rst(mitigationBRAMctrl_rst),
        .mitigationBRAMctrl_we(mitigationBRAMctrl_we),
        .mitigationClk(mitigationClk),
        .sysClk(sysClk),
        .sysReset_n(sysReset_n));
  IOBUF gpio_aurora_a_tri_iobuf_0
       (.I(gpio_aurora_a_tri_o_0),
        .IO(gpio_aurora_a_tri_io[0]),
        .O(gpio_aurora_a_tri_i_0),
        .T(gpio_aurora_a_tri_t_0));
  IOBUF gpio_aurora_a_tri_iobuf_1
       (.I(gpio_aurora_a_tri_o_1),
        .IO(gpio_aurora_a_tri_io[1]),
        .O(gpio_aurora_a_tri_i_1),
        .T(gpio_aurora_a_tri_t_1));
  IOBUF gpio_aurora_a_tri_iobuf_10
       (.I(gpio_aurora_a_tri_o_10),
        .IO(gpio_aurora_a_tri_io[10]),
        .O(gpio_aurora_a_tri_i_10),
        .T(gpio_aurora_a_tri_t_10));
  IOBUF gpio_aurora_a_tri_iobuf_11
       (.I(gpio_aurora_a_tri_o_11),
        .IO(gpio_aurora_a_tri_io[11]),
        .O(gpio_aurora_a_tri_i_11),
        .T(gpio_aurora_a_tri_t_11));
  IOBUF gpio_aurora_a_tri_iobuf_12
       (.I(gpio_aurora_a_tri_o_12),
        .IO(gpio_aurora_a_tri_io[12]),
        .O(gpio_aurora_a_tri_i_12),
        .T(gpio_aurora_a_tri_t_12));
  IOBUF gpio_aurora_a_tri_iobuf_13
       (.I(gpio_aurora_a_tri_o_13),
        .IO(gpio_aurora_a_tri_io[13]),
        .O(gpio_aurora_a_tri_i_13),
        .T(gpio_aurora_a_tri_t_13));
  IOBUF gpio_aurora_a_tri_iobuf_14
       (.I(gpio_aurora_a_tri_o_14),
        .IO(gpio_aurora_a_tri_io[14]),
        .O(gpio_aurora_a_tri_i_14),
        .T(gpio_aurora_a_tri_t_14));
  IOBUF gpio_aurora_a_tri_iobuf_15
       (.I(gpio_aurora_a_tri_o_15),
        .IO(gpio_aurora_a_tri_io[15]),
        .O(gpio_aurora_a_tri_i_15),
        .T(gpio_aurora_a_tri_t_15));
  IOBUF gpio_aurora_a_tri_iobuf_16
       (.I(gpio_aurora_a_tri_o_16),
        .IO(gpio_aurora_a_tri_io[16]),
        .O(gpio_aurora_a_tri_i_16),
        .T(gpio_aurora_a_tri_t_16));
  IOBUF gpio_aurora_a_tri_iobuf_17
       (.I(gpio_aurora_a_tri_o_17),
        .IO(gpio_aurora_a_tri_io[17]),
        .O(gpio_aurora_a_tri_i_17),
        .T(gpio_aurora_a_tri_t_17));
  IOBUF gpio_aurora_a_tri_iobuf_18
       (.I(gpio_aurora_a_tri_o_18),
        .IO(gpio_aurora_a_tri_io[18]),
        .O(gpio_aurora_a_tri_i_18),
        .T(gpio_aurora_a_tri_t_18));
  IOBUF gpio_aurora_a_tri_iobuf_19
       (.I(gpio_aurora_a_tri_o_19),
        .IO(gpio_aurora_a_tri_io[19]),
        .O(gpio_aurora_a_tri_i_19),
        .T(gpio_aurora_a_tri_t_19));
  IOBUF gpio_aurora_a_tri_iobuf_2
       (.I(gpio_aurora_a_tri_o_2),
        .IO(gpio_aurora_a_tri_io[2]),
        .O(gpio_aurora_a_tri_i_2),
        .T(gpio_aurora_a_tri_t_2));
  IOBUF gpio_aurora_a_tri_iobuf_20
       (.I(gpio_aurora_a_tri_o_20),
        .IO(gpio_aurora_a_tri_io[20]),
        .O(gpio_aurora_a_tri_i_20),
        .T(gpio_aurora_a_tri_t_20));
  IOBUF gpio_aurora_a_tri_iobuf_21
       (.I(gpio_aurora_a_tri_o_21),
        .IO(gpio_aurora_a_tri_io[21]),
        .O(gpio_aurora_a_tri_i_21),
        .T(gpio_aurora_a_tri_t_21));
  IOBUF gpio_aurora_a_tri_iobuf_22
       (.I(gpio_aurora_a_tri_o_22),
        .IO(gpio_aurora_a_tri_io[22]),
        .O(gpio_aurora_a_tri_i_22),
        .T(gpio_aurora_a_tri_t_22));
  IOBUF gpio_aurora_a_tri_iobuf_23
       (.I(gpio_aurora_a_tri_o_23),
        .IO(gpio_aurora_a_tri_io[23]),
        .O(gpio_aurora_a_tri_i_23),
        .T(gpio_aurora_a_tri_t_23));
  IOBUF gpio_aurora_a_tri_iobuf_24
       (.I(gpio_aurora_a_tri_o_24),
        .IO(gpio_aurora_a_tri_io[24]),
        .O(gpio_aurora_a_tri_i_24),
        .T(gpio_aurora_a_tri_t_24));
  IOBUF gpio_aurora_a_tri_iobuf_25
       (.I(gpio_aurora_a_tri_o_25),
        .IO(gpio_aurora_a_tri_io[25]),
        .O(gpio_aurora_a_tri_i_25),
        .T(gpio_aurora_a_tri_t_25));
  IOBUF gpio_aurora_a_tri_iobuf_26
       (.I(gpio_aurora_a_tri_o_26),
        .IO(gpio_aurora_a_tri_io[26]),
        .O(gpio_aurora_a_tri_i_26),
        .T(gpio_aurora_a_tri_t_26));
  IOBUF gpio_aurora_a_tri_iobuf_27
       (.I(gpio_aurora_a_tri_o_27),
        .IO(gpio_aurora_a_tri_io[27]),
        .O(gpio_aurora_a_tri_i_27),
        .T(gpio_aurora_a_tri_t_27));
  IOBUF gpio_aurora_a_tri_iobuf_28
       (.I(gpio_aurora_a_tri_o_28),
        .IO(gpio_aurora_a_tri_io[28]),
        .O(gpio_aurora_a_tri_i_28),
        .T(gpio_aurora_a_tri_t_28));
  IOBUF gpio_aurora_a_tri_iobuf_29
       (.I(gpio_aurora_a_tri_o_29),
        .IO(gpio_aurora_a_tri_io[29]),
        .O(gpio_aurora_a_tri_i_29),
        .T(gpio_aurora_a_tri_t_29));
  IOBUF gpio_aurora_a_tri_iobuf_3
       (.I(gpio_aurora_a_tri_o_3),
        .IO(gpio_aurora_a_tri_io[3]),
        .O(gpio_aurora_a_tri_i_3),
        .T(gpio_aurora_a_tri_t_3));
  IOBUF gpio_aurora_a_tri_iobuf_30
       (.I(gpio_aurora_a_tri_o_30),
        .IO(gpio_aurora_a_tri_io[30]),
        .O(gpio_aurora_a_tri_i_30),
        .T(gpio_aurora_a_tri_t_30));
  IOBUF gpio_aurora_a_tri_iobuf_31
       (.I(gpio_aurora_a_tri_o_31),
        .IO(gpio_aurora_a_tri_io[31]),
        .O(gpio_aurora_a_tri_i_31),
        .T(gpio_aurora_a_tri_t_31));
  IOBUF gpio_aurora_a_tri_iobuf_4
       (.I(gpio_aurora_a_tri_o_4),
        .IO(gpio_aurora_a_tri_io[4]),
        .O(gpio_aurora_a_tri_i_4),
        .T(gpio_aurora_a_tri_t_4));
  IOBUF gpio_aurora_a_tri_iobuf_5
       (.I(gpio_aurora_a_tri_o_5),
        .IO(gpio_aurora_a_tri_io[5]),
        .O(gpio_aurora_a_tri_i_5),
        .T(gpio_aurora_a_tri_t_5));
  IOBUF gpio_aurora_a_tri_iobuf_6
       (.I(gpio_aurora_a_tri_o_6),
        .IO(gpio_aurora_a_tri_io[6]),
        .O(gpio_aurora_a_tri_i_6),
        .T(gpio_aurora_a_tri_t_6));
  IOBUF gpio_aurora_a_tri_iobuf_7
       (.I(gpio_aurora_a_tri_o_7),
        .IO(gpio_aurora_a_tri_io[7]),
        .O(gpio_aurora_a_tri_i_7),
        .T(gpio_aurora_a_tri_t_7));
  IOBUF gpio_aurora_a_tri_iobuf_8
       (.I(gpio_aurora_a_tri_o_8),
        .IO(gpio_aurora_a_tri_io[8]),
        .O(gpio_aurora_a_tri_i_8),
        .T(gpio_aurora_a_tri_t_8));
  IOBUF gpio_aurora_a_tri_iobuf_9
       (.I(gpio_aurora_a_tri_o_9),
        .IO(gpio_aurora_a_tri_io[9]),
        .O(gpio_aurora_a_tri_i_9),
        .T(gpio_aurora_a_tri_t_9));
  IOBUF gpio_evr_gtx_tri_iobuf_0
       (.I(gpio_evr_gtx_tri_o_0),
        .IO(gpio_evr_gtx_tri_io[0]),
        .O(gpio_evr_gtx_tri_i_0),
        .T(gpio_evr_gtx_tri_t_0));
  IOBUF gpio_evr_gtx_tri_iobuf_1
       (.I(gpio_evr_gtx_tri_o_1),
        .IO(gpio_evr_gtx_tri_io[1]),
        .O(gpio_evr_gtx_tri_i_1),
        .T(gpio_evr_gtx_tri_t_1));
  IOBUF gpio_evr_gtx_tri_iobuf_2
       (.I(gpio_evr_gtx_tri_o_2),
        .IO(gpio_evr_gtx_tri_io[2]),
        .O(gpio_evr_gtx_tri_i_2),
        .T(gpio_evr_gtx_tri_t_2));
  IOBUF gpio_evr_gtx_tri_iobuf_3
       (.I(gpio_evr_gtx_tri_o_3),
        .IO(gpio_evr_gtx_tri_io[3]),
        .O(gpio_evr_gtx_tri_i_3),
        .T(gpio_evr_gtx_tri_t_3));
  IOBUF gpio_sysmon_readout_tri_iobuf_0
       (.I(gpio_sysmon_readout_tri_o_0),
        .IO(gpio_sysmon_readout_tri_io[0]),
        .O(gpio_sysmon_readout_tri_i_0),
        .T(gpio_sysmon_readout_tri_t_0));
  IOBUF gpio_sysmon_readout_tri_iobuf_1
       (.I(gpio_sysmon_readout_tri_o_1),
        .IO(gpio_sysmon_readout_tri_io[1]),
        .O(gpio_sysmon_readout_tri_i_1),
        .T(gpio_sysmon_readout_tri_t_1));
  IOBUF gpio_sysmon_readout_tri_iobuf_10
       (.I(gpio_sysmon_readout_tri_o_10),
        .IO(gpio_sysmon_readout_tri_io[10]),
        .O(gpio_sysmon_readout_tri_i_10),
        .T(gpio_sysmon_readout_tri_t_10));
  IOBUF gpio_sysmon_readout_tri_iobuf_11
       (.I(gpio_sysmon_readout_tri_o_11),
        .IO(gpio_sysmon_readout_tri_io[11]),
        .O(gpio_sysmon_readout_tri_i_11),
        .T(gpio_sysmon_readout_tri_t_11));
  IOBUF gpio_sysmon_readout_tri_iobuf_12
       (.I(gpio_sysmon_readout_tri_o_12),
        .IO(gpio_sysmon_readout_tri_io[12]),
        .O(gpio_sysmon_readout_tri_i_12),
        .T(gpio_sysmon_readout_tri_t_12));
  IOBUF gpio_sysmon_readout_tri_iobuf_13
       (.I(gpio_sysmon_readout_tri_o_13),
        .IO(gpio_sysmon_readout_tri_io[13]),
        .O(gpio_sysmon_readout_tri_i_13),
        .T(gpio_sysmon_readout_tri_t_13));
  IOBUF gpio_sysmon_readout_tri_iobuf_14
       (.I(gpio_sysmon_readout_tri_o_14),
        .IO(gpio_sysmon_readout_tri_io[14]),
        .O(gpio_sysmon_readout_tri_i_14),
        .T(gpio_sysmon_readout_tri_t_14));
  IOBUF gpio_sysmon_readout_tri_iobuf_15
       (.I(gpio_sysmon_readout_tri_o_15),
        .IO(gpio_sysmon_readout_tri_io[15]),
        .O(gpio_sysmon_readout_tri_i_15),
        .T(gpio_sysmon_readout_tri_t_15));
  IOBUF gpio_sysmon_readout_tri_iobuf_2
       (.I(gpio_sysmon_readout_tri_o_2),
        .IO(gpio_sysmon_readout_tri_io[2]),
        .O(gpio_sysmon_readout_tri_i_2),
        .T(gpio_sysmon_readout_tri_t_2));
  IOBUF gpio_sysmon_readout_tri_iobuf_3
       (.I(gpio_sysmon_readout_tri_o_3),
        .IO(gpio_sysmon_readout_tri_io[3]),
        .O(gpio_sysmon_readout_tri_i_3),
        .T(gpio_sysmon_readout_tri_t_3));
  IOBUF gpio_sysmon_readout_tri_iobuf_4
       (.I(gpio_sysmon_readout_tri_o_4),
        .IO(gpio_sysmon_readout_tri_io[4]),
        .O(gpio_sysmon_readout_tri_i_4),
        .T(gpio_sysmon_readout_tri_t_4));
  IOBUF gpio_sysmon_readout_tri_iobuf_5
       (.I(gpio_sysmon_readout_tri_o_5),
        .IO(gpio_sysmon_readout_tri_io[5]),
        .O(gpio_sysmon_readout_tri_i_5),
        .T(gpio_sysmon_readout_tri_t_5));
  IOBUF gpio_sysmon_readout_tri_iobuf_6
       (.I(gpio_sysmon_readout_tri_o_6),
        .IO(gpio_sysmon_readout_tri_io[6]),
        .O(gpio_sysmon_readout_tri_i_6),
        .T(gpio_sysmon_readout_tri_t_6));
  IOBUF gpio_sysmon_readout_tri_iobuf_7
       (.I(gpio_sysmon_readout_tri_o_7),
        .IO(gpio_sysmon_readout_tri_io[7]),
        .O(gpio_sysmon_readout_tri_i_7),
        .T(gpio_sysmon_readout_tri_t_7));
  IOBUF gpio_sysmon_readout_tri_iobuf_8
       (.I(gpio_sysmon_readout_tri_o_8),
        .IO(gpio_sysmon_readout_tri_io[8]),
        .O(gpio_sysmon_readout_tri_i_8),
        .T(gpio_sysmon_readout_tri_t_8));
  IOBUF gpio_sysmon_readout_tri_iobuf_9
       (.I(gpio_sysmon_readout_tri_o_9),
        .IO(gpio_sysmon_readout_tri_io[9]),
        .O(gpio_sysmon_readout_tri_i_9),
        .T(gpio_sysmon_readout_tri_t_9));
  IOBUF sfp_iic_scl_iobuf
       (.I(sfp_iic_scl_o),
        .IO(sfp_iic_scl_io),
        .O(sfp_iic_scl_i),
        .T(sfp_iic_scl_t));
  IOBUF sfp_iic_sda_iobuf
       (.I(sfp_iic_sda_o),
        .IO(sfp_iic_sda_io),
        .O(sfp_iic_sda_i),
        .T(sfp_iic_sda_t));
endmodule
