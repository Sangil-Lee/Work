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

module simple;

    reg [5:0] sel;
    
    reg [3:0] result1,result2,a,b; 
    
    always @(*) begin
    
        result1 = sel[0] ? a + b : sel[1] ? a - b : 
                  sel[2] ? a & b : sel[3] ? a ^ b :          
                  sel[4] ? ~a : ~ b;
                  
        if(~|sel)
            result1 = 4'b0;                           
    
    end // always
                     
    
    always @(*) begin
        casex(sel)
            6'bxxxxx1: result2 = a + b;
            6'bxxxx10: result2 = a - b;
            6'bxxx100: result2 = a & b;
            6'bxx1000: result2 = a ^+ b;
            6'bx10000: result2 = ~a;
            6'b100000: result2 = ~b;
            default:   result2 = 4'b0;
        endcase    
    end // always
    
    initial begin
        sel  = 0;
        a = 0;
        b = 0;
        
        #10;
        sel=1;
        
        repeat(6) begin
            #10;
            sel = sel * 2;
        end

        #10;
        sel=1;
        a = 4'hf;
        b = 4'hf;
        
        repeat(6) begin
            #10;
            sel = sel * 2;
        end

        #10;
        sel=1;
        a = 4'h3;
        b = 4'hc;
        
        repeat(6) begin
            #10;
            sel = sel * 2;
        end
            
        $finish(2);    
    end // initial
    
     
endmodule // simple


