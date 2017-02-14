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

module reset;

    reg clk,reset;
    
    reg shift0, shift1;


    initial begin
        clk = 0;
        forever #5 clk = ~clk;
    end
    
    initial begin
        reset = 1;
        #50;

        force shift1 = 1;
        
        @(negedge clk);
        reset = 0;
        
        @(posedge clk);
        #2;
        release shift1;
        
        #100;
        $finish(2);
        
    end 
        
    // positive edge clock, asynchronous active-high reset
    always @( posedge clk , posedge reset)
        if( reset ) begin
            shift0 <= 1'b0;
            shift1 <= 1'b1;
        end                
        else begin
            {shift1,shift0} <= #1 {shift0,shift1};
        end
        
endmodule // reset


    



