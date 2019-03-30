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
module comb_loops(input clk,
                  
                  input latch_s,latch_r,
                  output latch_q,latch_q_b,
                  
                  inout keeper0, keeper1,
                  input keeper_ext,
                  
                  input  reset_loop_i,
                  output reg reset_loop_o
                                    
                );  


    nor nor1(latch_q, latch_s,latch_q_b);
    nor nor2(latch_q_b, latch_r,latch_q);
    
    nor not1(keeper0, keeper_ext, keeper1);
    not not2(keeper1, keeper0);    
    
    
    wire reset_loop_i_b = ~reset_loop_o & reset_loop_i;
    
    always @(posedge clk, posedge reset_loop_i_b)
        if(reset_loop_i_b) begin
            reset_loop_o <= 'b0;
        end    
        else begin
            reset_loop_o <= reset_loop_i; 
        end       
    
       
endmodule // memories