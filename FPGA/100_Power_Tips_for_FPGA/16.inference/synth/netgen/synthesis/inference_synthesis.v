////////////////////////////////////////////////////////////////////////////////
// Copyright (c) 1995-2010 Xilinx, Inc.  All rights reserved.
////////////////////////////////////////////////////////////////////////////////
//   ____  ____
//  /   /\/   /
// /___/  \  /    Vendor: Xilinx
// \   \   \/     Version: M.53d
//  \   \         Application: netgen
//  /   /         Filename: inference_synthesis.v
// /___/   /\     Timestamp: Fri Mar 11 10:37:18 2011
// \   \  /  \ 
//  \___\/\___\
//             
// Command	: -intstyle ise -insert_glbl true -w -dir netgen/synthesis -ofmt verilog -sim inference.ngc inference_synthesis.v 
// Device	: xc6slx45-3-fgg676
// Input file	: inference.ngc
// Output file	: G:\fpga_book\src\16.inference\synth\netgen\synthesis\inference_synthesis.v
// # of Modules	: 1
// Design Name	: inference
// Xilinx        : C:\Xilinx\12.1\ISE_DS\ISE
//             
// Purpose:    
//     This verilog netlist is a verification model and uses simulation 
//     primitives which may not represent the true implementation of the 
//     device, however the netlist is functionally correct and should not 
//     be modified. This file cannot be synthesized and should only be used 
//     with supported simulation tools.
//             
// Reference:  
//     Command Line Tools User Guide, Chapter 23 and Synthesis and Simulation Design Guide, Chapter 6
//             
////////////////////////////////////////////////////////////////////////////////

`timescale 1 ns/1 ps

module inference (
  clk, areset, sreset, d_in, d_out
);
  input clk;
  input areset;
  input sreset;
  input [4 : 0] d_in;
  output [4 : 0] d_out;
  wire d_in_1_IBUF_0;
  wire d_in_2_IBUF_1;
  wire d_in_3_IBUF_2;
  wire d_in_0_IBUF_3;
  wire d_in_4_IBUF_4;
  wire clk_BUFGP_5;
  wire areset_IBUF_6;
  wire sreset_IBUF_BUFG_7;
  wire d_out_0_8;
  wire d_out_1_9;
  wire d_out_2_10;
  wire d_out_4;
  wire d_out_3_12;
  wire areset_sreset_AND_2_o;
  wire N1;
  wire areset_inv;
  wire d_out_3_rstpot_29;
  wire d_out_4_LDC_30;
  wire d_out_4_C_4_31;
  wire d_out_4_P_4_32;
  wire sreset_IBUF_33;
  VCC   XST_VCC (
    .P(N1)
  );
  
    ============================
  FD_1   d_out_2 (
    .C(clk_BUFGP_5),
    .D(d_in_2_IBUF_1),
    .Q(d_out_2_10)
    
      );
  
    ============================
  FDC   d_out_0 (
    .C(clk_BUFGP_5),
    .CLR(areset_IBUF_6),
    .D(d_in_0_IBUF_3),
    .Q(d_out_0_8)
  );
  
    ============================
  FDC_1   d_out_1 (
    .C(clk_BUFGP_5),
    .CLR(areset_inv),
    .D(d_in_1_IBUF_0),
    .Q(d_out_1_9)
  );
  LUT2 #(
    .INIT ( 4'h2 ))
  areset_sreset_AND_2_o1 (
    .I0(areset_IBUF_6),
    .I1(sreset_IBUF_33),
    .O(areset_sreset_AND_2_o)
  );
  
  
  IBUF   d_in_4_IBUF (
    .I(d_in[4]),
    .O(d_in_4_IBUF_4)
  );
  IBUF   d_in_3_IBUF (
    .I(d_in[3]),
    .O(d_in_3_IBUF_2)
  );
  IBUF   d_in_2_IBUF (
    .I(d_in[2]),
    .O(d_in_2_IBUF_1)
  );
  IBUF   d_in_1_IBUF (
    .I(d_in[1]),
    .O(d_in_1_IBUF_0)
  );
  IBUF   d_in_0_IBUF (
    .I(d_in[0]),
    .O(d_in_0_IBUF_3)
  );
  IBUF   areset_IBUF (
    .I(areset),
    .O(areset_IBUF_6)
  );
  IBUF   sreset_IBUF (
    .I(sreset),
    .O(sreset_IBUF_33)
  );
  OBUF   d_out_4_OBUF (
    .I(d_out_4),
    .O(d_out[4])
  );
  OBUF   d_out_3_OBUF (
    .I(d_out_3_12),
    .O(d_out[3])
  );
  OBUF   d_out_2_OBUF (
    .I(d_out_2_10),
    .O(d_out[2])
  );
  OBUF   d_out_1_OBUF (
    .I(d_out_1_9),
    .O(d_out[1])
  );
  OBUF   d_out_0_OBUF (
    .I(d_out_0_8),
    .O(d_out[0])
  );
  
  
  LUT2 #(
    .INIT ( 4'hE ))
  d_out_3_rstpot (
    .I0(d_in_3_IBUF_2),
    .I1(sreset_IBUF_33),
    .O(d_out_3_rstpot_29)
  );
  
  
  ============================
  FD   d_out_3 (
    .C(clk_BUFGP_5),
    .D(d_out_3_rstpot_29),
    .Q(d_out_3_12)
  );
  
  
  
  LDC   d_out_4_LDC (
    .CLR(areset_sreset_AND_2_o),
    .D(N1),
    .G(sreset_IBUF_BUFG_7),
    .Q(d_out_4_LDC_30)
  );
  
  ============================  
  FDC   d_out_4_C_4 (
    .C(clk_BUFGP_5),
    .CLR(areset_sreset_AND_2_o),
    .D(d_in_4_IBUF_4),
    .Q(d_out_4_C_4_31)
  );
  
    ============================
  FDP   d_out_4_P_4 (
    .C(clk_BUFGP_5),
    .D(d_in_4_IBUF_4),
    .PRE(sreset_IBUF_33),
    .Q(d_out_4_P_4_32)
  );
  LUT3 #(
    .INIT ( 8'hD8 ))
  d_out_41 (
    .I0(d_out_4_LDC_30),
    .I1(d_out_4_P_4_32),
    .I2(d_out_4_C_4_31),
    .O(d_out_4)
  );
  BUFGP   clk_BUFGP (
    .I(clk),
    .O(clk_BUFGP_5)
  );
  BUFG   sreset_IBUF_BUFG (
    .O(sreset_IBUF_BUFG_7),
    .I(sreset_IBUF_33)
  );
  INV   areset_inv1_INV_0 (
    .I(areset_IBUF_6),
    .O(areset_inv)
  );
endmodule


`ifndef GLBL
`define GLBL

`timescale  1 ps / 1 ps

module glbl ();

    parameter ROC_WIDTH = 100000;
    parameter TOC_WIDTH = 0;

    wire GSR;
    wire GTS;
    wire GWE;
    wire PRLD;
    tri1 p_up_tmp;
    tri (weak1, strong0) PLL_LOCKG = p_up_tmp;

    reg GSR_int;
    reg GTS_int;
    reg PRLD_int;

//--------   JTAG Globals --------------
    wire JTAG_TDO_GLBL;
    wire JTAG_TCK_GLBL;
    wire JTAG_TDI_GLBL;
    wire JTAG_TMS_GLBL;
    wire JTAG_TRST_GLBL;

    reg JTAG_CAPTURE_GLBL;
    reg JTAG_RESET_GLBL;
    reg JTAG_SHIFT_GLBL;
    reg JTAG_UPDATE_GLBL;
    reg JTAG_RUNTEST_GLBL;

    reg JTAG_SEL1_GLBL = 0;
    reg JTAG_SEL2_GLBL = 0 ;
    reg JTAG_SEL3_GLBL = 0;
    reg JTAG_SEL4_GLBL = 0;

    reg JTAG_USER_TDO1_GLBL = 1'bz;
    reg JTAG_USER_TDO2_GLBL = 1'bz;
    reg JTAG_USER_TDO3_GLBL = 1'bz;
    reg JTAG_USER_TDO4_GLBL = 1'bz;

    assign (weak1, weak0) GSR = GSR_int;
    assign (weak1, weak0) GTS = GTS_int;
    assign (weak1, weak0) PRLD = PRLD_int;

    initial begin
	GSR_int = 1'b1;
	PRLD_int = 1'b1;
	#(ROC_WIDTH)
	GSR_int = 1'b0;
	PRLD_int = 1'b0;
    end

    initial begin
	GTS_int = 1'b1;
	#(TOC_WIDTH)
	GTS_int = 1'b0;
    end

endmodule

`endif

