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

module axis_switch_v1_1_8_reg_bank_16x32 # (
///////////////////////////////////////////////////////////////////////////////
// Parameter Definitions
///////////////////////////////////////////////////////////////////////////////
  parameter integer C_ACTIVE_REG    = 8, // 1+
  // Bit mask of active bits in register set
  parameter integer C_REG_MASK      = 32'b1111_1111_1111_1111, 
  // Default init value
  parameter [16*32-1:0] C_INIT      = {16{32'b0}} 
)
(
///////////////////////////////////////////////////////////////////////////////
// Port Declarations
///////////////////////////////////////////////////////////////////////////////
  input wire                        aclk,
  input wire                        areset,
  input wire                        we,
  input wire  [ 3:0 ]               waddr,
  input wire  [ 32-1:0 ]            wdata,
  output wire [ 16*32-1:0 ]         rdata
);

////////////////////////////////////////////////////////////////////////////////
// Functions
////////////////////////////////////////////////////////////////////////////////

////////////////////////////////////////////////////////////////////////////////
// Local parameters
////////////////////////////////////////////////////////////////////////////////
localparam LP_MAX = 16;
localparam LP_WIDTH = 32;
////////////////////////////////////////////////////////////////////////////////
// Wires/Reg declarations
////////////////////////////////////////////////////////////////////////////////
reg [(LP_MAX*LP_WIDTH)-1:0] reg_data = C_INIT;
////////////////////////////////////////////////////////////////////////////////
// BEGIN RTL
////////////////////////////////////////////////////////////////////////////////

generate
genvar i; 
  for (i = 0; i < LP_MAX; i = i + 1) begin : gen_reg
    always @(posedge aclk) begin 
      if (areset) begin 
        reg_data[i*LP_WIDTH+:LP_WIDTH] <= C_INIT[i*LP_WIDTH+:LP_WIDTH];
      end
      else if ((waddr == i) && we && (i < C_ACTIVE_REG)) begin 
        reg_data[i*LP_WIDTH+:LP_WIDTH] <= wdata;
      end
    end
  end
endgenerate

assign rdata = reg_data;

endmodule // axis_switch_v1_1_8_reg_bank_16x32

`default_nettype wire
