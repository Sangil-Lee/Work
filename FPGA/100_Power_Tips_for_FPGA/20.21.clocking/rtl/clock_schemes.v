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
module clock_schemes(  input clk1,clk2,clk3,clk4,clk5,
                       input data_in,
                       output reg data_out1,data_out2,data_out3,data_out4,data_out5,data_out6

                       );  

    wire data_from_clock, reset_from_clock, control_from_clock; 

	// Clock used as data input
	assign data_from_clock = clk1;
	always @(posedge clk1)
	    data_out1 <= ~data_out1;

	always @(posedge clk2)
	    data_out2 <= ~data_out2 & data_from_clock;

	// Clock used as reset input
	assign reset_from_clock = clk3;
	always @(posedge clk3)
	    data_out3 <= ~data_out2;

	always @(posedge clk4, posedge reset_from_clock)
		if (reset_from_clock)
            data_out4 <= 0;
		else
		    data_out4 <= data_in;

	// Clock is used as control
	assign control_from_clock = clk5;
	always @(posedge clk5)
	    data_out5 <= ~data_out5;

	always @(*)
		data_out6 = control_from_clock ? data_in : data_out6;
    
    
endmodule // clock_schemes