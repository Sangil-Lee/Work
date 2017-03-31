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

module tb_deltas;

    reg clk1;
    reg clk2,clk3;
	reg data;
	reg d_in, d_out; // registers
	reg d_delta1,d_delta2; 
	reg reset;
    
	initial begin
	    reset    = 1;
		data     = 0;

		#15; reset = 0;
		#15;

        @(posedge clk1);
		data = 1;
        @(posedge clk1);
		data = 0;

        #50;
		$finish(2);
	end

    initial begin
		clk1 = 0;
        forever #5 clk1 = ~clk1; 
    end
    
	// two sim deltas on a clock 
	always @(*) begin
	    clk2 <= clk1;
		clk3 <= clk2;
	end

	always @(*) begin
        d_delta1 <= d_in;
        d_delta2 <= d_delta1;
	end 

    always @(posedge clk1)
	    if(reset)  d_in <= 1'b0;
		else 	   d_in <= data;


    always @(posedge clk2)
	    if(reset)  d_out <= 1'b0;
		else 	   d_out <= d_in;  


    always @(posedge clk1) 
	    $display("clk1");

    always @(posedge clk2) 
	    $display("clk2");

    always @(posedge clk3) 
	    $display("clk3");

    always @(posedge d_in) 
	    $display("d_in");

    always @(posedge d_delta1) 
	    $display("d_delta1");

    always @(posedge d_delta2) 
	    $display("d_delta2");

endmodule

