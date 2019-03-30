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
`timescale 1ns / 100ps

module dsp48_usage(  input clk,
                     input reset,
                     input [47:0] a,b, 
                     output [47:0] c_async_reset,c_sync_reset,

                     input signed [7:0] mult_a,mult_b, 
                     output signed [15:0] mult_c
                
                     );  
             
    adder_sync_reset  adder_sync_reset(clk,reset,a,b,c_sync_reset);  
    
    adder_async_reset adder_async_reset(clk,reset,a,b,c_async_reset);  
               
    mult mult(clk,reset,mult_a,mult_b,mult_c);  
    
endmodule // dsp48_usage


module adder_sync_reset( input clk,
              input reset,
              input [47:0] a,b, 
              output reg [47:0] c);  
                
    always @(posedge clk)
        if(reset) begin
            c <= 'b0;
        end    
        else begin
            c <= a + b ;
        end       
    
endmodule // adder_sync_reset



module adder_async_reset( input clk,
                          input reset,
                          input [47:0] a,b, 
                          output reg [47:0] c);  
                
    always @(posedge clk, posedge reset)
        if(reset) begin
            c <= 'b0;
        end    
        else begin
            c <= a + b ;
        end       
    
endmodule // adder_async_reset


module mult(  input clk,
              input reset,
              input signed [7:0] a ,b, 
              output reg signed [15:0] c);  
                
    always @(posedge clk)
        if(reset) begin
            c <= 'b0;
        end    
        else begin
            c <= a * b ;
        end       
    
endmodule // mult
