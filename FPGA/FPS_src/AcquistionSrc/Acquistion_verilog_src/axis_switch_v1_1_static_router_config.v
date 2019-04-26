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

module axis_switch_v1_1_8_static_router_config #
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
   input wire                         aclk,
   input wire                         areset,
   input wire [16*32-1:0]             reg_mi_mux,
   input wire                         reg_commit,
   output wire                        commit_reset,
   output wire                        ctrl_req,
   input  wire                        ctrl_ack,
   output wire [C_CTRL_REG_WIDTH-1:0] ctrl_reg,
   output wire                        ctrl_soft_reset 

);

////////////////////////////////////////////////////////////////////////////////
// Functions
////////////////////////////////////////////////////////////////////////////////

////////////////////////////////////////////////////////////////////////////////
// Local parameters
////////////////////////////////////////////////////////////////////////////////
localparam SM_IDLE  = 3'b000; 
localparam SM_START = 3'b001; 
localparam SM_WAIT  = 3'b010; 
localparam SM_COMMIT = 3'b011; 
localparam SM_COMPLETE = 3'b100; 
localparam integer LP_LOG_MAX_SLOTS = 4; 
localparam integer LP_NUM_MAX_SLOTS = 16;  

////////////////////////////////////////////////////////////////////////////////
// Wires/Reg declarations
////////////////////////////////////////////////////////////////////////////////
reg  [C_CTRL_REG_WIDTH-1:0]                ctrl_reg_r = {C_CTRL_REG_WIDTH{1'b0}};
reg  [2:0]                                 state = SM_IDLE;
wire                                       start;
wire                                       done;
wire [C_NUM_MI_SLOTS-1:0]                  mi_enable;
wire [C_NUM_MI_SLOTS*LP_LOG_MAX_SLOTS-1:0] mi_mux;
wire [C_NUM_SI_SLOTS-1:0]                  si_enable;
wire [C_NUM_SI_SLOTS*LP_LOG_MAX_SLOTS-1:0] si_mux;
reg                                        ctrl_req_r = 1'b0;
reg                                        ctrl_soft_reset_r = 1'b0;
genvar mi, si;

////////////////////////////////////////////////////////////////////////////////
// BEGIN RTL
////////////////////////////////////////////////////////////////////////////////

// Assign outputs
assign ctrl_req = ctrl_req_r;
assign ctrl_reg = ctrl_reg_r;
assign ctrl_soft_reset = ctrl_soft_reset_r;
assign commit_reset = (state == SM_COMPLETE) ? 1'b1 : 1'b0;

// To make xdc timing easier, we merge all the registers into one.
always @(posedge aclk) begin 
  ctrl_reg_r <= {si_enable[0+:C_NUM_SI_SLOTS], si_mux[0+:C_NUM_SI_SLOTS*4], 
                 mi_enable[0+:C_NUM_MI_SLOTS], mi_mux[0+:C_NUM_MI_SLOTS*4]};
end

// enter switch soft reset when not idle.
always @(posedge aclk) begin 
  ctrl_soft_reset_r <= (state == SM_IDLE) ? 1'b0 : 1'b1;
end

// State machine controls calculations.
// ctrl_req_r is explicitely defined for since it is a source in the cdc
// handhshake.
always @(posedge aclk) begin 
  if (areset) begin
    state <= SM_IDLE;
    ctrl_req_r <= 1'b0;
  end
  else begin 
    case (state)
      SM_IDLE:
        if (reg_commit)
          state <= SM_START;
        else
          state <= SM_IDLE;

      SM_START:
        // Stay in start state until done goes low.
        if (done)
          state <= SM_START;
        else
          state <= SM_WAIT;

      SM_WAIT:
        if (done) begin
          state <= SM_COMMIT;
          ctrl_req_r <= 1'b1;
        end
        else begin
          state <= SM_WAIT;
        end

      SM_COMMIT:
        if (ctrl_ack) begin
          state <= SM_COMPLETE;
          ctrl_req_r <= 1'b0;
        end
        else begin
          state <= SM_COMMIT;
        end

      SM_COMPLETE:
        state <= SM_IDLE;

      default:
        state <= SM_IDLE;
    endcase
  end
end

assign start = (state == SM_START) ? 1'b1 : 1'b0;

axis_switch_v1_1_8_static_router_config_dp #(
  .C_NUM_MI_SLOTS   ( C_NUM_MI_SLOTS   ) ,
  .C_NUM_SI_SLOTS   ( C_NUM_SI_SLOTS   ) ,
  .C_CONNECTIVITY   ( C_CONNECTIVITY   ) 
)
inst_start_router_config_dp (
  .aclk                 ( aclk   ) , // I
  // Register interface
  .reg_mi_mux           ( reg_mi_mux ) , // I
  .start                ( start      ) , // I
  .done                 ( done       ) , // O
  // Router interface
  .mi_enable            ( mi_enable  ) , // O 
  .mi_mux               ( mi_mux     ) , // O
  .si_enable            ( si_enable  ) , // O
  .si_mux               ( si_mux     )   // O
);


endmodule // axis_switch_v1_1_8_static_router_config

`default_nettype wire
