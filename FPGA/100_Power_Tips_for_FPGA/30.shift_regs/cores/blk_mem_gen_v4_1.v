/*******************************************************************************
*     This file is owned and controlled by Xilinx and must be used             *
*     solely for design, simulation, implementation and creation of            *
*     design files limited to Xilinx devices or technologies. Use              *
*     with non-Xilinx devices or technologies is expressly prohibited          *
*     and immediately terminates your license.                                 *
*                                                                              *
*     XILINX IS PROVIDING THIS DESIGN, CODE, OR INFORMATION "AS IS"            *
*     SOLELY FOR USE IN DEVELOPING PROGRAMS AND SOLUTIONS FOR                  *
*     XILINX DEVICES.  BY PROVIDING THIS DESIGN, CODE, OR INFORMATION          *
*     AS ONE POSSIBLE IMPLEMENTATION OF THIS FEATURE, APPLICATION              *
*     OR STANDARD, XILINX IS MAKING NO REPRESENTATION THAT THIS                *
*     IMPLEMENTATION IS FREE FROM ANY CLAIMS OF INFRINGEMENT,                  *
*     AND YOU ARE RESPONSIBLE FOR OBTAINING ANY RIGHTS YOU MAY REQUIRE         *
*     FOR YOUR IMPLEMENTATION.  XILINX EXPRESSLY DISCLAIMS ANY                 *
*     WARRANTY WHATSOEVER WITH RESPECT TO THE ADEQUACY OF THE                  *
*     IMPLEMENTATION, INCLUDING BUT NOT LIMITED TO ANY WARRANTIES OR           *
*     REPRESENTATIONS THAT THIS IMPLEMENTATION IS FREE FROM CLAIMS OF          *
*     INFRINGEMENT, IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS          *
*     FOR A PARTICULAR PURPOSE.                                                *
*                                                                              *
*     Xilinx products are not intended for use in life support                 *
*     appliances, devices, or systems. Use in such applications are            *
*     expressly prohibited.                                                    *
*                                                                              *
*     (c) Copyright 1995-2009 Xilinx, Inc.                                     *
*     All rights reserved.                                                     *
*******************************************************************************/
// The synthesis directives "translate_off/translate_on" specified below are
// supported by Xilinx, Mentor Graphics and Synplicity synthesis
// tools. Ensure they are correct for your synthesis tool(s).

// You must compile the wrapper file blk_mem_gen_v4_1.v when simulating
// the core, blk_mem_gen_v4_1. When compiling the wrapper file, be sure to
// reference the XilinxCoreLib Verilog simulation library. For detailed
// instructions, please refer to the "CORE Generator Help".

`timescale 1ns/1ps

module blk_mem_gen_v4_1(
	clka,
	wea,
	addra,
	dina,
	douta,
	clkb,
	web,
	addrb,
	dinb,
	doutb);


input clka;
input [0 : 0] wea;
input [0 : 0] addra;
input [63 : 0] dina;
output [63 : 0] douta;
input clkb;
input [0 : 0] web;
input [0 : 0] addrb;
input [63 : 0] dinb;
output [63 : 0] doutb;

// synthesis translate_off

      BLK_MEM_GEN_V4_1 #(
		.C_ADDRA_WIDTH(1),
		.C_ADDRB_WIDTH(1),
		.C_ALGORITHM(1),
		.C_BYTE_SIZE(9),
		.C_COMMON_CLK(1),
		.C_DEFAULT_DATA("0"),
		.C_DISABLE_WARN_BHV_COLL(1),
		.C_DISABLE_WARN_BHV_RANGE(1),
		.C_FAMILY("spartan6"),
		.C_HAS_ENA(0),
		.C_HAS_ENB(0),
		.C_HAS_INJECTERR(0),
		.C_HAS_MEM_OUTPUT_REGS_A(0),
		.C_HAS_MEM_OUTPUT_REGS_B(0),
		.C_HAS_MUX_OUTPUT_REGS_A(0),
		.C_HAS_MUX_OUTPUT_REGS_B(0),
		.C_HAS_REGCEA(0),
		.C_HAS_REGCEB(0),
		.C_HAS_RSTA(0),
		.C_HAS_RSTB(0),
		.C_HAS_SOFTECC_INPUT_REGS_A(0),
		.C_HAS_SOFTECC_INPUT_REGS_B(0),
		.C_HAS_SOFTECC_OUTPUT_REGS_A(0),
		.C_HAS_SOFTECC_OUTPUT_REGS_B(0),
		.C_INITA_VAL("0"),
		.C_INITB_VAL("0"),
		.C_INIT_FILE_NAME("no_coe_file_loaded"),
		.C_LOAD_INIT_FILE(0),
		.C_MEM_TYPE(2),
		.C_MUX_PIPELINE_STAGES(0),
		.C_PRIM_TYPE(1),
		.C_READ_DEPTH_A(2),
		.C_READ_DEPTH_B(2),
		.C_READ_WIDTH_A(64),
		.C_READ_WIDTH_B(64),
		.C_RSTRAM_A(0),
		.C_RSTRAM_B(0),
		.C_RST_PRIORITY_A("CE"),
		.C_RST_PRIORITY_B("CE"),
		.C_RST_TYPE("SYNC"),
		.C_SIM_COLLISION_CHECK("NONE"),
		.C_USE_BYTE_WEA(0),
		.C_USE_BYTE_WEB(0),
		.C_USE_DEFAULT_DATA(0),
		.C_USE_ECC(0),
		.C_USE_SOFTECC(0),
		.C_WEA_WIDTH(1),
		.C_WEB_WIDTH(1),
		.C_WRITE_DEPTH_A(2),
		.C_WRITE_DEPTH_B(2),
		.C_WRITE_MODE_A("WRITE_FIRST"),
		.C_WRITE_MODE_B("WRITE_FIRST"),
		.C_WRITE_WIDTH_A(64),
		.C_WRITE_WIDTH_B(64),
		.C_XDEVICEFAMILY("spartan6"))
	inst (
		.CLKA(clka),
		.WEA(wea),
		.ADDRA(addra),
		.DINA(dina),
		.DOUTA(douta),
		.CLKB(clkb),
		.WEB(web),
		.ADDRB(addrb),
		.DINB(dinb),
		.DOUTB(doutb),
		.RSTA(),
		.ENA(),
		.REGCEA(),
		.RSTB(),
		.ENB(),
		.REGCEB(),
		.INJECTSBITERR(),
		.INJECTDBITERR(),
		.SBITERR(),
		.DBITERR(),
		.RDADDRECC());


// synthesis translate_on

// XST black box declaration
// box_type "black_box"
// synthesis attribute box_type of blk_mem_gen_v4_1 is "black_box"

endmodule

