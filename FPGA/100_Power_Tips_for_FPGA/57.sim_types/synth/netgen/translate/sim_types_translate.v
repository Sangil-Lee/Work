////////////////////////////////////////////////////////////////////////////////
// Copyright (c) 1995-2010 Xilinx, Inc.  All rights reserved.
////////////////////////////////////////////////////////////////////////////////
//   ____  ____
//  /   /\/   /
// /___/  \  /    Vendor: Xilinx
// \   \   \/     Version: M.53d
//  \   \         Application: netgen
//  /   /         Filename: sim_types_translate.v
// /___/   /\     Timestamp: Sat Jan 29 18:20:00 2011
// \   \  /  \ 
//  \___\/\___\
//             
// Command	: -intstyle ise -insert_glbl true -w -dir netgen/translate -ofmt verilog -sim sim_types.ngd sim_types_translate.v 
// Device	: 6slx9csg225-3
// Input file	: sim_types.ngd
// Output file	: H:\fpga_book\src\57.sim_types\synth\netgen\translate\sim_types_translate.v
// # of Modules	: 1
// Design Name	: sim_types
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

module sim_types (
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
  X_LUT6 #(
    .INIT ( 64'hFFFFFFFFFFFFFFFE ))
  out1 (
    .ADR0(user_in_3_IBUF_8),
    .ADR1(user_in_2_IBUF_9),
    .ADR2(user_in_5_IBUF_6),
    .ADR3(user_in_4_IBUF_7),
    .ADR4(user_in_7_IBUF_4),
    .ADR5(user_in_6_IBUF_5),
    .O(out)
  );
  X_LUT6 #(
    .INIT ( 64'hFFFFFFFFFFFFFFFE ))
  out2 (
    .ADR0(user_in_9_IBUF_2),
    .ADR1(user_in_8_IBUF_3),
    .ADR2(user_in_11_IBUF_0),
    .ADR3(user_in_10_IBUF_1),
    .ADR4(user_in_1_IBUF_10),
    .ADR5(user_in_0_IBUF_11),
    .O(out1_14)
  );
  X_LUT2 #(
    .INIT ( 4'hE ))
  out3 (
    .ADR0(out),
    .ADR1(out1_14),
    .O(user_out_OBUF_12)
  );
  X_BUF   user_in_11_IBUF (
    .I(user_in[11]),
    .O(user_in_11_IBUF_0)
  );
  X_BUF   user_in_10_IBUF (
    .I(user_in[10]),
    .O(user_in_10_IBUF_1)
  );
  X_BUF   user_in_9_IBUF (
    .I(user_in[9]),
    .O(user_in_9_IBUF_2)
  );
  X_BUF   user_in_8_IBUF (
    .I(user_in[8]),
    .O(user_in_8_IBUF_3)
  );
  X_BUF   user_in_7_IBUF (
    .I(user_in[7]),
    .O(user_in_7_IBUF_4)
  );
  X_BUF   user_in_6_IBUF (
    .I(user_in[6]),
    .O(user_in_6_IBUF_5)
  );
  X_BUF   user_in_5_IBUF (
    .I(user_in[5]),
    .O(user_in_5_IBUF_6)
  );
  X_BUF   user_in_4_IBUF (
    .I(user_in[4]),
    .O(user_in_4_IBUF_7)
  );
  X_BUF   user_in_3_IBUF (
    .I(user_in[3]),
    .O(user_in_3_IBUF_8)
  );
  X_BUF   user_in_2_IBUF (
    .I(user_in[2]),
    .O(user_in_2_IBUF_9)
  );
  X_BUF   user_in_1_IBUF (
    .I(user_in[1]),
    .O(user_in_1_IBUF_10)
  );
  X_BUF   user_in_0_IBUF (
    .I(user_in[0]),
    .O(user_in_0_IBUF_11)
  );
  X_IPAD   \user_in<11>  (
    .PAD(user_in[11])
  );
  X_IPAD   \user_in<10>  (
    .PAD(user_in[10])
  );
  X_IPAD   \user_in<9>  (
    .PAD(user_in[9])
  );
  X_IPAD   \user_in<8>  (
    .PAD(user_in[8])
  );
  X_IPAD   \user_in<7>  (
    .PAD(user_in[7])
  );
  X_IPAD   \user_in<6>  (
    .PAD(user_in[6])
  );
  X_IPAD   \user_in<5>  (
    .PAD(user_in[5])
  );
  X_IPAD   \user_in<4>  (
    .PAD(user_in[4])
  );
  X_IPAD   \user_in<3>  (
    .PAD(user_in[3])
  );
  X_IPAD   \user_in<2>  (
    .PAD(user_in[2])
  );
  X_IPAD   \user_in<1>  (
    .PAD(user_in[1])
  );
  X_IPAD   \user_in<0>  (
    .PAD(user_in[0])
  );
  X_OPAD   user_out_29 (
    .PAD(user_out)
  );
  X_OBUF   user_out_OBUF (
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

