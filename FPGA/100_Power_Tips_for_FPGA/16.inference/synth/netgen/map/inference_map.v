////////////////////////////////////////////////////////////////////////////////
// Copyright (c) 1995-2010 Xilinx, Inc.  All rights reserved.
////////////////////////////////////////////////////////////////////////////////
//   ____  ____
//  /   /\/   /
// /___/  \  /    Vendor: Xilinx
// \   \   \/     Version: M.53d
//  \   \         Application: netgen
//  /   /         Filename: inference_map.v
// /___/   /\     Timestamp: Fri Mar 11 11:14:48 2011
// \   \  /  \ 
//  \___\/\___\
//             
// Command	: -intstyle ise -s 3 -pcf inference.pcf -sdf_anno true -sdf_path netgen/map -insert_glbl true -w -dir netgen/map -ofmt verilog -sim inference_map.ncd inference_map.v 
// Device	: 6slx45fgg676-3 (PRELIMINARY 1.08 2010-04-09)
// Input file	: inference_map.ncd
// Output file	: G:\fpga_book\src\16.inference\synth\netgen\map\inference_map.v
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
  clk, areset, sreset, srl_enable, srl1_out, srl2_out, d_in, d_out
);
  input clk;
  input areset;
  input sreset;
  input srl_enable;
  output srl1_out;
  output srl2_out;
  input [4 : 0] d_in;
  output [4 : 0] d_out;
  wire areset_sreset_AND_2_o;
  wire d_in_3_IBUF_0;
  wire areset_IBUF_0;
  wire sreset_IBUF_0;
  wire d_out_3_rstpot_0;
  wire srl_enable_IBUF_0;
  wire \clk_BUFGP/IBUFG_0 ;
  wire d_in_0_IBUF_0;
  wire d_out_0_127;
  wire d_in_1_IBUF_0;
  wire d_out_1_129;
  wire d_in_2_IBUF_0;
  wire d_out_2_131;
  wire d_out_3_132;
  wire d_in_4_IBUF_0;
  wire d_out_4;
  wire clk_BUFGP;
  wire areset_inv;
  wire sreset_IBUF_BUFG_137;
  wire d_out_4_P_4_138;
  wire d_out_4_LDC_139;
  wire d_out_4_C_4_140;
  wire d_in_0_IBUF_direct;
  wire srl_enable_IBUF_1;
  wire \clk_BUFGP/IBUFG_4 ;
  wire sreset_IBUF_7;
  wire areset_IBUF_12;
  wire d_in_0_IBUF_17;
  wire d_in_1_IBUF_22;
  wire d_in_2_IBUF_27;
  wire d_in_3_IBUF_32;
  wire d_in_4_IBUF_37;
  wire \ProtoComp3.D2OFFBYP_SRC.OUT ;
  wire \ProtoComp3.D2OBYPSEL_GND.0 ;
  wire \d_out_1/INV_d_out_1CLK ;
  wire \ProtoComp4.D2OFFBYP_SRC.OUT ;
  wire \ProtoComp4.D2OBYPSEL_GND.0 ;
  wire \d_out_2/INV_d_out_2CLK ;
  wire \ProtoComp5.D2OFFBYP_SRC.OUT ;
  wire \ProtoComp5.D2OBYPSEL_GND.0 ;
  wire d_out_3_rstpot_67;
  wire Mshreg_srl1_7_90;
  wire VCC;
  wire GND;
  wire \NLW_ProtoComp3.D2OFFBYP_SRC_IB_UNCONNECTED ;
  wire \NLW_ProtoComp3.D2OBYP_SRC_IB_UNCONNECTED ;
  wire \NLW_ProtoComp4.D2OFFBYP_SRC_IB_UNCONNECTED ;
  wire \NLW_ProtoComp5.D2OFFBYP_SRC_IB_UNCONNECTED ;
  wire NLW_Mshreg_srl1_7_Q15_UNCONNECTED;
  wire [7 : 7] srl1;
  initial $sdf_annotate("netgen/map/inference_map.sdf");
  X_IPAD #(
    .LOC ( "PAD216" ))
  srl_enable_4 (
    .PAD(srl_enable)
  );
  X_BUF #(
    .LOC ( "PAD216" ))
  srl_enable_IBUF (
    .O(srl_enable_IBUF_1),
    .I(srl_enable)
  );
  X_BUF #(
    .LOC ( "PAD216" ))
  \ProtoComp1.IMUX  (
    .I(srl_enable_IBUF_1),
    .O(srl_enable_IBUF_0)
  );
  X_IPAD #(
    .LOC ( "PAD318" ))
  clk_8 (
    .PAD(clk)
  );
  X_BUF #(
    .LOC ( "PAD318" ))
  \clk_BUFGP/IBUFG  (
    .O(\clk_BUFGP/IBUFG_4 ),
    .I(clk)
  );
  X_BUF #(
    .LOC ( "PAD318" ))
  \ProtoComp1.IMUX.1  (
    .I(\clk_BUFGP/IBUFG_4 ),
    .O(\clk_BUFGP/IBUFG_0 )
  );
  X_IPAD #(
    .LOC ( "PAD314" ))
  sreset_12 (
    .PAD(sreset)
  );
  X_BUF #(
    .LOC ( "PAD314" ))
  sreset_IBUF (
    .O(sreset_IBUF_7),
    .I(sreset)
  );
  X_BUF #(
    .LOC ( "PAD314" ))
  \ProtoComp1.IMUX.2  (
    .I(sreset_IBUF_7),
    .O(sreset_IBUF_0)
  );
  X_OPAD #(
    .LOC ( "PAD209" ))
  srl1_out_15 (
    .PAD(srl1_out)
  );
  X_OBUF #(
    .LOC ( "PAD209" ))
  srl1_out_OBUF (
    .I(srl1[7]),
    .O(srl1_out)
  );
  X_IPAD #(
    .LOC ( "PAD214" ))
  areset_19 (
    .PAD(areset)
  );
  X_BUF #(
    .LOC ( "PAD214" ))
  areset_IBUF (
    .O(areset_IBUF_12),
    .I(areset)
  );
  X_BUF #(
    .LOC ( "PAD214" ))
  \ProtoComp1.IMUX.3  (
    .I(areset_IBUF_12),
    .O(areset_IBUF_0)
  );
  X_OPAD #(
    .LOC ( "PAD213" ))
  srl2_out_22 (
    .PAD(srl2_out)
  );
  X_OBUF #(
    .LOC ( "PAD213" ))
  srl2_out_OBUF (
    .I(srl1[7]),
    .O(srl2_out)
  );
  X_IPAD #(
    .LOC ( "PAD210" ))
  \d_in<0>  (
    .PAD(d_in[0])
  );
  X_BUF #(
    .LOC ( "PAD210" ))
  d_in_0_IBUF (
    .O(d_in_0_IBUF_17),
    .I(d_in[0])
  );
  X_BUF #(
    .LOC ( "PAD210" ))
  \ProtoComp1.IMUX.4  (
    .I(d_in_0_IBUF_17),
    .O(d_in_0_IBUF_direct)
  );
  X_OPAD #(
    .LOC ( "PAD218" ))
  \d_out<0>  (
    .PAD(d_out[0])
  );
  X_OBUF #(
    .LOC ( "PAD218" ))
  d_out_0_OBUF (
    .I(d_out_0_127),
    .O(d_out[0])
  );
  X_IPAD #(
    .LOC ( "PAD215" ))
  \d_in<1>  (
    .PAD(d_in[1])
  );
  X_BUF #(
    .LOC ( "PAD215" ))
  d_in_1_IBUF (
    .O(d_in_1_IBUF_22),
    .I(d_in[1])
  );
  X_BUF #(
    .LOC ( "PAD215" ))
  \ProtoComp1.IMUX.5  (
    .I(d_in_1_IBUF_22),
    .O(d_in_1_IBUF_0)
  );
  X_OPAD #(
    .LOC ( "PAD219" ))
  \d_out<1>  (
    .PAD(d_out[1])
  );
  X_OBUF #(
    .LOC ( "PAD219" ))
  d_out_1_OBUF (
    .I(d_out_1_129),
    .O(d_out[1])
  );
  X_IPAD #(
    .LOC ( "PAD212" ))
  \d_in<2>  (
    .PAD(d_in[2])
  );
  X_BUF #(
    .LOC ( "PAD212" ))
  d_in_2_IBUF (
    .O(d_in_2_IBUF_27),
    .I(d_in[2])
  );
  X_BUF #(
    .LOC ( "PAD212" ))
  \ProtoComp1.IMUX.6  (
    .I(d_in_2_IBUF_27),
    .O(d_in_2_IBUF_0)
  );
  X_OPAD #(
    .LOC ( "PAD220" ))
  \d_out<2>  (
    .PAD(d_out[2])
  );
  X_OBUF #(
    .LOC ( "PAD220" ))
  d_out_2_OBUF (
    .I(d_out_2_131),
    .O(d_out[2])
  );
  X_IPAD #(
    .LOC ( "PAD217" ))
  \d_in<3>  (
    .PAD(d_in[3])
  );
  X_BUF #(
    .LOC ( "PAD217" ))
  d_in_3_IBUF (
    .O(d_in_3_IBUF_32),
    .I(d_in[3])
  );
  X_BUF #(
    .LOC ( "PAD217" ))
  \ProtoComp1.IMUX.7  (
    .I(d_in_3_IBUF_32),
    .O(d_in_3_IBUF_0)
  );
  X_OPAD #(
    .LOC ( "PAD224" ))
  \d_out<3>  (
    .PAD(d_out[3])
  );
  X_OBUF #(
    .LOC ( "PAD224" ))
  d_out_3_OBUF (
    .I(d_out_3_132),
    .O(d_out[3])
  );
  X_IPAD #(
    .LOC ( "PAD211" ))
  \d_in<4>  (
    .PAD(d_in[4])
  );
  X_BUF #(
    .LOC ( "PAD211" ))
  d_in_4_IBUF (
    .O(d_in_4_IBUF_37),
    .I(d_in[4])
  );
  X_BUF #(
    .LOC ( "PAD211" ))
  \ProtoComp1.IMUX.8  (
    .I(d_in_4_IBUF_37),
    .O(d_in_4_IBUF_0)
  );
  X_OPAD #(
    .LOC ( "PAD223" ))
  \d_out<4>  (
    .PAD(d_out[4])
  );
  X_OBUF #(
    .LOC ( "PAD223" ))
  d_out_4_OBUF (
    .I(d_out_4),
    .O(d_out[4])
  );
  X_FF #(
    .LOC ( "ILOGIC_X13Y0" ),
    .INIT ( 1'b0 ))
  d_out_0 (
    .CE(VCC),
    .CLK(clk_BUFGP),
    .I(\ProtoComp3.D2OFFBYP_SRC.OUT ),
    .O(d_out_0_127),
    .SET(GND),
    .RST(areset_IBUF_0)
  );
  X_MUX2 #(
    .LOC ( "ILOGIC_X13Y0" ))
  \ProtoComp3.D2OFFBYP_SRC  (
    .IA(d_in_0_IBUF_direct),
    .IB(\NLW_ProtoComp3.D2OFFBYP_SRC_IB_UNCONNECTED ),
    .O(\ProtoComp3.D2OFFBYP_SRC.OUT ),
    .SEL(\ProtoComp3.D2OBYPSEL_GND.0 )
  );
  X_MUX2 #(
    .LOC ( "ILOGIC_X13Y0" ))
  \ProtoComp3.D2OBYP_SRC  (
    .IA(d_in_0_IBUF_direct),
    .IB(\NLW_ProtoComp3.D2OBYP_SRC_IB_UNCONNECTED ),
    .O(d_in_0_IBUF_0),
    .SEL(\ProtoComp3.D2OBYPSEL_GND.0 )
  );
  X_ZERO #(
    .LOC ( "ILOGIC_X13Y0" ))
  \ProtoComp3.D2OBYPSEL_GND  (
    .O(\ProtoComp3.D2OBYPSEL_GND.0 )
  );
  X_INV   INV_d_out_1CLK (
    .I(clk_BUFGP),
    .O(\d_out_1/INV_d_out_1CLK )
  );
  X_FF #(
    .LOC ( "ILOGIC_X12Y3" ),
    .INIT ( 1'b0 ))
  d_out_1 (
    .CE(VCC),
    .CLK(\d_out_1/INV_d_out_1CLK ),
    .I(\ProtoComp4.D2OFFBYP_SRC.OUT ),
    .O(d_out_1_129),
    .SET(GND),
    .RST(areset_inv)
  );
  X_MUX2 #(
    .LOC ( "ILOGIC_X12Y3" ))
  \ProtoComp4.D2OFFBYP_SRC  (
    .IA(d_in_1_IBUF_0),
    .IB(\NLW_ProtoComp4.D2OFFBYP_SRC_IB_UNCONNECTED ),
    .O(\ProtoComp4.D2OFFBYP_SRC.OUT ),
    .SEL(\ProtoComp4.D2OBYPSEL_GND.0 )
  );
  X_ZERO #(
    .LOC ( "ILOGIC_X12Y3" ))
  \ProtoComp4.D2OBYPSEL_GND  (
    .O(\ProtoComp4.D2OBYPSEL_GND.0 )
  );
  X_INV   INV_d_out_2CLK (
    .I(clk_BUFGP),
    .O(\d_out_2/INV_d_out_2CLK )
  );
  X_SFF #(
    .LOC ( "ILOGIC_X13Y2" ),
    .INIT ( 1'b0 ))
  d_out_2 (
    .CE(VCC),
    .CLK(\d_out_2/INV_d_out_2CLK ),
    .I(\ProtoComp5.D2OFFBYP_SRC.OUT ),
    .O(d_out_2_131),
    .SSET(GND),
    .SRST(GND),
    .SET(GND),
    .RST(GND)
  );
  X_MUX2 #(
    .LOC ( "ILOGIC_X13Y2" ))
  \ProtoComp5.D2OFFBYP_SRC  (
    .IA(d_in_2_IBUF_0),
    .IB(\NLW_ProtoComp5.D2OFFBYP_SRC_IB_UNCONNECTED ),
    .O(\ProtoComp5.D2OFFBYP_SRC.OUT ),
    .SEL(\ProtoComp5.D2OBYPSEL_GND.0 )
  );
  X_ZERO #(
    .LOC ( "ILOGIC_X13Y2" ))
  \ProtoComp5.D2OBYPSEL_GND  (
    .O(\ProtoComp5.D2OBYPSEL_GND.0 )
  );
  X_SFF #(
    .LOC ( "OLOGIC_X10Y2" ),
    .INIT ( 1'b0 ))
  d_out_3 (
    .CE(VCC),
    .CLK(clk_BUFGP),
    .I(d_out_3_rstpot_0),
    .O(d_out_3_132),
    .SET(GND),
    .RST(GND),
    .SSET(GND),
    .SRST(GND)
  );
  X_CKBUF #(
    .LOC ( "BUFGMUX_X3Y16" ))
  \clk_BUFGP/BUFG  (
    .I(\clk_BUFGP/IBUFG_0 ),
    .O(clk_BUFGP)
  );
  X_CKBUF #(
    .LOC ( "BUFGMUX_X3Y13" ))
  sreset_IBUF_BUFG (
    .I(sreset_IBUF_0),
    .O(sreset_IBUF_BUFG_137)
  );
  X_BUF   \areset_sreset_AND_2_o/areset_sreset_AND_2_o_AMUX_Delay  (
    .I(d_out_3_rstpot_67),
    .O(d_out_3_rstpot_0)
  );
  X_LUT6 #(
    .LOC ( "SLICE_X22Y11" ),
    .INIT ( 64'h0000FF000000FF00 ))
  areset_sreset_AND_2_o1 (
    .ADR0(1'b1),
    .ADR1(1'b1),
    .ADR2(1'b1),
    .ADR3(areset_IBUF_0),
    .ADR4(sreset_IBUF_0),
    .ADR5(1'b1),
    .O(areset_sreset_AND_2_o)
  );
  X_LUT5 #(
    .LOC ( "SLICE_X22Y11" ),
    .INIT ( 32'hFFFFF0F0 ))
  d_out_3_rstpot (
    .ADR0(1'b1),
    .ADR1(1'b1),
    .ADR2(d_in_3_IBUF_0),
    .ADR3(1'b1),
    .ADR4(sreset_IBUF_0),
    .O(d_out_3_rstpot_67)
  );
  X_LUT6 #(
    .LOC ( "SLICE_X22Y20" ),
    .INIT ( 64'hFFFF00FFFF000000 ))
  d_out_41 (
    .ADR0(1'b1),
    .ADR1(1'b1),
    .ADR2(1'b1),
    .ADR3(d_out_4_LDC_139),
    .ADR4(d_out_4_P_4_138),
    .ADR5(d_out_4_C_4_140),
    .O(d_out_4)
  );
  X_FF #(
    .LOC ( "SLICE_X22Y23" ),
    .INIT ( 1'b1 ))
  d_out_4_P_4 (
    .CE(VCC),
    .CLK(clk_BUFGP),
    .I(d_in_4_IBUF_0),
    .O(d_out_4_P_4_138),
    .SET(sreset_IBUF_0),
    .RST(GND)
  );
  X_FF #(
    .LOC ( "SLICE_X23Y20" ),
    .INIT ( 1'b0 ))
  d_out_4_C_4 (
    .CE(VCC),
    .CLK(clk_BUFGP),
    .I(d_in_4_IBUF_0),
    .O(d_out_4_C_4_140),
    .RST(areset_sreset_AND_2_o),
    .SET(GND)
  );
  X_LATCHE #(
    .LOC ( "SLICE_X23Y31" ),
    .INIT ( 1'b0 ))
  d_out_4_LDC (
    .GE(VCC),
    .CLK(sreset_IBUF_BUFG_137),
    .I(1'b1),
    .O(d_out_4_LDC_139),
    .RST(areset_sreset_AND_2_o),
    .SET(GND)
  );
  X_FF #(
    .LOC ( "SLICE_X26Y2" ),
    .INIT ( 1'b0 ))
  srl1_7 (
    .CE(srl_enable_IBUF_0),
    .CLK(clk_BUFGP),
    .I(Mshreg_srl1_7_90),
    .O(srl1[7]),
    .RST(GND),
    .SET(GND)
  );
  X_SRLC16E #(
    .LOC ( "SLICE_X26Y2" ),
    .INIT ( 16'h0000 ))
  Mshreg_srl1_7 (
    .A0(1'b0),
    .A1(1'b1),
    .A2(1'b1),
    .A3(1'b0),
    .CLK(clk_BUFGP),
    .D(d_in_0_IBUF_0),
    .Q15(NLW_Mshreg_srl1_7_Q15_UNCONNECTED),
    .Q(Mshreg_srl1_7_90),
    .CE(srl_enable_IBUF_0)
  );
  X_LUT6 #(
    .LOC ( "SLICE_X27Y2" ),
    .INIT ( 64'h00000000FFFFFFFF ))
  areset_inv1_INV_0 (
    .ADR0(1'b1),
    .ADR1(1'b1),
    .ADR2(1'b1),
    .ADR3(1'b1),
    .ADR4(1'b1),
    .ADR5(areset_IBUF_0),
    .O(areset_inv)
  );
  X_ONE   NlwBlock_inference_VCC (
    .O(VCC)
  );
  X_ZERO   NlwBlock_inference_GND (
    .O(GND)
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

