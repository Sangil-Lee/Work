//  (c) Copyright 2011-2014 Xilinx, Inc. All rights reserved.
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
//   axisc_transfer_mux
//
//--------------------------------------------------------------------------

`timescale 1ps/1ps
`default_nettype none

(* DowngradeIPIdentifiedWarnings="yes" *)
module axis_switch_v1_1_8_axisc_transfer_mux #
(
///////////////////////////////////////////////////////////////////////////////
// Parameter Definitions
///////////////////////////////////////////////////////////////////////////////
   parameter         C_FAMILY           = "virtex7",
   parameter         C_TPAYLOAD_WIDTH   = 32,
   parameter integer C_AXIS_TDATA_WIDTH = 32,
   parameter integer C_AXIS_TID_WIDTH   = 1,
   parameter integer C_AXIS_TDEST_WIDTH = 1,
   parameter integer C_AXIS_TUSER_WIDTH = 1,
   parameter [31:0]  C_AXIS_SIGNAL_SET  = 32'hFF,
   // C_AXIS_SIGNAL_SET: each bit if enabled specifies which axis optional signals are present
   //   [0] => TREADY present
   //   [1] => TDATA present
   //   [2] => TSTRB present, TDATA must be present
   //   [3] => TKEEP present, TDATA must be present
   //   [4] => TLAST present
   //   [5] => TID present
   //   [6] => TDEST present
   //   [7] => TUSER present
   parameter integer C_NUM_SI_SLOTS     = 2,
   parameter integer C_LOG_SI_SLOTS     = 1,
   parameter integer C_ARB_ON_TLAST  = 1,
   // C_ARB_ON_TLAST: 
   //   0 => Ignore TLAST
   //   1 => Signal ARB_DONE on TLAST
   parameter integer C_ARB_ON_MAX_XFERS = 5,
   // C_ARB_ON_MAX_XFERS: 
   //  0 => Ignore number of transfers to signal DONE
   //  1+ => Signal ARB_DONE after x TRANSFERS
   parameter integer C_ARB_ON_NUM_CYCLES = 5,
   // C_ARB_ON_NUM_CYCLES  
   //  0 => Ignore TIMEOUT parameter
   //  1+ => Signal ARB_DONE after x cylces of TVALID low
   parameter integer C_OUTPUT_REG       = 1, 
   // C_OUTPUT_REG: Registers payload and ARB_REQ signal
   //   0 => BYPASS    = The channel is just wired through the module.
   //   1 => FWD_REV   = Both FWD and REV (fully-registered)
   parameter [C_NUM_SI_SLOTS-1:0] C_CONNECTIVITY = {C_NUM_SI_SLOTS{1'b1}},
   // Specifies connectivity matrix for sparse crossbar configurations
   parameter integer C_SINGLE_SLAVE_CONNECTIVITY = 1 , 
   parameter integer C_ROUTING_MODE = 0
)
(
///////////////////////////////////////////////////////////////////////////////
// Port Declarations
///////////////////////////////////////////////////////////////////////////////
   // System Signals
   input wire ACLK,
   input wire ARESET,
   input wire ACLKEN,

   // Slave side
   input  wire [C_NUM_SI_SLOTS-1:0]                  S_AXIS_TVALID,
   output wire [C_NUM_SI_SLOTS-1:0]                  S_AXIS_TREADY,
   input  wire [C_NUM_SI_SLOTS*C_TPAYLOAD_WIDTH-1:0] S_AXIS_TPAYLOAD,

   // Master side
   output wire                                       M_AXIS_TVALID,
   input  wire                                       M_AXIS_TREADY,
   output wire [C_TPAYLOAD_WIDTH-1:0]                M_AXIS_TPAYLOAD,

   // Arbiter side
   input  wire [C_NUM_SI_SLOTS-1:0]                  ARB_GNT,
   input  wire [C_LOG_SI_SLOTS-1:0]                  ARB_SEL,
   output wire                                       ARB_DONE,

   // Static routing 
   input wire                                        mi_enable,
   input wire  [3:0]                                 mi_mux
);

////////////////////////////////////////////////////////////////////////////////
// Functions
////////////////////////////////////////////////////////////////////////////////
`include "axis_infrastructure_v1_1_0_axis_infrastructure.vh"

localparam P_LOG_NUM_SLAVE_SLOTS = f_clogb2(C_NUM_SI_SLOTS);
function [C_NUM_SI_SLOTS-1:0] f_onehot_to_binary ( 
  input integer  onehot
);
begin : main 
  integer i;
  for (i = 0; onehot[i] != 1; i = i + 1) begin
  end
  f_onehot_to_binary = i;
end
endfunction
////////////////////////////////////////////////////////////////////////////////
// Local parameters
////////////////////////////////////////////////////////////////////////////////
// Check if Connectivity is a power of a 2 which would mean that there is only
// 1 slave connected.
// localparam P_SINGLE_SLAVE_CONNECTIVITY = ((C_CONNECTIVITY & (C_CONNECTIVITY-1)) == 0);
localparam P_FIXED_MUX_VALUE = f_onehot_to_binary(C_CONNECTIVITY);

////////////////////////////////////////////////////////////////////////////////
// Wires/Reg declarations
////////////////////////////////////////////////////////////////////////////////
wire [C_TPAYLOAD_WIDTH-1:0] axis_tpayload_mux_out;
wire                        axis_tvalid_mux_out;
wire                        axis_tready_mux_in;
wire                        reg_slice_reset;
genvar                      si;

generate
if (C_ROUTING_MODE == 0) begin : gen_tdest_router

////////////////////////////////////////////////////////////////////////////////
// Wires/Reg declarations
////////////////////////////////////////////////////////////////////////////////
  wire [C_NUM_SI_SLOTS-1:0]   busy_ns;
  wire [C_NUM_SI_SLOTS-1:0]   busy_r;
  wire [C_LOG_SI_SLOTS-1:0]   sel_i;
  wire                        axis_tlast_i;
  wire [C_NUM_SI_SLOTS-1:0]   output_enable;

////////////////////////////////////////////////////////////////////////////////
// BEGIN RTL
////////////////////////////////////////////////////////////////////////////////
  axis_switch_v1_1_8_axisc_arb_responder #( 
    .C_FAMILY                    ( C_FAMILY                    ) ,
    .C_ARB_GNT_WIDTH             ( C_NUM_SI_SLOTS              ) ,
    .C_ARB_ON_TLAST              ( C_ARB_ON_TLAST              ) ,
    .C_ARB_ON_MAX_XFERS          ( C_ARB_ON_MAX_XFERS          ) ,
    .C_ARB_ON_NUM_CYCLES         ( C_ARB_ON_NUM_CYCLES         ) ,
    .C_SINGLE_SLAVE_CONNECTIVITY ( C_SINGLE_SLAVE_CONNECTIVITY ) 
  )
  axisc_arb_responder (
    .ACLK        ( ACLK                ) ,
    .ARESET      ( ARESET              ) ,
    .ACLKEN      ( ACLKEN              ) ,
    .AXIS_TVALID ( axis_tvalid_mux_out ) ,
    .AXIS_TREADY ( axis_tready_mux_in  ) ,
    .AXIS_TLAST  ( axis_tlast_i        ) ,
    .ARB_GNT     ( ARB_GNT             ) ,
    .ARB_DONE    ( ARB_DONE            ) ,
    .ARB_BUSY    ( busy_r              ) 
  );

  assign sel_i = (C_SINGLE_SLAVE_CONNECTIVITY | C_NUM_SI_SLOTS == 1) ? P_FIXED_MUX_VALUE :
                 ARB_SEL;

  assign output_enable = ( C_NUM_SI_SLOTS == 1 ) ? 1'b1 : 
                         ( C_SINGLE_SLAVE_CONNECTIVITY ) ? C_CONNECTIVITY[C_NUM_SI_SLOTS-1:0] :
                         ARB_GNT | busy_r;

  axis_infrastructure_v1_1_0_mux_enc #(
    .C_FAMILY     ( C_FAMILY              ) ,
    .C_RATIO      ( C_NUM_SI_SLOTS        ) ,
    .C_SEL_WIDTH  ( P_LOG_NUM_SLAVE_SLOTS ) ,
    .C_DATA_WIDTH ( C_TPAYLOAD_WIDTH      ) 
  )
  mux_enc_0
  (
    .S  ( sel_i                 ) ,
    .A  ( S_AXIS_TPAYLOAD       ) ,
    .O  ( axis_tpayload_mux_out ) ,
    .OE ( 1'b1                  ) 
  );
  axis_infrastructure_v1_1_0_mux_enc #(
    .C_FAMILY     ( C_FAMILY              ) ,
    .C_RATIO      ( C_NUM_SI_SLOTS        ) ,
    .C_SEL_WIDTH  ( P_LOG_NUM_SLAVE_SLOTS ) ,
    .C_DATA_WIDTH ( 1                     ) 
  )
  mux_enc_1
  (
    .S  ( sel_i               ) ,
    .A  ( S_AXIS_TVALID       ) ,
    .O  ( axis_tvalid_mux_out ) ,
    .OE ( | output_enable     ) 
  );

  axis_infrastructure_v1_1_0_util_vector2axis #(
    .C_TDATA_WIDTH    ( C_AXIS_TDATA_WIDTH ) ,
    .C_TID_WIDTH      ( C_AXIS_TID_WIDTH   ) ,
    .C_TDEST_WIDTH    ( C_AXIS_TDEST_WIDTH ) ,
    .C_TUSER_WIDTH    ( C_AXIS_TUSER_WIDTH ) ,
    .C_TPAYLOAD_WIDTH ( C_TPAYLOAD_WIDTH   ) ,
    .C_SIGNAL_SET     ( C_AXIS_SIGNAL_SET  ) 
  )
  util_vector2axis_0 (
    .TPAYLOAD ( axis_tpayload_mux_out ) ,
    .TDATA    (                       ) ,
    .TSTRB    (                       ) ,
    .TKEEP    (                       ) ,
    .TLAST    ( axis_tlast_i          ) ,
    .TID      (                       ) ,
    .TDEST    (                       ) ,
    .TUSER    (                       ) 
  );

  assign S_AXIS_TREADY = {C_NUM_SI_SLOTS{axis_tready_mux_in}} & output_enable;

end
else begin : gen_static_router
////////////////////////////////////////////////////////////////////////////////
// Wires/Reg declarations
////////////////////////////////////////////////////////////////////////////////


  assign axis_tpayload_mux_out = S_AXIS_TPAYLOAD[mi_mux*C_TPAYLOAD_WIDTH+:C_TPAYLOAD_WIDTH];

  assign axis_tvalid_mux_out = C_OUTPUT_REG ? S_AXIS_TVALID[mi_mux]
                                            : S_AXIS_TVALID[mi_mux] & mi_enable;
  for (si = 0; si < C_NUM_SI_SLOTS; si = si + 1) begin : gen_si_tready
    assign S_AXIS_TREADY[si] = (si == mi_mux) ? axis_tready_mux_in | ~mi_enable : 1'b1;
  end

  // Tie-off 
  assign ARB_DONE = 1'b0;

end
endgenerate

assign reg_slice_reset = (C_ROUTING_MODE == 0) ? ARESET : ARESET | ~mi_enable;

// Output reg slice
axis_register_slice_v1_1_8_axisc_register_slice #(
  .C_FAMILY     ( C_FAMILY         ) ,
  .C_DATA_WIDTH ( C_TPAYLOAD_WIDTH ) ,
  .C_REG_CONFIG ( C_OUTPUT_REG     ) 
)
axisc_register_slice_0 (
  .ACLK           ( ACLK                  ) ,
  .ARESET         ( reg_slice_reset       ) ,
  .ACLKEN         ( ACLKEN                ) ,
  .S_VALID        ( axis_tvalid_mux_out   ) ,
  .S_READY        ( axis_tready_mux_in    ) ,
  .S_PAYLOAD_DATA ( axis_tpayload_mux_out ) ,

  .M_VALID        ( M_AXIS_TVALID         ) ,
  .M_READY        ( M_AXIS_TREADY         ) ,
  .M_PAYLOAD_DATA ( M_AXIS_TPAYLOAD       ) 
);

endmodule // axisc_transfer_mux

`default_nettype wire
