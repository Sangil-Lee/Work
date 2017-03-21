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
`timescale 1ns/100ps

module tb;

    reg clk1,clk2;
	reg reset;
    
    localparam HALF_PERIOD1 = 2.5,
               HALF_PERIOD2 = 3.333;
    
    initial begin
		clk1 = 0;
        forever #HALF_PERIOD1 clk1 = ~clk1; 
    end
    
    initial begin
		clk2 = 0;
        forever #HALF_PERIOD2 clk2 = ~clk2; 
    end

	initial begin
	    $display("[%0t] starting simulation", $realtime);

		reset = 1;
        
        #10000;

	    $display("[%0t] finishing simulation", $realtime);

        $finish(2);
	end
endmodule

