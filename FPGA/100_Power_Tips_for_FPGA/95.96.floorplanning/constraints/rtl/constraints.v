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

module constraints(input clk, reset,
                   output reg data_out_0,data_out_1);
                
    always @(posedge clk) begin
        if (reset) begin
            data_out_0 <= 1'b0;
            data_out_1 <= 1'b0;
        end
        else begin
            data_out_0 <= ~data_out_0;
            data_out_1 <= ~data_out_1;
        end
    end // always   
    
        
endmodule // constraints


