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
module constraints(  input clk1,clk2,
                     input reset, 
                    
                     input data_in,
                                  
                     output reg data_out);  

    reg data_in_q, data_out_q;
        
        
    always @(posedge clk1, posedge reset)
        if(reset)
            data_in_q <= 1'b0;
        else 
            data_in_q <= data_in; 
    
    always @(posedge clk2, posedge reset)
        if(reset) begin
            data_out_q  <= 1'b0;
            data_out    <= 1'b0;
        end    
        else begin
            data_out_q <= data_in_q; 
            data_out   <= data_out_q; 
        end       

endmodule // constraints