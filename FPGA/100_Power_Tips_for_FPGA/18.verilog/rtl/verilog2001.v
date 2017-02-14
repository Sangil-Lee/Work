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
`timescale 1ns / 100ps

module verilog2001(  input clk,
                     input reset,
                     input [63:0] a,b, 
                     output [63:0] sum1, sum2, sum3,

                     input [2:0] exp_in,
                     output [7:0] exp_out1, 
					 output [7:0] exp_out2
                
                     );  
    // array         
	generate
	genvar ix;
		for (ix=0; ix<=7; ix=ix+1) begin : adder_gen1
			adder add (a[8*ix+7 -: 8], b[8*ix+7 -: 8], sum1[8*ix+7 -: 8]); 
	    end
	endgenerate


    // if...else
	parameter WIDTH = 4;

	generate
		if (WIDTH < 64) begin : adder_gen2
		    // pad sum2
			assign sum2[63 -: (64-WIDTH)] = 'b0;
			adder # (WIDTH) adder1 (a[WIDTH-1 -: WIDTH], 
									b[WIDTH-1 -: WIDTH], 
									sum2[WIDTH-1 -: WIDTH]);
	    end
		else begin : adder_default
			adder # (64) adder2 (a, b, sum2);
		end
	endgenerate

	// case
	generate
		case (WIDTH)
			1: begin : case1
				assign sum3[63 -: 63] = 'b0;
				adder #(WIDTH) adder3 (a[0], b[0], sum3[0]);
				end
			default: begin : def
				adder # (64) adder4 (a, b, sum3);
				end
		endcase
	endgenerate


    localparam BASE = 3,
	           EXP = 4;

    assign exp_out1 = 2**exp_in;
    assign exp_out2 = BASE**EXP;

endmodule // verilog2001




module  adder #(parameter WIDTH = 8)  (input [WIDTH-1:0] a,b, output [WIDTH-1:0] sum );  
             
	assign sum = a + b;

endmodule // adder
