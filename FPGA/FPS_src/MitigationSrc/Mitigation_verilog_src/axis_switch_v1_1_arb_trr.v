//  (c) Copyright 2011-2015 Xilinx, Inc. All rights reserved.
//
//  This file contains confidential and proprietary information
//  of Xilinx, Inc. and is protected under U.S. and
//  international copyright and other intellectual property
//  laws.
//
//  DISCLAIMER
//  This disclaimer is not a license and does not grant any
//  rights to the materials distributed herewith. Except as
//  otherwise provided in a valid license issued to you by
//  Xilinx, and to the maximum extent permitted by applicable
//  law: (1) THESE MATERIALS ARE MADE AVAILABLE "AS IS" AND
//  WITH ALL FAULTS, AND XILINX HEREBY DISCLAIMS ALL WARRANTIES
//  AND CONDITIONS, EXPRESS, IMPLIED, OR STATUTORY, INCLUDING
//  BUT NOT LIMITED TO WARRANTIES OF MERCHANTABILITY, NON-
//  INFRINGEMENT, OR FITNESS FOR ANY PARTICULAR PURPOSE; and
//  (2) Xilinx shall not be liable (whether in contract or tort,
//  including negligence, or under any other theory of
//  liability) for any loss or damage of any kind or nature
//  related to, arising under or in connection with these
//  materials, including for any direct, or any indirect,
//  special, incidental, or consequential loss or damage
//  (including loss of data, profits, goodwill, or any type of
//  loss or damage suffered as a result of any action brought
//  by a third party) even if such damage or loss was
//  reasonably foreseeable or Xilinx had been advised of the
//  possibility of the same.
//
//  CRITICAL APPLICATIONS
//  Xilinx products are not designed or intended to be fail-
//  safe, or for use in any application requiring fail-safe
//  performance, such as life-support or safety devices or
//  systems, Class III medical devices, nuclear facilities,
//  applications related to the deployment of airbags, or any
//  other applications that could lead to death, personal
//  injury, or severe property or environmental damage
//  (individually and collectively, "Critical
//  Applications"). Customer assumes the sole risk and
//  liability of any use of Xilinx products in Critical
//  Applications, subject only to applicable laws and
//  regulations governing limitations on product liability.
//
//  THIS COPYRIGHT NOTICE AND DISCLAIMER MUST BE RETAINED AS
//  PART OF THIS FILE AT ALL TIMES. 
//-----------------------------------------------------------------------------
//
//
// Verilog-standard:  Verilog 2001
//--------------------------------------------------------------------------
//
// Structure:
//   arb_rr
//
//--------------------------------------------------------------------------

`timescale 1ps/1ps
`default_nettype none

(* DowngradeIPIdentifiedWarnings="yes" *)
module axis_switch_v1_1_8_arb_trr #
(
///////////////////////////////////////////////////////////////////////////////
// Parameter Definitions
///////////////////////////////////////////////////////////////////////////////
   parameter         C_FAMILY        = "virtex7",
   parameter integer C_NUM_SI_SLOTS  = 8,
   parameter integer C_LOG_SI_SLOTS  = 3
)
(
///////////////////////////////////////////////////////////////////////////////
// Port Declarations
///////////////////////////////////////////////////////////////////////////////
   // System Signals
   input wire aclk,
   input wire areset,
   input wire aclken,

   input  wire [C_NUM_SI_SLOTS-1:0] arb_req,
   input  wire                      arb_done, 
   output wire [C_NUM_SI_SLOTS-1:0] arb_gnt,
   output wire [C_LOG_SI_SLOTS-1:0] arb_sel
);

////////////////////////////////////////////////////////////////////////////////
// Functions
////////////////////////////////////////////////////////////////////////////////
function [C_NUM_SI_SLOTS*C_LOG_SI_SLOTS-1:0] f_port_priority_init (
  input integer num_slaves
);
  begin : main
    integer i;
    for (i = 0; i < num_slaves; i = i + 1) begin
      f_port_priority_init[i*C_LOG_SI_SLOTS+:C_LOG_SI_SLOTS] = i[C_LOG_SI_SLOTS-1:0];
    end
  end
endfunction

////////////////////////////////////////////////////////////////////////////////
// Local parameters
////////////////////////////////////////////////////////////////////////////////

////////////////////////////////////////////////////////////////////////////////
// Wires/Reg declarations
////////////////////////////////////////////////////////////////////////////////
reg  [C_LOG_SI_SLOTS-1:0]                sel_r;
wire [C_LOG_SI_SLOTS-1:0]                sel_i;
wire [C_NUM_SI_SLOTS-1:0]                arb_req_rot;
wire [C_NUM_SI_SLOTS-1:0]                arb_req_i;
wire                                     valid_i; 
reg  [C_LOG_SI_SLOTS-1:0]                barrel_cntr;
wire [C_LOG_SI_SLOTS-1:0]                barrel_cntr_ns;
wire                                     advance;
wire [C_NUM_SI_SLOTS*C_LOG_SI_SLOTS-1:0] port_priority_rot;
wire [C_NUM_SI_SLOTS*C_LOG_SI_SLOTS-1:0] port_priority_init = f_port_priority_init(C_NUM_SI_SLOTS);

reg  [C_NUM_SI_SLOTS-1:0]                arb_gnt_r;
reg                                      arb_busy_r;
wire [C_NUM_SI_SLOTS-1:0]                sel_decode_i;
////////////////////////////////////////////////////////////////////////////////
// BEGIN RTL
////////////////////////////////////////////////////////////////////////////////

assign arb_sel = sel_r;

always @(posedge aclk) begin 
  if (aclken) begin 
    sel_r <= valid_i & (~arb_busy_r | arb_done) ? sel_i : sel_r;
  end
end

axis_switch_v1_1_8_dynamic_priority_encoder #(
  .C_FAMILY    ( C_FAMILY       ) ,
  .C_REQ_WIDTH ( C_NUM_SI_SLOTS ) ,
  .C_ENC_WIDTH ( C_LOG_SI_SLOTS ) 
)
dynamic_priority_encoder_0
(
  .REQ           ( arb_req_rot     ) ,
  .PORT_PRIORITY ( port_priority_rot ) ,
  .SEL           ( sel_i           ) ,
  .VALID         ( valid_i         ) 
);

assign arb_req_i = arb_req & ~arb_gnt_r;
assign arb_req_rot = {arb_req_i, arb_req_i} >> barrel_cntr;

always @(posedge aclk) begin
  if (areset) begin
    barrel_cntr <= {C_LOG_SI_SLOTS{1'b0}};
  end
  else if (aclken) begin
    barrel_cntr <= advance ? barrel_cntr_ns : barrel_cntr;
  end
end

assign barrel_cntr_ns = (sel_i == C_NUM_SI_SLOTS-1) ? {C_LOG_SI_SLOTS{1'b0}} : sel_i + 1'b1 ; 

assign port_priority_rot = {port_priority_init, port_priority_init} >> barrel_cntr*C_LOG_SI_SLOTS;

assign arb_gnt = arb_gnt_r;

always @(posedge aclk) begin 
  if (areset) begin
    arb_gnt_r <= {C_LOG_SI_SLOTS{1'b0}};
  end
  else if (aclken) begin
    arb_gnt_r <= valid_i & (~arb_busy_r | arb_done) ? sel_decode_i : {C_LOG_SI_SLOTS{1'b0}};
  end
end

always @(posedge aclk) begin 
  if (areset) begin
    arb_busy_r <= 1'b0;
  end
  else if (aclken) begin
    arb_busy_r <= valid_i & (~arb_busy_r|arb_done) ? 1'b1 : arb_done ? 1'b0 : arb_busy_r;
  end
end

// Decode sel from integer to one-hot
generate
  genvar i;
  for (i = 0; i < C_NUM_SI_SLOTS; i = i + 1) begin : gen_sel_decode_one_hot
    assign sel_decode_i[i] = (i == sel_i); 
  end
endgenerate

assign advance = arb_done;
endmodule // arb_trr

`default_nettype wire
