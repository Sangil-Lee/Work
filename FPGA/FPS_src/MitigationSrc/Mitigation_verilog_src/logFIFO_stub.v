// Copyright 1986-2016 Xilinx, Inc. All Rights Reserved.
// --------------------------------------------------------------------------------
// Tool Version: Vivado v.2016.1 (lin64) Build 1538259 Fri Apr  8 15:45:23 MDT 2016
// Date        : Fri Oct 28 08:19:25 2016
// Host        : localhost.localdomain running 64-bit CentOS Linux release 7.2.1511 (Core)
// Command     : write_verilog -force -mode synth_stub
//               /home/fastprot/src/FastProtect/FastProtect.srcs/sources_1/ip/logFIFO/logFIFO_stub.v
// Design      : logFIFO
// Purpose     : Stub declaration of top-level module interface
// Device      : xc7z045ffg900-2
// --------------------------------------------------------------------------------

// This empty module with port declaration file causes synthesis tools to infer a black box for IP.
// The synthesis directives are for Synopsys Synplify support to prevent IO buffer insertion.
// Please paste the declaration into a Verilog source file or add the file as an additional source.
(* x_core_info = "fifo_generator_v13_1_0,Vivado 2016.1" *)
module logFIFO(rst, wr_clk, rd_clk, din, wr_en, rd_en, dout, full, empty)
/* synthesis syn_black_box black_box_pad_pin="rst,wr_clk,rd_clk,din[214:0],wr_en,rd_en,dout[214:0],full,empty" */;
  input rst;
  input wr_clk;
  input rd_clk;
  input [214:0]din;
  input wr_en;
  input rd_en;
  output [214:0]dout;
  output full;
  output empty;
endmodule
