`timescale 1ns / 1ns

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

module edge_tb;

	// Inputs
	reg clk2;
	reg reset;
	reg data_in;

	// Outputs
	wire data_out;

	edge_detector uut (
		.clk2(clk2), 
		.reset(reset), 
		.data_in(data_in), 
		.data_out(data_out));

	initial begin
		reset = 1;
		data_in = 0;

		#100;
		reset = 0;
		        
		#100;
		data_in = 1;
		#5;
		data_in = 0;

		#200;
		$finish(2);
	end
      
	initial begin
		clk2 = 0;
		forever #10 clk2 = ~clk2;
	end

endmodule

