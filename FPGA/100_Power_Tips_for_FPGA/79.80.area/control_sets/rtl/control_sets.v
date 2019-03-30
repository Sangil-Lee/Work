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

// 1 LUT
module reg_reset_only(input clk, reset,set,
                      input clk_en,
                      input [3:0] data_in, 
                      output reg data_out);  
                    
    always @(posedge clk or posedge reset) 
        if(reset) 
            data_out <= 1'b0;
        else if(clk_en) 
            data_out <= &data_in;
        else            
            data_out <= data_out;

endmodule // reg_reset_only


// 2 LUTS in both sync and async reset 
// even if LUT can accomodate another two inputs
module reg_set_reset(input clk, reset,set,
                     input clk_en,
                     input [3:0] data_in, 
                     output reg data_out);  

    always @(posedge clk , posedge reset)  
        if(reset) 
            data_out <= 1'b0;
        else if(set)
            data_out <= 1'b1; 
        else if(clk_en) 
            data_out <= &data_in;
        else            
            data_out <= data_out;

endmodule // reg_set_reset


// mixing synchronous and asynchronous resets 
module control_sets1(input clk, areset,sreset,
                     input clk_en,
                     input [5:0] data_in1,data_in2, 
                     output reg data_out1,data_out2);  

    always @(posedge clk , posedge areset)  
        if(areset) 
            data_out1 <= 1'b0;
        else
            data_out1 <= clk_en ? &data_in1 : data_out1;

    always @(posedge clk)  
        if(sreset) 
            data_out2 <= 1'b0;
        else            
            data_out2 <= clk_en ? &data_in2 : data_out2;

endmodule // control_sets1



// single reset: logic packed into one slice
module control_sets2(input clk, sreset,
                     input clk_en,
                     input [5:0] data_in1,data_in2, 
                     output reg data_out1,data_out2);  

    always @(posedge clk)  
        if(sreset) 
            data_out1 <= 1'b0;
        else
            data_out1 <= clk_en ? &data_in1 : data_out1;

    always @(posedge clk)  
        if(sreset) 
            data_out2 <= 1'b0;
        else            
            data_out2 <= clk_en ? &data_in2 : data_out2;

endmodule // control_sets2



