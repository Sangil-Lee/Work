////////////////////////////////////////////////////////////////////////////////
// Copyright (c) 1995-2010 Xilinx, Inc.  All rights reserved.
////////////////////////////////////////////////////////////////////////////////
//   ____  ____
//  /   /\/   /
// /___/  \  /    Vendor: Xilinx
// \   \   \/     Version: M.53d
//  \   \         Application: netgen
//  /   /         Filename: signed_arithmetic_synthesis.v
// /___/   /\     Timestamp: Tue Feb 01 19:29:02 2011
// \   \  /  \ 
//  \___\/\___\
//             
// Command	: -intstyle ise -insert_glbl true -w -dir netgen/synthesis -ofmt verilog -sim signed_arithmetic.ngc signed_arithmetic_synthesis.v 
// Device	: xc6slx25-3-csg324
// Input file	: signed_arithmetic.ngc
// Output file	: h:\26.signed\synth\netgen\synthesis\signed_arithmetic_synthesis.v
// # of Modules	: 3
// Design Name	: signed_arithmetic
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

module unsigned_shift_unsigned_shift (
  clk, reset, shift_unsigned
);
  input clk;
  input reset;
  output [15 : 0] shift_unsigned;
  wire N1;
  wire [15 : 1] NlwRenamedSig_OI_shift_unsigned;
  assign
    shift_unsigned[15] = NlwRenamedSig_OI_shift_unsigned[15],
    shift_unsigned[14] = NlwRenamedSig_OI_shift_unsigned[14],
    shift_unsigned[13] = NlwRenamedSig_OI_shift_unsigned[13],
    shift_unsigned[12] = NlwRenamedSig_OI_shift_unsigned[12],
    shift_unsigned[11] = NlwRenamedSig_OI_shift_unsigned[11],
    shift_unsigned[10] = NlwRenamedSig_OI_shift_unsigned[10],
    shift_unsigned[9] = NlwRenamedSig_OI_shift_unsigned[9],
    shift_unsigned[8] = NlwRenamedSig_OI_shift_unsigned[8],
    shift_unsigned[7] = NlwRenamedSig_OI_shift_unsigned[7],
    shift_unsigned[6] = NlwRenamedSig_OI_shift_unsigned[6],
    shift_unsigned[5] = NlwRenamedSig_OI_shift_unsigned[5],
    shift_unsigned[4] = NlwRenamedSig_OI_shift_unsigned[4],
    shift_unsigned[3] = NlwRenamedSig_OI_shift_unsigned[3],
    shift_unsigned[2] = NlwRenamedSig_OI_shift_unsigned[2],
    shift_unsigned[1] = NlwRenamedSig_OI_shift_unsigned[1];
  GND   XST_GND (
    .G(N1)
  );
  FDR   shift_unsigned_0 (
    .C(clk),
    .D(NlwRenamedSig_OI_shift_unsigned[1]),
    .R(reset),
    .Q(shift_unsigned[0])
  );
  FDR   shift_unsigned_1 (
    .C(clk),
    .D(NlwRenamedSig_OI_shift_unsigned[2]),
    .R(reset),
    .Q(NlwRenamedSig_OI_shift_unsigned[1])
  );
  FDR   shift_unsigned_2 (
    .C(clk),
    .D(NlwRenamedSig_OI_shift_unsigned[3]),
    .R(reset),
    .Q(NlwRenamedSig_OI_shift_unsigned[2])
  );
  FDR   shift_unsigned_3 (
    .C(clk),
    .D(NlwRenamedSig_OI_shift_unsigned[4]),
    .R(reset),
    .Q(NlwRenamedSig_OI_shift_unsigned[3])
  );
  FDR   shift_unsigned_4 (
    .C(clk),
    .D(NlwRenamedSig_OI_shift_unsigned[5]),
    .R(reset),
    .Q(NlwRenamedSig_OI_shift_unsigned[4])
  );
  FDR   shift_unsigned_5 (
    .C(clk),
    .D(NlwRenamedSig_OI_shift_unsigned[6]),
    .R(reset),
    .Q(NlwRenamedSig_OI_shift_unsigned[5])
  );
  FDR   shift_unsigned_6 (
    .C(clk),
    .D(NlwRenamedSig_OI_shift_unsigned[7]),
    .R(reset),
    .Q(NlwRenamedSig_OI_shift_unsigned[6])
  );
  FDR   shift_unsigned_7 (
    .C(clk),
    .D(NlwRenamedSig_OI_shift_unsigned[8]),
    .R(reset),
    .Q(NlwRenamedSig_OI_shift_unsigned[7])
  );
  FDR   shift_unsigned_8 (
    .C(clk),
    .D(NlwRenamedSig_OI_shift_unsigned[9]),
    .R(reset),
    .Q(NlwRenamedSig_OI_shift_unsigned[8])
  );
  FDR   shift_unsigned_9 (
    .C(clk),
    .D(NlwRenamedSig_OI_shift_unsigned[10]),
    .R(reset),
    .Q(NlwRenamedSig_OI_shift_unsigned[9])
  );
  FDR   shift_unsigned_10 (
    .C(clk),
    .D(NlwRenamedSig_OI_shift_unsigned[11]),
    .R(reset),
    .Q(NlwRenamedSig_OI_shift_unsigned[10])
  );
  FDR   shift_unsigned_11 (
    .C(clk),
    .D(NlwRenamedSig_OI_shift_unsigned[12]),
    .R(reset),
    .Q(NlwRenamedSig_OI_shift_unsigned[11])
  );
  FDR   shift_unsigned_12 (
    .C(clk),
    .D(NlwRenamedSig_OI_shift_unsigned[13]),
    .R(reset),
    .Q(NlwRenamedSig_OI_shift_unsigned[12])
  );
  FDR   shift_unsigned_13 (
    .C(clk),
    .D(NlwRenamedSig_OI_shift_unsigned[14]),
    .R(reset),
    .Q(NlwRenamedSig_OI_shift_unsigned[13])
  );
  FDR   shift_unsigned_14 (
    .C(clk),
    .D(NlwRenamedSig_OI_shift_unsigned[15]),
    .R(reset),
    .Q(NlwRenamedSig_OI_shift_unsigned[14])
  );
  FDS   shift_unsigned_15 (
    .C(clk),
    .D(N1),
    .S(reset),
    .Q(NlwRenamedSig_OI_shift_unsigned[15])
  );
endmodule

module signed_shift_signed_shift (
  clk, reset, shift_signed
);
  input clk;
  input reset;
  output [15 : 0] shift_signed;
  wire [15 : 1] NlwRenamedSig_OI_shift_signed;
  assign
    shift_signed[15] = NlwRenamedSig_OI_shift_signed[15],
    shift_signed[14] = NlwRenamedSig_OI_shift_signed[14],
    shift_signed[13] = NlwRenamedSig_OI_shift_signed[13],
    shift_signed[12] = NlwRenamedSig_OI_shift_signed[12],
    shift_signed[11] = NlwRenamedSig_OI_shift_signed[11],
    shift_signed[10] = NlwRenamedSig_OI_shift_signed[10],
    shift_signed[9] = NlwRenamedSig_OI_shift_signed[9],
    shift_signed[8] = NlwRenamedSig_OI_shift_signed[8],
    shift_signed[7] = NlwRenamedSig_OI_shift_signed[7],
    shift_signed[6] = NlwRenamedSig_OI_shift_signed[6],
    shift_signed[5] = NlwRenamedSig_OI_shift_signed[5],
    shift_signed[4] = NlwRenamedSig_OI_shift_signed[4],
    shift_signed[3] = NlwRenamedSig_OI_shift_signed[3],
    shift_signed[2] = NlwRenamedSig_OI_shift_signed[2],
    shift_signed[1] = NlwRenamedSig_OI_shift_signed[1];
  VCC   XST_VCC (
    .P(NlwRenamedSig_OI_shift_signed[15])
  );
  FDR   shift_signed_14 (
    .C(clk),
    .D(NlwRenamedSig_OI_shift_signed[15]),
    .R(reset),
    .Q(NlwRenamedSig_OI_shift_signed[14])
  );
  FDR   shift_signed_13 (
    .C(clk),
    .D(NlwRenamedSig_OI_shift_signed[14]),
    .R(reset),
    .Q(NlwRenamedSig_OI_shift_signed[13])
  );
  FDR   shift_signed_12 (
    .C(clk),
    .D(NlwRenamedSig_OI_shift_signed[13]),
    .R(reset),
    .Q(NlwRenamedSig_OI_shift_signed[12])
  );
  FDR   shift_signed_11 (
    .C(clk),
    .D(NlwRenamedSig_OI_shift_signed[12]),
    .R(reset),
    .Q(NlwRenamedSig_OI_shift_signed[11])
  );
  FDR   shift_signed_10 (
    .C(clk),
    .D(NlwRenamedSig_OI_shift_signed[11]),
    .R(reset),
    .Q(NlwRenamedSig_OI_shift_signed[10])
  );
  FDR   shift_signed_9 (
    .C(clk),
    .D(NlwRenamedSig_OI_shift_signed[10]),
    .R(reset),
    .Q(NlwRenamedSig_OI_shift_signed[9])
  );
  FDR   shift_signed_8 (
    .C(clk),
    .D(NlwRenamedSig_OI_shift_signed[9]),
    .R(reset),
    .Q(NlwRenamedSig_OI_shift_signed[8])
  );
  FDR   shift_signed_7 (
    .C(clk),
    .D(NlwRenamedSig_OI_shift_signed[8]),
    .R(reset),
    .Q(NlwRenamedSig_OI_shift_signed[7])
  );
  FDR   shift_signed_6 (
    .C(clk),
    .D(NlwRenamedSig_OI_shift_signed[7]),
    .R(reset),
    .Q(NlwRenamedSig_OI_shift_signed[6])
  );
  FDR   shift_signed_5 (
    .C(clk),
    .D(NlwRenamedSig_OI_shift_signed[6]),
    .R(reset),
    .Q(NlwRenamedSig_OI_shift_signed[5])
  );
  FDR   shift_signed_4 (
    .C(clk),
    .D(NlwRenamedSig_OI_shift_signed[5]),
    .R(reset),
    .Q(NlwRenamedSig_OI_shift_signed[4])
  );
  FDR   shift_signed_3 (
    .C(clk),
    .D(NlwRenamedSig_OI_shift_signed[4]),
    .R(reset),
    .Q(NlwRenamedSig_OI_shift_signed[3])
  );
  FDR   shift_signed_2 (
    .C(clk),
    .D(NlwRenamedSig_OI_shift_signed[3]),
    .R(reset),
    .Q(NlwRenamedSig_OI_shift_signed[2])
  );
  FDR   shift_signed_1 (
    .C(clk),
    .D(NlwRenamedSig_OI_shift_signed[2]),
    .R(reset),
    .Q(NlwRenamedSig_OI_shift_signed[1])
  );
  FDR   shift_signed_0 (
    .C(clk),
    .D(NlwRenamedSig_OI_shift_signed[1]),
    .R(reset),
    .Q(shift_signed[0])
  );
endmodule

module signed_arithmetic (
  clk, reset, shift_signed, shift_unsigned
);
  input clk;
  input reset;
  output [15 : 0] shift_signed;
  output [15 : 0] shift_unsigned;
  wire clk_BUFGP_37;
  wire reset_IBUF_38;
  wire shift_signed_15_OBUF_39;
  wire shift_signed_14_OBUF_40;
  wire shift_signed_13_OBUF_41;
  wire shift_signed_12_OBUF_42;
  wire shift_signed_11_OBUF_43;
  wire shift_signed_10_OBUF_44;
  wire shift_signed_9_OBUF_45;
  wire shift_signed_8_OBUF_46;
  wire shift_signed_7_OBUF_47;
  wire shift_signed_6_OBUF_48;
  wire shift_signed_5_OBUF_49;
  wire shift_signed_4_OBUF_50;
  wire shift_signed_3_OBUF_51;
  wire shift_signed_2_OBUF_52;
  wire shift_signed_1_OBUF_53;
  wire shift_signed_0_OBUF_54;
  wire shift_unsigned_15_OBUF_55;
  wire shift_unsigned_14_OBUF_56;
  wire shift_unsigned_13_OBUF_57;
  wire shift_unsigned_12_OBUF_58;
  wire shift_unsigned_11_OBUF_59;
  wire shift_unsigned_10_OBUF_60;
  wire shift_unsigned_9_OBUF_61;
  wire shift_unsigned_8_OBUF_62;
  wire shift_unsigned_7_OBUF_63;
  wire shift_unsigned_6_OBUF_64;
  wire shift_unsigned_5_OBUF_65;
  wire shift_unsigned_4_OBUF_66;
  wire shift_unsigned_3_OBUF_67;
  wire shift_unsigned_2_OBUF_68;
  wire shift_unsigned_1_OBUF_69;
  wire shift_unsigned_0_OBUF_70;
  signed_shift_signed_shift   signed_shift (
    .clk(clk_BUFGP_37),
    .reset(reset_IBUF_38),
    .shift_signed({shift_signed_15_OBUF_39, shift_signed_14_OBUF_40, shift_signed_13_OBUF_41, shift_signed_12_OBUF_42, shift_signed_11_OBUF_43, 
shift_signed_10_OBUF_44, shift_signed_9_OBUF_45, shift_signed_8_OBUF_46, shift_signed_7_OBUF_47, shift_signed_6_OBUF_48, shift_signed_5_OBUF_49, 
shift_signed_4_OBUF_50, shift_signed_3_OBUF_51, shift_signed_2_OBUF_52, shift_signed_1_OBUF_53, shift_signed_0_OBUF_54})
  );
  unsigned_shift_unsigned_shift   unsigned_shift (
    .clk(clk_BUFGP_37),
    .reset(reset_IBUF_38),
    .shift_unsigned({shift_unsigned_15_OBUF_55, shift_unsigned_14_OBUF_56, shift_unsigned_13_OBUF_57, shift_unsigned_12_OBUF_58, 
shift_unsigned_11_OBUF_59, shift_unsigned_10_OBUF_60, shift_unsigned_9_OBUF_61, shift_unsigned_8_OBUF_62, shift_unsigned_7_OBUF_63, 
shift_unsigned_6_OBUF_64, shift_unsigned_5_OBUF_65, shift_unsigned_4_OBUF_66, shift_unsigned_3_OBUF_67, shift_unsigned_2_OBUF_68, 
shift_unsigned_1_OBUF_69, shift_unsigned_0_OBUF_70})
  );
  IBUF   reset_IBUF (
    .I(reset),
    .O(reset_IBUF_38)
  );
  OBUF   shift_signed_15_OBUF (
    .I(shift_signed_15_OBUF_39),
    .O(shift_signed[15])
  );
  OBUF   shift_signed_14_OBUF (
    .I(shift_signed_14_OBUF_40),
    .O(shift_signed[14])
  );
  OBUF   shift_signed_13_OBUF (
    .I(shift_signed_13_OBUF_41),
    .O(shift_signed[13])
  );
  OBUF   shift_signed_12_OBUF (
    .I(shift_signed_12_OBUF_42),
    .O(shift_signed[12])
  );
  OBUF   shift_signed_11_OBUF (
    .I(shift_signed_11_OBUF_43),
    .O(shift_signed[11])
  );
  OBUF   shift_signed_10_OBUF (
    .I(shift_signed_10_OBUF_44),
    .O(shift_signed[10])
  );
  OBUF   shift_signed_9_OBUF (
    .I(shift_signed_9_OBUF_45),
    .O(shift_signed[9])
  );
  OBUF   shift_signed_8_OBUF (
    .I(shift_signed_8_OBUF_46),
    .O(shift_signed[8])
  );
  OBUF   shift_signed_7_OBUF (
    .I(shift_signed_7_OBUF_47),
    .O(shift_signed[7])
  );
  OBUF   shift_signed_6_OBUF (
    .I(shift_signed_6_OBUF_48),
    .O(shift_signed[6])
  );
  OBUF   shift_signed_5_OBUF (
    .I(shift_signed_5_OBUF_49),
    .O(shift_signed[5])
  );
  OBUF   shift_signed_4_OBUF (
    .I(shift_signed_4_OBUF_50),
    .O(shift_signed[4])
  );
  OBUF   shift_signed_3_OBUF (
    .I(shift_signed_3_OBUF_51),
    .O(shift_signed[3])
  );
  OBUF   shift_signed_2_OBUF (
    .I(shift_signed_2_OBUF_52),
    .O(shift_signed[2])
  );
  OBUF   shift_signed_1_OBUF (
    .I(shift_signed_1_OBUF_53),
    .O(shift_signed[1])
  );
  OBUF   shift_signed_0_OBUF (
    .I(shift_signed_0_OBUF_54),
    .O(shift_signed[0])
  );
  OBUF   shift_unsigned_15_OBUF (
    .I(shift_unsigned_15_OBUF_55),
    .O(shift_unsigned[15])
  );
  OBUF   shift_unsigned_14_OBUF (
    .I(shift_unsigned_14_OBUF_56),
    .O(shift_unsigned[14])
  );
  OBUF   shift_unsigned_13_OBUF (
    .I(shift_unsigned_13_OBUF_57),
    .O(shift_unsigned[13])
  );
  OBUF   shift_unsigned_12_OBUF (
    .I(shift_unsigned_12_OBUF_58),
    .O(shift_unsigned[12])
  );
  OBUF   shift_unsigned_11_OBUF (
    .I(shift_unsigned_11_OBUF_59),
    .O(shift_unsigned[11])
  );
  OBUF   shift_unsigned_10_OBUF (
    .I(shift_unsigned_10_OBUF_60),
    .O(shift_unsigned[10])
  );
  OBUF   shift_unsigned_9_OBUF (
    .I(shift_unsigned_9_OBUF_61),
    .O(shift_unsigned[9])
  );
  OBUF   shift_unsigned_8_OBUF (
    .I(shift_unsigned_8_OBUF_62),
    .O(shift_unsigned[8])
  );
  OBUF   shift_unsigned_7_OBUF (
    .I(shift_unsigned_7_OBUF_63),
    .O(shift_unsigned[7])
  );
  OBUF   shift_unsigned_6_OBUF (
    .I(shift_unsigned_6_OBUF_64),
    .O(shift_unsigned[6])
  );
  OBUF   shift_unsigned_5_OBUF (
    .I(shift_unsigned_5_OBUF_65),
    .O(shift_unsigned[5])
  );
  OBUF   shift_unsigned_4_OBUF (
    .I(shift_unsigned_4_OBUF_66),
    .O(shift_unsigned[4])
  );
  OBUF   shift_unsigned_3_OBUF (
    .I(shift_unsigned_3_OBUF_67),
    .O(shift_unsigned[3])
  );
  OBUF   shift_unsigned_2_OBUF (
    .I(shift_unsigned_2_OBUF_68),
    .O(shift_unsigned[2])
  );
  OBUF   shift_unsigned_1_OBUF (
    .I(shift_unsigned_1_OBUF_69),
    .O(shift_unsigned[1])
  );
  OBUF   shift_unsigned_0_OBUF (
    .I(shift_unsigned_0_OBUF_70),
    .O(shift_unsigned[0])
  );
  BUFGP   clk_BUFGP (
    .I(clk),
    .O(clk_BUFGP_37)
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

