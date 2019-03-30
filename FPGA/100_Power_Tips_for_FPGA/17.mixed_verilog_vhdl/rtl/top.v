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
module top(input enable, rst, clk,
           output reg done_qo  );  

    always @(posedge clk, posedge rst)
        if(rst) begin
            done_qo <= 'b0;
        end    
        else begin
            done_qo <= lfsr_done; 
        end   
        
        
    lfsr_counter lfsr_counter( 
            enable , rst, clk,
            lfsr_done);
    
endmodule // top