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
	reg reset;
	reg enable_shift_ram;
	reg enable_shift_flops;
	reg enable_shift_bram;
	reg shift_ram_din;
	reg shift_flops_din;
	reg shift_bram_din;

	// Outputs
	wire shift_ram_dout;
	wire shift_flops_dout;
	wire shift_bram_dout;

	// Instantiate the Unit Under Test (UUT)
	shift_regs uut (
		.clk_ff(clk), 
		.clk_srl(clk), 
		.clk_bram(clk), 
		.reset(reset), 
		.enable_shift_ram(enable_shift_ram), 
		.enable_shift_flops(enable_shift_flops), 
		.enable_shift_bram(enable_shift_bram), 
		.shift_ram_din(shift_ram_din), 
		.shift_flops_din(shift_flops_din), 
		.shift_bram_din(shift_bram_din), 
		.shift_ram_dout(shift_ram_dout), 
		.shift_flops_dout(shift_flops_dout), 
		.shift_bram_dout(shift_bram_dout)
	);

	initial begin
	    $display("[%0t] starting simulation", $realtime);
    
		reset = 1;
		enable_shift_ram = 0;
		enable_shift_flops = 0;
		enable_shift_bram = 0;
		shift_ram_din = 0;
		shift_flops_din = 0;
		shift_bram_din = 0;

		#100;
		reset = 0;
        
		#100;
        @(posedge clk); #1;
		enable_shift_ram = 1;
		enable_shift_flops = 1;
		enable_shift_bram = 1;
        
		#100;        
        @(posedge clk); #1;        
		shift_ram_din = 1;
		shift_flops_din = 1;
		shift_bram_din = 1;
        
        #1000;
        @(posedge clk); #1;        
		shift_ram_din = 0;
		shift_flops_din = 0;
		shift_bram_din = 0;

        #1000;
        @(posedge clk); #1;        
		enable_shift_ram = 0;
		enable_shift_flops = 0;
		enable_shift_bram = 0;
        
        #100;
	    $display("[%0t] finishing simulation", $realtime);
        $finish;
	end
    
    initial begin
		clk = 0;
        forever #5 clk = ~clk; 
    end
    
      
endmodule

