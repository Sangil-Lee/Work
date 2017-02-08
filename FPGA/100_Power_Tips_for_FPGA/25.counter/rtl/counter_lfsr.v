//-----------------------------------------------------------------------------
// Copyright (C) 2011 OutputLogic.com 
// This source file may be used and distributed without restriction 
// provided that this copyright statement is not removed from the file 
// and that any derivative work contains the original copyright notice 
// and the associated disclaimer. 
// 
// THIS SOURCE FILE IS PROVIDED "AS IS" AND WITHOUT ANY EXPRESS 
// OR IMPLIED WARRANTIES, INCLUDING, WITHOUT LIMITATION, THE IMPLIED	
// WARRANTIES OF MERCHANTIBILITY AND FITNESS FOR A PARTICULAR PURPOSE. 
//-----------------------------------------------------------------------------
`define MATCH_PATTERN_64       64'h8123456790ABCDEF
`define MATCH_PATTERN          32'h81234567
`define MATCH_PATTERN_PRESCALE 16'h8123

`timescale 1ns / 100ps


// match 32'h81234567
module lfsr_counter(
        input clk,
        input reset,
        input ce,
        output reg [31:0] lfsr);

    
    wire d0;

    xnor(d0,lfsr[31],lfsr[21],lfsr[1],lfsr[0]);

    always @(posedge clk,posedge reset) begin
        if(reset) begin
            lfsr <= 32'h0;
        end
        else begin
            if(ce)
                lfsr <= {lfsr[30:0],d0};
        end
    end
endmodule
   

