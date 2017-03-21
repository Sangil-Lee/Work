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
    wire signed [15:0] shift_signed;
    wire [15:0] shift_unsigned;    

    reg signed [15:0] comp_a, comp_b;
    wire signed comp_signed;
    wire comp_unsigned;                


	signed_arithmetic uut (
		.clk(clk), 
		.reset(reset), 
		.shift_signed(shift_signed),
        .shift_unsigned(shift_unsigned),
		.shift_init_signed(16'h8000),
		.shift_init_unsigned(16'h8000),

        .comp_a(comp_a),
        .comp_b(comp_b),
        .comp_signed(comp_signed),
        .comp_unsigned(comp_unsigned) 
	);

	initial begin
        comp_a = 16'h7f00;
        comp_b = 16'h8000;
        
		reset = 1;
        #100;
        reset = 0;
        
	    repeat(100) begin
            #10;
            comp_a = comp_a + 1;
        end    
        
        $finish;
	end
    
    initial begin
        clk = 0;
        forever #5 clk = ~clk;
    end    
      
endmodule

