////////////////////////////////////////////////////////////////////////////////
// Copyright (c) 1995-2010 Xilinx, Inc.  All rights reserved.
////////////////////////////////////////////////////////////////////////////////
//   ____  ____
//  /   /\/   /
// /___/  \  /    Vendor: Xilinx
// \   \   \/     Version: M.53d
//  \   \         Application: netgen
//  /   /         Filename: carry_chains_timesim.v
// /___/   /\     Timestamp: Fri Jan 28 17:54:44 2011
// \   \  /  \ 
//  \___\/\___\
//             
// Command	: -intstyle ise -s 3 -pcf carry_chains.pcf -sdf_anno true -sdf_path netgen/par -insert_glbl true -insert_pp_buffers true -w -dir netgen/par -ofmt verilog -sim carry_chains.ncd carry_chains_timesim.v 
// Device	: 6slx9csg225-3 (ADVANCED 1.08 2010-04-09)
// Input file	: carry_chains.ncd
// Output file	: G:\src\32.carry_chains\synth\netgen\par\carry_chains_timesim.v
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
  wire user_in_0_IBUF_0;
  wire user_in_1_IBUF_0;
  wire user_in_2_IBUF_0;
  wire user_in_3_IBUF_0;
  wire user_in_4_IBUF_0;
  wire user_in_5_IBUF_0;
  wire user_in_6_IBUF_0;
  wire user_in_7_IBUF_0;
  wire user_in_8_IBUF_0;
  wire user_in_9_IBUF_0;
  wire user_out_OBUF_78;
  wire user_in_10_IBUF_0;
  wire user_in_11_IBUF_0;
  wire out;
  wire out1_82;
  wire user_in_0_IBUF_1;
  wire user_in_1_IBUF_4;
  wire user_in_2_IBUF_7;
  wire user_in_3_IBUF_10;
  wire user_in_4_IBUF_13;
  wire user_in_5_IBUF_16;
  wire user_in_6_IBUF_19;
  wire user_in_7_IBUF_22;
  wire user_in_8_IBUF_25;
  wire user_in_9_IBUF_28;
  wire user_in_10_IBUF_33;
  wire user_in_11_IBUF_36;
  wire \NlwBufferSignal_user_out_OBUF/I ;
  wire \NlwBufferSignal_out2/ADR4 ;
  wire \NlwBufferSignal_out2/ADR2 ;
  wire \NlwBufferSignal_out2/ADR0 ;
  wire \NlwBufferSignal_out2/ADR1 ;
  wire \NlwBufferSignal_out2/ADR5 ;
  wire \NlwBufferSignal_out2/ADR3 ;
  wire \NlwBufferSignal_out3/ADR5 ;
  wire \NlwBufferSignal_out1/ADR3 ;
  wire \NlwBufferSignal_out1/ADR4 ;
  wire \NlwBufferSignal_out1/ADR1 ;
  wire \NlwBufferSignal_out1/ADR0 ;
  wire \NlwBufferSignal_out1/ADR2 ;
  wire \NlwBufferSignal_out1/ADR5 ;
  initial $sdf_annotate("netgen/par/carry_chains_timesim.sdf");
  X_IPAD #(
    .LOC ( "PAD121" ))
  \user_in<0>  (
    .PAD(user_in[0])
  );
  X_BUF #(
    .LOC ( "PAD121" ))
  user_in_0_IBUF (
    .O(user_in_0_IBUF_1),
    .I(user_in[0])
  );
  X_BUF #(
    .LOC ( "PAD121" ))
  \ProtoComp0.IMUX  (
    .I(user_in_0_IBUF_1),
    .O(user_in_0_IBUF_0)
  );
  X_IPAD #(
    .LOC ( "PAD122" ))
  \user_in<1>  (
    .PAD(user_in[1])
  );
  X_BUF #(
    .LOC ( "PAD122" ))
  user_in_1_IBUF (
    .O(user_in_1_IBUF_4),
    .I(user_in[1])
  );
  X_BUF #(
    .LOC ( "PAD122" ))
  \ProtoComp0.IMUX.1  (
    .I(user_in_1_IBUF_4),
    .O(user_in_1_IBUF_0)
  );
  X_IPAD #(
    .LOC ( "PAD123" ))
  \user_in<2>  (
    .PAD(user_in[2])
  );
  X_BUF #(
    .LOC ( "PAD123" ))
  user_in_2_IBUF (
    .O(user_in_2_IBUF_7),
    .I(user_in[2])
  );
  X_BUF #(
    .LOC ( "PAD123" ))
  \ProtoComp0.IMUX.2  (
    .I(user_in_2_IBUF_7),
    .O(user_in_2_IBUF_0)
  );
  X_IPAD #(
    .LOC ( "PAD124" ))
  \user_in<3>  (
    .PAD(user_in[3])
  );
  X_BUF #(
    .LOC ( "PAD124" ))
  user_in_3_IBUF (
    .O(user_in_3_IBUF_10),
    .I(user_in[3])
  );
  X_BUF #(
    .LOC ( "PAD124" ))
  \ProtoComp0.IMUX.3  (
    .I(user_in_3_IBUF_10),
    .O(user_in_3_IBUF_0)
  );
  X_IPAD #(
    .LOC ( "PAD131" ))
  \user_in<4>  (
    .PAD(user_in[4])
  );
  X_BUF #(
    .LOC ( "PAD131" ))
  user_in_4_IBUF (
    .O(user_in_4_IBUF_13),
    .I(user_in[4])
  );
  X_BUF #(
    .LOC ( "PAD131" ))
  \ProtoComp0.IMUX.4  (
    .I(user_in_4_IBUF_13),
    .O(user_in_4_IBUF_0)
  );
  X_IPAD #(
    .LOC ( "PAD132" ))
  \user_in<5>  (
    .PAD(user_in[5])
  );
  X_BUF #(
    .LOC ( "PAD132" ))
  user_in_5_IBUF (
    .O(user_in_5_IBUF_16),
    .I(user_in[5])
  );
  X_BUF #(
    .LOC ( "PAD132" ))
  \ProtoComp0.IMUX.5  (
    .I(user_in_5_IBUF_16),
    .O(user_in_5_IBUF_0)
  );
  X_IPAD #(
    .LOC ( "PAD133" ))
  \user_in<6>  (
    .PAD(user_in[6])
  );
  X_BUF #(
    .LOC ( "PAD133" ))
  user_in_6_IBUF (
    .O(user_in_6_IBUF_19),
    .I(user_in[6])
  );
  X_BUF #(
    .LOC ( "PAD133" ))
  \ProtoComp0.IMUX.6  (
    .I(user_in_6_IBUF_19),
    .O(user_in_6_IBUF_0)
  );
  X_IPAD #(
    .LOC ( "PAD134" ))
  \user_in<7>  (
    .PAD(user_in[7])
  );
  X_BUF #(
    .LOC ( "PAD134" ))
  user_in_7_IBUF (
    .O(user_in_7_IBUF_22),
    .I(user_in[7])
  );
  X_BUF #(
    .LOC ( "PAD134" ))
  \ProtoComp0.IMUX.7  (
    .I(user_in_7_IBUF_22),
    .O(user_in_7_IBUF_0)
  );
  X_IPAD #(
    .LOC ( "PAD135" ))
  \user_in<8>  (
    .PAD(user_in[8])
  );
  X_BUF #(
    .LOC ( "PAD135" ))
  user_in_8_IBUF (
    .O(user_in_8_IBUF_25),
    .I(user_in[8])
  );
  X_BUF #(
    .LOC ( "PAD135" ))
  \ProtoComp0.IMUX.8  (
    .I(user_in_8_IBUF_25),
    .O(user_in_8_IBUF_0)
  );
  X_IPAD #(
    .LOC ( "PAD136" ))
  \user_in<9>  (
    .PAD(user_in[9])
  );
  X_BUF #(
    .LOC ( "PAD136" ))
  user_in_9_IBUF (
    .O(user_in_9_IBUF_28),
    .I(user_in[9])
  );
  X_BUF #(
    .LOC ( "PAD136" ))
  \ProtoComp0.IMUX.9  (
    .I(user_in_9_IBUF_28),
    .O(user_in_9_IBUF_0)
  );
  X_OPAD #(
    .LOC ( "PAD139" ))
  user_out_43 (
    .PAD(user_out)
  );
  X_OBUF #(
    .LOC ( "PAD139" ))
  user_out_OBUF (
    .I(\NlwBufferSignal_user_out_OBUF/I ),
    .O(user_out)
  );
  X_IPAD #(
    .LOC ( "PAD137" ))
  \user_in<10>  (
    .PAD(user_in[10])
  );
  X_BUF #(
    .LOC ( "PAD137" ))
  user_in_10_IBUF (
    .O(user_in_10_IBUF_33),
    .I(user_in[10])
  );
  X_BUF #(
    .LOC ( "PAD137" ))
  \ProtoComp0.IMUX.10  (
    .I(user_in_10_IBUF_33),
    .O(user_in_10_IBUF_0)
  );
  X_IPAD #(
    .LOC ( "PAD140" ))
  \user_in<11>  (
    .PAD(user_in[11])
  );
  X_BUF #(
    .LOC ( "PAD140" ))
  user_in_11_IBUF (
    .O(user_in_11_IBUF_36),
    .I(user_in[11])
  );
  X_BUF #(
    .LOC ( "PAD140" ))
  \ProtoComp0.IMUX.11  (
    .I(user_in_11_IBUF_36),
    .O(user_in_11_IBUF_0)
  );
  X_LUT6 #(
    .LOC ( "SLICE_X4Y2" ),
    .INIT ( 64'hFFFFFFFFFFFFFFFE ))
  out2 (
    .ADR4(\NlwBufferSignal_out2/ADR4 ),
    .ADR2(\NlwBufferSignal_out2/ADR2 ),
    .ADR0(\NlwBufferSignal_out2/ADR0 ),
    .ADR1(\NlwBufferSignal_out2/ADR1 ),
    .ADR5(\NlwBufferSignal_out2/ADR5 ),
    .ADR3(\NlwBufferSignal_out2/ADR3 ),
    .O(out1_82)
  );
  X_LUT6 #(
    .LOC ( "SLICE_X4Y2" ),
    .INIT ( 64'hFFFFFFFFFFFF0000 ))
  out3 (
    .ADR0(1'b1),
    .ADR1(1'b1),
    .ADR2(1'b1),
    .ADR3(1'b1),
    .ADR5(\NlwBufferSignal_out3/ADR5 ),
    .ADR4(out1_82),
    .O(user_out_OBUF_78)
  );
  X_LUT6 #(
    .LOC ( "SLICE_X7Y2" ),
    .INIT ( 64'hFFFFFFFFFFFFFFFE ))
  out1 (
    .ADR3(\NlwBufferSignal_out1/ADR3 ),
    .ADR4(\NlwBufferSignal_out1/ADR4 ),
    .ADR1(\NlwBufferSignal_out1/ADR1 ),
    .ADR0(\NlwBufferSignal_out1/ADR0 ),
    .ADR2(\NlwBufferSignal_out1/ADR2 ),
    .ADR5(\NlwBufferSignal_out1/ADR5 ),
    .O(out)
  );
  X_BUF   \NlwBufferBlock_user_out_OBUF/I  (
    .I(user_out_OBUF_78),
    .O(\NlwBufferSignal_user_out_OBUF/I )
  );
  X_BUF   \NlwBufferBlock_out2/ADR4  (
    .I(user_in_9_IBUF_0),
    .O(\NlwBufferSignal_out2/ADR4 )
  );
  X_BUF   \NlwBufferBlock_out2/ADR2  (
    .I(user_in_8_IBUF_0),
    .O(\NlwBufferSignal_out2/ADR2 )
  );
  X_BUF   \NlwBufferBlock_out2/ADR0  (
    .I(user_in_11_IBUF_0),
    .O(\NlwBufferSignal_out2/ADR0 )
  );
  X_BUF   \NlwBufferBlock_out2/ADR1  (
    .I(user_in_10_IBUF_0),
    .O(\NlwBufferSignal_out2/ADR1 )
  );
  X_BUF   \NlwBufferBlock_out2/ADR5  (
    .I(user_in_1_IBUF_0),
    .O(\NlwBufferSignal_out2/ADR5 )
  );
  X_BUF   \NlwBufferBlock_out2/ADR3  (
    .I(user_in_0_IBUF_0),
    .O(\NlwBufferSignal_out2/ADR3 )
  );
  X_BUF   \NlwBufferBlock_out3/ADR5  (
    .I(out),
    .O(\NlwBufferSignal_out3/ADR5 )
  );
  X_BUF   \NlwBufferBlock_out1/ADR3  (
    .I(user_in_3_IBUF_0),
    .O(\NlwBufferSignal_out1/ADR3 )
  );
  X_BUF   \NlwBufferBlock_out1/ADR4  (
    .I(user_in_2_IBUF_0),
    .O(\NlwBufferSignal_out1/ADR4 )
  );
  X_BUF   \NlwBufferBlock_out1/ADR1  (
    .I(user_in_5_IBUF_0),
    .O(\NlwBufferSignal_out1/ADR1 )
  );
  X_BUF   \NlwBufferBlock_out1/ADR0  (
    .I(user_in_4_IBUF_0),
    .O(\NlwBufferSignal_out1/ADR0 )
  );
  X_BUF   \NlwBufferBlock_out1/ADR2  (
    .I(user_in_7_IBUF_0),
    .O(\NlwBufferSignal_out1/ADR2 )
  );
  X_BUF   \NlwBufferBlock_out1/ADR5  (
    .I(user_in_6_IBUF_0),
    .O(\NlwBufferSignal_out1/ADR5 )
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

