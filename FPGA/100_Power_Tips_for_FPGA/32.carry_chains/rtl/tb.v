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
	reg [11:0] user_in;
	// Outputs
	wire user_out;

    carry_chains uut(user_in,user_out);                               

	initial begin
        user_in = 12'h0;
        
        repeat(10) begin
            #100;
            user_in = user_in + 4'hF;
        end
        
        $finish;    
                    
    end
    
      
endmodule

