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
// File name: axi_ctrl_top.v
//
///////////////////////////////////////////////////////////////////////////////
`timescale 1ps/1ps
`default_nettype none

module axis_switch_v1_1_8_static_router #
(
///////////////////////////////////////////////////////////////////////////////
// Parameter Definitions
///////////////////////////////////////////////////////////////////////////////
  // Width of AXI-4-Lite address bus
  parameter integer C_AXI_ADDR_WIDTH              = 7, 
  // Width of AXI-4-Lite data buses
  parameter integer C_AXI_DATA_WIDTH              = 32,
  // C_NUM_SI_SLOTS: Number of slave interfaces (input channels)
  parameter integer C_NUM_SI_SLOTS                = 1,
  // C_NUM_MI_SLOTS: Number of master interfaces (output channels)
  parameter integer C_NUM_MI_SLOTS                = 2,
  parameter [C_NUM_MI_SLOTS*C_NUM_SI_SLOTS-1:0] C_CONNECTIVITY = {C_NUM_MI_SLOTS*C_NUM_SI_SLOTS{1'b1}},
  parameter integer C_CTRL_REG_WIDTH = C_NUM_SI_SLOTS*5 + C_NUM_MI_SLOTS*5
)
(
///////////////////////////////////////////////////////////////////////////////
// Port Declarations     
///////////////////////////////////////////////////////////////////////////////
  // AXI4-Lite Slave Interface
  // Slave Interface System Signals           
  input  wire                               aclk              , 
  input  wire                               areset            , 
  // Slave Interface Write Address Ports
  input  wire                               s_axi_awvalid     , 
  output wire                               s_axi_awready     , 
  input  wire [C_AXI_ADDR_WIDTH-1:0]        s_axi_awaddr      , 
  // Slave Interface Write Data Ports
  input  wire                               s_axi_wvalid      , 
  output wire                               s_axi_wready      , 
  input  wire [C_AXI_DATA_WIDTH-1:0]        s_axi_wdata       , 
  // Slave Interface Write Response Ports
  output wire                               s_axi_bvalid      , 
  input  wire                               s_axi_bready      , 
  output wire [1:0]                         s_axi_bresp       , 
  // Slave Interface Read Address Ports
  input  wire                               s_axi_arvalid     , 
  output wire                               s_axi_arready     , 
  input  wire [C_AXI_ADDR_WIDTH-1:0]        s_axi_araddr      , 
  // Slave Interface Read Data Ports
  output wire                               s_axi_rvalid      , 
  input  wire                               s_axi_rready      , 
  output wire [C_AXI_DATA_WIDTH-1:0]        s_axi_rdata       , 
  output wire [1:0]                         s_axi_rresp       , 

  // Router control outputs
  output wire                               ctrl_req        , 
  input  wire                               ctrl_ack        , 
  output wire [C_CTRL_REG_WIDTH-1:0]        ctrl_reg        ,
  // Soft reset output
  output wire                               ctrl_soft_reset
  
);

/////////////////////////////////////////////////////////////////////////////
// Functions
/////////////////////////////////////////////////////////////////////////////

////////////////////////////////////////////////////////////////////////////////
// Local parameters
////////////////////////////////////////////////////////////////////////////////

////////////////////////////////////////////////////////////////////////////////
// Wires/Reg declarations
////////////////////////////////////////////////////////////////////////////////
wire [16*32-1:0] reg_mi_mux;
wire             reg_commit;
wire             commit_reset;
////////////////////////////////////////////////////////////////////////////////
// BEGIN RTL
///////////////////////////////////////////////////////////////////////////////

axis_switch_v1_1_8_axi_ctrl_top #(
  .C_AXI_ADDR_WIDTH ( C_AXI_ADDR_WIDTH ) ,
  .C_AXI_DATA_WIDTH ( C_AXI_DATA_WIDTH ) ,
  .C_NUM_MI_SLOTS   ( C_NUM_MI_SLOTS   ) ,
  .C_NUM_SI_SLOTS   ( C_NUM_SI_SLOTS   ) ,
  .C_CONNECTIVITY   ( C_CONNECTIVITY   ) ,
  .C_CTRL_REG_WIDTH ( C_CTRL_REG_WIDTH )
 
)
inst_axi_ctrl_top (
  .aclk                  ( aclk    ) , // I
  .areset                ( areset  ) , // I
  // AXI4-Lite Interface
  .s_axi_awvalid         ( s_axi_awvalid ) , // I
  .s_axi_awready         ( s_axi_awready ) , // O
  .s_axi_awaddr          ( s_axi_awaddr  ) , // I
  .s_axi_wvalid          ( s_axi_wvalid  ) , // I
  .s_axi_wready          ( s_axi_wready  ) , // O
  .s_axi_wdata           ( s_axi_wdata   ) , // I
  .s_axi_bvalid          ( s_axi_bvalid  ) , // O
  .s_axi_bready          ( s_axi_bready  ) , // I
  .s_axi_bresp           ( s_axi_bresp   ) , // O
  .s_axi_arvalid         ( s_axi_arvalid ) , // I
  .s_axi_arready         ( s_axi_arready ) , // O
  .s_axi_araddr          ( s_axi_araddr  ) , // I
  .s_axi_rvalid          ( s_axi_rvalid  ) , // O
  .s_axi_rready          ( s_axi_rready  ) , // I
  .s_axi_rdata           ( s_axi_rdata   ) , // O
  .s_axi_rresp           ( s_axi_rresp   ) , // O
  // Router interface
  .reg_mi_mux            ( reg_mi_mux         ) , // O
  .reg_commit            ( reg_commit         ) , // O
  .commit_reset          ( commit_reset       ) , // I
  .ctrl_reg              ( ctrl_reg           )   // I (debug)
);

axis_switch_v1_1_8_static_router_config #(
  .C_NUM_MI_SLOTS   ( C_NUM_MI_SLOTS   ) ,
  .C_NUM_SI_SLOTS   ( C_NUM_SI_SLOTS   ) ,
  .C_CONNECTIVITY   ( C_CONNECTIVITY   ) 
)
inst_start_router_config (
  .aclk                 ( aclk   ) , // I
  .areset               ( areset ) , // I
  // Register interface
  .reg_mi_mux           ( reg_mi_mux        ) , // I
  .reg_commit           ( reg_commit        ) , // I
  .commit_reset         ( commit_reset      ) , // O
  // Router interface
  .ctrl_req           ( ctrl_req        ) , // O
  .ctrl_ack           ( ctrl_ack        ) , // I 
  .ctrl_reg           ( ctrl_reg        ) , // O
  .ctrl_soft_reset      ( ctrl_soft_reset   )   // O
);

endmodule // axis_switch_v1_1_8_static_router.v 

`default_nettype wire
