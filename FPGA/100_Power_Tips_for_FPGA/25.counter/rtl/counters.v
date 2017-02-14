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
`define MATCH_PATTERN_64       64'h8123456790ABCDEF
`define MATCH_PATTERN          32'h81234567
`define MATCH_PATTERN_PRESCALE 16'h8123

`timescale 1ns / 100ps


module counters(  input clk_prescale, clk_binary , clk_binary_64, clk_binary_core_32 , 
                        clk_binary_core_64, clk_binary_dsp48 , clk_lfsr , clk_johnson,

                  input reset, 
                  
                  input counter_binary_en,
		       		    counter_binary_64_en,
                        counter_prescale_en,
                        counter_binary_core_32_en,
                        counter_binary_core_64_en,
                        counter_binary_dsp_en,
                        counter_lfsr_en,
                        counter_johnson_en,
                  
                  output [31:0]  counter_binary_q,
                                 counter_johnson_q, 
                                 counter_binary_core_32_q,
                                 counter_binary_dsp_q,                                   

				  output [63:0]  counter_binary_64_q,
                                 counter_binary_core_64_q,

                  output [15:0]  counter_prescale_q,

                  output         counter_binary_core_32_match,
				 			     counter_binary_core_64_match,
                                 counter_binary_match, 
                                 counter_binary_64_match, 
                                 counter_johnson_match,
                                 counter_prescale_match,
                                 counter_binary_dsp_match,
                                 counter_lfsr_done_q    
                );  
                
    counter_binary counter_binary(clk_binary,reset,
                                  counter_binary_en, 
                                  counter_binary_q, 
                                  counter_binary_match);  

    counter_binary_64 counter_binary_64(clk_binary_64,reset,
  		                                  counter_binary_64_en, 
        		                          counter_binary_64_q, 
                		                  counter_binary_64_match);  
   
    counter_prescale counter_prescale(clk_prescale,reset,
                                      counter_prescale_en, 
                                      counter_prescale_q, 
                                      counter_prescale_match);  
   

	counter_binary_core_32 counter_binary_core_32 (
		.clk(clk_binary_core_32),
		.ce(counter_binary_core_32_en),
		.sclr(reset),
		.q(counter_binary_core_32_q), 
        .thresh0(counter_binary_core_32_match));
        
        
	counter_binary_core_64 counter_binary_core_64 (
		.clk(clk_binary_core_64),
		.ce(counter_binary_core_64_en),
		.sclr(reset),
		.q(counter_binary_core_64_q), 
        .thresh0(counter_binary_core_64_match));


	counter_binary_dsp48 counter_binary_dsp48 (
		.clk(clk_binary_dsp48),
		.ce(counter_binary_dsp_en),
		.sclr(reset),
		.q(counter_binary_dsp_q),
        .thresh0(counter_binary_dsp_match)); 
        
        
    lfsr_counter lfsr_counter(clk_lfsr,reset,counter_lfsr_en,counter_lfsr_done_q); 
    
    counter_johnson counter_johnson(clk_johnson,reset,counter_johnson_en, counter_johnson_q, counter_johnson_match);
        
    
endmodule // counters


module counter_binary(input clk,
                      input reset, 
                      input counter_binary_en,
                      output reg [31:0] counter_binary,
                      output reg  counter_binary_match                      
                    );  
                    
    always @(posedge clk)
        if(reset) begin
            counter_binary       <= 'b0;
            counter_binary_match <= 1'b0;
        end    
        else begin
            counter_binary       <= counter_binary_en ? counter_binary + 1'b1 : counter_binary;
            counter_binary_match <= counter_binary == `MATCH_PATTERN;
        end       
    
endmodule // counter_binary


module counter_binary_64(input clk,
                         input reset, 
                         input counter_binary_en,
                         output reg [63:0] counter_binary,
                         output reg  counter_binary_match                      
                       );  
                    
    always @(posedge clk)
        if(reset) begin
            counter_binary       <= 'b0;
            counter_binary_match <= 1'b0;
        end    
        else begin
            counter_binary       <= counter_binary_en ? counter_binary + 1'b1 : counter_binary;
            counter_binary_match <= counter_binary == `MATCH_PATTERN_64;
        end       
    
endmodule // counter_binary


module counter_prescale(input clk,
                        input reset, 
                        input counter_prescale_en,
                        output [15:0]  counter_prescale,
                        output reg counter_prescale_match);  
                    
    reg [15:0] counter_prescale_low,counter_prescale;

    always @(posedge clk)
        if(reset) begin
            counter_prescale_low <= 'b0;
        end    
        else begin
            counter_prescale_low <= counter_prescale_en  ? counter_prescale_low + 1'b1 : counter_prescale_low;
        end       


    always @(posedge counter_prescale_low[15], posedge reset)
        if(reset) begin
            counter_prescale       <= 'b0;
            counter_prescale_match <= 1'b0;
        end    
        else begin
            counter_prescale       <= counter_prescale + 1'b1;
            counter_prescale_match <= counter_prescale == `MATCH_PATTERN_PRESCALE;  
        end       
    
endmodule // counter_prescale


// match 32'h81234567
module lfsr_counter(
        input clk,
        input reset,
        input ce,
        output reg lfsr_done);

    reg [31:0] lfsr;
    wire d0,lfsr_equal;

    xnor(d0,lfsr[31],lfsr[21],lfsr[1],lfsr[0]);
    assign lfsr_equal = (lfsr == 32'h1565249);

    always @(posedge clk,posedge reset) begin
        if(reset) begin
            lfsr <= 32'h0;
            lfsr_done <= 0;
        end
        else begin
            if(ce)
                lfsr <= lfsr_equal ? 32'h0 : {lfsr[30:0],d0};
            lfsr_done <= lfsr_equal;
        end
    end
endmodule
   

module counter_johnson(input clk,
                       input reset, 
                       input counter_johnson_en,
                       output reg [31:0] counter_johnson_q,
                       output reg counter_johnson_match
                     );  
    
    always @(posedge clk, posedge reset)
        if(reset) begin
            counter_johnson_q     <= 'b0;
            counter_johnson_match <= 1'b0;
        end    
        else begin   
            counter_johnson_q     <= counter_johnson_en ? {counter_johnson_q[30:0], ~counter_johnson_q[31]} : counter_johnson_q; 
            counter_johnson_match <= counter_johnson_q == `MATCH_PATTERN;
        end    
    
endmodule // counter_binary
