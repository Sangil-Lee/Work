//Copyright 1986-2016 Xilinx, Inc. All Rights Reserved.
//--------------------------------------------------------------------------------
//Tool Version: Vivado v.2016.1 (lin64) Build 1538259 Fri Apr  8 15:45:23 MDT 2016
//Date        : Sun Nov 13 09:56:08 2016
//Host        : localhost.localdomain running 64-bit CentOS Linux release 7.2.1511 (Core)
//Command     : generate_target base_zynq_design_wrapper.bd
//Design      : base_zynq_design_wrapper
//Purpose     : IP block netlist
//--------------------------------------------------------------------------------
`timescale 1 ps / 1 ps

module base_zynq_design_wrapper
   (DDR_addr,
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
    UART_0_rxd,
    UART_0_txd,
    gpio_date_tri_i,
    gpio_diag_0_tri_i,
    gpio_diag_1_tri_i,
    gpio_evg_csr_tri_io,
    gpio_evg_heartbeat_interval_tri_io,
    gpio_gps_status_tri_io,
    gpio_gtx_control_tri_io,
    gpio_tdc_tri_i,
    sysClk);
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
  input UART_0_rxd;
  output UART_0_txd;
  input [31:0]gpio_date_tri_i;
  input [31:0]gpio_diag_0_tri_i;
  input [31:0]gpio_diag_1_tri_i;
  inout [31:0]gpio_evg_csr_tri_io;
  inout [31:0]gpio_evg_heartbeat_interval_tri_io;
  inout [7:0]gpio_gps_status_tri_io;
  inout [5:0]gpio_gtx_control_tri_io;
  input [31:0]gpio_tdc_tri_i;
  output sysClk;

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
  wire UART_0_rxd;
  wire UART_0_txd;
  wire [31:0]gpio_date_tri_i;
  wire [31:0]gpio_diag_0_tri_i;
  wire [31:0]gpio_diag_1_tri_i;
  wire [0:0]gpio_evg_csr_tri_i_0;
  wire [1:1]gpio_evg_csr_tri_i_1;
  wire [10:10]gpio_evg_csr_tri_i_10;
  wire [11:11]gpio_evg_csr_tri_i_11;
  wire [12:12]gpio_evg_csr_tri_i_12;
  wire [13:13]gpio_evg_csr_tri_i_13;
  wire [14:14]gpio_evg_csr_tri_i_14;
  wire [15:15]gpio_evg_csr_tri_i_15;
  wire [16:16]gpio_evg_csr_tri_i_16;
  wire [17:17]gpio_evg_csr_tri_i_17;
  wire [18:18]gpio_evg_csr_tri_i_18;
  wire [19:19]gpio_evg_csr_tri_i_19;
  wire [2:2]gpio_evg_csr_tri_i_2;
  wire [20:20]gpio_evg_csr_tri_i_20;
  wire [21:21]gpio_evg_csr_tri_i_21;
  wire [22:22]gpio_evg_csr_tri_i_22;
  wire [23:23]gpio_evg_csr_tri_i_23;
  wire [24:24]gpio_evg_csr_tri_i_24;
  wire [25:25]gpio_evg_csr_tri_i_25;
  wire [26:26]gpio_evg_csr_tri_i_26;
  wire [27:27]gpio_evg_csr_tri_i_27;
  wire [28:28]gpio_evg_csr_tri_i_28;
  wire [29:29]gpio_evg_csr_tri_i_29;
  wire [3:3]gpio_evg_csr_tri_i_3;
  wire [30:30]gpio_evg_csr_tri_i_30;
  wire [31:31]gpio_evg_csr_tri_i_31;
  wire [4:4]gpio_evg_csr_tri_i_4;
  wire [5:5]gpio_evg_csr_tri_i_5;
  wire [6:6]gpio_evg_csr_tri_i_6;
  wire [7:7]gpio_evg_csr_tri_i_7;
  wire [8:8]gpio_evg_csr_tri_i_8;
  wire [9:9]gpio_evg_csr_tri_i_9;
  wire [0:0]gpio_evg_csr_tri_io_0;
  wire [1:1]gpio_evg_csr_tri_io_1;
  wire [10:10]gpio_evg_csr_tri_io_10;
  wire [11:11]gpio_evg_csr_tri_io_11;
  wire [12:12]gpio_evg_csr_tri_io_12;
  wire [13:13]gpio_evg_csr_tri_io_13;
  wire [14:14]gpio_evg_csr_tri_io_14;
  wire [15:15]gpio_evg_csr_tri_io_15;
  wire [16:16]gpio_evg_csr_tri_io_16;
  wire [17:17]gpio_evg_csr_tri_io_17;
  wire [18:18]gpio_evg_csr_tri_io_18;
  wire [19:19]gpio_evg_csr_tri_io_19;
  wire [2:2]gpio_evg_csr_tri_io_2;
  wire [20:20]gpio_evg_csr_tri_io_20;
  wire [21:21]gpio_evg_csr_tri_io_21;
  wire [22:22]gpio_evg_csr_tri_io_22;
  wire [23:23]gpio_evg_csr_tri_io_23;
  wire [24:24]gpio_evg_csr_tri_io_24;
  wire [25:25]gpio_evg_csr_tri_io_25;
  wire [26:26]gpio_evg_csr_tri_io_26;
  wire [27:27]gpio_evg_csr_tri_io_27;
  wire [28:28]gpio_evg_csr_tri_io_28;
  wire [29:29]gpio_evg_csr_tri_io_29;
  wire [3:3]gpio_evg_csr_tri_io_3;
  wire [30:30]gpio_evg_csr_tri_io_30;
  wire [31:31]gpio_evg_csr_tri_io_31;
  wire [4:4]gpio_evg_csr_tri_io_4;
  wire [5:5]gpio_evg_csr_tri_io_5;
  wire [6:6]gpio_evg_csr_tri_io_6;
  wire [7:7]gpio_evg_csr_tri_io_7;
  wire [8:8]gpio_evg_csr_tri_io_8;
  wire [9:9]gpio_evg_csr_tri_io_9;
  wire [0:0]gpio_evg_csr_tri_o_0;
  wire [1:1]gpio_evg_csr_tri_o_1;
  wire [10:10]gpio_evg_csr_tri_o_10;
  wire [11:11]gpio_evg_csr_tri_o_11;
  wire [12:12]gpio_evg_csr_tri_o_12;
  wire [13:13]gpio_evg_csr_tri_o_13;
  wire [14:14]gpio_evg_csr_tri_o_14;
  wire [15:15]gpio_evg_csr_tri_o_15;
  wire [16:16]gpio_evg_csr_tri_o_16;
  wire [17:17]gpio_evg_csr_tri_o_17;
  wire [18:18]gpio_evg_csr_tri_o_18;
  wire [19:19]gpio_evg_csr_tri_o_19;
  wire [2:2]gpio_evg_csr_tri_o_2;
  wire [20:20]gpio_evg_csr_tri_o_20;
  wire [21:21]gpio_evg_csr_tri_o_21;
  wire [22:22]gpio_evg_csr_tri_o_22;
  wire [23:23]gpio_evg_csr_tri_o_23;
  wire [24:24]gpio_evg_csr_tri_o_24;
  wire [25:25]gpio_evg_csr_tri_o_25;
  wire [26:26]gpio_evg_csr_tri_o_26;
  wire [27:27]gpio_evg_csr_tri_o_27;
  wire [28:28]gpio_evg_csr_tri_o_28;
  wire [29:29]gpio_evg_csr_tri_o_29;
  wire [3:3]gpio_evg_csr_tri_o_3;
  wire [30:30]gpio_evg_csr_tri_o_30;
  wire [31:31]gpio_evg_csr_tri_o_31;
  wire [4:4]gpio_evg_csr_tri_o_4;
  wire [5:5]gpio_evg_csr_tri_o_5;
  wire [6:6]gpio_evg_csr_tri_o_6;
  wire [7:7]gpio_evg_csr_tri_o_7;
  wire [8:8]gpio_evg_csr_tri_o_8;
  wire [9:9]gpio_evg_csr_tri_o_9;
  wire [0:0]gpio_evg_csr_tri_t_0;
  wire [1:1]gpio_evg_csr_tri_t_1;
  wire [10:10]gpio_evg_csr_tri_t_10;
  wire [11:11]gpio_evg_csr_tri_t_11;
  wire [12:12]gpio_evg_csr_tri_t_12;
  wire [13:13]gpio_evg_csr_tri_t_13;
  wire [14:14]gpio_evg_csr_tri_t_14;
  wire [15:15]gpio_evg_csr_tri_t_15;
  wire [16:16]gpio_evg_csr_tri_t_16;
  wire [17:17]gpio_evg_csr_tri_t_17;
  wire [18:18]gpio_evg_csr_tri_t_18;
  wire [19:19]gpio_evg_csr_tri_t_19;
  wire [2:2]gpio_evg_csr_tri_t_2;
  wire [20:20]gpio_evg_csr_tri_t_20;
  wire [21:21]gpio_evg_csr_tri_t_21;
  wire [22:22]gpio_evg_csr_tri_t_22;
  wire [23:23]gpio_evg_csr_tri_t_23;
  wire [24:24]gpio_evg_csr_tri_t_24;
  wire [25:25]gpio_evg_csr_tri_t_25;
  wire [26:26]gpio_evg_csr_tri_t_26;
  wire [27:27]gpio_evg_csr_tri_t_27;
  wire [28:28]gpio_evg_csr_tri_t_28;
  wire [29:29]gpio_evg_csr_tri_t_29;
  wire [3:3]gpio_evg_csr_tri_t_3;
  wire [30:30]gpio_evg_csr_tri_t_30;
  wire [31:31]gpio_evg_csr_tri_t_31;
  wire [4:4]gpio_evg_csr_tri_t_4;
  wire [5:5]gpio_evg_csr_tri_t_5;
  wire [6:6]gpio_evg_csr_tri_t_6;
  wire [7:7]gpio_evg_csr_tri_t_7;
  wire [8:8]gpio_evg_csr_tri_t_8;
  wire [9:9]gpio_evg_csr_tri_t_9;
  wire [0:0]gpio_evg_heartbeat_interval_tri_i_0;
  wire [1:1]gpio_evg_heartbeat_interval_tri_i_1;
  wire [10:10]gpio_evg_heartbeat_interval_tri_i_10;
  wire [11:11]gpio_evg_heartbeat_interval_tri_i_11;
  wire [12:12]gpio_evg_heartbeat_interval_tri_i_12;
  wire [13:13]gpio_evg_heartbeat_interval_tri_i_13;
  wire [14:14]gpio_evg_heartbeat_interval_tri_i_14;
  wire [15:15]gpio_evg_heartbeat_interval_tri_i_15;
  wire [16:16]gpio_evg_heartbeat_interval_tri_i_16;
  wire [17:17]gpio_evg_heartbeat_interval_tri_i_17;
  wire [18:18]gpio_evg_heartbeat_interval_tri_i_18;
  wire [19:19]gpio_evg_heartbeat_interval_tri_i_19;
  wire [2:2]gpio_evg_heartbeat_interval_tri_i_2;
  wire [20:20]gpio_evg_heartbeat_interval_tri_i_20;
  wire [21:21]gpio_evg_heartbeat_interval_tri_i_21;
  wire [22:22]gpio_evg_heartbeat_interval_tri_i_22;
  wire [23:23]gpio_evg_heartbeat_interval_tri_i_23;
  wire [24:24]gpio_evg_heartbeat_interval_tri_i_24;
  wire [25:25]gpio_evg_heartbeat_interval_tri_i_25;
  wire [26:26]gpio_evg_heartbeat_interval_tri_i_26;
  wire [27:27]gpio_evg_heartbeat_interval_tri_i_27;
  wire [28:28]gpio_evg_heartbeat_interval_tri_i_28;
  wire [29:29]gpio_evg_heartbeat_interval_tri_i_29;
  wire [3:3]gpio_evg_heartbeat_interval_tri_i_3;
  wire [30:30]gpio_evg_heartbeat_interval_tri_i_30;
  wire [31:31]gpio_evg_heartbeat_interval_tri_i_31;
  wire [4:4]gpio_evg_heartbeat_interval_tri_i_4;
  wire [5:5]gpio_evg_heartbeat_interval_tri_i_5;
  wire [6:6]gpio_evg_heartbeat_interval_tri_i_6;
  wire [7:7]gpio_evg_heartbeat_interval_tri_i_7;
  wire [8:8]gpio_evg_heartbeat_interval_tri_i_8;
  wire [9:9]gpio_evg_heartbeat_interval_tri_i_9;
  wire [0:0]gpio_evg_heartbeat_interval_tri_io_0;
  wire [1:1]gpio_evg_heartbeat_interval_tri_io_1;
  wire [10:10]gpio_evg_heartbeat_interval_tri_io_10;
  wire [11:11]gpio_evg_heartbeat_interval_tri_io_11;
  wire [12:12]gpio_evg_heartbeat_interval_tri_io_12;
  wire [13:13]gpio_evg_heartbeat_interval_tri_io_13;
  wire [14:14]gpio_evg_heartbeat_interval_tri_io_14;
  wire [15:15]gpio_evg_heartbeat_interval_tri_io_15;
  wire [16:16]gpio_evg_heartbeat_interval_tri_io_16;
  wire [17:17]gpio_evg_heartbeat_interval_tri_io_17;
  wire [18:18]gpio_evg_heartbeat_interval_tri_io_18;
  wire [19:19]gpio_evg_heartbeat_interval_tri_io_19;
  wire [2:2]gpio_evg_heartbeat_interval_tri_io_2;
  wire [20:20]gpio_evg_heartbeat_interval_tri_io_20;
  wire [21:21]gpio_evg_heartbeat_interval_tri_io_21;
  wire [22:22]gpio_evg_heartbeat_interval_tri_io_22;
  wire [23:23]gpio_evg_heartbeat_interval_tri_io_23;
  wire [24:24]gpio_evg_heartbeat_interval_tri_io_24;
  wire [25:25]gpio_evg_heartbeat_interval_tri_io_25;
  wire [26:26]gpio_evg_heartbeat_interval_tri_io_26;
  wire [27:27]gpio_evg_heartbeat_interval_tri_io_27;
  wire [28:28]gpio_evg_heartbeat_interval_tri_io_28;
  wire [29:29]gpio_evg_heartbeat_interval_tri_io_29;
  wire [3:3]gpio_evg_heartbeat_interval_tri_io_3;
  wire [30:30]gpio_evg_heartbeat_interval_tri_io_30;
  wire [31:31]gpio_evg_heartbeat_interval_tri_io_31;
  wire [4:4]gpio_evg_heartbeat_interval_tri_io_4;
  wire [5:5]gpio_evg_heartbeat_interval_tri_io_5;
  wire [6:6]gpio_evg_heartbeat_interval_tri_io_6;
  wire [7:7]gpio_evg_heartbeat_interval_tri_io_7;
  wire [8:8]gpio_evg_heartbeat_interval_tri_io_8;
  wire [9:9]gpio_evg_heartbeat_interval_tri_io_9;
  wire [0:0]gpio_evg_heartbeat_interval_tri_o_0;
  wire [1:1]gpio_evg_heartbeat_interval_tri_o_1;
  wire [10:10]gpio_evg_heartbeat_interval_tri_o_10;
  wire [11:11]gpio_evg_heartbeat_interval_tri_o_11;
  wire [12:12]gpio_evg_heartbeat_interval_tri_o_12;
  wire [13:13]gpio_evg_heartbeat_interval_tri_o_13;
  wire [14:14]gpio_evg_heartbeat_interval_tri_o_14;
  wire [15:15]gpio_evg_heartbeat_interval_tri_o_15;
  wire [16:16]gpio_evg_heartbeat_interval_tri_o_16;
  wire [17:17]gpio_evg_heartbeat_interval_tri_o_17;
  wire [18:18]gpio_evg_heartbeat_interval_tri_o_18;
  wire [19:19]gpio_evg_heartbeat_interval_tri_o_19;
  wire [2:2]gpio_evg_heartbeat_interval_tri_o_2;
  wire [20:20]gpio_evg_heartbeat_interval_tri_o_20;
  wire [21:21]gpio_evg_heartbeat_interval_tri_o_21;
  wire [22:22]gpio_evg_heartbeat_interval_tri_o_22;
  wire [23:23]gpio_evg_heartbeat_interval_tri_o_23;
  wire [24:24]gpio_evg_heartbeat_interval_tri_o_24;
  wire [25:25]gpio_evg_heartbeat_interval_tri_o_25;
  wire [26:26]gpio_evg_heartbeat_interval_tri_o_26;
  wire [27:27]gpio_evg_heartbeat_interval_tri_o_27;
  wire [28:28]gpio_evg_heartbeat_interval_tri_o_28;
  wire [29:29]gpio_evg_heartbeat_interval_tri_o_29;
  wire [3:3]gpio_evg_heartbeat_interval_tri_o_3;
  wire [30:30]gpio_evg_heartbeat_interval_tri_o_30;
  wire [31:31]gpio_evg_heartbeat_interval_tri_o_31;
  wire [4:4]gpio_evg_heartbeat_interval_tri_o_4;
  wire [5:5]gpio_evg_heartbeat_interval_tri_o_5;
  wire [6:6]gpio_evg_heartbeat_interval_tri_o_6;
  wire [7:7]gpio_evg_heartbeat_interval_tri_o_7;
  wire [8:8]gpio_evg_heartbeat_interval_tri_o_8;
  wire [9:9]gpio_evg_heartbeat_interval_tri_o_9;
  wire [0:0]gpio_evg_heartbeat_interval_tri_t_0;
  wire [1:1]gpio_evg_heartbeat_interval_tri_t_1;
  wire [10:10]gpio_evg_heartbeat_interval_tri_t_10;
  wire [11:11]gpio_evg_heartbeat_interval_tri_t_11;
  wire [12:12]gpio_evg_heartbeat_interval_tri_t_12;
  wire [13:13]gpio_evg_heartbeat_interval_tri_t_13;
  wire [14:14]gpio_evg_heartbeat_interval_tri_t_14;
  wire [15:15]gpio_evg_heartbeat_interval_tri_t_15;
  wire [16:16]gpio_evg_heartbeat_interval_tri_t_16;
  wire [17:17]gpio_evg_heartbeat_interval_tri_t_17;
  wire [18:18]gpio_evg_heartbeat_interval_tri_t_18;
  wire [19:19]gpio_evg_heartbeat_interval_tri_t_19;
  wire [2:2]gpio_evg_heartbeat_interval_tri_t_2;
  wire [20:20]gpio_evg_heartbeat_interval_tri_t_20;
  wire [21:21]gpio_evg_heartbeat_interval_tri_t_21;
  wire [22:22]gpio_evg_heartbeat_interval_tri_t_22;
  wire [23:23]gpio_evg_heartbeat_interval_tri_t_23;
  wire [24:24]gpio_evg_heartbeat_interval_tri_t_24;
  wire [25:25]gpio_evg_heartbeat_interval_tri_t_25;
  wire [26:26]gpio_evg_heartbeat_interval_tri_t_26;
  wire [27:27]gpio_evg_heartbeat_interval_tri_t_27;
  wire [28:28]gpio_evg_heartbeat_interval_tri_t_28;
  wire [29:29]gpio_evg_heartbeat_interval_tri_t_29;
  wire [3:3]gpio_evg_heartbeat_interval_tri_t_3;
  wire [30:30]gpio_evg_heartbeat_interval_tri_t_30;
  wire [31:31]gpio_evg_heartbeat_interval_tri_t_31;
  wire [4:4]gpio_evg_heartbeat_interval_tri_t_4;
  wire [5:5]gpio_evg_heartbeat_interval_tri_t_5;
  wire [6:6]gpio_evg_heartbeat_interval_tri_t_6;
  wire [7:7]gpio_evg_heartbeat_interval_tri_t_7;
  wire [8:8]gpio_evg_heartbeat_interval_tri_t_8;
  wire [9:9]gpio_evg_heartbeat_interval_tri_t_9;
  wire [0:0]gpio_gps_status_tri_i_0;
  wire [1:1]gpio_gps_status_tri_i_1;
  wire [2:2]gpio_gps_status_tri_i_2;
  wire [3:3]gpio_gps_status_tri_i_3;
  wire [4:4]gpio_gps_status_tri_i_4;
  wire [5:5]gpio_gps_status_tri_i_5;
  wire [6:6]gpio_gps_status_tri_i_6;
  wire [7:7]gpio_gps_status_tri_i_7;
  wire [0:0]gpio_gps_status_tri_io_0;
  wire [1:1]gpio_gps_status_tri_io_1;
  wire [2:2]gpio_gps_status_tri_io_2;
  wire [3:3]gpio_gps_status_tri_io_3;
  wire [4:4]gpio_gps_status_tri_io_4;
  wire [5:5]gpio_gps_status_tri_io_5;
  wire [6:6]gpio_gps_status_tri_io_6;
  wire [7:7]gpio_gps_status_tri_io_7;
  wire [0:0]gpio_gps_status_tri_o_0;
  wire [1:1]gpio_gps_status_tri_o_1;
  wire [2:2]gpio_gps_status_tri_o_2;
  wire [3:3]gpio_gps_status_tri_o_3;
  wire [4:4]gpio_gps_status_tri_o_4;
  wire [5:5]gpio_gps_status_tri_o_5;
  wire [6:6]gpio_gps_status_tri_o_6;
  wire [7:7]gpio_gps_status_tri_o_7;
  wire [0:0]gpio_gps_status_tri_t_0;
  wire [1:1]gpio_gps_status_tri_t_1;
  wire [2:2]gpio_gps_status_tri_t_2;
  wire [3:3]gpio_gps_status_tri_t_3;
  wire [4:4]gpio_gps_status_tri_t_4;
  wire [5:5]gpio_gps_status_tri_t_5;
  wire [6:6]gpio_gps_status_tri_t_6;
  wire [7:7]gpio_gps_status_tri_t_7;
  wire [0:0]gpio_gtx_control_tri_i_0;
  wire [1:1]gpio_gtx_control_tri_i_1;
  wire [2:2]gpio_gtx_control_tri_i_2;
  wire [3:3]gpio_gtx_control_tri_i_3;
  wire [4:4]gpio_gtx_control_tri_i_4;
  wire [5:5]gpio_gtx_control_tri_i_5;
  wire [0:0]gpio_gtx_control_tri_io_0;
  wire [1:1]gpio_gtx_control_tri_io_1;
  wire [2:2]gpio_gtx_control_tri_io_2;
  wire [3:3]gpio_gtx_control_tri_io_3;
  wire [4:4]gpio_gtx_control_tri_io_4;
  wire [5:5]gpio_gtx_control_tri_io_5;
  wire [0:0]gpio_gtx_control_tri_o_0;
  wire [1:1]gpio_gtx_control_tri_o_1;
  wire [2:2]gpio_gtx_control_tri_o_2;
  wire [3:3]gpio_gtx_control_tri_o_3;
  wire [4:4]gpio_gtx_control_tri_o_4;
  wire [5:5]gpio_gtx_control_tri_o_5;
  wire [0:0]gpio_gtx_control_tri_t_0;
  wire [1:1]gpio_gtx_control_tri_t_1;
  wire [2:2]gpio_gtx_control_tri_t_2;
  wire [3:3]gpio_gtx_control_tri_t_3;
  wire [4:4]gpio_gtx_control_tri_t_4;
  wire [5:5]gpio_gtx_control_tri_t_5;
  wire [31:0]gpio_tdc_tri_i;
  wire sysClk;

  base_zynq_design base_zynq_design_i
       (.DDR_addr(DDR_addr),
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
        .GPIO_DATE_tri_i(gpio_date_tri_i),
        .GPIO_DIAG_0_tri_i(gpio_diag_0_tri_i),
        .GPIO_DIAG_1_tri_i(gpio_diag_1_tri_i),
        .GPIO_EVG_CSR_tri_i({gpio_evg_csr_tri_i_31,gpio_evg_csr_tri_i_30,gpio_evg_csr_tri_i_29,gpio_evg_csr_tri_i_28,gpio_evg_csr_tri_i_27,gpio_evg_csr_tri_i_26,gpio_evg_csr_tri_i_25,gpio_evg_csr_tri_i_24,gpio_evg_csr_tri_i_23,gpio_evg_csr_tri_i_22,gpio_evg_csr_tri_i_21,gpio_evg_csr_tri_i_20,gpio_evg_csr_tri_i_19,gpio_evg_csr_tri_i_18,gpio_evg_csr_tri_i_17,gpio_evg_csr_tri_i_16,gpio_evg_csr_tri_i_15,gpio_evg_csr_tri_i_14,gpio_evg_csr_tri_i_13,gpio_evg_csr_tri_i_12,gpio_evg_csr_tri_i_11,gpio_evg_csr_tri_i_10,gpio_evg_csr_tri_i_9,gpio_evg_csr_tri_i_8,gpio_evg_csr_tri_i_7,gpio_evg_csr_tri_i_6,gpio_evg_csr_tri_i_5,gpio_evg_csr_tri_i_4,gpio_evg_csr_tri_i_3,gpio_evg_csr_tri_i_2,gpio_evg_csr_tri_i_1,gpio_evg_csr_tri_i_0}),
        .GPIO_EVG_CSR_tri_o({gpio_evg_csr_tri_o_31,gpio_evg_csr_tri_o_30,gpio_evg_csr_tri_o_29,gpio_evg_csr_tri_o_28,gpio_evg_csr_tri_o_27,gpio_evg_csr_tri_o_26,gpio_evg_csr_tri_o_25,gpio_evg_csr_tri_o_24,gpio_evg_csr_tri_o_23,gpio_evg_csr_tri_o_22,gpio_evg_csr_tri_o_21,gpio_evg_csr_tri_o_20,gpio_evg_csr_tri_o_19,gpio_evg_csr_tri_o_18,gpio_evg_csr_tri_o_17,gpio_evg_csr_tri_o_16,gpio_evg_csr_tri_o_15,gpio_evg_csr_tri_o_14,gpio_evg_csr_tri_o_13,gpio_evg_csr_tri_o_12,gpio_evg_csr_tri_o_11,gpio_evg_csr_tri_o_10,gpio_evg_csr_tri_o_9,gpio_evg_csr_tri_o_8,gpio_evg_csr_tri_o_7,gpio_evg_csr_tri_o_6,gpio_evg_csr_tri_o_5,gpio_evg_csr_tri_o_4,gpio_evg_csr_tri_o_3,gpio_evg_csr_tri_o_2,gpio_evg_csr_tri_o_1,gpio_evg_csr_tri_o_0}),
        .GPIO_EVG_CSR_tri_t({gpio_evg_csr_tri_t_31,gpio_evg_csr_tri_t_30,gpio_evg_csr_tri_t_29,gpio_evg_csr_tri_t_28,gpio_evg_csr_tri_t_27,gpio_evg_csr_tri_t_26,gpio_evg_csr_tri_t_25,gpio_evg_csr_tri_t_24,gpio_evg_csr_tri_t_23,gpio_evg_csr_tri_t_22,gpio_evg_csr_tri_t_21,gpio_evg_csr_tri_t_20,gpio_evg_csr_tri_t_19,gpio_evg_csr_tri_t_18,gpio_evg_csr_tri_t_17,gpio_evg_csr_tri_t_16,gpio_evg_csr_tri_t_15,gpio_evg_csr_tri_t_14,gpio_evg_csr_tri_t_13,gpio_evg_csr_tri_t_12,gpio_evg_csr_tri_t_11,gpio_evg_csr_tri_t_10,gpio_evg_csr_tri_t_9,gpio_evg_csr_tri_t_8,gpio_evg_csr_tri_t_7,gpio_evg_csr_tri_t_6,gpio_evg_csr_tri_t_5,gpio_evg_csr_tri_t_4,gpio_evg_csr_tri_t_3,gpio_evg_csr_tri_t_2,gpio_evg_csr_tri_t_1,gpio_evg_csr_tri_t_0}),
        .GPIO_EVG_HEARTBEAT_INTERVAL_tri_i({gpio_evg_heartbeat_interval_tri_i_31,gpio_evg_heartbeat_interval_tri_i_30,gpio_evg_heartbeat_interval_tri_i_29,gpio_evg_heartbeat_interval_tri_i_28,gpio_evg_heartbeat_interval_tri_i_27,gpio_evg_heartbeat_interval_tri_i_26,gpio_evg_heartbeat_interval_tri_i_25,gpio_evg_heartbeat_interval_tri_i_24,gpio_evg_heartbeat_interval_tri_i_23,gpio_evg_heartbeat_interval_tri_i_22,gpio_evg_heartbeat_interval_tri_i_21,gpio_evg_heartbeat_interval_tri_i_20,gpio_evg_heartbeat_interval_tri_i_19,gpio_evg_heartbeat_interval_tri_i_18,gpio_evg_heartbeat_interval_tri_i_17,gpio_evg_heartbeat_interval_tri_i_16,gpio_evg_heartbeat_interval_tri_i_15,gpio_evg_heartbeat_interval_tri_i_14,gpio_evg_heartbeat_interval_tri_i_13,gpio_evg_heartbeat_interval_tri_i_12,gpio_evg_heartbeat_interval_tri_i_11,gpio_evg_heartbeat_interval_tri_i_10,gpio_evg_heartbeat_interval_tri_i_9,gpio_evg_heartbeat_interval_tri_i_8,gpio_evg_heartbeat_interval_tri_i_7,gpio_evg_heartbeat_interval_tri_i_6,gpio_evg_heartbeat_interval_tri_i_5,gpio_evg_heartbeat_interval_tri_i_4,gpio_evg_heartbeat_interval_tri_i_3,gpio_evg_heartbeat_interval_tri_i_2,gpio_evg_heartbeat_interval_tri_i_1,gpio_evg_heartbeat_interval_tri_i_0}),
        .GPIO_EVG_HEARTBEAT_INTERVAL_tri_o({gpio_evg_heartbeat_interval_tri_o_31,gpio_evg_heartbeat_interval_tri_o_30,gpio_evg_heartbeat_interval_tri_o_29,gpio_evg_heartbeat_interval_tri_o_28,gpio_evg_heartbeat_interval_tri_o_27,gpio_evg_heartbeat_interval_tri_o_26,gpio_evg_heartbeat_interval_tri_o_25,gpio_evg_heartbeat_interval_tri_o_24,gpio_evg_heartbeat_interval_tri_o_23,gpio_evg_heartbeat_interval_tri_o_22,gpio_evg_heartbeat_interval_tri_o_21,gpio_evg_heartbeat_interval_tri_o_20,gpio_evg_heartbeat_interval_tri_o_19,gpio_evg_heartbeat_interval_tri_o_18,gpio_evg_heartbeat_interval_tri_o_17,gpio_evg_heartbeat_interval_tri_o_16,gpio_evg_heartbeat_interval_tri_o_15,gpio_evg_heartbeat_interval_tri_o_14,gpio_evg_heartbeat_interval_tri_o_13,gpio_evg_heartbeat_interval_tri_o_12,gpio_evg_heartbeat_interval_tri_o_11,gpio_evg_heartbeat_interval_tri_o_10,gpio_evg_heartbeat_interval_tri_o_9,gpio_evg_heartbeat_interval_tri_o_8,gpio_evg_heartbeat_interval_tri_o_7,gpio_evg_heartbeat_interval_tri_o_6,gpio_evg_heartbeat_interval_tri_o_5,gpio_evg_heartbeat_interval_tri_o_4,gpio_evg_heartbeat_interval_tri_o_3,gpio_evg_heartbeat_interval_tri_o_2,gpio_evg_heartbeat_interval_tri_o_1,gpio_evg_heartbeat_interval_tri_o_0}),
        .GPIO_EVG_HEARTBEAT_INTERVAL_tri_t({gpio_evg_heartbeat_interval_tri_t_31,gpio_evg_heartbeat_interval_tri_t_30,gpio_evg_heartbeat_interval_tri_t_29,gpio_evg_heartbeat_interval_tri_t_28,gpio_evg_heartbeat_interval_tri_t_27,gpio_evg_heartbeat_interval_tri_t_26,gpio_evg_heartbeat_interval_tri_t_25,gpio_evg_heartbeat_interval_tri_t_24,gpio_evg_heartbeat_interval_tri_t_23,gpio_evg_heartbeat_interval_tri_t_22,gpio_evg_heartbeat_interval_tri_t_21,gpio_evg_heartbeat_interval_tri_t_20,gpio_evg_heartbeat_interval_tri_t_19,gpio_evg_heartbeat_interval_tri_t_18,gpio_evg_heartbeat_interval_tri_t_17,gpio_evg_heartbeat_interval_tri_t_16,gpio_evg_heartbeat_interval_tri_t_15,gpio_evg_heartbeat_interval_tri_t_14,gpio_evg_heartbeat_interval_tri_t_13,gpio_evg_heartbeat_interval_tri_t_12,gpio_evg_heartbeat_interval_tri_t_11,gpio_evg_heartbeat_interval_tri_t_10,gpio_evg_heartbeat_interval_tri_t_9,gpio_evg_heartbeat_interval_tri_t_8,gpio_evg_heartbeat_interval_tri_t_7,gpio_evg_heartbeat_interval_tri_t_6,gpio_evg_heartbeat_interval_tri_t_5,gpio_evg_heartbeat_interval_tri_t_4,gpio_evg_heartbeat_interval_tri_t_3,gpio_evg_heartbeat_interval_tri_t_2,gpio_evg_heartbeat_interval_tri_t_1,gpio_evg_heartbeat_interval_tri_t_0}),
        .GPIO_GPS_STATUS_tri_i({gpio_gps_status_tri_i_7,gpio_gps_status_tri_i_6,gpio_gps_status_tri_i_5,gpio_gps_status_tri_i_4,gpio_gps_status_tri_i_3,gpio_gps_status_tri_i_2,gpio_gps_status_tri_i_1,gpio_gps_status_tri_i_0}),
        .GPIO_GPS_STATUS_tri_o({gpio_gps_status_tri_o_7,gpio_gps_status_tri_o_6,gpio_gps_status_tri_o_5,gpio_gps_status_tri_o_4,gpio_gps_status_tri_o_3,gpio_gps_status_tri_o_2,gpio_gps_status_tri_o_1,gpio_gps_status_tri_o_0}),
        .GPIO_GPS_STATUS_tri_t({gpio_gps_status_tri_t_7,gpio_gps_status_tri_t_6,gpio_gps_status_tri_t_5,gpio_gps_status_tri_t_4,gpio_gps_status_tri_t_3,gpio_gps_status_tri_t_2,gpio_gps_status_tri_t_1,gpio_gps_status_tri_t_0}),
        .GPIO_GTX_CONTROL_tri_i({gpio_gtx_control_tri_i_5,gpio_gtx_control_tri_i_4,gpio_gtx_control_tri_i_3,gpio_gtx_control_tri_i_2,gpio_gtx_control_tri_i_1,gpio_gtx_control_tri_i_0}),
        .GPIO_GTX_CONTROL_tri_o({gpio_gtx_control_tri_o_5,gpio_gtx_control_tri_o_4,gpio_gtx_control_tri_o_3,gpio_gtx_control_tri_o_2,gpio_gtx_control_tri_o_1,gpio_gtx_control_tri_o_0}),
        .GPIO_GTX_CONTROL_tri_t({gpio_gtx_control_tri_t_5,gpio_gtx_control_tri_t_4,gpio_gtx_control_tri_t_3,gpio_gtx_control_tri_t_2,gpio_gtx_control_tri_t_1,gpio_gtx_control_tri_t_0}),
        .GPIO_TDC_tri_i(gpio_tdc_tri_i),
        .UART_0_rxd(UART_0_rxd),
        .UART_0_txd(UART_0_txd),
        .sysClk(sysClk));
  IOBUF gpio_evg_csr_tri_iobuf_0
       (.I(gpio_evg_csr_tri_o_0),
        .IO(gpio_evg_csr_tri_io[0]),
        .O(gpio_evg_csr_tri_i_0),
        .T(gpio_evg_csr_tri_t_0));
  IOBUF gpio_evg_csr_tri_iobuf_1
       (.I(gpio_evg_csr_tri_o_1),
        .IO(gpio_evg_csr_tri_io[1]),
        .O(gpio_evg_csr_tri_i_1),
        .T(gpio_evg_csr_tri_t_1));
  IOBUF gpio_evg_csr_tri_iobuf_10
       (.I(gpio_evg_csr_tri_o_10),
        .IO(gpio_evg_csr_tri_io[10]),
        .O(gpio_evg_csr_tri_i_10),
        .T(gpio_evg_csr_tri_t_10));
  IOBUF gpio_evg_csr_tri_iobuf_11
       (.I(gpio_evg_csr_tri_o_11),
        .IO(gpio_evg_csr_tri_io[11]),
        .O(gpio_evg_csr_tri_i_11),
        .T(gpio_evg_csr_tri_t_11));
  IOBUF gpio_evg_csr_tri_iobuf_12
       (.I(gpio_evg_csr_tri_o_12),
        .IO(gpio_evg_csr_tri_io[12]),
        .O(gpio_evg_csr_tri_i_12),
        .T(gpio_evg_csr_tri_t_12));
  IOBUF gpio_evg_csr_tri_iobuf_13
       (.I(gpio_evg_csr_tri_o_13),
        .IO(gpio_evg_csr_tri_io[13]),
        .O(gpio_evg_csr_tri_i_13),
        .T(gpio_evg_csr_tri_t_13));
  IOBUF gpio_evg_csr_tri_iobuf_14
       (.I(gpio_evg_csr_tri_o_14),
        .IO(gpio_evg_csr_tri_io[14]),
        .O(gpio_evg_csr_tri_i_14),
        .T(gpio_evg_csr_tri_t_14));
  IOBUF gpio_evg_csr_tri_iobuf_15
       (.I(gpio_evg_csr_tri_o_15),
        .IO(gpio_evg_csr_tri_io[15]),
        .O(gpio_evg_csr_tri_i_15),
        .T(gpio_evg_csr_tri_t_15));
  IOBUF gpio_evg_csr_tri_iobuf_16
       (.I(gpio_evg_csr_tri_o_16),
        .IO(gpio_evg_csr_tri_io[16]),
        .O(gpio_evg_csr_tri_i_16),
        .T(gpio_evg_csr_tri_t_16));
  IOBUF gpio_evg_csr_tri_iobuf_17
       (.I(gpio_evg_csr_tri_o_17),
        .IO(gpio_evg_csr_tri_io[17]),
        .O(gpio_evg_csr_tri_i_17),
        .T(gpio_evg_csr_tri_t_17));
  IOBUF gpio_evg_csr_tri_iobuf_18
       (.I(gpio_evg_csr_tri_o_18),
        .IO(gpio_evg_csr_tri_io[18]),
        .O(gpio_evg_csr_tri_i_18),
        .T(gpio_evg_csr_tri_t_18));
  IOBUF gpio_evg_csr_tri_iobuf_19
       (.I(gpio_evg_csr_tri_o_19),
        .IO(gpio_evg_csr_tri_io[19]),
        .O(gpio_evg_csr_tri_i_19),
        .T(gpio_evg_csr_tri_t_19));
  IOBUF gpio_evg_csr_tri_iobuf_2
       (.I(gpio_evg_csr_tri_o_2),
        .IO(gpio_evg_csr_tri_io[2]),
        .O(gpio_evg_csr_tri_i_2),
        .T(gpio_evg_csr_tri_t_2));
  IOBUF gpio_evg_csr_tri_iobuf_20
       (.I(gpio_evg_csr_tri_o_20),
        .IO(gpio_evg_csr_tri_io[20]),
        .O(gpio_evg_csr_tri_i_20),
        .T(gpio_evg_csr_tri_t_20));
  IOBUF gpio_evg_csr_tri_iobuf_21
       (.I(gpio_evg_csr_tri_o_21),
        .IO(gpio_evg_csr_tri_io[21]),
        .O(gpio_evg_csr_tri_i_21),
        .T(gpio_evg_csr_tri_t_21));
  IOBUF gpio_evg_csr_tri_iobuf_22
       (.I(gpio_evg_csr_tri_o_22),
        .IO(gpio_evg_csr_tri_io[22]),
        .O(gpio_evg_csr_tri_i_22),
        .T(gpio_evg_csr_tri_t_22));
  IOBUF gpio_evg_csr_tri_iobuf_23
       (.I(gpio_evg_csr_tri_o_23),
        .IO(gpio_evg_csr_tri_io[23]),
        .O(gpio_evg_csr_tri_i_23),
        .T(gpio_evg_csr_tri_t_23));
  IOBUF gpio_evg_csr_tri_iobuf_24
       (.I(gpio_evg_csr_tri_o_24),
        .IO(gpio_evg_csr_tri_io[24]),
        .O(gpio_evg_csr_tri_i_24),
        .T(gpio_evg_csr_tri_t_24));
  IOBUF gpio_evg_csr_tri_iobuf_25
       (.I(gpio_evg_csr_tri_o_25),
        .IO(gpio_evg_csr_tri_io[25]),
        .O(gpio_evg_csr_tri_i_25),
        .T(gpio_evg_csr_tri_t_25));
  IOBUF gpio_evg_csr_tri_iobuf_26
       (.I(gpio_evg_csr_tri_o_26),
        .IO(gpio_evg_csr_tri_io[26]),
        .O(gpio_evg_csr_tri_i_26),
        .T(gpio_evg_csr_tri_t_26));
  IOBUF gpio_evg_csr_tri_iobuf_27
       (.I(gpio_evg_csr_tri_o_27),
        .IO(gpio_evg_csr_tri_io[27]),
        .O(gpio_evg_csr_tri_i_27),
        .T(gpio_evg_csr_tri_t_27));
  IOBUF gpio_evg_csr_tri_iobuf_28
       (.I(gpio_evg_csr_tri_o_28),
        .IO(gpio_evg_csr_tri_io[28]),
        .O(gpio_evg_csr_tri_i_28),
        .T(gpio_evg_csr_tri_t_28));
  IOBUF gpio_evg_csr_tri_iobuf_29
       (.I(gpio_evg_csr_tri_o_29),
        .IO(gpio_evg_csr_tri_io[29]),
        .O(gpio_evg_csr_tri_i_29),
        .T(gpio_evg_csr_tri_t_29));
  IOBUF gpio_evg_csr_tri_iobuf_3
       (.I(gpio_evg_csr_tri_o_3),
        .IO(gpio_evg_csr_tri_io[3]),
        .O(gpio_evg_csr_tri_i_3),
        .T(gpio_evg_csr_tri_t_3));
  IOBUF gpio_evg_csr_tri_iobuf_30
       (.I(gpio_evg_csr_tri_o_30),
        .IO(gpio_evg_csr_tri_io[30]),
        .O(gpio_evg_csr_tri_i_30),
        .T(gpio_evg_csr_tri_t_30));
  IOBUF gpio_evg_csr_tri_iobuf_31
       (.I(gpio_evg_csr_tri_o_31),
        .IO(gpio_evg_csr_tri_io[31]),
        .O(gpio_evg_csr_tri_i_31),
        .T(gpio_evg_csr_tri_t_31));
  IOBUF gpio_evg_csr_tri_iobuf_4
       (.I(gpio_evg_csr_tri_o_4),
        .IO(gpio_evg_csr_tri_io[4]),
        .O(gpio_evg_csr_tri_i_4),
        .T(gpio_evg_csr_tri_t_4));
  IOBUF gpio_evg_csr_tri_iobuf_5
       (.I(gpio_evg_csr_tri_o_5),
        .IO(gpio_evg_csr_tri_io[5]),
        .O(gpio_evg_csr_tri_i_5),
        .T(gpio_evg_csr_tri_t_5));
  IOBUF gpio_evg_csr_tri_iobuf_6
       (.I(gpio_evg_csr_tri_o_6),
        .IO(gpio_evg_csr_tri_io[6]),
        .O(gpio_evg_csr_tri_i_6),
        .T(gpio_evg_csr_tri_t_6));
  IOBUF gpio_evg_csr_tri_iobuf_7
       (.I(gpio_evg_csr_tri_o_7),
        .IO(gpio_evg_csr_tri_io[7]),
        .O(gpio_evg_csr_tri_i_7),
        .T(gpio_evg_csr_tri_t_7));
  IOBUF gpio_evg_csr_tri_iobuf_8
       (.I(gpio_evg_csr_tri_o_8),
        .IO(gpio_evg_csr_tri_io[8]),
        .O(gpio_evg_csr_tri_i_8),
        .T(gpio_evg_csr_tri_t_8));
  IOBUF gpio_evg_csr_tri_iobuf_9
       (.I(gpio_evg_csr_tri_o_9),
        .IO(gpio_evg_csr_tri_io[9]),
        .O(gpio_evg_csr_tri_i_9),
        .T(gpio_evg_csr_tri_t_9));
  IOBUF gpio_evg_heartbeat_interval_tri_iobuf_0
       (.I(gpio_evg_heartbeat_interval_tri_o_0),
        .IO(gpio_evg_heartbeat_interval_tri_io[0]),
        .O(gpio_evg_heartbeat_interval_tri_i_0),
        .T(gpio_evg_heartbeat_interval_tri_t_0));
  IOBUF gpio_evg_heartbeat_interval_tri_iobuf_1
       (.I(gpio_evg_heartbeat_interval_tri_o_1),
        .IO(gpio_evg_heartbeat_interval_tri_io[1]),
        .O(gpio_evg_heartbeat_interval_tri_i_1),
        .T(gpio_evg_heartbeat_interval_tri_t_1));
  IOBUF gpio_evg_heartbeat_interval_tri_iobuf_10
       (.I(gpio_evg_heartbeat_interval_tri_o_10),
        .IO(gpio_evg_heartbeat_interval_tri_io[10]),
        .O(gpio_evg_heartbeat_interval_tri_i_10),
        .T(gpio_evg_heartbeat_interval_tri_t_10));
  IOBUF gpio_evg_heartbeat_interval_tri_iobuf_11
       (.I(gpio_evg_heartbeat_interval_tri_o_11),
        .IO(gpio_evg_heartbeat_interval_tri_io[11]),
        .O(gpio_evg_heartbeat_interval_tri_i_11),
        .T(gpio_evg_heartbeat_interval_tri_t_11));
  IOBUF gpio_evg_heartbeat_interval_tri_iobuf_12
       (.I(gpio_evg_heartbeat_interval_tri_o_12),
        .IO(gpio_evg_heartbeat_interval_tri_io[12]),
        .O(gpio_evg_heartbeat_interval_tri_i_12),
        .T(gpio_evg_heartbeat_interval_tri_t_12));
  IOBUF gpio_evg_heartbeat_interval_tri_iobuf_13
       (.I(gpio_evg_heartbeat_interval_tri_o_13),
        .IO(gpio_evg_heartbeat_interval_tri_io[13]),
        .O(gpio_evg_heartbeat_interval_tri_i_13),
        .T(gpio_evg_heartbeat_interval_tri_t_13));
  IOBUF gpio_evg_heartbeat_interval_tri_iobuf_14
       (.I(gpio_evg_heartbeat_interval_tri_o_14),
        .IO(gpio_evg_heartbeat_interval_tri_io[14]),
        .O(gpio_evg_heartbeat_interval_tri_i_14),
        .T(gpio_evg_heartbeat_interval_tri_t_14));
  IOBUF gpio_evg_heartbeat_interval_tri_iobuf_15
       (.I(gpio_evg_heartbeat_interval_tri_o_15),
        .IO(gpio_evg_heartbeat_interval_tri_io[15]),
        .O(gpio_evg_heartbeat_interval_tri_i_15),
        .T(gpio_evg_heartbeat_interval_tri_t_15));
  IOBUF gpio_evg_heartbeat_interval_tri_iobuf_16
       (.I(gpio_evg_heartbeat_interval_tri_o_16),
        .IO(gpio_evg_heartbeat_interval_tri_io[16]),
        .O(gpio_evg_heartbeat_interval_tri_i_16),
        .T(gpio_evg_heartbeat_interval_tri_t_16));
  IOBUF gpio_evg_heartbeat_interval_tri_iobuf_17
       (.I(gpio_evg_heartbeat_interval_tri_o_17),
        .IO(gpio_evg_heartbeat_interval_tri_io[17]),
        .O(gpio_evg_heartbeat_interval_tri_i_17),
        .T(gpio_evg_heartbeat_interval_tri_t_17));
  IOBUF gpio_evg_heartbeat_interval_tri_iobuf_18
       (.I(gpio_evg_heartbeat_interval_tri_o_18),
        .IO(gpio_evg_heartbeat_interval_tri_io[18]),
        .O(gpio_evg_heartbeat_interval_tri_i_18),
        .T(gpio_evg_heartbeat_interval_tri_t_18));
  IOBUF gpio_evg_heartbeat_interval_tri_iobuf_19
       (.I(gpio_evg_heartbeat_interval_tri_o_19),
        .IO(gpio_evg_heartbeat_interval_tri_io[19]),
        .O(gpio_evg_heartbeat_interval_tri_i_19),
        .T(gpio_evg_heartbeat_interval_tri_t_19));
  IOBUF gpio_evg_heartbeat_interval_tri_iobuf_2
       (.I(gpio_evg_heartbeat_interval_tri_o_2),
        .IO(gpio_evg_heartbeat_interval_tri_io[2]),
        .O(gpio_evg_heartbeat_interval_tri_i_2),
        .T(gpio_evg_heartbeat_interval_tri_t_2));
  IOBUF gpio_evg_heartbeat_interval_tri_iobuf_20
       (.I(gpio_evg_heartbeat_interval_tri_o_20),
        .IO(gpio_evg_heartbeat_interval_tri_io[20]),
        .O(gpio_evg_heartbeat_interval_tri_i_20),
        .T(gpio_evg_heartbeat_interval_tri_t_20));
  IOBUF gpio_evg_heartbeat_interval_tri_iobuf_21
       (.I(gpio_evg_heartbeat_interval_tri_o_21),
        .IO(gpio_evg_heartbeat_interval_tri_io[21]),
        .O(gpio_evg_heartbeat_interval_tri_i_21),
        .T(gpio_evg_heartbeat_interval_tri_t_21));
  IOBUF gpio_evg_heartbeat_interval_tri_iobuf_22
       (.I(gpio_evg_heartbeat_interval_tri_o_22),
        .IO(gpio_evg_heartbeat_interval_tri_io[22]),
        .O(gpio_evg_heartbeat_interval_tri_i_22),
        .T(gpio_evg_heartbeat_interval_tri_t_22));
  IOBUF gpio_evg_heartbeat_interval_tri_iobuf_23
       (.I(gpio_evg_heartbeat_interval_tri_o_23),
        .IO(gpio_evg_heartbeat_interval_tri_io[23]),
        .O(gpio_evg_heartbeat_interval_tri_i_23),
        .T(gpio_evg_heartbeat_interval_tri_t_23));
  IOBUF gpio_evg_heartbeat_interval_tri_iobuf_24
       (.I(gpio_evg_heartbeat_interval_tri_o_24),
        .IO(gpio_evg_heartbeat_interval_tri_io[24]),
        .O(gpio_evg_heartbeat_interval_tri_i_24),
        .T(gpio_evg_heartbeat_interval_tri_t_24));
  IOBUF gpio_evg_heartbeat_interval_tri_iobuf_25
       (.I(gpio_evg_heartbeat_interval_tri_o_25),
        .IO(gpio_evg_heartbeat_interval_tri_io[25]),
        .O(gpio_evg_heartbeat_interval_tri_i_25),
        .T(gpio_evg_heartbeat_interval_tri_t_25));
  IOBUF gpio_evg_heartbeat_interval_tri_iobuf_26
       (.I(gpio_evg_heartbeat_interval_tri_o_26),
        .IO(gpio_evg_heartbeat_interval_tri_io[26]),
        .O(gpio_evg_heartbeat_interval_tri_i_26),
        .T(gpio_evg_heartbeat_interval_tri_t_26));
  IOBUF gpio_evg_heartbeat_interval_tri_iobuf_27
       (.I(gpio_evg_heartbeat_interval_tri_o_27),
        .IO(gpio_evg_heartbeat_interval_tri_io[27]),
        .O(gpio_evg_heartbeat_interval_tri_i_27),
        .T(gpio_evg_heartbeat_interval_tri_t_27));
  IOBUF gpio_evg_heartbeat_interval_tri_iobuf_28
       (.I(gpio_evg_heartbeat_interval_tri_o_28),
        .IO(gpio_evg_heartbeat_interval_tri_io[28]),
        .O(gpio_evg_heartbeat_interval_tri_i_28),
        .T(gpio_evg_heartbeat_interval_tri_t_28));
  IOBUF gpio_evg_heartbeat_interval_tri_iobuf_29
       (.I(gpio_evg_heartbeat_interval_tri_o_29),
        .IO(gpio_evg_heartbeat_interval_tri_io[29]),
        .O(gpio_evg_heartbeat_interval_tri_i_29),
        .T(gpio_evg_heartbeat_interval_tri_t_29));
  IOBUF gpio_evg_heartbeat_interval_tri_iobuf_3
       (.I(gpio_evg_heartbeat_interval_tri_o_3),
        .IO(gpio_evg_heartbeat_interval_tri_io[3]),
        .O(gpio_evg_heartbeat_interval_tri_i_3),
        .T(gpio_evg_heartbeat_interval_tri_t_3));
  IOBUF gpio_evg_heartbeat_interval_tri_iobuf_30
       (.I(gpio_evg_heartbeat_interval_tri_o_30),
        .IO(gpio_evg_heartbeat_interval_tri_io[30]),
        .O(gpio_evg_heartbeat_interval_tri_i_30),
        .T(gpio_evg_heartbeat_interval_tri_t_30));
  IOBUF gpio_evg_heartbeat_interval_tri_iobuf_31
       (.I(gpio_evg_heartbeat_interval_tri_o_31),
        .IO(gpio_evg_heartbeat_interval_tri_io[31]),
        .O(gpio_evg_heartbeat_interval_tri_i_31),
        .T(gpio_evg_heartbeat_interval_tri_t_31));
  IOBUF gpio_evg_heartbeat_interval_tri_iobuf_4
       (.I(gpio_evg_heartbeat_interval_tri_o_4),
        .IO(gpio_evg_heartbeat_interval_tri_io[4]),
        .O(gpio_evg_heartbeat_interval_tri_i_4),
        .T(gpio_evg_heartbeat_interval_tri_t_4));
  IOBUF gpio_evg_heartbeat_interval_tri_iobuf_5
       (.I(gpio_evg_heartbeat_interval_tri_o_5),
        .IO(gpio_evg_heartbeat_interval_tri_io[5]),
        .O(gpio_evg_heartbeat_interval_tri_i_5),
        .T(gpio_evg_heartbeat_interval_tri_t_5));
  IOBUF gpio_evg_heartbeat_interval_tri_iobuf_6
       (.I(gpio_evg_heartbeat_interval_tri_o_6),
        .IO(gpio_evg_heartbeat_interval_tri_io[6]),
        .O(gpio_evg_heartbeat_interval_tri_i_6),
        .T(gpio_evg_heartbeat_interval_tri_t_6));
  IOBUF gpio_evg_heartbeat_interval_tri_iobuf_7
       (.I(gpio_evg_heartbeat_interval_tri_o_7),
        .IO(gpio_evg_heartbeat_interval_tri_io[7]),
        .O(gpio_evg_heartbeat_interval_tri_i_7),
        .T(gpio_evg_heartbeat_interval_tri_t_7));
  IOBUF gpio_evg_heartbeat_interval_tri_iobuf_8
       (.I(gpio_evg_heartbeat_interval_tri_o_8),
        .IO(gpio_evg_heartbeat_interval_tri_io[8]),
        .O(gpio_evg_heartbeat_interval_tri_i_8),
        .T(gpio_evg_heartbeat_interval_tri_t_8));
  IOBUF gpio_evg_heartbeat_interval_tri_iobuf_9
       (.I(gpio_evg_heartbeat_interval_tri_o_9),
        .IO(gpio_evg_heartbeat_interval_tri_io[9]),
        .O(gpio_evg_heartbeat_interval_tri_i_9),
        .T(gpio_evg_heartbeat_interval_tri_t_9));
  IOBUF gpio_gps_status_tri_iobuf_0
       (.I(gpio_gps_status_tri_o_0),
        .IO(gpio_gps_status_tri_io[0]),
        .O(gpio_gps_status_tri_i_0),
        .T(gpio_gps_status_tri_t_0));
  IOBUF gpio_gps_status_tri_iobuf_1
       (.I(gpio_gps_status_tri_o_1),
        .IO(gpio_gps_status_tri_io[1]),
        .O(gpio_gps_status_tri_i_1),
        .T(gpio_gps_status_tri_t_1));
  IOBUF gpio_gps_status_tri_iobuf_2
       (.I(gpio_gps_status_tri_o_2),
        .IO(gpio_gps_status_tri_io[2]),
        .O(gpio_gps_status_tri_i_2),
        .T(gpio_gps_status_tri_t_2));
  IOBUF gpio_gps_status_tri_iobuf_3
       (.I(gpio_gps_status_tri_o_3),
        .IO(gpio_gps_status_tri_io[3]),
        .O(gpio_gps_status_tri_i_3),
        .T(gpio_gps_status_tri_t_3));
  IOBUF gpio_gps_status_tri_iobuf_4
       (.I(gpio_gps_status_tri_o_4),
        .IO(gpio_gps_status_tri_io[4]),
        .O(gpio_gps_status_tri_i_4),
        .T(gpio_gps_status_tri_t_4));
  IOBUF gpio_gps_status_tri_iobuf_5
       (.I(gpio_gps_status_tri_o_5),
        .IO(gpio_gps_status_tri_io[5]),
        .O(gpio_gps_status_tri_i_5),
        .T(gpio_gps_status_tri_t_5));
  IOBUF gpio_gps_status_tri_iobuf_6
       (.I(gpio_gps_status_tri_o_6),
        .IO(gpio_gps_status_tri_io[6]),
        .O(gpio_gps_status_tri_i_6),
        .T(gpio_gps_status_tri_t_6));
  IOBUF gpio_gps_status_tri_iobuf_7
       (.I(gpio_gps_status_tri_o_7),
        .IO(gpio_gps_status_tri_io[7]),
        .O(gpio_gps_status_tri_i_7),
        .T(gpio_gps_status_tri_t_7));
  IOBUF gpio_gtx_control_tri_iobuf_0
       (.I(gpio_gtx_control_tri_o_0),
        .IO(gpio_gtx_control_tri_io[0]),
        .O(gpio_gtx_control_tri_i_0),
        .T(gpio_gtx_control_tri_t_0));
  IOBUF gpio_gtx_control_tri_iobuf_1
       (.I(gpio_gtx_control_tri_o_1),
        .IO(gpio_gtx_control_tri_io[1]),
        .O(gpio_gtx_control_tri_i_1),
        .T(gpio_gtx_control_tri_t_1));
  IOBUF gpio_gtx_control_tri_iobuf_2
       (.I(gpio_gtx_control_tri_o_2),
        .IO(gpio_gtx_control_tri_io[2]),
        .O(gpio_gtx_control_tri_i_2),
        .T(gpio_gtx_control_tri_t_2));
  IOBUF gpio_gtx_control_tri_iobuf_3
       (.I(gpio_gtx_control_tri_o_3),
        .IO(gpio_gtx_control_tri_io[3]),
        .O(gpio_gtx_control_tri_i_3),
        .T(gpio_gtx_control_tri_t_3));
  IOBUF gpio_gtx_control_tri_iobuf_4
       (.I(gpio_gtx_control_tri_o_4),
        .IO(gpio_gtx_control_tri_io[4]),
        .O(gpio_gtx_control_tri_i_4),
        .T(gpio_gtx_control_tri_t_4));
  IOBUF gpio_gtx_control_tri_iobuf_5
       (.I(gpio_gtx_control_tri_o_5),
        .IO(gpio_gtx_control_tri_io[5]),
        .O(gpio_gtx_control_tri_i_5),
        .T(gpio_gtx_control_tri_t_5));
endmodule
