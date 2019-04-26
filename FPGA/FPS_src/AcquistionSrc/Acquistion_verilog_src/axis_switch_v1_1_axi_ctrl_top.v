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

module axis_switch_v1_1_8_axi_ctrl_top #
(
///////////////////////////////////////////////////////////////////////////////
// Parameter Definitions
///////////////////////////////////////////////////////////////////////////////
  // Width of AXI-4-Lite address bus
  parameter integer C_AXI_ADDR_WIDTH    = 7, 
  // Width of AXI-4-Lite data buses
  parameter integer C_AXI_DATA_WIDTH    = 32,
  // C_NUM_SI_SLOTS: Number of slave interfaces (input channels)
  parameter integer C_NUM_SI_SLOTS      = 1,
  // C_NUM_MI_SLOTS: Number of master interfaces (output channels)
  parameter integer C_NUM_MI_SLOTS      = 2,
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

  // Registers outputs
  output wire [16*C_AXI_DATA_WIDTH-1:0]     reg_mi_mux        ,
  output wire                               reg_commit        , 
  input  wire                               commit_reset      ,
  input  wire [C_CTRL_REG_WIDTH-1:0]        ctrl_reg
  
);
////////////////////////////////////////////////////////////////////////////////
// Localparams
////////////////////////////////////////////////////////////////////////////////
localparam [C_AXI_DATA_WIDTH-1:0]    LP_CTRL_REG_MASK   = 32'b0000_0000_0000_0010;
// Initial value will initiate a commit.
localparam [16*C_AXI_DATA_WIDTH-1:0] LP_CTRL_REG_INIT   = { {15{32'b0}}, 32'b0000_0000_0000_0010 };
localparam [C_AXI_DATA_WIDTH-1:0]    LP_MI_MUX_REG_MASK = 32'b1000_0000_0000_1111;
localparam [16*C_AXI_DATA_WIDTH-1:0] LP_MI_MUX_REG_INIT = {16{32'h8000_0000}};

////////////////////////////////////////////////////////////////////////////////
// Wires/Reg declarations
////////////////////////////////////////////////////////////////////////////////
wire                           rb0_we;
wire [3:0]                     rb0_waddr;
wire [C_AXI_DATA_WIDTH-1:0]    rb0_wdata;

wire [16*C_AXI_DATA_WIDTH-1:0] reg_bank_0;
wire [16*C_AXI_DATA_WIDTH-1:0] reg_bank_1;
wire [1:0]                     write_enable;
wire [3:0]                     write_addr;
wire [C_AXI_DATA_WIDTH-1:0]    write_data;
wire                           write_busy;
wire [160-1:0]                 ctrl_reg_debug;

////////////////////////////////////////////////////////////////////////////////
// BEGIN RTL
///////////////////////////////////////////////////////////////////////////////

// Instantiate AXI4-Lite read channel module
axis_switch_v1_1_8_axi_ctrl_read # ( 
  .C_ADDR_WIDTH     ( C_AXI_ADDR_WIDTH ) ,
  .C_DATA_WIDTH     ( C_AXI_DATA_WIDTH )
)
inst_axi_ctrl_read (
  .aclk           ( aclk          ) , // I
  .areset         ( areset        ) , // I
  .arvalid        ( s_axi_arvalid ) , // I 
  .arready        ( s_axi_arready ) , // O 
  .araddr         ( s_axi_araddr  ) , // I 
  .rvalid         ( s_axi_rvalid  ) , // O 
  .rready         ( s_axi_rready  ) , // I
  .rresp          ( s_axi_rresp   ) , // O
  .rdata          ( s_axi_rdata   ) , // O 
  .reg_bank_0     ( reg_bank_0    ) , // I 
  .reg_bank_1     ( reg_bank_1    ) , // I
  .ctrl_reg_debug ( ctrl_reg_debug )  // I
);


// Instantiate AXI4-Lite write channel module
axis_switch_v1_1_8_axi_ctrl_write # ( 
  .C_ADDR_WIDTH     ( C_AXI_ADDR_WIDTH ) ,
  .C_DATA_WIDTH     ( C_AXI_DATA_WIDTH )
)
inst_axi_ctrl_write (
  .aclk           ( aclk           ) ,
  .areset         ( areset         ) ,
  .awvalid        ( s_axi_awvalid  ) , // I 
  .awready        ( s_axi_awready  ) , // O
  .awaddr         ( s_axi_awaddr   ) , // I 
  .wvalid         ( s_axi_wvalid   ) , // I 
  .wready         ( s_axi_wready   ) , // O
  .wdata          ( s_axi_wdata    ) , // I 
  .bvalid         ( s_axi_bvalid   ) , // O
  .bready         ( s_axi_bready   ) , // I 
  .bresp          ( s_axi_bresp    ) , // O
  .enable         ( write_enable   ) , // O
  .addr           ( write_addr     ) , // O
  .data           ( write_data     ) , // O
  .busy           ( write_busy     )   // I
);

// This reg bank holds 0x0-0x3C register maps
// 0x0: Control Register
//      2 - Commit
axis_switch_v1_1_8_reg_bank_16x32 #( 
  .C_ACTIVE_REG (1),
  .C_REG_MASK   (LP_CTRL_REG_MASK),
  .C_INIT       (LP_CTRL_REG_INIT)
)
inst_reg_bank_0
(
  .aclk   ( aclk       ) ,
  .areset ( areset     ) ,
  .we     ( rb0_we     ) ,
  .waddr  ( rb0_waddr  ) ,
  .wdata  ( rb0_wdata  ) ,
  .rdata  ( reg_bank_0 ) 
);

assign rb0_we    = commit_reset | write_enable[0];
assign rb0_waddr = commit_reset ? 4'b0 : write_addr;
assign rb0_wdata = commit_reset ? 32'h0 : write_data;

assign write_busy = reg_bank_0[1];
assign reg_commit = reg_bank_0[1];


// This reg bank holds 0x40-0x7C register maps
// 0x40: MI MUX 0
//       3:0 - MI Mux Value
//       31  - MI Disable
// 0x44: MI MUX 1
//       3:0 - MI Mux Value
//       31  - MI Disable
// 0x7C: MI MUX 15
//       3:0 - MI Mux Value
//       31  - MI Disable
axis_switch_v1_1_8_reg_bank_16x32 #( 
  .C_ACTIVE_REG (C_NUM_MI_SLOTS),
  .C_REG_MASK   (LP_MI_MUX_REG_MASK),
  .C_INIT       (LP_MI_MUX_REG_INIT)
)
inst_reg_bank_1
(
  .aclk   ( aclk            ) ,
  .areset ( areset          ) ,
  .we     ( write_enable[1] ) ,
  .waddr  ( write_addr      ) ,
  .wdata  ( write_data      ) ,
  .rdata  ( reg_bank_1      ) 
);

assign reg_mi_mux = reg_bank_1;


// 16 == Max number of slots, ever.
// ctrl_reg is expanded for debug
generate 
  genvar i;
  for (i = 0; i < 16; i = i + 1) begin : gen_ctrl_debug
    if (i < C_NUM_MI_SLOTS) begin : gen_mi_map
      // Assign mi mux values
      assign ctrl_reg_debug[i*4+:4]   = ctrl_reg[i*4+:4];
      // asssign mi enable values
      assign ctrl_reg_debug[16*4*2+i] = ctrl_reg[C_NUM_MI_SLOTS*4+i];
    end
    else begin : gen_mi_nomap
      // Assign mi mux values
      assign ctrl_reg_debug[i*4+:4]   = 4'h0;
      // asssign mi enable values
      assign ctrl_reg_debug[16*4*2+i] = 1'b0;
    end
    if (i < C_NUM_SI_SLOTS) begin : gen_si_map
      // Assign si mux values
      assign ctrl_reg_debug[16*4+i*4+:4] = ctrl_reg[C_NUM_MI_SLOTS*5+i*4+:4];
      // asssign si enable values
      assign ctrl_reg_debug[16*4*2+16+i] = ctrl_reg[C_NUM_SI_SLOTS*4+C_NUM_MI_SLOTS*5+i];
    end
    else begin :gen_si_nomap
      // Assign si mux values
      assign ctrl_reg_debug[16*4+i*4+:4] = 4'h0;
      // asssign si enable values
      assign ctrl_reg_debug[16*4*2+16+i] = 1'b0;
    end
  end
endgenerate


endmodule // axis_switch_v1_1_8_axi_ctrl_top.v

`default_nettype wire
