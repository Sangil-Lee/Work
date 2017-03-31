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
module latches( input clk,
                input reset, 
                input [2:0] d_in,
                input [2:0] d_en,                  
                output reg [2:0] d_out
                );  

    always @(*) 
        if(reset)
            d_out[0] = 1'b0;
        else if( d_en[0])
            d_out[0] = d_in[0];    

    always @(*) 
        if(reset)
            d_out[1] = 1'b1;
        else if( d_en[1])
            d_out[1] = d_in[1];    

    always @(*) 
        if( d_en[2])
            d_out[2] = d_in[2];    

endmodule // latches