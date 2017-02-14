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

`timescale 1ns / 1ps

module tb;

	// Inputs
	reg clk;
	reg reset;
	reg [3:0] state_inputs;

	// Outputs
	wire [3:0] state_outputs;

	state_machines uut (
		.clk(clk), 
		.reset(reset), 
		.state_inputs(state_inputs), 
		.state_outputs(state_outputs)
	);

	initial begin
		reset = 1;
		state_inputs = 0;
        #100;
        reset = 0;
		#100;
        
        state_inputs = 4'h1;
        #100;
        state_inputs = 4'h2;
        #100;
        state_inputs = 4'h4;
        #100;
        state_inputs = 4'h8;
        #100;
        
        $finish;
	end
      
	initial begin
		clk = 0;
        forever #5 clk = ~clk;
    end
        
endmodule

