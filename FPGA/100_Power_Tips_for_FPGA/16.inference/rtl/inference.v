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

module inference( input  clk,areset, sreset,
                  input  [4:0] d_in,
                  output reg [4:0] d_out,
                  
                  input srl_enable,
                  output srl1_out,
                  
                  input ddr_in,
                  output  ddr_out,
                  
                  input latch_en,latch_data,
                  output reg latch_out,
                  
                  output tri_out,
                  input tri_en, tri_in,
                  
                  output reg tri_out2,
                  input tri_en1,tri_en2, tri_in2
                  
                     );  
                     
    // positive edge clock, asynchronous active-high reset
    always @( posedge clk , posedge areset)
        if( areset )
            d_out[0] <= 1'b0;
        else
            d_out[0] <= d_in[0];


    // negative edge clock, asynchronous active-low reset
    always @( negedge clk , negedge areset)
        if( ~areset )
            d_out[1] <= 1'b0;
        else
            d_out[1] <= d_in[1];
     
     
    // negative edge clock, no reset
    always @( negedge clk )
        d_out[2] <= d_in[2];


    // positive edge clock, synchronous active-high reset. 
    // The register is initialized with '1' upon reset release
    always @( posedge clk )
        if( sreset )
            d_out[3] <= 1'b1;
        else
            d_out[3] <= d_in[3];


    // cannot infer both synchronous and asynchronous resets
    // The circuit is implemented using additional logic
    always @( posedge clk , posedge areset)
        if( sreset )
            d_out[4] <= 1'b1;
        else if( areset )
            d_out[4] <= 1'b0;
        else
            d_out[4] <= d_in[4];
            
          
   /*       
    // Will work in simulation. Initial block is ignored during synthesis      
    initial begin
        d_out[2] = 1'b0;
    end
            
    // Register initialization during declaration        
    reg my_reg = 1'b0;

    // Register initialization upon reset
    always @( posedge clk , posedge reset)
        if( reset )
            d_out[1] <= 1'b0;
        else
            d_out[1] <= d_in[1];
   */
   
    reg [7:0] srl1;
   
    always @(posedge clk)
       srl1 <= srl_enable ? {srl1[6:0] , d_in[0]} : srl1;

    assign srl1_out = srl1[7]; 
    
    

    reg q1, q2;    
    assign ddr_out = q1 & q2;

    always @ (posedge clk, posedge areset)
        if (areset)
            q1 <=1'b0;
        else
            q1 <= ddr_in;
    
    always @ (negedge clk, posedge areset)
        if (areset)
            q2 <=1'b0;
        else
            q2 <= ddr_in;
    

    always @ (latch_en)
        if (latch_en)
            latch_out = latch_data;
    
    
    assign tri_out = tri_en ? tri_in : 1'bz;
    
    
    always @ (*)
        if ( tri_en1 & tri_en2 )
            tri_out2 = 1'bz;
        else
            tri_out2<=tri_in2;
    
    
endmodule // inference


    



