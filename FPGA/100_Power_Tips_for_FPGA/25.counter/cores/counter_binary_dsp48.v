////////////////////////////////////////////////////////////////////////////////
// Copyright (c) 1995-2010 Xilinx, Inc.  All rights reserved.
////////////////////////////////////////////////////////////////////////////////
//   ____  ____
//  /   /\/   /
// /___/  \  /    Vendor: Xilinx
// \   \   \/     Version: M.53d
//  \   \         Application: netgen
//  /   /         Filename: counter_binary_dsp48.v
// /___/   /\     Timestamp: Wed Jan 26 05:29:37 2011
// \   \  /  \ 
//  \___\/\___\
//             
// Command	: -intstyle ise -w -sim -ofmt verilog ./tmp/_cg\counter_binary_dsp48.ngc ./tmp/_cg\counter_binary_dsp48.v 
// Device	: 6slx45fgg676-3
// Input file	: ./tmp/_cg/counter_binary_dsp48.ngc
// Output file	: ./tmp/_cg/counter_binary_dsp48.v
// # of Modules	: 1
// Design Name	: counter_binary_dsp48
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

module counter_binary_dsp48 (
  sclr, ce, clk, thresh0, q
)/* synthesis syn_black_box syn_noprune=1 */;
  input sclr;
  input ce;
  input clk;
  output thresh0;
  output [31 : 0] q;
  
  // synthesis translate_off
  
  wire \BU2/U0/i_baseip.i_xbip_counter/i_thresh_detect.i_norm.i_gate/tier_gen[1].i_tier/loop_tiles[0].i_tile/lut_o<0> ;
  wire \BU2/U0/i_baseip.i_xbip_counter/i_thresh_detect.i_norm.i_gate/tier_gen[1].i_tier/loop_tiles[0].i_tile/async_o<1> ;
  wire \BU2/U0/i_baseip.i_xbip_counter/i_thresh_detect.i_norm.i_gate/tier_gen[1].i_tier/loop_tiles[0].i_tile/lut_o<1> ;
  wire \BU2/U0/i_baseip.i_xbip_counter/i_thresh_detect.i_norm.i_gate/tier_gen[1].i_tier/loop_tiles[0].i_tile/async_o<2> ;
  wire \BU2/U0/i_baseip.i_xbip_counter/i_thresh_detect.i_norm.i_gate/tier_gen[1].i_tier/loop_tiles[0].i_tile/lut_o<2> ;
  wire \BU2/U0/i_baseip.i_xbip_counter/i_thresh_detect.i_norm.i_gate/tier_gen[1].i_tier/loop_tiles[0].i_tile/async_o<3> ;
  wire \BU2/U0/i_baseip.i_xbip_counter/i_thresh_detect.i_norm.i_gate/tier_gen[1].i_tier/loop_tiles[0].i_tile/lut_o<3> ;
  wire \BU2/U0/i_baseip.i_xbip_counter/i_thresh_detect.i_norm.i_gate/tier_gen[1].i_tier/loop_tiles[0].i_tile/async_o<4> ;
  wire \BU2/U0/i_baseip.i_xbip_counter/i_thresh_detect.i_norm.i_gate/tier_gen[1].i_tier/loop_tiles[0].i_tile/lut_o<4> ;
  wire \BU2/U0/i_baseip.i_xbip_counter/i_thresh_detect.i_norm.i_gate/tier_gen[1].i_tier/loop_tiles[0].i_tile/async_o<5> ;
  wire \BU2/U0/i_baseip.i_xbip_counter/i_thresh_detect.i_norm.i_gate/tier_gen[1].i_tier/loop_tiles[0].i_tile/lut_o<5> ;
  wire \BU2/N1 ;
  wire \BU2/N0 ;
  wire NLW_VCC_P_UNCONNECTED;
  wire NLW_GND_G_UNCONNECTED;
  wire \NLW_BU2/U0/i_baseip.i_xbip_counter/i_dsp48.i_dsp/i_sp3adsp.i_dsp48a_wrap/i_primitive_CARRYOUTF_UNCONNECTED ;
  wire \NLW_BU2/U0/i_baseip.i_xbip_counter/i_dsp48.i_dsp/i_sp3adsp.i_dsp48a_wrap/i_primitive_CARRYOUT_UNCONNECTED ;
  wire \NLW_BU2/U0/i_baseip.i_xbip_counter/i_dsp48.i_dsp/i_sp3adsp.i_dsp48a_wrap/i_primitive_BCOUT<17>_UNCONNECTED ;
  wire \NLW_BU2/U0/i_baseip.i_xbip_counter/i_dsp48.i_dsp/i_sp3adsp.i_dsp48a_wrap/i_primitive_BCOUT<16>_UNCONNECTED ;
  wire \NLW_BU2/U0/i_baseip.i_xbip_counter/i_dsp48.i_dsp/i_sp3adsp.i_dsp48a_wrap/i_primitive_BCOUT<15>_UNCONNECTED ;
  wire \NLW_BU2/U0/i_baseip.i_xbip_counter/i_dsp48.i_dsp/i_sp3adsp.i_dsp48a_wrap/i_primitive_BCOUT<14>_UNCONNECTED ;
  wire \NLW_BU2/U0/i_baseip.i_xbip_counter/i_dsp48.i_dsp/i_sp3adsp.i_dsp48a_wrap/i_primitive_BCOUT<13>_UNCONNECTED ;
  wire \NLW_BU2/U0/i_baseip.i_xbip_counter/i_dsp48.i_dsp/i_sp3adsp.i_dsp48a_wrap/i_primitive_BCOUT<12>_UNCONNECTED ;
  wire \NLW_BU2/U0/i_baseip.i_xbip_counter/i_dsp48.i_dsp/i_sp3adsp.i_dsp48a_wrap/i_primitive_BCOUT<11>_UNCONNECTED ;
  wire \NLW_BU2/U0/i_baseip.i_xbip_counter/i_dsp48.i_dsp/i_sp3adsp.i_dsp48a_wrap/i_primitive_BCOUT<10>_UNCONNECTED ;
  wire \NLW_BU2/U0/i_baseip.i_xbip_counter/i_dsp48.i_dsp/i_sp3adsp.i_dsp48a_wrap/i_primitive_BCOUT<9>_UNCONNECTED ;
  wire \NLW_BU2/U0/i_baseip.i_xbip_counter/i_dsp48.i_dsp/i_sp3adsp.i_dsp48a_wrap/i_primitive_BCOUT<8>_UNCONNECTED ;
  wire \NLW_BU2/U0/i_baseip.i_xbip_counter/i_dsp48.i_dsp/i_sp3adsp.i_dsp48a_wrap/i_primitive_BCOUT<7>_UNCONNECTED ;
  wire \NLW_BU2/U0/i_baseip.i_xbip_counter/i_dsp48.i_dsp/i_sp3adsp.i_dsp48a_wrap/i_primitive_BCOUT<6>_UNCONNECTED ;
  wire \NLW_BU2/U0/i_baseip.i_xbip_counter/i_dsp48.i_dsp/i_sp3adsp.i_dsp48a_wrap/i_primitive_BCOUT<5>_UNCONNECTED ;
  wire \NLW_BU2/U0/i_baseip.i_xbip_counter/i_dsp48.i_dsp/i_sp3adsp.i_dsp48a_wrap/i_primitive_BCOUT<4>_UNCONNECTED ;
  wire \NLW_BU2/U0/i_baseip.i_xbip_counter/i_dsp48.i_dsp/i_sp3adsp.i_dsp48a_wrap/i_primitive_BCOUT<3>_UNCONNECTED ;
  wire \NLW_BU2/U0/i_baseip.i_xbip_counter/i_dsp48.i_dsp/i_sp3adsp.i_dsp48a_wrap/i_primitive_BCOUT<2>_UNCONNECTED ;
  wire \NLW_BU2/U0/i_baseip.i_xbip_counter/i_dsp48.i_dsp/i_sp3adsp.i_dsp48a_wrap/i_primitive_BCOUT<1>_UNCONNECTED ;
  wire \NLW_BU2/U0/i_baseip.i_xbip_counter/i_dsp48.i_dsp/i_sp3adsp.i_dsp48a_wrap/i_primitive_BCOUT<0>_UNCONNECTED ;
  wire \NLW_BU2/U0/i_baseip.i_xbip_counter/i_dsp48.i_dsp/i_sp3adsp.i_dsp48a_wrap/i_primitive_P<47>_UNCONNECTED ;
  wire \NLW_BU2/U0/i_baseip.i_xbip_counter/i_dsp48.i_dsp/i_sp3adsp.i_dsp48a_wrap/i_primitive_P<46>_UNCONNECTED ;
  wire \NLW_BU2/U0/i_baseip.i_xbip_counter/i_dsp48.i_dsp/i_sp3adsp.i_dsp48a_wrap/i_primitive_P<45>_UNCONNECTED ;
  wire \NLW_BU2/U0/i_baseip.i_xbip_counter/i_dsp48.i_dsp/i_sp3adsp.i_dsp48a_wrap/i_primitive_P<44>_UNCONNECTED ;
  wire \NLW_BU2/U0/i_baseip.i_xbip_counter/i_dsp48.i_dsp/i_sp3adsp.i_dsp48a_wrap/i_primitive_P<43>_UNCONNECTED ;
  wire \NLW_BU2/U0/i_baseip.i_xbip_counter/i_dsp48.i_dsp/i_sp3adsp.i_dsp48a_wrap/i_primitive_P<42>_UNCONNECTED ;
  wire \NLW_BU2/U0/i_baseip.i_xbip_counter/i_dsp48.i_dsp/i_sp3adsp.i_dsp48a_wrap/i_primitive_P<41>_UNCONNECTED ;
  wire \NLW_BU2/U0/i_baseip.i_xbip_counter/i_dsp48.i_dsp/i_sp3adsp.i_dsp48a_wrap/i_primitive_P<40>_UNCONNECTED ;
  wire \NLW_BU2/U0/i_baseip.i_xbip_counter/i_dsp48.i_dsp/i_sp3adsp.i_dsp48a_wrap/i_primitive_P<39>_UNCONNECTED ;
  wire \NLW_BU2/U0/i_baseip.i_xbip_counter/i_dsp48.i_dsp/i_sp3adsp.i_dsp48a_wrap/i_primitive_P<38>_UNCONNECTED ;
  wire \NLW_BU2/U0/i_baseip.i_xbip_counter/i_dsp48.i_dsp/i_sp3adsp.i_dsp48a_wrap/i_primitive_P<37>_UNCONNECTED ;
  wire \NLW_BU2/U0/i_baseip.i_xbip_counter/i_dsp48.i_dsp/i_sp3adsp.i_dsp48a_wrap/i_primitive_P<36>_UNCONNECTED ;
  wire \NLW_BU2/U0/i_baseip.i_xbip_counter/i_dsp48.i_dsp/i_sp3adsp.i_dsp48a_wrap/i_primitive_P<35>_UNCONNECTED ;
  wire \NLW_BU2/U0/i_baseip.i_xbip_counter/i_dsp48.i_dsp/i_sp3adsp.i_dsp48a_wrap/i_primitive_P<34>_UNCONNECTED ;
  wire \NLW_BU2/U0/i_baseip.i_xbip_counter/i_dsp48.i_dsp/i_sp3adsp.i_dsp48a_wrap/i_primitive_P<33>_UNCONNECTED ;
  wire \NLW_BU2/U0/i_baseip.i_xbip_counter/i_dsp48.i_dsp/i_sp3adsp.i_dsp48a_wrap/i_primitive_P<32>_UNCONNECTED ;
  wire \NLW_BU2/U0/i_baseip.i_xbip_counter/i_dsp48.i_dsp/i_sp3adsp.i_dsp48a_wrap/i_primitive_PCOUT<47>_UNCONNECTED ;
  wire \NLW_BU2/U0/i_baseip.i_xbip_counter/i_dsp48.i_dsp/i_sp3adsp.i_dsp48a_wrap/i_primitive_PCOUT<46>_UNCONNECTED ;
  wire \NLW_BU2/U0/i_baseip.i_xbip_counter/i_dsp48.i_dsp/i_sp3adsp.i_dsp48a_wrap/i_primitive_PCOUT<45>_UNCONNECTED ;
  wire \NLW_BU2/U0/i_baseip.i_xbip_counter/i_dsp48.i_dsp/i_sp3adsp.i_dsp48a_wrap/i_primitive_PCOUT<44>_UNCONNECTED ;
  wire \NLW_BU2/U0/i_baseip.i_xbip_counter/i_dsp48.i_dsp/i_sp3adsp.i_dsp48a_wrap/i_primitive_PCOUT<43>_UNCONNECTED ;
  wire \NLW_BU2/U0/i_baseip.i_xbip_counter/i_dsp48.i_dsp/i_sp3adsp.i_dsp48a_wrap/i_primitive_PCOUT<42>_UNCONNECTED ;
  wire \NLW_BU2/U0/i_baseip.i_xbip_counter/i_dsp48.i_dsp/i_sp3adsp.i_dsp48a_wrap/i_primitive_PCOUT<41>_UNCONNECTED ;
  wire \NLW_BU2/U0/i_baseip.i_xbip_counter/i_dsp48.i_dsp/i_sp3adsp.i_dsp48a_wrap/i_primitive_PCOUT<40>_UNCONNECTED ;
  wire \NLW_BU2/U0/i_baseip.i_xbip_counter/i_dsp48.i_dsp/i_sp3adsp.i_dsp48a_wrap/i_primitive_PCOUT<39>_UNCONNECTED ;
  wire \NLW_BU2/U0/i_baseip.i_xbip_counter/i_dsp48.i_dsp/i_sp3adsp.i_dsp48a_wrap/i_primitive_PCOUT<38>_UNCONNECTED ;
  wire \NLW_BU2/U0/i_baseip.i_xbip_counter/i_dsp48.i_dsp/i_sp3adsp.i_dsp48a_wrap/i_primitive_PCOUT<37>_UNCONNECTED ;
  wire \NLW_BU2/U0/i_baseip.i_xbip_counter/i_dsp48.i_dsp/i_sp3adsp.i_dsp48a_wrap/i_primitive_PCOUT<36>_UNCONNECTED ;
  wire \NLW_BU2/U0/i_baseip.i_xbip_counter/i_dsp48.i_dsp/i_sp3adsp.i_dsp48a_wrap/i_primitive_PCOUT<35>_UNCONNECTED ;
  wire \NLW_BU2/U0/i_baseip.i_xbip_counter/i_dsp48.i_dsp/i_sp3adsp.i_dsp48a_wrap/i_primitive_PCOUT<34>_UNCONNECTED ;
  wire \NLW_BU2/U0/i_baseip.i_xbip_counter/i_dsp48.i_dsp/i_sp3adsp.i_dsp48a_wrap/i_primitive_PCOUT<33>_UNCONNECTED ;
  wire \NLW_BU2/U0/i_baseip.i_xbip_counter/i_dsp48.i_dsp/i_sp3adsp.i_dsp48a_wrap/i_primitive_PCOUT<32>_UNCONNECTED ;
  wire \NLW_BU2/U0/i_baseip.i_xbip_counter/i_dsp48.i_dsp/i_sp3adsp.i_dsp48a_wrap/i_primitive_PCOUT<31>_UNCONNECTED ;
  wire \NLW_BU2/U0/i_baseip.i_xbip_counter/i_dsp48.i_dsp/i_sp3adsp.i_dsp48a_wrap/i_primitive_PCOUT<30>_UNCONNECTED ;
  wire \NLW_BU2/U0/i_baseip.i_xbip_counter/i_dsp48.i_dsp/i_sp3adsp.i_dsp48a_wrap/i_primitive_PCOUT<29>_UNCONNECTED ;
  wire \NLW_BU2/U0/i_baseip.i_xbip_counter/i_dsp48.i_dsp/i_sp3adsp.i_dsp48a_wrap/i_primitive_PCOUT<28>_UNCONNECTED ;
  wire \NLW_BU2/U0/i_baseip.i_xbip_counter/i_dsp48.i_dsp/i_sp3adsp.i_dsp48a_wrap/i_primitive_PCOUT<27>_UNCONNECTED ;
  wire \NLW_BU2/U0/i_baseip.i_xbip_counter/i_dsp48.i_dsp/i_sp3adsp.i_dsp48a_wrap/i_primitive_PCOUT<26>_UNCONNECTED ;
  wire \NLW_BU2/U0/i_baseip.i_xbip_counter/i_dsp48.i_dsp/i_sp3adsp.i_dsp48a_wrap/i_primitive_PCOUT<25>_UNCONNECTED ;
  wire \NLW_BU2/U0/i_baseip.i_xbip_counter/i_dsp48.i_dsp/i_sp3adsp.i_dsp48a_wrap/i_primitive_PCOUT<24>_UNCONNECTED ;
  wire \NLW_BU2/U0/i_baseip.i_xbip_counter/i_dsp48.i_dsp/i_sp3adsp.i_dsp48a_wrap/i_primitive_PCOUT<23>_UNCONNECTED ;
  wire \NLW_BU2/U0/i_baseip.i_xbip_counter/i_dsp48.i_dsp/i_sp3adsp.i_dsp48a_wrap/i_primitive_PCOUT<22>_UNCONNECTED ;
  wire \NLW_BU2/U0/i_baseip.i_xbip_counter/i_dsp48.i_dsp/i_sp3adsp.i_dsp48a_wrap/i_primitive_PCOUT<21>_UNCONNECTED ;
  wire \NLW_BU2/U0/i_baseip.i_xbip_counter/i_dsp48.i_dsp/i_sp3adsp.i_dsp48a_wrap/i_primitive_PCOUT<20>_UNCONNECTED ;
  wire \NLW_BU2/U0/i_baseip.i_xbip_counter/i_dsp48.i_dsp/i_sp3adsp.i_dsp48a_wrap/i_primitive_PCOUT<19>_UNCONNECTED ;
  wire \NLW_BU2/U0/i_baseip.i_xbip_counter/i_dsp48.i_dsp/i_sp3adsp.i_dsp48a_wrap/i_primitive_PCOUT<18>_UNCONNECTED ;
  wire \NLW_BU2/U0/i_baseip.i_xbip_counter/i_dsp48.i_dsp/i_sp3adsp.i_dsp48a_wrap/i_primitive_PCOUT<17>_UNCONNECTED ;
  wire \NLW_BU2/U0/i_baseip.i_xbip_counter/i_dsp48.i_dsp/i_sp3adsp.i_dsp48a_wrap/i_primitive_PCOUT<16>_UNCONNECTED ;
  wire \NLW_BU2/U0/i_baseip.i_xbip_counter/i_dsp48.i_dsp/i_sp3adsp.i_dsp48a_wrap/i_primitive_PCOUT<15>_UNCONNECTED ;
  wire \NLW_BU2/U0/i_baseip.i_xbip_counter/i_dsp48.i_dsp/i_sp3adsp.i_dsp48a_wrap/i_primitive_PCOUT<14>_UNCONNECTED ;
  wire \NLW_BU2/U0/i_baseip.i_xbip_counter/i_dsp48.i_dsp/i_sp3adsp.i_dsp48a_wrap/i_primitive_PCOUT<13>_UNCONNECTED ;
  wire \NLW_BU2/U0/i_baseip.i_xbip_counter/i_dsp48.i_dsp/i_sp3adsp.i_dsp48a_wrap/i_primitive_PCOUT<12>_UNCONNECTED ;
  wire \NLW_BU2/U0/i_baseip.i_xbip_counter/i_dsp48.i_dsp/i_sp3adsp.i_dsp48a_wrap/i_primitive_PCOUT<11>_UNCONNECTED ;
  wire \NLW_BU2/U0/i_baseip.i_xbip_counter/i_dsp48.i_dsp/i_sp3adsp.i_dsp48a_wrap/i_primitive_PCOUT<10>_UNCONNECTED ;
  wire \NLW_BU2/U0/i_baseip.i_xbip_counter/i_dsp48.i_dsp/i_sp3adsp.i_dsp48a_wrap/i_primitive_PCOUT<9>_UNCONNECTED ;
  wire \NLW_BU2/U0/i_baseip.i_xbip_counter/i_dsp48.i_dsp/i_sp3adsp.i_dsp48a_wrap/i_primitive_PCOUT<8>_UNCONNECTED ;
  wire \NLW_BU2/U0/i_baseip.i_xbip_counter/i_dsp48.i_dsp/i_sp3adsp.i_dsp48a_wrap/i_primitive_PCOUT<7>_UNCONNECTED ;
  wire \NLW_BU2/U0/i_baseip.i_xbip_counter/i_dsp48.i_dsp/i_sp3adsp.i_dsp48a_wrap/i_primitive_PCOUT<6>_UNCONNECTED ;
  wire \NLW_BU2/U0/i_baseip.i_xbip_counter/i_dsp48.i_dsp/i_sp3adsp.i_dsp48a_wrap/i_primitive_PCOUT<5>_UNCONNECTED ;
  wire \NLW_BU2/U0/i_baseip.i_xbip_counter/i_dsp48.i_dsp/i_sp3adsp.i_dsp48a_wrap/i_primitive_PCOUT<4>_UNCONNECTED ;
  wire \NLW_BU2/U0/i_baseip.i_xbip_counter/i_dsp48.i_dsp/i_sp3adsp.i_dsp48a_wrap/i_primitive_PCOUT<3>_UNCONNECTED ;
  wire \NLW_BU2/U0/i_baseip.i_xbip_counter/i_dsp48.i_dsp/i_sp3adsp.i_dsp48a_wrap/i_primitive_PCOUT<2>_UNCONNECTED ;
  wire \NLW_BU2/U0/i_baseip.i_xbip_counter/i_dsp48.i_dsp/i_sp3adsp.i_dsp48a_wrap/i_primitive_PCOUT<1>_UNCONNECTED ;
  wire \NLW_BU2/U0/i_baseip.i_xbip_counter/i_dsp48.i_dsp/i_sp3adsp.i_dsp48a_wrap/i_primitive_PCOUT<0>_UNCONNECTED ;
  wire \NLW_BU2/U0/i_baseip.i_xbip_counter/i_dsp48.i_dsp/i_sp3adsp.i_dsp48a_wrap/i_primitive_M<35>_UNCONNECTED ;
  wire \NLW_BU2/U0/i_baseip.i_xbip_counter/i_dsp48.i_dsp/i_sp3adsp.i_dsp48a_wrap/i_primitive_M<34>_UNCONNECTED ;
  wire \NLW_BU2/U0/i_baseip.i_xbip_counter/i_dsp48.i_dsp/i_sp3adsp.i_dsp48a_wrap/i_primitive_M<33>_UNCONNECTED ;
  wire \NLW_BU2/U0/i_baseip.i_xbip_counter/i_dsp48.i_dsp/i_sp3adsp.i_dsp48a_wrap/i_primitive_M<32>_UNCONNECTED ;
  wire \NLW_BU2/U0/i_baseip.i_xbip_counter/i_dsp48.i_dsp/i_sp3adsp.i_dsp48a_wrap/i_primitive_M<31>_UNCONNECTED ;
  wire \NLW_BU2/U0/i_baseip.i_xbip_counter/i_dsp48.i_dsp/i_sp3adsp.i_dsp48a_wrap/i_primitive_M<30>_UNCONNECTED ;
  wire \NLW_BU2/U0/i_baseip.i_xbip_counter/i_dsp48.i_dsp/i_sp3adsp.i_dsp48a_wrap/i_primitive_M<29>_UNCONNECTED ;
  wire \NLW_BU2/U0/i_baseip.i_xbip_counter/i_dsp48.i_dsp/i_sp3adsp.i_dsp48a_wrap/i_primitive_M<28>_UNCONNECTED ;
  wire \NLW_BU2/U0/i_baseip.i_xbip_counter/i_dsp48.i_dsp/i_sp3adsp.i_dsp48a_wrap/i_primitive_M<27>_UNCONNECTED ;
  wire \NLW_BU2/U0/i_baseip.i_xbip_counter/i_dsp48.i_dsp/i_sp3adsp.i_dsp48a_wrap/i_primitive_M<26>_UNCONNECTED ;
  wire \NLW_BU2/U0/i_baseip.i_xbip_counter/i_dsp48.i_dsp/i_sp3adsp.i_dsp48a_wrap/i_primitive_M<25>_UNCONNECTED ;
  wire \NLW_BU2/U0/i_baseip.i_xbip_counter/i_dsp48.i_dsp/i_sp3adsp.i_dsp48a_wrap/i_primitive_M<24>_UNCONNECTED ;
  wire \NLW_BU2/U0/i_baseip.i_xbip_counter/i_dsp48.i_dsp/i_sp3adsp.i_dsp48a_wrap/i_primitive_M<23>_UNCONNECTED ;
  wire \NLW_BU2/U0/i_baseip.i_xbip_counter/i_dsp48.i_dsp/i_sp3adsp.i_dsp48a_wrap/i_primitive_M<22>_UNCONNECTED ;
  wire \NLW_BU2/U0/i_baseip.i_xbip_counter/i_dsp48.i_dsp/i_sp3adsp.i_dsp48a_wrap/i_primitive_M<21>_UNCONNECTED ;
  wire \NLW_BU2/U0/i_baseip.i_xbip_counter/i_dsp48.i_dsp/i_sp3adsp.i_dsp48a_wrap/i_primitive_M<20>_UNCONNECTED ;
  wire \NLW_BU2/U0/i_baseip.i_xbip_counter/i_dsp48.i_dsp/i_sp3adsp.i_dsp48a_wrap/i_primitive_M<19>_UNCONNECTED ;
  wire \NLW_BU2/U0/i_baseip.i_xbip_counter/i_dsp48.i_dsp/i_sp3adsp.i_dsp48a_wrap/i_primitive_M<18>_UNCONNECTED ;
  wire \NLW_BU2/U0/i_baseip.i_xbip_counter/i_dsp48.i_dsp/i_sp3adsp.i_dsp48a_wrap/i_primitive_M<17>_UNCONNECTED ;
  wire \NLW_BU2/U0/i_baseip.i_xbip_counter/i_dsp48.i_dsp/i_sp3adsp.i_dsp48a_wrap/i_primitive_M<16>_UNCONNECTED ;
  wire \NLW_BU2/U0/i_baseip.i_xbip_counter/i_dsp48.i_dsp/i_sp3adsp.i_dsp48a_wrap/i_primitive_M<15>_UNCONNECTED ;
  wire \NLW_BU2/U0/i_baseip.i_xbip_counter/i_dsp48.i_dsp/i_sp3adsp.i_dsp48a_wrap/i_primitive_M<14>_UNCONNECTED ;
  wire \NLW_BU2/U0/i_baseip.i_xbip_counter/i_dsp48.i_dsp/i_sp3adsp.i_dsp48a_wrap/i_primitive_M<13>_UNCONNECTED ;
  wire \NLW_BU2/U0/i_baseip.i_xbip_counter/i_dsp48.i_dsp/i_sp3adsp.i_dsp48a_wrap/i_primitive_M<12>_UNCONNECTED ;
  wire \NLW_BU2/U0/i_baseip.i_xbip_counter/i_dsp48.i_dsp/i_sp3adsp.i_dsp48a_wrap/i_primitive_M<11>_UNCONNECTED ;
  wire \NLW_BU2/U0/i_baseip.i_xbip_counter/i_dsp48.i_dsp/i_sp3adsp.i_dsp48a_wrap/i_primitive_M<10>_UNCONNECTED ;
  wire \NLW_BU2/U0/i_baseip.i_xbip_counter/i_dsp48.i_dsp/i_sp3adsp.i_dsp48a_wrap/i_primitive_M<9>_UNCONNECTED ;
  wire \NLW_BU2/U0/i_baseip.i_xbip_counter/i_dsp48.i_dsp/i_sp3adsp.i_dsp48a_wrap/i_primitive_M<8>_UNCONNECTED ;
  wire \NLW_BU2/U0/i_baseip.i_xbip_counter/i_dsp48.i_dsp/i_sp3adsp.i_dsp48a_wrap/i_primitive_M<7>_UNCONNECTED ;
  wire \NLW_BU2/U0/i_baseip.i_xbip_counter/i_dsp48.i_dsp/i_sp3adsp.i_dsp48a_wrap/i_primitive_M<6>_UNCONNECTED ;
  wire \NLW_BU2/U0/i_baseip.i_xbip_counter/i_dsp48.i_dsp/i_sp3adsp.i_dsp48a_wrap/i_primitive_M<5>_UNCONNECTED ;
  wire \NLW_BU2/U0/i_baseip.i_xbip_counter/i_dsp48.i_dsp/i_sp3adsp.i_dsp48a_wrap/i_primitive_M<4>_UNCONNECTED ;
  wire \NLW_BU2/U0/i_baseip.i_xbip_counter/i_dsp48.i_dsp/i_sp3adsp.i_dsp48a_wrap/i_primitive_M<3>_UNCONNECTED ;
  wire \NLW_BU2/U0/i_baseip.i_xbip_counter/i_dsp48.i_dsp/i_sp3adsp.i_dsp48a_wrap/i_primitive_M<2>_UNCONNECTED ;
  wire \NLW_BU2/U0/i_baseip.i_xbip_counter/i_dsp48.i_dsp/i_sp3adsp.i_dsp48a_wrap/i_primitive_M<1>_UNCONNECTED ;
  wire \NLW_BU2/U0/i_baseip.i_xbip_counter/i_dsp48.i_dsp/i_sp3adsp.i_dsp48a_wrap/i_primitive_M<0>_UNCONNECTED ;
  wire [31 : 0] NlwRenamedSig_OI_q;
  wire [31 : 0] \BU2/l ;
  assign
    q[31] = NlwRenamedSig_OI_q[31],
    q[30] = NlwRenamedSig_OI_q[30],
    q[29] = NlwRenamedSig_OI_q[29],
    q[28] = NlwRenamedSig_OI_q[28],
    q[27] = NlwRenamedSig_OI_q[27],
    q[26] = NlwRenamedSig_OI_q[26],
    q[25] = NlwRenamedSig_OI_q[25],
    q[24] = NlwRenamedSig_OI_q[24],
    q[23] = NlwRenamedSig_OI_q[23],
    q[22] = NlwRenamedSig_OI_q[22],
    q[21] = NlwRenamedSig_OI_q[21],
    q[20] = NlwRenamedSig_OI_q[20],
    q[19] = NlwRenamedSig_OI_q[19],
    q[18] = NlwRenamedSig_OI_q[18],
    q[17] = NlwRenamedSig_OI_q[17],
    q[16] = NlwRenamedSig_OI_q[16],
    q[15] = NlwRenamedSig_OI_q[15],
    q[14] = NlwRenamedSig_OI_q[14],
    q[13] = NlwRenamedSig_OI_q[13],
    q[12] = NlwRenamedSig_OI_q[12],
    q[11] = NlwRenamedSig_OI_q[11],
    q[10] = NlwRenamedSig_OI_q[10],
    q[9] = NlwRenamedSig_OI_q[9],
    q[8] = NlwRenamedSig_OI_q[8],
    q[7] = NlwRenamedSig_OI_q[7],
    q[6] = NlwRenamedSig_OI_q[6],
    q[5] = NlwRenamedSig_OI_q[5],
    q[4] = NlwRenamedSig_OI_q[4],
    q[3] = NlwRenamedSig_OI_q[3],
    q[2] = NlwRenamedSig_OI_q[2],
    q[1] = NlwRenamedSig_OI_q[1],
    q[0] = NlwRenamedSig_OI_q[0];
  VCC   VCC_0 (
    .P(NLW_VCC_P_UNCONNECTED)
  );
  GND   GND_1 (
    .G(NLW_GND_G_UNCONNECTED)
  );
  LUT6 #(
    .INIT ( 64'h0000008000000000 ))
  \BU2/U0/i_baseip.i_xbip_counter/i_thresh_detect.i_norm.i_gate/tier_gen[1].i_tier/loop_tiles[0].i_tile/lut_o<0>1  (
    .I0(NlwRenamedSig_OI_q[0]),
    .I1(NlwRenamedSig_OI_q[1]),
    .I2(NlwRenamedSig_OI_q[2]),
    .I3(NlwRenamedSig_OI_q[3]),
    .I4(NlwRenamedSig_OI_q[4]),
    .I5(NlwRenamedSig_OI_q[5]),
    .O(\BU2/U0/i_baseip.i_xbip_counter/i_thresh_detect.i_norm.i_gate/tier_gen[1].i_tier/loop_tiles[0].i_tile/lut_o<0> )
  );
  LUT6 #(
    .INIT ( 64'h0010000000000000 ))
  \BU2/U0/i_baseip.i_xbip_counter/i_thresh_detect.i_norm.i_gate/tier_gen[1].i_tier/loop_tiles[0].i_tile/lut_o<1>1  (
    .I0(NlwRenamedSig_OI_q[11]),
    .I1(NlwRenamedSig_OI_q[7]),
    .I2(NlwRenamedSig_OI_q[10]),
    .I3(NlwRenamedSig_OI_q[9]),
    .I4(NlwRenamedSig_OI_q[8]),
    .I5(NlwRenamedSig_OI_q[6]),
    .O(\BU2/U0/i_baseip.i_xbip_counter/i_thresh_detect.i_norm.i_gate/tier_gen[1].i_tier/loop_tiles[0].i_tile/lut_o<1> )
  );
  LUT6 #(
    .INIT ( 64'h0010000000000000 ))
  \BU2/U0/i_baseip.i_xbip_counter/i_thresh_detect.i_norm.i_gate/tier_gen[1].i_tier/loop_tiles[0].i_tile/lut_o<2>1  (
    .I0(NlwRenamedSig_OI_q[12]),
    .I1(NlwRenamedSig_OI_q[13]),
    .I2(NlwRenamedSig_OI_q[14]),
    .I3(NlwRenamedSig_OI_q[15]),
    .I4(NlwRenamedSig_OI_q[16]),
    .I5(NlwRenamedSig_OI_q[17]),
    .O(\BU2/U0/i_baseip.i_xbip_counter/i_thresh_detect.i_norm.i_gate/tier_gen[1].i_tier/loop_tiles[0].i_tile/lut_o<2> )
  );
  LUT6 #(
    .INIT ( 64'h0000000000000002 ))
  \BU2/U0/i_baseip.i_xbip_counter/i_thresh_detect.i_norm.i_gate/tier_gen[1].i_tier/loop_tiles[0].i_tile/lut_o<3>1  (
    .I0(NlwRenamedSig_OI_q[21]),
    .I1(NlwRenamedSig_OI_q[18]),
    .I2(NlwRenamedSig_OI_q[19]),
    .I3(NlwRenamedSig_OI_q[20]),
    .I4(NlwRenamedSig_OI_q[22]),
    .I5(NlwRenamedSig_OI_q[23]),
    .O(\BU2/U0/i_baseip.i_xbip_counter/i_thresh_detect.i_norm.i_gate/tier_gen[1].i_tier/loop_tiles[0].i_tile/lut_o<3> )
  );
  LUT6 #(
    .INIT ( 64'h0000000000000002 ))
  \BU2/U0/i_baseip.i_xbip_counter/i_thresh_detect.i_norm.i_gate/tier_gen[1].i_tier/loop_tiles[0].i_tile/lut_o<4>1  (
    .I0(NlwRenamedSig_OI_q[24]),
    .I1(NlwRenamedSig_OI_q[25]),
    .I2(NlwRenamedSig_OI_q[26]),
    .I3(NlwRenamedSig_OI_q[27]),
    .I4(NlwRenamedSig_OI_q[28]),
    .I5(NlwRenamedSig_OI_q[29]),
    .O(\BU2/U0/i_baseip.i_xbip_counter/i_thresh_detect.i_norm.i_gate/tier_gen[1].i_tier/loop_tiles[0].i_tile/lut_o<4> )
  );
  LUT2 #(
    .INIT ( 4'h4 ))
  \BU2/U0/i_baseip.i_xbip_counter/i_thresh_detect.i_norm.i_gate/tier_gen[1].i_tier/loop_tiles[0].i_tile/lut_o<5>1  (
    .I0(NlwRenamedSig_OI_q[30]),
    .I1(NlwRenamedSig_OI_q[31]),
    .O(\BU2/U0/i_baseip.i_xbip_counter/i_thresh_detect.i_norm.i_gate/tier_gen[1].i_tier/loop_tiles[0].i_tile/lut_o<5> )
  );
  MUXCY 
  \BU2/U0/i_baseip.i_xbip_counter/i_thresh_detect.i_norm.i_gate/tier_gen[1].i_tier/loop_tiles[0].i_tile/opt_carry_tile.and_or.carry_muxs[0].i_mux  (
    .CI(\BU2/U0/i_baseip.i_xbip_counter/i_thresh_detect.i_norm.i_gate/tier_gen[1].i_tier/loop_tiles[0].i_tile/async_o<1> ),
    .DI(\BU2/N0 ),
    .S(\BU2/U0/i_baseip.i_xbip_counter/i_thresh_detect.i_norm.i_gate/tier_gen[1].i_tier/loop_tiles[0].i_tile/lut_o<0> ),
    .O(thresh0)
  );
  MUXCY 
  \BU2/U0/i_baseip.i_xbip_counter/i_thresh_detect.i_norm.i_gate/tier_gen[1].i_tier/loop_tiles[0].i_tile/opt_carry_tile.and_or.carry_muxs[1].i_mux  (
    .CI(\BU2/U0/i_baseip.i_xbip_counter/i_thresh_detect.i_norm.i_gate/tier_gen[1].i_tier/loop_tiles[0].i_tile/async_o<2> ),
    .DI(\BU2/N0 ),
    .S(\BU2/U0/i_baseip.i_xbip_counter/i_thresh_detect.i_norm.i_gate/tier_gen[1].i_tier/loop_tiles[0].i_tile/lut_o<1> ),
    .O(\BU2/U0/i_baseip.i_xbip_counter/i_thresh_detect.i_norm.i_gate/tier_gen[1].i_tier/loop_tiles[0].i_tile/async_o<1> )
  );
  MUXCY 
  \BU2/U0/i_baseip.i_xbip_counter/i_thresh_detect.i_norm.i_gate/tier_gen[1].i_tier/loop_tiles[0].i_tile/opt_carry_tile.and_or.carry_muxs[2].i_mux  (
    .CI(\BU2/U0/i_baseip.i_xbip_counter/i_thresh_detect.i_norm.i_gate/tier_gen[1].i_tier/loop_tiles[0].i_tile/async_o<3> ),
    .DI(\BU2/N0 ),
    .S(\BU2/U0/i_baseip.i_xbip_counter/i_thresh_detect.i_norm.i_gate/tier_gen[1].i_tier/loop_tiles[0].i_tile/lut_o<2> ),
    .O(\BU2/U0/i_baseip.i_xbip_counter/i_thresh_detect.i_norm.i_gate/tier_gen[1].i_tier/loop_tiles[0].i_tile/async_o<2> )
  );
  MUXCY 
  \BU2/U0/i_baseip.i_xbip_counter/i_thresh_detect.i_norm.i_gate/tier_gen[1].i_tier/loop_tiles[0].i_tile/opt_carry_tile.and_or.carry_muxs[3].i_mux  (
    .CI(\BU2/U0/i_baseip.i_xbip_counter/i_thresh_detect.i_norm.i_gate/tier_gen[1].i_tier/loop_tiles[0].i_tile/async_o<4> ),
    .DI(\BU2/N0 ),
    .S(\BU2/U0/i_baseip.i_xbip_counter/i_thresh_detect.i_norm.i_gate/tier_gen[1].i_tier/loop_tiles[0].i_tile/lut_o<3> ),
    .O(\BU2/U0/i_baseip.i_xbip_counter/i_thresh_detect.i_norm.i_gate/tier_gen[1].i_tier/loop_tiles[0].i_tile/async_o<3> )
  );
  MUXCY 
  \BU2/U0/i_baseip.i_xbip_counter/i_thresh_detect.i_norm.i_gate/tier_gen[1].i_tier/loop_tiles[0].i_tile/opt_carry_tile.and_or.carry_muxs[4].i_mux  (
    .CI(\BU2/U0/i_baseip.i_xbip_counter/i_thresh_detect.i_norm.i_gate/tier_gen[1].i_tier/loop_tiles[0].i_tile/async_o<5> ),
    .DI(\BU2/N0 ),
    .S(\BU2/U0/i_baseip.i_xbip_counter/i_thresh_detect.i_norm.i_gate/tier_gen[1].i_tier/loop_tiles[0].i_tile/lut_o<4> ),
    .O(\BU2/U0/i_baseip.i_xbip_counter/i_thresh_detect.i_norm.i_gate/tier_gen[1].i_tier/loop_tiles[0].i_tile/async_o<4> )
  );
  MUXCY 
  \BU2/U0/i_baseip.i_xbip_counter/i_thresh_detect.i_norm.i_gate/tier_gen[1].i_tier/loop_tiles[0].i_tile/opt_carry_tile.and_or.carry_muxs[5].i_mux  (
    .CI(\BU2/N1 ),
    .DI(\BU2/N0 ),
    .S(\BU2/U0/i_baseip.i_xbip_counter/i_thresh_detect.i_norm.i_gate/tier_gen[1].i_tier/loop_tiles[0].i_tile/lut_o<5> ),
    .O(\BU2/U0/i_baseip.i_xbip_counter/i_thresh_detect.i_norm.i_gate/tier_gen[1].i_tier/loop_tiles[0].i_tile/async_o<5> )
  );
  DSP48A1 #(
    .A0REG ( 0 ),
    .A1REG ( 0 ),
    .B0REG ( 0 ),
    .B1REG ( 0 ),
    .CARRYINREG ( 0 ),
    .CARRYINSEL ( "OPMODE5" ),
    .CREG ( 0 ),
    .DREG ( 0 ),
    .MREG ( 0 ),
    .OPMODEREG ( 0 ),
    .PREG ( 1 ),
    .RSTTYPE ( "SYNC" ),
    .CARRYOUTREG ( 0 ))
  \BU2/U0/i_baseip.i_xbip_counter/i_dsp48.i_dsp/i_sp3adsp.i_dsp48a_wrap/i_primitive  (
    .CECARRYIN(\BU2/N0 ),
    .RSTC(\BU2/N0 ),
    .RSTCARRYIN(\BU2/N0 ),
    .CED(\BU2/N0 ),
    .RSTD(\BU2/N0 ),
    .CEOPMODE(\BU2/N0 ),
    .CEC(\BU2/N0 ),
    .CARRYOUTF(\NLW_BU2/U0/i_baseip.i_xbip_counter/i_dsp48.i_dsp/i_sp3adsp.i_dsp48a_wrap/i_primitive_CARRYOUTF_UNCONNECTED ),
    .RSTOPMODE(\BU2/N0 ),
    .RSTM(\BU2/N0 ),
    .CLK(clk),
    .RSTB(\BU2/N0 ),
    .CEM(\BU2/N0 ),
    .CEB(\BU2/N0 ),
    .CARRYIN(\BU2/N0 ),
    .CEP(ce),
    .CEA(\BU2/N0 ),
    .CARRYOUT(\NLW_BU2/U0/i_baseip.i_xbip_counter/i_dsp48.i_dsp/i_sp3adsp.i_dsp48a_wrap/i_primitive_CARRYOUT_UNCONNECTED ),
    .RSTA(\BU2/N0 ),
    .RSTP(sclr),
    .B({\BU2/N0 , \BU2/N0 , \BU2/N0 , \BU2/N0 , \BU2/N0 , \BU2/N0 , \BU2/N0 , \BU2/N0 , \BU2/N0 , \BU2/N0 , \BU2/N0 , \BU2/N0 , \BU2/N0 , \BU2/N0 , 
\BU2/N0 , \BU2/N0 , \BU2/N0 , \BU2/N1 }),
    .BCOUT({\NLW_BU2/U0/i_baseip.i_xbip_counter/i_dsp48.i_dsp/i_sp3adsp.i_dsp48a_wrap/i_primitive_BCOUT<17>_UNCONNECTED , 
\NLW_BU2/U0/i_baseip.i_xbip_counter/i_dsp48.i_dsp/i_sp3adsp.i_dsp48a_wrap/i_primitive_BCOUT<16>_UNCONNECTED , 
\NLW_BU2/U0/i_baseip.i_xbip_counter/i_dsp48.i_dsp/i_sp3adsp.i_dsp48a_wrap/i_primitive_BCOUT<15>_UNCONNECTED , 
\NLW_BU2/U0/i_baseip.i_xbip_counter/i_dsp48.i_dsp/i_sp3adsp.i_dsp48a_wrap/i_primitive_BCOUT<14>_UNCONNECTED , 
\NLW_BU2/U0/i_baseip.i_xbip_counter/i_dsp48.i_dsp/i_sp3adsp.i_dsp48a_wrap/i_primitive_BCOUT<13>_UNCONNECTED , 
\NLW_BU2/U0/i_baseip.i_xbip_counter/i_dsp48.i_dsp/i_sp3adsp.i_dsp48a_wrap/i_primitive_BCOUT<12>_UNCONNECTED , 
\NLW_BU2/U0/i_baseip.i_xbip_counter/i_dsp48.i_dsp/i_sp3adsp.i_dsp48a_wrap/i_primitive_BCOUT<11>_UNCONNECTED , 
\NLW_BU2/U0/i_baseip.i_xbip_counter/i_dsp48.i_dsp/i_sp3adsp.i_dsp48a_wrap/i_primitive_BCOUT<10>_UNCONNECTED , 
\NLW_BU2/U0/i_baseip.i_xbip_counter/i_dsp48.i_dsp/i_sp3adsp.i_dsp48a_wrap/i_primitive_BCOUT<9>_UNCONNECTED , 
\NLW_BU2/U0/i_baseip.i_xbip_counter/i_dsp48.i_dsp/i_sp3adsp.i_dsp48a_wrap/i_primitive_BCOUT<8>_UNCONNECTED , 
\NLW_BU2/U0/i_baseip.i_xbip_counter/i_dsp48.i_dsp/i_sp3adsp.i_dsp48a_wrap/i_primitive_BCOUT<7>_UNCONNECTED , 
\NLW_BU2/U0/i_baseip.i_xbip_counter/i_dsp48.i_dsp/i_sp3adsp.i_dsp48a_wrap/i_primitive_BCOUT<6>_UNCONNECTED , 
\NLW_BU2/U0/i_baseip.i_xbip_counter/i_dsp48.i_dsp/i_sp3adsp.i_dsp48a_wrap/i_primitive_BCOUT<5>_UNCONNECTED , 
\NLW_BU2/U0/i_baseip.i_xbip_counter/i_dsp48.i_dsp/i_sp3adsp.i_dsp48a_wrap/i_primitive_BCOUT<4>_UNCONNECTED , 
\NLW_BU2/U0/i_baseip.i_xbip_counter/i_dsp48.i_dsp/i_sp3adsp.i_dsp48a_wrap/i_primitive_BCOUT<3>_UNCONNECTED , 
\NLW_BU2/U0/i_baseip.i_xbip_counter/i_dsp48.i_dsp/i_sp3adsp.i_dsp48a_wrap/i_primitive_BCOUT<2>_UNCONNECTED , 
\NLW_BU2/U0/i_baseip.i_xbip_counter/i_dsp48.i_dsp/i_sp3adsp.i_dsp48a_wrap/i_primitive_BCOUT<1>_UNCONNECTED , 
\NLW_BU2/U0/i_baseip.i_xbip_counter/i_dsp48.i_dsp/i_sp3adsp.i_dsp48a_wrap/i_primitive_BCOUT<0>_UNCONNECTED }),
    .PCIN({\BU2/N0 , \BU2/N0 , \BU2/N0 , \BU2/N0 , \BU2/N0 , \BU2/N0 , \BU2/N0 , \BU2/N0 , \BU2/N0 , \BU2/N0 , \BU2/N0 , \BU2/N0 , \BU2/N0 , \BU2/N0 
, \BU2/N0 , \BU2/N0 , \BU2/N0 , \BU2/N0 , \BU2/N0 , \BU2/N0 , \BU2/N0 , \BU2/N0 , \BU2/N0 , \BU2/N0 , \BU2/N0 , \BU2/N0 , \BU2/N0 , \BU2/N0 , \BU2/N0 
, \BU2/N0 , \BU2/N0 , \BU2/N0 , \BU2/N0 , \BU2/N0 , \BU2/N0 , \BU2/N0 , \BU2/N0 , \BU2/N0 , \BU2/N0 , \BU2/N0 , \BU2/N0 , \BU2/N0 , \BU2/N0 , \BU2/N0 
, \BU2/N0 , \BU2/N0 , \BU2/N0 , \BU2/N0 }),
    .C({\BU2/N0 , \BU2/N0 , \BU2/N0 , \BU2/N0 , \BU2/N0 , \BU2/N0 , \BU2/N0 , \BU2/N0 , \BU2/N0 , \BU2/N0 , \BU2/N0 , \BU2/N0 , \BU2/N0 , \BU2/N0 , 
\BU2/N0 , \BU2/N0 , \BU2/l [31], \BU2/l [30], \BU2/l [29], \BU2/l [28], \BU2/l [27], \BU2/l [26], \BU2/l [25], \BU2/l [24], \BU2/l [23], \BU2/l [22], 
\BU2/l [21], \BU2/l [20], \BU2/l [19], \BU2/l [18], \BU2/l [17], \BU2/l [16], \BU2/l [15], \BU2/l [14], \BU2/l [13], \BU2/l [12], \BU2/l [11], 
\BU2/l [10], \BU2/l [9], \BU2/l [8], \BU2/l [7], \BU2/l [6], \BU2/l [5], \BU2/l [4], \BU2/l [3], \BU2/l [2], \BU2/l [1], \BU2/l [0]}),
    .P({\NLW_BU2/U0/i_baseip.i_xbip_counter/i_dsp48.i_dsp/i_sp3adsp.i_dsp48a_wrap/i_primitive_P<47>_UNCONNECTED , 
\NLW_BU2/U0/i_baseip.i_xbip_counter/i_dsp48.i_dsp/i_sp3adsp.i_dsp48a_wrap/i_primitive_P<46>_UNCONNECTED , 
\NLW_BU2/U0/i_baseip.i_xbip_counter/i_dsp48.i_dsp/i_sp3adsp.i_dsp48a_wrap/i_primitive_P<45>_UNCONNECTED , 
\NLW_BU2/U0/i_baseip.i_xbip_counter/i_dsp48.i_dsp/i_sp3adsp.i_dsp48a_wrap/i_primitive_P<44>_UNCONNECTED , 
\NLW_BU2/U0/i_baseip.i_xbip_counter/i_dsp48.i_dsp/i_sp3adsp.i_dsp48a_wrap/i_primitive_P<43>_UNCONNECTED , 
\NLW_BU2/U0/i_baseip.i_xbip_counter/i_dsp48.i_dsp/i_sp3adsp.i_dsp48a_wrap/i_primitive_P<42>_UNCONNECTED , 
\NLW_BU2/U0/i_baseip.i_xbip_counter/i_dsp48.i_dsp/i_sp3adsp.i_dsp48a_wrap/i_primitive_P<41>_UNCONNECTED , 
\NLW_BU2/U0/i_baseip.i_xbip_counter/i_dsp48.i_dsp/i_sp3adsp.i_dsp48a_wrap/i_primitive_P<40>_UNCONNECTED , 
\NLW_BU2/U0/i_baseip.i_xbip_counter/i_dsp48.i_dsp/i_sp3adsp.i_dsp48a_wrap/i_primitive_P<39>_UNCONNECTED , 
\NLW_BU2/U0/i_baseip.i_xbip_counter/i_dsp48.i_dsp/i_sp3adsp.i_dsp48a_wrap/i_primitive_P<38>_UNCONNECTED , 
\NLW_BU2/U0/i_baseip.i_xbip_counter/i_dsp48.i_dsp/i_sp3adsp.i_dsp48a_wrap/i_primitive_P<37>_UNCONNECTED , 
\NLW_BU2/U0/i_baseip.i_xbip_counter/i_dsp48.i_dsp/i_sp3adsp.i_dsp48a_wrap/i_primitive_P<36>_UNCONNECTED , 
\NLW_BU2/U0/i_baseip.i_xbip_counter/i_dsp48.i_dsp/i_sp3adsp.i_dsp48a_wrap/i_primitive_P<35>_UNCONNECTED , 
\NLW_BU2/U0/i_baseip.i_xbip_counter/i_dsp48.i_dsp/i_sp3adsp.i_dsp48a_wrap/i_primitive_P<34>_UNCONNECTED , 
\NLW_BU2/U0/i_baseip.i_xbip_counter/i_dsp48.i_dsp/i_sp3adsp.i_dsp48a_wrap/i_primitive_P<33>_UNCONNECTED , 
\NLW_BU2/U0/i_baseip.i_xbip_counter/i_dsp48.i_dsp/i_sp3adsp.i_dsp48a_wrap/i_primitive_P<32>_UNCONNECTED , NlwRenamedSig_OI_q[31], 
NlwRenamedSig_OI_q[30], NlwRenamedSig_OI_q[29], NlwRenamedSig_OI_q[28], NlwRenamedSig_OI_q[27], NlwRenamedSig_OI_q[26], NlwRenamedSig_OI_q[25], 
NlwRenamedSig_OI_q[24], NlwRenamedSig_OI_q[23], NlwRenamedSig_OI_q[22], NlwRenamedSig_OI_q[21], NlwRenamedSig_OI_q[20], NlwRenamedSig_OI_q[19], 
NlwRenamedSig_OI_q[18], NlwRenamedSig_OI_q[17], NlwRenamedSig_OI_q[16], NlwRenamedSig_OI_q[15], NlwRenamedSig_OI_q[14], NlwRenamedSig_OI_q[13], 
NlwRenamedSig_OI_q[12], NlwRenamedSig_OI_q[11], NlwRenamedSig_OI_q[10], NlwRenamedSig_OI_q[9], NlwRenamedSig_OI_q[8], NlwRenamedSig_OI_q[7], 
NlwRenamedSig_OI_q[6], NlwRenamedSig_OI_q[5], NlwRenamedSig_OI_q[4], NlwRenamedSig_OI_q[3], NlwRenamedSig_OI_q[2], NlwRenamedSig_OI_q[1], 
NlwRenamedSig_OI_q[0]}),
    .OPMODE({\BU2/N0 , \BU2/N0 , \BU2/N0 , \BU2/N0 , \BU2/N1 , \BU2/N0 , \BU2/N1 , \BU2/N1 }),
    .D({\BU2/N0 , \BU2/N0 , \BU2/N0 , \BU2/N0 , \BU2/N0 , \BU2/N0 , \BU2/N0 , \BU2/N0 , \BU2/N0 , \BU2/N0 , \BU2/N0 , \BU2/N0 , \BU2/N0 , \BU2/N0 , 
\BU2/N0 , \BU2/N0 , \BU2/N0 , \BU2/N0 }),
    .PCOUT({\NLW_BU2/U0/i_baseip.i_xbip_counter/i_dsp48.i_dsp/i_sp3adsp.i_dsp48a_wrap/i_primitive_PCOUT<47>_UNCONNECTED , 
\NLW_BU2/U0/i_baseip.i_xbip_counter/i_dsp48.i_dsp/i_sp3adsp.i_dsp48a_wrap/i_primitive_PCOUT<46>_UNCONNECTED , 
\NLW_BU2/U0/i_baseip.i_xbip_counter/i_dsp48.i_dsp/i_sp3adsp.i_dsp48a_wrap/i_primitive_PCOUT<45>_UNCONNECTED , 
\NLW_BU2/U0/i_baseip.i_xbip_counter/i_dsp48.i_dsp/i_sp3adsp.i_dsp48a_wrap/i_primitive_PCOUT<44>_UNCONNECTED , 
\NLW_BU2/U0/i_baseip.i_xbip_counter/i_dsp48.i_dsp/i_sp3adsp.i_dsp48a_wrap/i_primitive_PCOUT<43>_UNCONNECTED , 
\NLW_BU2/U0/i_baseip.i_xbip_counter/i_dsp48.i_dsp/i_sp3adsp.i_dsp48a_wrap/i_primitive_PCOUT<42>_UNCONNECTED , 
\NLW_BU2/U0/i_baseip.i_xbip_counter/i_dsp48.i_dsp/i_sp3adsp.i_dsp48a_wrap/i_primitive_PCOUT<41>_UNCONNECTED , 
\NLW_BU2/U0/i_baseip.i_xbip_counter/i_dsp48.i_dsp/i_sp3adsp.i_dsp48a_wrap/i_primitive_PCOUT<40>_UNCONNECTED , 
\NLW_BU2/U0/i_baseip.i_xbip_counter/i_dsp48.i_dsp/i_sp3adsp.i_dsp48a_wrap/i_primitive_PCOUT<39>_UNCONNECTED , 
\NLW_BU2/U0/i_baseip.i_xbip_counter/i_dsp48.i_dsp/i_sp3adsp.i_dsp48a_wrap/i_primitive_PCOUT<38>_UNCONNECTED , 
\NLW_BU2/U0/i_baseip.i_xbip_counter/i_dsp48.i_dsp/i_sp3adsp.i_dsp48a_wrap/i_primitive_PCOUT<37>_UNCONNECTED , 
\NLW_BU2/U0/i_baseip.i_xbip_counter/i_dsp48.i_dsp/i_sp3adsp.i_dsp48a_wrap/i_primitive_PCOUT<36>_UNCONNECTED , 
\NLW_BU2/U0/i_baseip.i_xbip_counter/i_dsp48.i_dsp/i_sp3adsp.i_dsp48a_wrap/i_primitive_PCOUT<35>_UNCONNECTED , 
\NLW_BU2/U0/i_baseip.i_xbip_counter/i_dsp48.i_dsp/i_sp3adsp.i_dsp48a_wrap/i_primitive_PCOUT<34>_UNCONNECTED , 
\NLW_BU2/U0/i_baseip.i_xbip_counter/i_dsp48.i_dsp/i_sp3adsp.i_dsp48a_wrap/i_primitive_PCOUT<33>_UNCONNECTED , 
\NLW_BU2/U0/i_baseip.i_xbip_counter/i_dsp48.i_dsp/i_sp3adsp.i_dsp48a_wrap/i_primitive_PCOUT<32>_UNCONNECTED , 
\NLW_BU2/U0/i_baseip.i_xbip_counter/i_dsp48.i_dsp/i_sp3adsp.i_dsp48a_wrap/i_primitive_PCOUT<31>_UNCONNECTED , 
\NLW_BU2/U0/i_baseip.i_xbip_counter/i_dsp48.i_dsp/i_sp3adsp.i_dsp48a_wrap/i_primitive_PCOUT<30>_UNCONNECTED , 
\NLW_BU2/U0/i_baseip.i_xbip_counter/i_dsp48.i_dsp/i_sp3adsp.i_dsp48a_wrap/i_primitive_PCOUT<29>_UNCONNECTED , 
\NLW_BU2/U0/i_baseip.i_xbip_counter/i_dsp48.i_dsp/i_sp3adsp.i_dsp48a_wrap/i_primitive_PCOUT<28>_UNCONNECTED , 
\NLW_BU2/U0/i_baseip.i_xbip_counter/i_dsp48.i_dsp/i_sp3adsp.i_dsp48a_wrap/i_primitive_PCOUT<27>_UNCONNECTED , 
\NLW_BU2/U0/i_baseip.i_xbip_counter/i_dsp48.i_dsp/i_sp3adsp.i_dsp48a_wrap/i_primitive_PCOUT<26>_UNCONNECTED , 
\NLW_BU2/U0/i_baseip.i_xbip_counter/i_dsp48.i_dsp/i_sp3adsp.i_dsp48a_wrap/i_primitive_PCOUT<25>_UNCONNECTED , 
\NLW_BU2/U0/i_baseip.i_xbip_counter/i_dsp48.i_dsp/i_sp3adsp.i_dsp48a_wrap/i_primitive_PCOUT<24>_UNCONNECTED , 
\NLW_BU2/U0/i_baseip.i_xbip_counter/i_dsp48.i_dsp/i_sp3adsp.i_dsp48a_wrap/i_primitive_PCOUT<23>_UNCONNECTED , 
\NLW_BU2/U0/i_baseip.i_xbip_counter/i_dsp48.i_dsp/i_sp3adsp.i_dsp48a_wrap/i_primitive_PCOUT<22>_UNCONNECTED , 
\NLW_BU2/U0/i_baseip.i_xbip_counter/i_dsp48.i_dsp/i_sp3adsp.i_dsp48a_wrap/i_primitive_PCOUT<21>_UNCONNECTED , 
\NLW_BU2/U0/i_baseip.i_xbip_counter/i_dsp48.i_dsp/i_sp3adsp.i_dsp48a_wrap/i_primitive_PCOUT<20>_UNCONNECTED , 
\NLW_BU2/U0/i_baseip.i_xbip_counter/i_dsp48.i_dsp/i_sp3adsp.i_dsp48a_wrap/i_primitive_PCOUT<19>_UNCONNECTED , 
\NLW_BU2/U0/i_baseip.i_xbip_counter/i_dsp48.i_dsp/i_sp3adsp.i_dsp48a_wrap/i_primitive_PCOUT<18>_UNCONNECTED , 
\NLW_BU2/U0/i_baseip.i_xbip_counter/i_dsp48.i_dsp/i_sp3adsp.i_dsp48a_wrap/i_primitive_PCOUT<17>_UNCONNECTED , 
\NLW_BU2/U0/i_baseip.i_xbip_counter/i_dsp48.i_dsp/i_sp3adsp.i_dsp48a_wrap/i_primitive_PCOUT<16>_UNCONNECTED , 
\NLW_BU2/U0/i_baseip.i_xbip_counter/i_dsp48.i_dsp/i_sp3adsp.i_dsp48a_wrap/i_primitive_PCOUT<15>_UNCONNECTED , 
\NLW_BU2/U0/i_baseip.i_xbip_counter/i_dsp48.i_dsp/i_sp3adsp.i_dsp48a_wrap/i_primitive_PCOUT<14>_UNCONNECTED , 
\NLW_BU2/U0/i_baseip.i_xbip_counter/i_dsp48.i_dsp/i_sp3adsp.i_dsp48a_wrap/i_primitive_PCOUT<13>_UNCONNECTED , 
\NLW_BU2/U0/i_baseip.i_xbip_counter/i_dsp48.i_dsp/i_sp3adsp.i_dsp48a_wrap/i_primitive_PCOUT<12>_UNCONNECTED , 
\NLW_BU2/U0/i_baseip.i_xbip_counter/i_dsp48.i_dsp/i_sp3adsp.i_dsp48a_wrap/i_primitive_PCOUT<11>_UNCONNECTED , 
\NLW_BU2/U0/i_baseip.i_xbip_counter/i_dsp48.i_dsp/i_sp3adsp.i_dsp48a_wrap/i_primitive_PCOUT<10>_UNCONNECTED , 
\NLW_BU2/U0/i_baseip.i_xbip_counter/i_dsp48.i_dsp/i_sp3adsp.i_dsp48a_wrap/i_primitive_PCOUT<9>_UNCONNECTED , 
\NLW_BU2/U0/i_baseip.i_xbip_counter/i_dsp48.i_dsp/i_sp3adsp.i_dsp48a_wrap/i_primitive_PCOUT<8>_UNCONNECTED , 
\NLW_BU2/U0/i_baseip.i_xbip_counter/i_dsp48.i_dsp/i_sp3adsp.i_dsp48a_wrap/i_primitive_PCOUT<7>_UNCONNECTED , 
\NLW_BU2/U0/i_baseip.i_xbip_counter/i_dsp48.i_dsp/i_sp3adsp.i_dsp48a_wrap/i_primitive_PCOUT<6>_UNCONNECTED , 
\NLW_BU2/U0/i_baseip.i_xbip_counter/i_dsp48.i_dsp/i_sp3adsp.i_dsp48a_wrap/i_primitive_PCOUT<5>_UNCONNECTED , 
\NLW_BU2/U0/i_baseip.i_xbip_counter/i_dsp48.i_dsp/i_sp3adsp.i_dsp48a_wrap/i_primitive_PCOUT<4>_UNCONNECTED , 
\NLW_BU2/U0/i_baseip.i_xbip_counter/i_dsp48.i_dsp/i_sp3adsp.i_dsp48a_wrap/i_primitive_PCOUT<3>_UNCONNECTED , 
\NLW_BU2/U0/i_baseip.i_xbip_counter/i_dsp48.i_dsp/i_sp3adsp.i_dsp48a_wrap/i_primitive_PCOUT<2>_UNCONNECTED , 
\NLW_BU2/U0/i_baseip.i_xbip_counter/i_dsp48.i_dsp/i_sp3adsp.i_dsp48a_wrap/i_primitive_PCOUT<1>_UNCONNECTED , 
\NLW_BU2/U0/i_baseip.i_xbip_counter/i_dsp48.i_dsp/i_sp3adsp.i_dsp48a_wrap/i_primitive_PCOUT<0>_UNCONNECTED }),
    .A({\BU2/N0 , \BU2/N0 , \BU2/N0 , \BU2/N0 , \BU2/N0 , \BU2/N0 , \BU2/N0 , \BU2/N0 , \BU2/N0 , \BU2/N0 , \BU2/N0 , \BU2/N0 , \BU2/N0 , \BU2/N0 , 
\BU2/N0 , \BU2/N0 , \BU2/N0 , \BU2/N0 }),
    .M({\NLW_BU2/U0/i_baseip.i_xbip_counter/i_dsp48.i_dsp/i_sp3adsp.i_dsp48a_wrap/i_primitive_M<35>_UNCONNECTED , 
\NLW_BU2/U0/i_baseip.i_xbip_counter/i_dsp48.i_dsp/i_sp3adsp.i_dsp48a_wrap/i_primitive_M<34>_UNCONNECTED , 
\NLW_BU2/U0/i_baseip.i_xbip_counter/i_dsp48.i_dsp/i_sp3adsp.i_dsp48a_wrap/i_primitive_M<33>_UNCONNECTED , 
\NLW_BU2/U0/i_baseip.i_xbip_counter/i_dsp48.i_dsp/i_sp3adsp.i_dsp48a_wrap/i_primitive_M<32>_UNCONNECTED , 
\NLW_BU2/U0/i_baseip.i_xbip_counter/i_dsp48.i_dsp/i_sp3adsp.i_dsp48a_wrap/i_primitive_M<31>_UNCONNECTED , 
\NLW_BU2/U0/i_baseip.i_xbip_counter/i_dsp48.i_dsp/i_sp3adsp.i_dsp48a_wrap/i_primitive_M<30>_UNCONNECTED , 
\NLW_BU2/U0/i_baseip.i_xbip_counter/i_dsp48.i_dsp/i_sp3adsp.i_dsp48a_wrap/i_primitive_M<29>_UNCONNECTED , 
\NLW_BU2/U0/i_baseip.i_xbip_counter/i_dsp48.i_dsp/i_sp3adsp.i_dsp48a_wrap/i_primitive_M<28>_UNCONNECTED , 
\NLW_BU2/U0/i_baseip.i_xbip_counter/i_dsp48.i_dsp/i_sp3adsp.i_dsp48a_wrap/i_primitive_M<27>_UNCONNECTED , 
\NLW_BU2/U0/i_baseip.i_xbip_counter/i_dsp48.i_dsp/i_sp3adsp.i_dsp48a_wrap/i_primitive_M<26>_UNCONNECTED , 
\NLW_BU2/U0/i_baseip.i_xbip_counter/i_dsp48.i_dsp/i_sp3adsp.i_dsp48a_wrap/i_primitive_M<25>_UNCONNECTED , 
\NLW_BU2/U0/i_baseip.i_xbip_counter/i_dsp48.i_dsp/i_sp3adsp.i_dsp48a_wrap/i_primitive_M<24>_UNCONNECTED , 
\NLW_BU2/U0/i_baseip.i_xbip_counter/i_dsp48.i_dsp/i_sp3adsp.i_dsp48a_wrap/i_primitive_M<23>_UNCONNECTED , 
\NLW_BU2/U0/i_baseip.i_xbip_counter/i_dsp48.i_dsp/i_sp3adsp.i_dsp48a_wrap/i_primitive_M<22>_UNCONNECTED , 
\NLW_BU2/U0/i_baseip.i_xbip_counter/i_dsp48.i_dsp/i_sp3adsp.i_dsp48a_wrap/i_primitive_M<21>_UNCONNECTED , 
\NLW_BU2/U0/i_baseip.i_xbip_counter/i_dsp48.i_dsp/i_sp3adsp.i_dsp48a_wrap/i_primitive_M<20>_UNCONNECTED , 
\NLW_BU2/U0/i_baseip.i_xbip_counter/i_dsp48.i_dsp/i_sp3adsp.i_dsp48a_wrap/i_primitive_M<19>_UNCONNECTED , 
\NLW_BU2/U0/i_baseip.i_xbip_counter/i_dsp48.i_dsp/i_sp3adsp.i_dsp48a_wrap/i_primitive_M<18>_UNCONNECTED , 
\NLW_BU2/U0/i_baseip.i_xbip_counter/i_dsp48.i_dsp/i_sp3adsp.i_dsp48a_wrap/i_primitive_M<17>_UNCONNECTED , 
\NLW_BU2/U0/i_baseip.i_xbip_counter/i_dsp48.i_dsp/i_sp3adsp.i_dsp48a_wrap/i_primitive_M<16>_UNCONNECTED , 
\NLW_BU2/U0/i_baseip.i_xbip_counter/i_dsp48.i_dsp/i_sp3adsp.i_dsp48a_wrap/i_primitive_M<15>_UNCONNECTED , 
\NLW_BU2/U0/i_baseip.i_xbip_counter/i_dsp48.i_dsp/i_sp3adsp.i_dsp48a_wrap/i_primitive_M<14>_UNCONNECTED , 
\NLW_BU2/U0/i_baseip.i_xbip_counter/i_dsp48.i_dsp/i_sp3adsp.i_dsp48a_wrap/i_primitive_M<13>_UNCONNECTED , 
\NLW_BU2/U0/i_baseip.i_xbip_counter/i_dsp48.i_dsp/i_sp3adsp.i_dsp48a_wrap/i_primitive_M<12>_UNCONNECTED , 
\NLW_BU2/U0/i_baseip.i_xbip_counter/i_dsp48.i_dsp/i_sp3adsp.i_dsp48a_wrap/i_primitive_M<11>_UNCONNECTED , 
\NLW_BU2/U0/i_baseip.i_xbip_counter/i_dsp48.i_dsp/i_sp3adsp.i_dsp48a_wrap/i_primitive_M<10>_UNCONNECTED , 
\NLW_BU2/U0/i_baseip.i_xbip_counter/i_dsp48.i_dsp/i_sp3adsp.i_dsp48a_wrap/i_primitive_M<9>_UNCONNECTED , 
\NLW_BU2/U0/i_baseip.i_xbip_counter/i_dsp48.i_dsp/i_sp3adsp.i_dsp48a_wrap/i_primitive_M<8>_UNCONNECTED , 
\NLW_BU2/U0/i_baseip.i_xbip_counter/i_dsp48.i_dsp/i_sp3adsp.i_dsp48a_wrap/i_primitive_M<7>_UNCONNECTED , 
\NLW_BU2/U0/i_baseip.i_xbip_counter/i_dsp48.i_dsp/i_sp3adsp.i_dsp48a_wrap/i_primitive_M<6>_UNCONNECTED , 
\NLW_BU2/U0/i_baseip.i_xbip_counter/i_dsp48.i_dsp/i_sp3adsp.i_dsp48a_wrap/i_primitive_M<5>_UNCONNECTED , 
\NLW_BU2/U0/i_baseip.i_xbip_counter/i_dsp48.i_dsp/i_sp3adsp.i_dsp48a_wrap/i_primitive_M<4>_UNCONNECTED , 
\NLW_BU2/U0/i_baseip.i_xbip_counter/i_dsp48.i_dsp/i_sp3adsp.i_dsp48a_wrap/i_primitive_M<3>_UNCONNECTED , 
\NLW_BU2/U0/i_baseip.i_xbip_counter/i_dsp48.i_dsp/i_sp3adsp.i_dsp48a_wrap/i_primitive_M<2>_UNCONNECTED , 
\NLW_BU2/U0/i_baseip.i_xbip_counter/i_dsp48.i_dsp/i_sp3adsp.i_dsp48a_wrap/i_primitive_M<1>_UNCONNECTED , 
\NLW_BU2/U0/i_baseip.i_xbip_counter/i_dsp48.i_dsp/i_sp3adsp.i_dsp48a_wrap/i_primitive_M<0>_UNCONNECTED })
  );
  VCC   \BU2/XST_VCC  (
    .P(\BU2/N1 )
  );
  GND   \BU2/XST_GND  (
    .G(\BU2/N0 )
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
