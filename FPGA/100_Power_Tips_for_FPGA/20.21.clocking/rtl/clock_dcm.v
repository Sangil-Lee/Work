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
module clock_dcm(input clk, reset,
                  input data_in,
                   output reg data_out

                       );  

   wire clk_i,locked,clkfb;

  clk_dcm dcm(// Clock in ports
    .CLK_IN1            (clk),     
    .CLKFB_IN           (clkfb), 
    .CLK_OUT1           (clk_i),    
    .CLKFB_OUT          (clkfb),
    .RESET              (reset),     
    .LOCKED             (locked));   


	always @(posedge clk_i, posedge locked)
		if (locked)
            data_out <= 0;
		else
		    data_out <= data_in;

endmodule // clock_dcm