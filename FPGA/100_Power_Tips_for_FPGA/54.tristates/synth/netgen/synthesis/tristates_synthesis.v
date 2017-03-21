////////////////////////////////////////////////////////////////////////////////
// Copyright (c) 1995-2010 Xilinx, Inc.  All rights reserved.
////////////////////////////////////////////////////////////////////////////////
//   ____  ____
//  /   /\/   /
// /___/  \  /    Vendor: Xilinx
// \   \   \/     Version: M.53d
//  \   \         Application: netgen
//  /   /         Filename: tristates_synthesis.v
// /___/   /\     Timestamp: Fri Jan 28 20:37:51 2011
// \   \  /  \ 
//  \___\/\___\
//             
// Command	: -intstyle ise -insert_glbl true -w -dir netgen/synthesis -ofmt verilog -sim tristates.ngc tristates_synthesis.v 
// Device	: xc6slx9-3-csg225
// Input file	: tristates.ngc
// Output file	: H:\fpga_book\src\54.tristates\synth\netgen\synthesis\tristates_synthesis.v
// # of Modules	: 1
// Design Name	: tristates
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

module tristates (
  clk, reset, d1_en, d2_en, dout, d1_in, d2_in
);
  input clk;
  input reset;
  input d1_en;
  input d2_en;
  output dout;
  input [1 : 0] d1_in;
  input [1 : 0] d2_in;
  wire d1_in_1_IBUF_0;
  wire d1_in_0_IBUF_1;
  wire d2_in_1_IBUF_2;
  wire d2_in_0_IBUF_3;
  wire clk_BUFGP_4;
  wire reset_IBUF_5;
  wire d1_en_IBUF_6;
  wire d2_en_IBUF_7;
  wire dout_OBUF_8;
  wire tri_internal;
  FDC   dout_1 (
    .C(clk_BUFGP_4),
    .CLR(reset_IBUF_5),
    .D(tri_internal),
    .Q(dout_OBUF_8)
  );
  LUT6 #(
    .INIT ( 64'hFF7DFF7DFF7D287D ))
  tri_internalLogicTrst1 (
    .I0(d2_en_IBUF_7),
    .I1(d2_in_0_IBUF_3),
    .I2(d2_in_1_IBUF_2),
    .I3(d1_en_IBUF_6),
    .I4(d1_in_0_IBUF_1),
    .I5(d1_in_1_IBUF_0),
    .O(tri_internal)
  );
  IBUF   d1_in_1_IBUF (
    .I(d1_in[1]),
    .O(d1_in_1_IBUF_0)
  );
  IBUF   d1_in_0_IBUF (
    .I(d1_in[0]),
    .O(d1_in_0_IBUF_1)
  );
  IBUF   d2_in_1_IBUF (
    .I(d2_in[1]),
    .O(d2_in_1_IBUF_2)
  );
  IBUF   d2_in_0_IBUF (
    .I(d2_in[0]),
    .O(d2_in_0_IBUF_3)
  );
  IBUF   reset_IBUF (
    .I(reset),
    .O(reset_IBUF_5)
  );
  IBUF   d1_en_IBUF (
    .I(d1_en),
    .O(d1_en_IBUF_6)
  );
  IBUF   d2_en_IBUF (
    .I(d2_en),
    .O(d2_en_IBUF_7)
  );
  OBUF   dout_OBUF (
    .I(dout_OBUF_8),
    .O(dout)
  );
  BUFGP   clk_BUFGP (
    .I(clk),
    .O(clk_BUFGP_4)
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

