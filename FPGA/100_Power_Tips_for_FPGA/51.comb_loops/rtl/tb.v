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
	reg latch_s;
	reg latch_r;
	reg keeper_ext;
	reg reset_loop_i;

	// Outputs
	wire latch_q;
	wire latch_q_b;
	wire reset_loop_o;

	// Bidirs
	tri0 keeper0;
	tri1 keeper1;

	comb_loops uut (
		.clk(clk), 
		.latch_s(latch_s), 
		.latch_r(latch_r), 
		.latch_q(latch_q), 
		.latch_q_b(latch_q_b), 
		.keeper0(keeper0), 
		.keeper1(keeper1), 
		.keeper_ext(keeper_ext), 
		.reset_loop_i(reset_loop_i), 
		.reset_loop_o(reset_loop_o)
	);

	initial begin
		latch_s = 0;
		latch_r = 0;
		keeper_ext = 0;
		reset_loop_i = 0;

		#100;
        
        latch_s = 1;
        latch_r = 0;
        #50;
        
        latch_s = 0;
        latch_r = 1;
        #50;
        
        latch_s = 1;
        latch_r = 1;
        #50;
        
		latch_s = 0;
		latch_r = 0;
        #50;
        
        keeper_ext = 1;
        #50;
        keeper_ext = 0;
        #50;
        force keeper0 = 1;
        #50
        release keeper0;
        #50;
        force keeper0 = 0;
        #50
        release keeper0;
        #500;
        
        reset_loop_i = 1;
        #50;
        reset_loop_i = 0;
        #500;
        
        
        $finish;
        
	end
    
    
    initial begin
		clk = 0;
        forever #5 clk = ~clk; 
    end
    
      
endmodule

