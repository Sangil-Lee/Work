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
// File name: axi_ctrl_write.v
//
///////////////////////////////////////////////////////////////////////////////
`timescale 1ps/1ps
`default_nettype none

module axis_switch_v1_1_8_axi_ctrl_write #
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
  input  wire                               awvalid     , 
  output wire                               awready     , 
  input  wire [C_ADDR_WIDTH-1:0]            awaddr      , 
  input  wire                               wvalid      , 
  output wire                               wready      , 
  input  wire [C_DATA_WIDTH-1:0]            wdata       , 
  output wire                               bvalid      , 
  input  wire                               bready      , 
  output wire [1:0]                         bresp       ,

  output wire [1:0]                         enable      ,
  output wire [3:0]                         addr        ,
  output wire [C_DATA_WIDTH-1:0]            data        ,
  input wire                                busy

);

////////////////////////////////////////////////////////////////////////////////
// Local parameters
////////////////////////////////////////////////////////////////////////////////
localparam integer LP_SM_WIDTH = 3;
// Outputs are encoded in register bits
//                                          /--- bvalid
//                                          |/-- awready/wready/enable
localparam [LP_SM_WIDTH-1:0] SM_IDLE  = 3'b000;
localparam [LP_SM_WIDTH-1:0] SM_WRITE = 3'b001;
localparam [LP_SM_WIDTH-1:0] SM_RESP  = 3'b010;
localparam [LP_SM_WIDTH-1:0] SM_BUSY  = 3'b100;

////////////////////////////////////////////////////////////////////////////////
// Wires/Reg declarations
////////////////////////////////////////////////////////////////////////////////
(* fsm_encoding = "none" *) reg  [LP_SM_WIDTH-1:0]  state  = SM_IDLE;
reg  [C_ADDR_WIDTH-1:0] addr_r = {C_ADDR_WIDTH{1'b0}};
reg  [C_DATA_WIDTH-1:0] data_r = {C_DATA_WIDTH{1'b0}};

////////////////////////////////////////////////////////////////////////////////
// BEGIN RTL
///////////////////////////////////////////////////////////////////////////////

// State machine for AXI
always @(posedge aclk) begin 
  if (areset) begin
    state <= SM_IDLE;
  end
  else begin 
    case (state) 
      SM_IDLE:
        state <= (awvalid & wvalid & ~busy) ? SM_WRITE : SM_IDLE;
      SM_WRITE:
        state <= SM_BUSY;
      SM_BUSY:
        state <= busy ? SM_BUSY : SM_RESP;
      SM_RESP:
        state <= bready ? SM_IDLE : SM_RESP;
      default:
        state <= SM_IDLE;
    endcase
  end
end

assign awready = state[0];
assign wready  = state[0];
assign bvalid  = state[1];
assign bresp   = 2'b0;

// Reg bank logic
always @(posedge aclk) begin 
  addr_r <= awaddr[0+:C_ADDR_WIDTH];
end
      
always @(posedge aclk) begin 
  data_r <= wdata;
end

assign addr = addr_r[2+:4];
assign data = data_r;
assign enable[0]    = ~addr_r[6] ? state[0] : 1'b0;
assign enable[1]    = addr_r[6] ? state[0] : 1'b0;

endmodule // axis_switch_v1_1_8_axi_ctrl_write.v

`default_nettype wire
