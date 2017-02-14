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
module edge_detector(  input clk2,reset, 
                       input data_in,
		               output reg data_out);  

    reg data_in_q, data_out_q;
        
    wire reset_in;
    
    assign reset_in = (~data_in & data_out) | reset;
        
    always @(posedge data_in, posedge reset_in)
        if(reset_in)
            data_in_q <= 1'b0;
        else 
            data_in_q <= 1'b1; 
    
    always @(posedge clk2, posedge reset)
        if(reset) begin
            data_out_q  <= 1'b0;
            data_out    <= 1'b0;
        end    
        else begin
            data_out_q <= data_in_q; 
            data_out   <= data_out_q; 
        end       

endmodule // edge_detector