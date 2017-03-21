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
	reg [63:0] a;
	reg [63:0] b;
	reg [2:0] exp_in;
	reg [31:0] part_sel_in;
    reg        always_block_in;
    
	wire [63:0] sum1;
	wire [63:0] sum2;
	wire [63:0] sum3;
	wire [7:0] exp_out1;
	wire [7:0] exp_out2;
	wire [0:31] part_sel_out;
    
	wire port_a;

	coding_style uut (
		.clk(clk), 
		.reset(reset), 
		.a(a), 
		.b(b), 
		.sum1(sum1), 
		.sum2(sum2), 
		.sum3(sum3), 
		.exp_in(exp_in), 
		.exp_out1(exp_out1), 
		.exp_out2(exp_out2), 
		.port_a(port_a), 
		.part_sel_in(part_sel_in), 
		.part_sel_out(part_sel_out),
        .always_block_in(always_block_in),
        .always_block_out()
	);
    
    
	initial begin
		a = 0;
		b = 0;
		exp_in = 0;
		part_sel_in = 1;
        always_block_in = 0;
        
		reset = 1;
        #100;
        reset = 0;
        
	    repeat(100) begin
            #10;
            part_sel_in = part_sel_in * 2;
        end    
        
        $finish(2);
	end
    
    initial begin
        clk = 0;
        forever #5 clk = ~clk;
    end    
endmodule

