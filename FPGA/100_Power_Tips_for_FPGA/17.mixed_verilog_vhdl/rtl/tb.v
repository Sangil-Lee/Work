`timescale 1ns / 100ps

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

module tb;

	// Inputs
	reg clk;
	reg enable;
	reg rst;

	// Outputs
	wire done_qo;

    top top (
        .enable(enable), 
        .rst(rst), 
        .clk(clk), 
        .done_qo(done_qo)
        );

	initial begin
		rst = 1;
        enable = 0;
		#100;
		rst = 0;
        enable = 1;
        #10000;
        
        $finish;
        
	end
    
    
    initial begin
		clk = 0;
        forever #5 clk = ~clk; 
    end
    
      
endmodule

