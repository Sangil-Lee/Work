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

module tb;

	reg wr_clk;
	reg rd_clk;
	reg reset;
	reg enable;

	wire [15:0] data_out;

	floorplanning uut (
		.wr_clk(wr_clk), 
		.rd_clk(rd_clk), 
		.reset(reset), 
		.enable(enable), 
		.data_out(data_out)
	);

	initial begin
		reset = 1;
		enable = 0;
		#100;
		reset = 0;
        enable = 1;

        #1000;
        
        $finish;
	end
      
	initial begin
		wr_clk = 0;
        forever #5 wr_clk = ~wr_clk;
    end
    
	initial begin
		rd_clk = 0;
        forever #8 rd_clk = ~rd_clk;
    end
      
endmodule

