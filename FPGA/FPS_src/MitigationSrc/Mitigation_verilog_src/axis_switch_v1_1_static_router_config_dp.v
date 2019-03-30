//  (c) Copyright 2014 Xilinx, Inc. All rights reserved.
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
// Verilog-standard:  Verilog 2001
//--------------------------------------------------------------------------

`timescale 1ps/1ps
`default_nettype none

module axis_switch_v1_1_8_static_router_config_dp #
(
///////////////////////////////////////////////////////////////////////////////
// Parameter Definitions
///////////////////////////////////////////////////////////////////////////////
  parameter integer C_NUM_MI_SLOTS = 16,
  parameter integer C_NUM_SI_SLOTS = 16,
  parameter [C_NUM_MI_SLOTS*C_NUM_SI_SLOTS-1:0] C_CONNECTIVITY = {C_NUM_MI_SLOTS*C_NUM_SI_SLOTS{1'b1}},
  parameter integer C_CTRL_REG_WIDTH = C_NUM_MI_SLOTS*5 + C_NUM_SI_SLOTS*5
)
(
///////////////////////////////////////////////////////////////////////////////
// Port Declarations
///////////////////////////////////////////////////////////////////////////////
   // System Signals
   input  wire                        aclk,

   input  wire                        start,
   input  wire [16*32-1:0]            reg_mi_mux,

   output wire                        done,
   output wire [C_NUM_MI_SLOTS-1:0]   mi_enable,
   output wire [C_NUM_MI_SLOTS*4-1:0] mi_mux,
   output wire [C_NUM_SI_SLOTS-1:0]   si_enable,
   output wire [C_NUM_SI_SLOTS*4-1:0] si_mux

);
////////////////////////////////////////////////////////////////////////////////
// Local parameters
////////////////////////////////////////////////////////////////////////////////
localparam integer LP_LOG_MAX_SLOTS = 4; 
localparam integer LP_NUM_MAX_SLOTS = 16;  

////////////////////////////////////////////////////////////////////////////////
// Wires/Reg declarations
////////////////////////////////////////////////////////////////////////////////
genvar mi, si;

// Init signals
reg                                           start_r = 1'b0;
reg                                           stg1_reset = 1'b0;
reg  [C_NUM_MI_SLOTS*LP_LOG_MAX_SLOTS-1:0]    mi_mux_in = {C_NUM_MI_SLOTS*LP_LOG_MAX_SLOTS{1'b0}};
reg  [C_NUM_MI_SLOTS-1:0]                     mi_mux_en_in = {C_NUM_MI_SLOTS{1'b0}};

// Stage 1
reg  [LP_LOG_MAX_SLOTS-1:0]                   mi_cntr = (C_NUM_MI_SLOTS[0+:LP_LOG_MAX_SLOTS]-1'b1);
reg  [LP_LOG_MAX_SLOTS-1:0]                   selector = 4'H0;
reg  [LP_LOG_MAX_SLOTS-1:0]                   selectee = 4'h0;
reg                                           selector_enable = 1'b0;
reg                                           stg1_done = 1'b1;
reg  [LP_NUM_MAX_SLOTS-1:0]                   mi_connectivity = {LP_NUM_MAX_SLOTS{1'b0}};

// Stage 2
reg                                           stg2_reset = 1'b0;
wire                                          connected;
reg  [C_NUM_SI_SLOTS*LP_LOG_MAX_SLOTS-1:0]    si_mux_r = {C_NUM_SI_SLOTS*LP_LOG_MAX_SLOTS{1'b0}};
reg  [C_NUM_MI_SLOTS-1:0]                     mi_enable_r = {C_NUM_MI_SLOTS{1'b0}};
reg  [LP_NUM_MAX_SLOTS-1:0]                   si_enable_r = {{LP_NUM_MAX_SLOTS-C_NUM_SI_SLOTS{1'b1}},{C_NUM_SI_SLOTS{1'b0}}};
reg                                           stg2_done = 1'b1;

////////////////////////////////////////////////////////////////////////////////
// BEGIN RTL
////////////////////////////////////////////////////////////////////////////////

// Assign outputs
assign done = stg2_done;
assign mi_mux = mi_mux_in;
assign si_mux = si_mux_r;
assign mi_enable = mi_enable_r;
assign si_enable = si_enable_r[0+:C_NUM_SI_SLOTS];

////////////////////////////////////////////////////////////////////////////////
// Init
always @(posedge aclk) begin 
  start_r <= start;
end

// generate a reset pulse
always @(posedge aclk) begin 
  stg1_reset <= start & ~start_r;
end

// Re-organize input register into nice to use vectors
generate 
  for (mi = 0; mi < C_NUM_MI_SLOTS; mi = mi + 1) begin : gen_mi_mux_in
    always @(posedge aclk) begin 
      mi_mux_in[mi*LP_LOG_MAX_SLOTS+:LP_LOG_MAX_SLOTS] <= stg1_reset ? reg_mi_mux[mi*32+:LP_LOG_MAX_SLOTS]
                                                          :  mi_mux_in[mi*LP_LOG_MAX_SLOTS+:LP_LOG_MAX_SLOTS];
    end
    always @(posedge aclk) begin 
      mi_mux_en_in[mi*1+:1] <= stg1_reset ? ~reg_mi_mux[(mi*32)+31+:1] : mi_mux_en_in[mi*1+:1] ;
    end
  end
endgenerate 

// counter is used to iterate through the mi mux registers
always @(posedge aclk) begin 
  if (stg1_reset) begin 
    mi_cntr <= {LP_LOG_MAX_SLOTS{1'b0}};
  end
  else begin 
    mi_cntr <= ~(mi_cntr == (C_NUM_MI_SLOTS[0+:LP_LOG_MAX_SLOTS]-1'b1)) ? mi_cntr + 1'b1 : mi_cntr;
  end
end

////////////////////////////////////////////////////////////////////////////////
// Stage 1 of the pipeline
always @(posedge aclk) begin 
  stg1_done <= stg1_reset ? 1'b0 : (mi_cntr == (C_NUM_MI_SLOTS[0+:LP_LOG_MAX_SLOTS]-1'b1));
end


// Current MI source
always @(posedge aclk) begin 
  selector <= mi_cntr;
end

// Current SI targeted
always @(posedge aclk) begin 
  selectee <= mi_mux_in[mi_cntr*LP_LOG_MAX_SLOTS+:LP_LOG_MAX_SLOTS];
end

always @(posedge aclk) begin 
  selector_enable <= mi_mux_en_in[mi_cntr+:1];
end

always @(posedge aclk) begin 
  mi_connectivity[0+:C_NUM_SI_SLOTS] <= C_CONNECTIVITY[mi_cntr*C_NUM_SI_SLOTS+:C_NUM_SI_SLOTS];
end

////////////////////////////////////////////////////////////////////////////////
// Stage 2 of the pipeline
always @(posedge aclk) begin 
  stg2_reset <= stg1_reset;
end

always @(posedge aclk) begin 
  stg2_done <= stg1_reset ? 1'b0 : stg1_done;
end

assign connected = mi_connectivity[selectee];
// Generate mux registers
generate
  for (si = 0; si < C_NUM_SI_SLOTS; si = si + 1) begin : gen_si_mux
    always @(posedge aclk) begin 
      si_mux_r[si*LP_LOG_MAX_SLOTS+:LP_LOG_MAX_SLOTS] <= ~si_enable[si] ? selector[0+:LP_LOG_MAX_SLOTS]
                                                           : si_mux_r[si*LP_LOG_MAX_SLOTS+:LP_LOG_MAX_SLOTS];
    end
  end
  // It's possible to target a non-existant SI, therefore we extend out the
  // si_enable signal to the max number of slots and enable the unused values,
  // this essentially marks them as invalid targets since they are already
  // 'used'.
  for (si = 0; si < LP_NUM_MAX_SLOTS; si = si + 1) begin : gen_si_enable
    always @(posedge aclk) begin 
      if (stg2_reset) begin 
        si_enable_r[si] <= (si < C_NUM_SI_SLOTS) ? 1'b0 : 1'b1;
      end
      else begin
        si_enable_r[si] <= ((si == selectee) & selector_enable & connected) | si_enable_r[si];
      end
    end
  end
  for (mi = 0; mi < C_NUM_MI_SLOTS; mi = mi + 1) begin : gen_mi_enable
    always @(posedge aclk) begin 
      if (stg2_reset) begin 
        mi_enable_r[mi] <= 1'b0;
      end
      else begin
        mi_enable_r[mi] <= (selector == mi) ? (~si_enable_r[selectee] & selector_enable & connected)|mi_enable_r[mi] : mi_enable_r[mi];
      end
    end
  end
endgenerate

endmodule // axis_switch_v1_1_8_static_router_config

`default_nettype wire
