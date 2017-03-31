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

module priority_encoder( input [5:0] sel1,sel2,sel3, 
                     input [3:0] a1,b1,a2,b2,a3,b3,
                     output [3:0] result1,result2,result3
                    );  
    
    example1 example1(sel1, a1,b1, result1 );  

    example2 example2(sel2, a2,b2, result2 );  
    
    example3 example3(sel3, a3,b3, result3 );      
    
endmodule // coding_example

module example1( input [5:0] sel, 
               input [3:0] a,b,
               output reg [3:0] result1
                    );  
    
    always @(*) begin
    
        result1 = sel[0] ? a + b : sel[1] ? a - b : 
                  sel[2] ? a & b : sel[3] ? a ^ b :          
                  sel[4] ? ~a : ~ b;
                  
        if(~|sel)
            result1 = 4'b0;                           
    
    end // always
                     
endmodule // example1


module example2( input [5:0] sel, 
               input [3:0] a,b,
               output reg [3:0] result2
                    );  
    
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
     
endmodule // example2

module example3( input [5:0] sel, 
               input [3:0] a,b,
               output reg [3:0] result2
                    );  
    
    always @(*) begin
        result2 = 4'b0;
    
        casex(1'b1)
            sel[0]: result2 = a + b;
            sel[1]: result2 = a - b;
            sel[2]: result2 = a & b;
            sel[3]: result2 = a ^+ b;
            sel[4]: result2 = ~a;
            sel[5]: result2 = ~b;
        endcase    
    end // always
     
endmodule // example3
