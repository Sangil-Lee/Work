// (c) Copyright 2014 Xilinx, Inc. All rights reserved. 
//                                                             
// This file contains confidential and proprietary information 
// of Xilinx, Inc. and is protected under U.S. and             
// international copyright and other intellectual property     
// laws.                                                       
//                                                             
// DISCLAIMER                                                  
// This disclaimer is not a license and does not grant any     
// rights to the materials distributed herewith. Except as     
// otherwise provided in a valid license issued to you by      
// Xilinx, and to the maximum extent permitted by applicable   
// law: (1) THESE MATERIALS ARE MADE AVAILABLE "AS IS" AND     
// WITH ALL FAULTS, AND XILINX HEREBY DISCLAIMS ALL WARRANTIES 
// AND CONDITIONS, EXPRESS, IMPLIED, OR STATUTORY, INCLUDING   
// BUT NOT LIMITED TO WARRANTIES OF MERCHANTABILITY, NON-      
// INFRINGEMENT, OR FITNESS FOR ANY PARTICULAR PURPOSE; and    
// (2) Xilinx shall not be liable (whether in contract or tort,
// including negligence, or under any other theory of          
// liability) for any loss or damage of any kind or nature     
// related to, arising under or in connection with these       
// materials, including for any direct, or any indirect,       
// special, incidental, or consequential loss or damage        
// (including loss of data, profits, goodwill, or any type of  
// loss or damage suffered as a result of any action brought   
// by a third party) even if such damage or loss was           
// reasonably foreseeable or Xilinx had been advised of the    
// possibility of the same.                                    
//                                                             
// CRITICAL APPLICATIONS                                       
// Xilinx products are not designed or intended to be fail-    
// safe, or for use in any application requiring fail-safe     
// performance, such as life-support or safety devices or      
// systems, Class III medical devices, nuclear facilities,     
// applications related to the deployment of airbags, or any   
// other applications that could lead to death, personal       
// injury, or severe property or environmental damage          
// (individually and collectively, "Critical                   
// Applications"). Customer assumes the sole risk and          
// liability of any use of Xilinx products in Critical         
// Applications, subject only to applicable laws and           
// regulations governing limitations on product liability.     
//                                                             
// THIS COPYRIGHT NOTICE AND DISCLAIMER MUST BE RETAINED AS    
// PART OF THIS FILE AT ALL TIMES.                             
//  
///////////////////////////////////////////////////////////////////////////////
//
// File name: axi_ctrl_read.v
//
///////////////////////////////////////////////////////////////////////////////
`timescale 1ps/1ps
`default_nettype none

module axis_switch_v1_1_8_axi_ctrl_read #
(
///////////////////////////////////////////////////////////////////////////////
// Parameter Definitions
///////////////////////////////////////////////////////////////////////////////
  // Width of AXI-4-Lite address bus
  parameter integer C_ADDR_WIDTH        = 7,
  // Width of AXI-4-Lite data buses
  parameter integer C_DATA_WIDTH        = 32
)
(
///////////////////////////////////////////////////////////////////////////////
// Port Declarations     
///////////////////////////////////////////////////////////////////////////////
  // AXI4-Lite Slave Interface
  // Slave Interface System Signals           
  input  wire                               aclk        , 
  input  wire                               areset      , 
  // Slave Interface Read Address Ports
  input  wire                               arvalid     ,
  output wire                               arready     ,
  input  wire [C_ADDR_WIDTH-1:0]            araddr      , 
  // Slave Interface Read Data Ports
  output wire                               rvalid      , 
  input  wire                               rready      , 
  output wire [C_DATA_WIDTH-1:0]            rdata       , 
  output wire [1:0]                         rresp       ,

  input wire  [16*C_DATA_WIDTH-1:0]         reg_bank_0  ,
  input wire  [16*C_DATA_WIDTH-1:0]         reg_bank_1  , 
  input wire  [160-1:0]                     ctrl_reg_debug

);

////////////////////////////////////////////////////////////////////////////////
// Local parameters
////////////////////////////////////////////////////////////////////////////////
// Outputs are encoded in register bits
//                                /--- RVALID
//                                |/-- ARREADY/READ
localparam [1:0] SM_IDLE     = 2'b00;
localparam [1:0] SM_READ     = 2'b01;
localparam [1:0] SM_RESP     = 2'b10;

localparam integer LP_DEBUG_CTRL_REG = 0;

////////////////////////////////////////////////////////////////////////////////
// Wires/Reg declarations
////////////////////////////////////////////////////////////////////////////////
reg  [1:0]              state       = 2'b0;
reg  [C_ADDR_WIDTH-1:0] addr_r      = {C_ADDR_WIDTH{1'b0}};
wire [3:0]              addr;
reg  [C_DATA_WIDTH-1:0] data        = {C_ADDR_WIDTH{1'b0}};
wire                    sel; 
wire [C_DATA_WIDTH-1:0] sel_bank_0;
wire [C_DATA_WIDTH-1:0] sel_bank_1;
wire                    read_enable;
wire [16*C_DATA_WIDTH-1:0] reg_bank_0_expanded;
////////////////////////////////////////////////////////////////////////////////
// BEGIN RTL
///////////////////////////////////////////////////////////////////////////////

// Assign axi_outputs
assign arready = state[0];
assign rvalid = state[1];
assign rdata  = data;
assign rresp  = 2'b0;

// State machine for AXI
always @(posedge aclk) begin 
  if (areset) begin
    state <= SM_IDLE;
  end
  else begin 
    case (state) 
      SM_IDLE:
        if (arvalid)
          state <= SM_READ;
        else 
          state <= SM_IDLE;
      SM_READ:
        state <= SM_RESP;
      SM_RESP:
        if (rready) 
          state <= SM_IDLE;
        else
          state <= SM_RESP;
      default:
        state <= SM_IDLE;
    endcase
  end
end

// Reg bank logic
always @(posedge aclk) begin 
  addr_r <= araddr;
end

assign addr = addr_r[2+:4];
assign sel  = ~addr_r[6];

assign read_enable = state[0];
assign sel_bank_0 = reg_bank_0_expanded[addr*C_DATA_WIDTH+:C_DATA_WIDTH];
                    
assign sel_bank_1 = reg_bank_1[addr*C_DATA_WIDTH+:C_DATA_WIDTH];

always @(posedge aclk) begin 
  if (read_enable) begin 
    data <= sel ? sel_bank_0 : sel_bank_1;
  end
end

assign reg_bank_0_expanded[0+:C_DATA_WIDTH] = reg_bank_0[0+:C_DATA_WIDTH];
assign reg_bank_0_expanded[1*C_DATA_WIDTH+:5*C_DATA_WIDTH] = (LP_DEBUG_CTRL_REG) ? ctrl_reg_debug : 160'h0;
assign reg_bank_0_expanded[6*C_DATA_WIDTH+:10*C_DATA_WIDTH] = 320'h0;

endmodule // axis_switch_v1_1_8_axi_ctrl_read.v

`default_nettype wire
