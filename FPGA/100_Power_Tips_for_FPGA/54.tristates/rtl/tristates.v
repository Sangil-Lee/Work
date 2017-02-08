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
module tristates( input clk,
                  input reset, 
                  input [1:0] d1_in,
                  input d1_en,                  
                  input [1:0] d2_in,                  
                  input d2_en,
                  output reg dout
                );  

    tri tri_internal;
    
    assign tri_internal = d1_en ? |d1_in : 1'bz;
    assign tri_internal = d2_en ? ^d2_in : 1'bz;
    
    always @(posedge clk, posedge reset) begin
        if(reset)
            dout <= 0;
        else
            dout <= tri_internal;    
    end
endmodule // tristates