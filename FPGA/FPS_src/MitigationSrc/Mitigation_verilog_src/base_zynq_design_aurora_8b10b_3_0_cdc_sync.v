///////////////////////////////////////////////////////////////////////////////
// (c) Copyright 2013 Xilinx, Inc. All rights reserved.
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
//
////////////////////////////////////////////////////////////////////////////////
//Generic Help
//C_CDC_TYPE : Defines the type of CDC needed
//             0 means pulse synchronizer. Used to transfer one clock pulse 
//               from prmry domain to scndry domain.
//             1 means level synchronizer. Used to transfer level signal.
//             2 means level synchronizer with ack. Used to transfer level 
//               signal. Input signal should change only when prmry_ack is detected
//
//C_FLOP_INPUT : when set to 1 adds one flop stage to the input prmry_in signal
//               Set to 0 when incoming signal is purely floped signal.
//
//C_RESET_STATE : Generally sync flops need not have resets. However, in some cases
//                it might be needed.
//              0 means reset not needed for sync flops 
//              1 means reset needed for sync flops. i
//                In this case prmry_resetn should be in prmry clock, 
//                while scndry_reset should be in scndry clock.
//
//C_SINGLE_BIT : CDC should normally be done for single bit signals only. 
//               However, based on design buses can also be CDC'ed.
//               0 means it is a bus. In this case input be connected to prmry_vect_in.
//                 Output is on scndry_vect_out.
//               1 means it is a single bit. In this case input be connected to prmry_in. 
//                 Output is on scndry_out.
//
//C_VECTOR_WIDTH : defines the size of bus. This is irrelevant when C_SINGLE_BIT = 1
//
//C_MTBF_STAGES : Defines the number of sync stages needed. Allowed values are 0 to 6.
//                Value of 0, 1 is allowed only for level CDC.
//                Min value for Pulse CDC is 2
//
//Whenever this file is used following XDC constraint has to be added 

//         set_false_path -to [get_pins -hier *cdc_to*]        


//IO Ports 
//
//        prmry_aclk      : clock of originating domain (source domain)
//        prmry_resetn    : sync reset of originating clock domain (source domain)
//        prmry_in        : input signal bit. This should be a pure flop output without 
//                          any combi logic. This is source. 
//        prmry_vect_in   : bus signal. From Source domain.
//        prmry_ack       : Ack signal, valid for one clock period, in prmry_aclk domain.
//                          Used only when C_CDC_TYPE = 2
//        scndry_aclk     : destination clock.
//        scndry_resetn   : sync reset of destination domain
//        scndry_out      : sync'ed output in destination domain. Single bit.
//        scndry_vect_out : sync'ed output in destination domain. bus.



`timescale 1ps / 1ps
(* DowngradeIPIdentifiedWarnings="yes" *)
module base_zynq_design_aurora_8b10b_3_0_cdc_sync
   # (
       parameter [1:0] c_cdc_type      = 1,   // 0 Pulse synchronizer, 1 level synchronizer 2 level synchronizer with ACK 
       parameter [0:0] c_flop_input    = 0,   // 1 Adds one flop stage to the input prmry_in signal
       parameter [0:0] c_reset_state   = 0,   // 1 Reset needed for sync flops 
       parameter [0:0] c_single_bit    = 1,   // 1 single bit input.
       parameter [5:0] c_vector_width  = 32,  // defines the size of bus and irrelevant when C_SINGLE_BIT = 1
       parameter [2:0] c_mtbf_stages   = 2    // Number of sync stages needed
     )  
     (
       input                           prmry_aclk,
       input                           prmry_rst_n,
       input                           prmry_in,
       input [(c_vector_width-1 ):0]   prmry_vect_in,
       input                           scndry_aclk,
       input                           scndry_rst_n,
       output                          prmry_ack,
       output                          scndry_out,
       output [(c_vector_width-1 ):0]  scndry_vect_out
      );

  
    // Internal signal declarations
    wire s_out_re;
    reg p_in_d1_cdc_from = 1'b0;
    (* ASYNC_REG = "true" *) (* shift_extract = "{no}" *) reg s_out_d1_cdc_to = 1'b0;
    (* ASYNC_REG = "true" *) (* shift_extract = "{no}" *) reg s_out_d2 = 1'b0;
    (* ASYNC_REG = "true" *) (* shift_extract = "{no}" *) reg s_out_d3 = 1'b0;
    (* ASYNC_REG = "true" *) (* shift_extract = "{no}" *) reg s_out_d4 = 1'b0;
    (* ASYNC_REG = "true" *) (* shift_extract = "{no}" *) reg s_out_d5 = 1'b0;
    (* ASYNC_REG = "true" *) (* shift_extract = "{no}" *) reg s_out_d6 = 1'b0;
    (* ASYNC_REG = "true" *) (* shift_extract = "{no}" *) reg s_out_d7 = 1'b0;
    reg scndry_out_int_d1 = 1'b0;

    (* ASYNC_REG = "true" *) (* shift_extract = "{no}" *) reg s_level_out_d1_cdc_to = 1'b0;
    (* ASYNC_REG = "true" *) (* shift_extract = "{no}" *) reg s_level_out_d2 = 1'b0;
    (* ASYNC_REG = "true" *) (* shift_extract = "{no}" *) reg s_level_out_d3 = 1'b0;
    (* ASYNC_REG = "true" *) (* shift_extract = "{no}" *) reg s_level_out_d4 = 1'b0;
    (* ASYNC_REG = "true" *) (* shift_extract = "{no}" *) reg s_level_out_d5 = 1'b0;
    (* ASYNC_REG = "true" *) (* shift_extract = "{no}" *) reg s_level_out_d6 = 1'b0;

    (* KEEP = "TRUE" *) reg p_level_in_d1_cdc_from = 1'b0;
    wire p_level_in_int;

    wire [( c_vector_width - 1 ):0]p_level_in_bus_d1_cdc_from;
    wire [( c_vector_width - 1 ):0]s_level_out_bus_d1_cdc_tig;
    (* ASYNC_REG = "true" *) (* shift_extract = "{no}" *) reg [( c_vector_width - 1 ):0] s_level_out_bus_d1_cdc_to = 'b0;
    (* ASYNC_REG = "true" *) (* shift_extract = "{no}" *) reg [( c_vector_width - 1 ):0] s_level_out_bus_d2 = 'b0;
    (* ASYNC_REG = "true" *) (* shift_extract = "{no}" *) reg [( c_vector_width - 1 ):0] s_level_out_bus_d3 = 'b0;
    (* ASYNC_REG = "true" *) (* shift_extract = "{no}" *) reg [( c_vector_width - 1 ):0] s_level_out_bus_d4 = 'b0;
    (* ASYNC_REG = "true" *) (* shift_extract = "{no}" *) reg [( c_vector_width - 1 ):0] s_level_out_bus_d5 = 'b0;
    (* ASYNC_REG = "true" *) (* shift_extract = "{no}" *) reg [( c_vector_width - 1 ):0] s_level_out_bus_d6 = 'b0;


    wire scndry_out_int;
    wire prmry_pulse_ack;
    reg prmry_ack_int = 1'b0;
    reg p_level_out_d1_cdc_to = 1'b0;
    reg p_level_out_d2 = 1'b0;
    reg p_level_out_d3 = 1'b0;
    reg p_level_out_d4 = 1'b0;
    reg p_level_out_d5 = 1'b0;
    reg p_level_out_d6 = 1'b0;
    reg p_level_out_d7 = 1'b0;


// Pulse synchronizer Logic
generate if (c_cdc_type == 0) begin 

    always @ (  posedge prmry_aclk)
    begin : REG_P_IN
            if ( ( prmry_rst_n == 1'b0 ) & ( c_reset_state == 1 ) ) 
            begin
                p_in_d1_cdc_from <= 1'b0;
            end
            else
            begin 
                p_in_d1_cdc_from <= prmry_in ^ p_in_d1_cdc_from;
            end
    end

    always @ (  posedge scndry_aclk)
    begin : P_IN_CROSS2SCNDRY
            if ( ( scndry_rst_n == 1'b0 ) & ( c_reset_state == 1 ) ) 
            begin
                s_out_d1_cdc_to <= 1'b0;
                s_out_d2 <= 1'b0;
                s_out_d3 <= 1'b0;
                s_out_d4 <= 1'b0;
                s_out_d5 <= 1'b0;
                s_out_d6 <= 1'b0;
                s_out_d7 <= 1'b0;
                scndry_out_int_d1 <= 1'b0;
            end
            else
            begin 
                s_out_d1_cdc_to <= p_in_d1_cdc_from;
                s_out_d2 <= s_out_d1_cdc_to;
                s_out_d3 <= s_out_d2;
                s_out_d4 <= s_out_d3;
                s_out_d5 <= s_out_d4;
                s_out_d6 <= s_out_d5;
                s_out_d7 <= s_out_d6;
                scndry_out_int_d1 <= s_out_re;
            end
    end
    assign scndry_out = scndry_out_int_d1;
    assign prmry_ack = 1'b0; 
    assign scndry_vect_out = 0;
end
endgenerate


generate if (c_mtbf_stages == 2 & c_cdc_type == 0) begin

    assign s_out_re = ( s_out_d2 ^ s_out_d3 );

end
endgenerate

generate if (c_mtbf_stages == 3 & c_cdc_type == 0) begin

    assign s_out_re = ( s_out_d3 ^ s_out_d4 );

end
endgenerate

generate if (c_mtbf_stages == 4 & c_cdc_type == 0) begin

    assign s_out_re = ( s_out_d4 ^ s_out_d5 );

end
endgenerate

generate if (c_mtbf_stages == 5 & c_cdc_type == 0) begin

    assign s_out_re = ( s_out_d5 ^ s_out_d6 );

end
endgenerate

generate if (c_mtbf_stages == 6 & c_cdc_type == 0) begin

    assign s_out_re = ( s_out_d6 ^ s_out_d7 );

end
endgenerate


//Level Synchronizer Logic with out ACK

generate if (c_flop_input == 1 & c_cdc_type == 1 & c_single_bit == 1) begin

    always @ (  posedge prmry_aclk)
    begin : FLOP_IN
            if ( ( prmry_rst_n == 1'b0 ) & ( c_reset_state == 1 ) ) 
            begin
                p_level_in_d1_cdc_from <= 1'b0;
            end
            else
            begin 
                p_level_in_d1_cdc_from <= prmry_in;
            end
    end

   assign p_level_in_int = p_level_in_d1_cdc_from;

end
endgenerate


generate if (c_flop_input == 0 & c_cdc_type == 1 & c_single_bit == 1) begin


   assign p_level_in_int = prmry_in;

end
endgenerate


//generate if (c_cdc_type == 1) begin 
generate if (c_single_bit == 1 & c_cdc_type == 1) begin

    assign prmry_ack = 1'b0; 
    assign scndry_vect_out = 0;

    always @ (  posedge scndry_aclk)
    begin : CROSS_PLEVEL_IN2SCNDRY
            if ( ( scndry_rst_n == 1'b0 ) & ( c_reset_state == 1 ) ) 
            begin
                s_level_out_d1_cdc_to <= 1'b0;
                s_level_out_d2 <= 1'b0;
                s_level_out_d3 <= 1'b0;
                s_level_out_d4 <= 1'b0;
                s_level_out_d5 <= 1'b0;
                s_level_out_d6 <= 1'b0;
            end
            else
            begin 
                s_level_out_d1_cdc_to <= p_level_in_int;
                s_level_out_d2 <= s_level_out_d1_cdc_to;
                s_level_out_d3 <= s_level_out_d2;
                s_level_out_d4 <= s_level_out_d3;
                s_level_out_d5 <= s_level_out_d4;
                s_level_out_d6 <= s_level_out_d5;
            end
    end
end
endgenerate


generate if (c_mtbf_stages == 1 & c_cdc_type == 1 & c_single_bit == 1) begin

    assign scndry_out = s_level_out_d1_cdc_to;
end
endgenerate

generate if (c_mtbf_stages == 2 & c_cdc_type == 1 & c_single_bit == 1) begin

    assign scndry_out = s_level_out_d2;
end
endgenerate

generate if (c_mtbf_stages == 3 & c_cdc_type == 1 & c_single_bit == 1) begin

    assign scndry_out = s_level_out_d3;
end
endgenerate

generate if (c_mtbf_stages == 4 & c_cdc_type == 1 & c_single_bit == 1) begin

    assign scndry_out = s_level_out_d4;
end
endgenerate

generate if (c_mtbf_stages == 5 & c_cdc_type == 1 & c_single_bit == 1) begin

    assign scndry_out = s_level_out_d5;
end
endgenerate

generate if (c_mtbf_stages == 6 & c_cdc_type == 1 & c_single_bit == 1) begin

    assign scndry_out = s_level_out_d6;
end
endgenerate

generate if (c_single_bit == 0 & c_cdc_type == 1) begin

    assign prmry_ack = 1'b0; 
    assign scndry_out = 1'b0;

    always @ (  posedge scndry_aclk)
    begin : CROSS_PLEVEL_IN2SCNDRY
            if ( ( scndry_rst_n == 1'b0 ) & ( c_reset_state == 1 ) ) 
            begin
                s_level_out_bus_d1_cdc_to <= 0;
                s_level_out_bus_d2 <= 0 ;
                s_level_out_bus_d3 <= 0 ;
                s_level_out_bus_d4 <= 0 ;
                s_level_out_bus_d5 <= 0 ;
                s_level_out_bus_d6 <= 0 ;
            end
            else
            begin 
                s_level_out_bus_d1_cdc_to <= prmry_vect_in;
                s_level_out_bus_d2 <= s_level_out_bus_d1_cdc_to;
                s_level_out_bus_d3 <= s_level_out_bus_d2;
                s_level_out_bus_d4 <= s_level_out_bus_d3;
                s_level_out_bus_d5 <= s_level_out_bus_d4;
                s_level_out_bus_d6 <= s_level_out_bus_d5;
            end
    end

end
endgenerate

generate if (c_mtbf_stages == 1 & c_single_bit == 0 & c_cdc_type == 1) begin

    assign scndry_vect_out = s_level_out_bus_d1_cdc_to;
end
endgenerate

generate if (c_mtbf_stages == 2 & c_single_bit == 0 & c_cdc_type == 1) begin

    assign scndry_vect_out = s_level_out_bus_d2;
end
endgenerate

generate if (c_mtbf_stages == 3 & c_single_bit == 0 & c_cdc_type == 1) begin

    assign scndry_vect_out = s_level_out_bus_d3;
end
endgenerate

generate if (c_mtbf_stages == 4 & c_single_bit == 0 & c_cdc_type == 1) begin

    assign scndry_vect_out = s_level_out_bus_d4;
end
endgenerate

generate if (c_mtbf_stages == 5 & c_single_bit == 0 & c_cdc_type == 1) begin

    assign scndry_vect_out = s_level_out_bus_d5;
end
endgenerate

generate if (c_mtbf_stages == 6 & c_single_bit == 0 & c_cdc_type == 1) begin

    assign scndry_vect_out = s_level_out_bus_d6;
end
endgenerate


//Level synchronizer logic with ACK
generate if (c_flop_input == 1 & c_cdc_type == 2) begin

    always @ (  posedge prmry_aclk)
    begin : FLOP_IN
            if ( ( prmry_rst_n == 1'b0 ) & ( c_reset_state == 1 ) ) 
            begin
                p_level_in_d1_cdc_from <= 1'b0;
            end
            else
            begin 
                p_level_in_d1_cdc_from <= prmry_in;
            end
    end

   assign p_level_in_int = p_level_in_d1_cdc_from;

end
endgenerate


generate if (c_flop_input == 0 & c_cdc_type == 2) begin

   assign p_level_in_int = prmry_in;

end
endgenerate

generate if (c_cdc_type == 2) begin
    always @ (  posedge scndry_aclk)
    begin : CROSS_PLEVEL_IN2SCNDRY
            if ( ( scndry_rst_n == 1'b0 ) & ( c_reset_state == 1 ) ) 
            begin
                s_level_out_d1_cdc_to <= 1'b0;
                s_level_out_d2 <= 1'b0;
                s_level_out_d3 <= 1'b0;
                s_level_out_d4 <= 1'b0;
                s_level_out_d5 <= 1'b0;
                s_level_out_d6 <= 1'b0;
            end
            else
            begin 
                s_level_out_d1_cdc_to <= p_level_in_int;
                s_level_out_d2 <= s_level_out_d1_cdc_to;
                s_level_out_d3 <= s_level_out_d2;
                s_level_out_d4 <= s_level_out_d3;
                s_level_out_d5 <= s_level_out_d4;
                s_level_out_d6 <= s_level_out_d5;
            end
    end

    always @ (  posedge prmry_aclk)
    begin : CROSS_PLEVEL_SCNDRY2PRMRY
            if ( ( prmry_rst_n == 1'b0 ) & ( c_reset_state == 1 ) ) 
            begin
                p_level_out_d1_cdc_to <= 1'b0;
                p_level_out_d2 <= 1'b0;
                p_level_out_d3 <= 1'b0;
                p_level_out_d4 <= 1'b0;
                p_level_out_d5 <= 1'b0;
                p_level_out_d6 <= 1'b0;
                p_level_out_d7 <= 1'b0;
                prmry_ack_int <= 1'b0;
            end
            else
            begin 
                p_level_out_d1_cdc_to <= scndry_out_int;
                p_level_out_d2 <= p_level_out_d1_cdc_to;
                p_level_out_d3 <= p_level_out_d2;
                p_level_out_d4 <= p_level_out_d3;
                p_level_out_d5 <= p_level_out_d4;
                p_level_out_d6 <= p_level_out_d5;
                p_level_out_d7 <= p_level_out_d6;
                prmry_ack_int <= prmry_pulse_ack;
            end
    end
    assign prmry_ack = prmry_ack_int;
    assign scndry_out = scndry_out_int;
    assign scndry_vect_out = 0;
end
endgenerate

  generate if ((c_mtbf_stages == 2 || c_mtbf_stages == 1) & c_cdc_type == 2) begin
  
      assign scndry_out_int = s_level_out_d2;
      assign prmry_pulse_ack = ( p_level_out_d3 ^ p_level_out_d2 );
  end
  endgenerate
  
  generate if (c_mtbf_stages == 3 & c_cdc_type == 2) begin
  
      assign scndry_out_int = s_level_out_d3;
      assign prmry_pulse_ack = ( p_level_out_d4 ^ p_level_out_d3 );
  end
  endgenerate
  
  generate if (c_mtbf_stages == 4 & c_cdc_type == 2) begin
  
      assign scndry_out_int = s_level_out_d4;
      assign prmry_pulse_ack = ( p_level_out_d5 ^ p_level_out_d4 );
  end
  endgenerate
  
  generate if (c_mtbf_stages == 5 & c_cdc_type == 2) begin
  
      assign scndry_out_int = s_level_out_d5;
      assign prmry_pulse_ack = ( p_level_out_d6 ^ p_level_out_d5 );
  end
  endgenerate
  
  generate if (c_mtbf_stages == 6 & c_cdc_type == 2) begin
  
      assign scndry_out_int = s_level_out_d6;
      assign prmry_pulse_ack = ( p_level_out_d7 ^ p_level_out_d6 );
  end
  endgenerate

endmodule 
