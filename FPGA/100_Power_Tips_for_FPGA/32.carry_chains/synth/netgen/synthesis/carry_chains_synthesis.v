////////////////////////////////////////////////////////////////////////////////
// Copyright (c) 1995-2010 Xilinx, Inc.  All rights reserved.
////////////////////////////////////////////////////////////////////////////////
//   ____  ____
//  /   /\/   /
// /___/  \  /    Vendor: Xilinx
// \   \   \/     Version: M.53d
//  \   \         Application: netgen
//  /   /         Filename: carry_chains_synthesis.v
// /___/   /\     Timestamp: Fri Jan 28 18:22:39 2011
// \   \  /  \ 
//  \___\/\___\
//             
// Command	: -intstyle ise -insert_glbl true -w -dir netgen/synthesis -ofmt verilog -sim carry_chains.ngc carry_chains_synthesis.v 
// Device	: xc6slx9-3-csg225
// Input file	: carry_chains.ngc
// Output file	: G:\src\32.carry_chains\synth\netgen\synthesis\carry_chains_synthesis.v
// # of Modules	: 1
// Design Name	: carry_chains
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

module carry_chains (
  user_out, user_in
);
  output user_out;
  input [11 : 0] user_in;
  wire user_in_11_IBUF_0;
  wire user_in_10_IBUF_1;
  wire user_in_9_IBUF_2;
  wire user_in_8_IBUF_3;
  wire user_in_7_IBUF_4;
  wire user_in_6_IBUF_5;
  wire user_in_5_IBUF_6;
  wire user_in_4_IBUF_7;
  wire user_in_3_IBUF_8;
  wire user_in_2_IBUF_9;
  wire user_in_1_IBUF_10;
  wire user_in_0_IBUF_11;
  wire user_out_OBUF_12;
  wire out;
  wire out1_14;
  LUT6 #(
    .INIT ( 64'hFFFFFFFFFFFFFFFE ))
  out1 (
    .I0(user_in_3_IBUF_8),
    .I1(user_in_2_IBUF_9),
    .I2(user_in_5_IBUF_6),
    .I3(user_in_4_IBUF_7),
    .I4(user_in_7_IBUF_4),
    .I5(user_in_6_IBUF_5),
    .O(out)
  );
  LUT6 #(
    .INIT ( 64'hFFFFFFFFFFFFFFFE ))
  out2 (
    .I0(user_in_9_IBUF_2),
    .I1(user_in_8_IBUF_3),
    .I2(user_in_11_IBUF_0),
    .I3(user_in_10_IBUF_1),
    .I4(user_in_1_IBUF_10),
    .I5(user_in_0_IBUF_11),
    .O(out1_14)
  );
  LUT2 #(
    .INIT ( 4'hE ))
  out3 (
    .I0(out),
    .I1(out1_14),
    .O(user_out_OBUF_12)
  );
  IBUF   user_in_11_IBUF (
    .I(user_in[11]),
    .O(user_in_11_IBUF_0)
  );
  IBUF   user_in_10_IBUF (
    .I(user_in[10]),
    .O(user_in_10_IBUF_1)
  );
  IBUF   user_in_9_IBUF (
    .I(user_in[9]),
    .O(user_in_9_IBUF_2)
  );
  IBUF   user_in_8_IBUF (
    .I(user_in[8]),
    .O(user_in_8_IBUF_3)
  );
  IBUF   user_in_7_IBUF (
    .I(user_in[7]),
    .O(user_in_7_IBUF_4)
  );
  IBUF   user_in_6_IBUF (
    .I(user_in[6]),
    .O(user_in_6_IBUF_5)
  );
  IBUF   user_in_5_IBUF (
    .I(user_in[5]),
    .O(user_in_5_IBUF_6)
  );
  IBUF   user_in_4_IBUF (
    .I(user_in[4]),
    .O(user_in_4_IBUF_7)
  );
  IBUF   user_in_3_IBUF (
    .I(user_in[3]),
    .O(user_in_3_IBUF_8)
  );
  IBUF   user_in_2_IBUF (
    .I(user_in[2]),
    .O(user_in_2_IBUF_9)
  );
  IBUF   user_in_1_IBUF (
    .I(user_in[1]),
    .O(user_in_1_IBUF_10)
  );
  IBUF   user_in_0_IBUF (
    .I(user_in[0]),
    .O(user_in_0_IBUF_11)
  );
  OBUF   user_out_OBUF (
    .I(user_out_OBUF_12),
    .O(user_out)
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

