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
module bram_inference(   input clk,

                         input [15:0]      mem_din,
                         input [9:0]       mem_addr,
                         input             mem_we,
                         output reg [15:0] mem_dout
                );  

   
    reg [15:0] ram [1:1024];
    
    always @(posedge clk) begin
        if(mem_we)
            ram[mem_addr] <= mem_din;
        
        mem_dout <= ram[mem_addr];
    end       
    
endmodule // bram_inference

