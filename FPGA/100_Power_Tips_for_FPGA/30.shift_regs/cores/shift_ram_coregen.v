////////////////////////////////////////////////////////////////////////////////
// Copyright (c) 1995-2010 Xilinx, Inc.  All rights reserved.
////////////////////////////////////////////////////////////////////////////////
//   ____  ____
//  /   /\/   /
// /___/  \  /    Vendor: Xilinx
// \   \   \/     Version: M.53d
//  \   \         Application: netgen
//  /   /         Filename: shift_ram_coregen.v
// /___/   /\     Timestamp: Mon Jan 31 18:33:42 2011
// \   \  /  \ 
//  \___\/\___\
//             
// Command	: -intstyle ise -w -sim -ofmt verilog ./tmp/_cg\shift_ram_coregen.ngc ./tmp/_cg\shift_ram_coregen.v 
// Device	: 6slx45fgg676-3
// Input file	: ./tmp/_cg/shift_ram_coregen.ngc
// Output file	: ./tmp/_cg/shift_ram_coregen.v
// # of Modules	: 1
// Design Name	: shift_ram_coregen
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

module shift_ram_coregen (
  sclr, ce, clk, d, q
)/* synthesis syn_black_box syn_noprune=1 */;
  input sclr;
  input ce;
  input clk;
  input [0 : 0] d;
  output [0 : 0] q;
  
  // synthesis translate_off
  
  wire \BU2/sinit ;
  wire \BU2/sset ;
  wire \BU2/U0/i_bb_inst/Mshreg_f1.only_clb.srl_sig_62_1_9 ;
  wire \BU2/U0/i_bb_inst/Mshreg_f1.only_clb.srl_sig_62_0_8 ;
  wire \BU2/U0/i_bb_inst/N1 ;
  wire \BU2/U0/i_bb_inst/N0 ;
  wire \BU2/U0/i_bb_inst/f1.only_clb.srl_sig_62_5 ;
  wire NLW_VCC_P_UNCONNECTED;
  wire NLW_GND_G_UNCONNECTED;
  wire \NLW_BU2/U0/i_bb_inst/Mshreg_f1.only_clb.srl_sig_62_1_Q31_UNCONNECTED ;
  wire \NLW_BU2/U0/i_bb_inst/Mshreg_f1.only_clb.srl_sig_62_0_Q_UNCONNECTED ;
  wire [0 : 0] d_2;
  wire [0 : 0] q_3;
  wire [3 : 0] \BU2/a ;
  assign
    d_2[0] = d[0],
    q[0] = q_3[0];
  VCC   VCC_0 (
    .P(NLW_VCC_P_UNCONNECTED)
  );
  GND   GND_1 (
    .G(NLW_GND_G_UNCONNECTED)
  );
  FDE #(
    .INIT ( 1'b0 ))
  \BU2/U0/i_bb_inst/f1.only_clb.srl_sig_62  (
    .C(clk),
    .CE(ce),
    .D(\BU2/U0/i_bb_inst/Mshreg_f1.only_clb.srl_sig_62_1_9 ),
    .Q(\BU2/U0/i_bb_inst/f1.only_clb.srl_sig_62_5 )
  );
  SRLC32E #(
    .INIT ( 32'h00000000 ))
  \BU2/U0/i_bb_inst/Mshreg_f1.only_clb.srl_sig_62_1  (
    .CLK(clk),
    .D(\BU2/U0/i_bb_inst/Mshreg_f1.only_clb.srl_sig_62_0_8 ),
    .CE(ce),
    .Q(\BU2/U0/i_bb_inst/Mshreg_f1.only_clb.srl_sig_62_1_9 ),
    .Q31(\NLW_BU2/U0/i_bb_inst/Mshreg_f1.only_clb.srl_sig_62_1_Q31_UNCONNECTED ),
    .A({\BU2/U0/i_bb_inst/N1 , \BU2/U0/i_bb_inst/N1 , \BU2/U0/i_bb_inst/N1 , \BU2/U0/i_bb_inst/N0 , \BU2/U0/i_bb_inst/N1 })
  );
  SRLC32E #(
    .INIT ( 32'h00000000 ))
  \BU2/U0/i_bb_inst/Mshreg_f1.only_clb.srl_sig_62_0  (
    .CLK(clk),
    .D(d_2[0]),
    .CE(ce),
    .Q(\NLW_BU2/U0/i_bb_inst/Mshreg_f1.only_clb.srl_sig_62_0_Q_UNCONNECTED ),
    .Q31(\BU2/U0/i_bb_inst/Mshreg_f1.only_clb.srl_sig_62_0_8 ),
    .A({\BU2/U0/i_bb_inst/N1 , \BU2/U0/i_bb_inst/N1 , \BU2/U0/i_bb_inst/N1 , \BU2/U0/i_bb_inst/N1 , \BU2/U0/i_bb_inst/N1 })
  );
  VCC   \BU2/U0/i_bb_inst/XST_VCC  (
    .P(\BU2/U0/i_bb_inst/N1 )
  );
  GND   \BU2/U0/i_bb_inst/XST_GND  (
    .G(\BU2/U0/i_bb_inst/N0 )
  );
  FDRE #(
    .INIT ( 1'b0 ))
  \BU2/U0/i_bb_inst/gen_output_regs.output_regs/fd/output_1  (
    .C(clk),
    .CE(ce),
    .D(\BU2/U0/i_bb_inst/f1.only_clb.srl_sig_62_5 ),
    .R(sclr),
    .Q(q_3[0])
  );

// synthesis translate_on

endmodule

// synthesis translate_off

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

// synthesis translate_on
