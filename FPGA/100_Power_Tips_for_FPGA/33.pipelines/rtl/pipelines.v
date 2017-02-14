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

module pipeline(   input clk, reset, enable,
                   input [127:0] data_in,
                   output reg [127:0] data_out
                );  

    reg [127:0] data_in_q,data_in_dq, data_in_2dq, data_in_3dq;
                     
    always @(posedge clk) 
        if(reset) begin
            data_in_q    <= 'b0;
            data_in_dq   <= 'b0;
            data_in_2dq  <= 'b0;
            data_in_3dq  <= 'b0;
            data_out     <= 'b0;
        end    
        else begin
            data_in_q    <= enable ? data_in      : data_in_q;
            data_in_dq   <= enable ? data_in_q    : data_in_dq;
            data_in_2dq  <= enable ? data_in_dq   : data_in_2dq;
            data_in_3dq  <= enable ? data_in_2dq  : data_in_3dq;
            data_out     <= enable ? data_in_3dq  : data_out;            
        end       
    
endmodule // pipeline


