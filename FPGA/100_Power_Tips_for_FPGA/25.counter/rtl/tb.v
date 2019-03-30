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

    reg clk;
    
	reg reset;
	reg counter_binary_en;
	reg counter_binary_64_en;
	reg counter_prescale_en;
	reg counter_binary_core_32_en;
	reg counter_binary_core_64_en;
	reg counter_binary_dsp_en;
	reg counter_lfsr_en;
	reg counter_johnson_en;

	// Outputs
	wire [31:0] counter_binary_q;
	wire [31:0] counter_johnson_q;
	wire [15:0] counter_prescale_q;
	wire [31:0] counter_binary_core_32_q;
	wire [31:0] counter_binary_dsp_q;
	wire counter_binary_core_32_match;
	wire counter_binary_match;
	wire counter_johnson_match;
	wire counter_prescale_match;
	wire counter_binary_dsp_match;
	wire counter_lfsr_done_q;

	// Instantiate the Unit Under Test (UUT)
	counters uut (
		.clk_prescale(clk), 
		.clk_binary(clk), 
        .clk_binary_64(clk), 
        .clk_binary_core_32(clk), 
		.clk_binary_core_64(clk), 
		.clk_binary_dsp48(clk),
		.clk_lfsr(clk_lfsr), 
		.clk_johnson(clk_johnson), 

		.reset(reset), 
		.counter_binary_en(counter_binary_en), 
		.counter_binary_64_en(counter_binary_64_en),
		.counter_prescale_en(counter_prescale_en), 
		.counter_binary_core_32_en(counter_binary_core_32_en), 
		.counter_binary_core_64_en(counter_binary_core_64_en), 
		.counter_binary_dsp_en(counter_binary_dsp_en), 
		.counter_lfsr_en(counter_lfsr_en), 
		.counter_johnson_en(counter_johnson_en), 
		.counter_binary_q(counter_binary_q), 
		.counter_johnson_q(counter_johnson_q), 
		.counter_prescale_q(counter_prescale_q), 
		.counter_binary_core_32_q(counter_binary_core_32_q), 
		.counter_binary_dsp_q(counter_binary_dsp_q), 
		.counter_binary_core_32_match(counter_binary_core_32_match), 
		.counter_binary_match(counter_binary_match), 
		.counter_johnson_match(counter_johnson_match), 
		.counter_prescale_match(counter_prescale_match), 
		.counter_binary_dsp_match(counter_binary_dsp_match), 
		.counter_lfsr_done_q(counter_lfsr_done_q)
	);
    
    
    initial begin
		clk = 0;
        forever #5 clk = ~clk; 
    end
    

	initial begin
	    $display("[%0t] starting simulation", $realtime);

		reset = 1;
		counter_binary_en = 0;
		counter_binary_64_en = 0;
		counter_prescale_en = 0;
		counter_binary_core_32_en = 0;
		counter_binary_core_64_en = 0;
		counter_binary_dsp_en = 0;
		counter_lfsr_en = 0;
		counter_johnson_en = 0;

        #200;
		reset = 0;
        #100;
        
		counter_binary_en = 1;
		counter_binary_64_en = 1;
		counter_prescale_en = 1;
		counter_binary_core_32_en = 1;
		counter_binary_core_64_en = 1;
		counter_binary_dsp_en = 1;
		counter_lfsr_en = 1;
		counter_johnson_en = 1;
        
        
        #(1000*1000*1000);

	    $display("[%0t] finishing simulation", $realtime);

        $finish;
	end
    
      
endmodule

