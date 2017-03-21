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

module signed_arithmetic(input clk, reset, 
                  
                         output signed [15:0] shift_signed,
                         output [15:0] shift_unsigned,
						 input signed [15:0] shift_init_signed,
						 input [15:0] shift_init_unsigned,

                         input signed [15:0] comp_a, comp_b,
                         output signed comp_signed,
                         output comp_unsigned,

                         input invert,
                         input signed [3:0]  arith_negation_in,
                         output signed [3:0] arith_negation_out                         
                                              
                          );  
                

    signed_shift   signed_shift(clk, reset,shift_signed,shift_init_signed);
    unsigned_shift unsigned_shift(clk, reset,shift_unsigned,shift_init_unsigned);

    signed_compare   signed_compare(clk, reset, comp_a, comp_b, comp_signed);  
    unsigned_compare unsigned_compare(clk, reset, comp_a, comp_b, comp_unsigned);  


    reg [7:0]        init1 = 8'h1;
    reg signed [7:0] init2 = 8'h80;

	reg [7:0]       shift1, shift2;
	reg signed[7:0] shift3, shift4;
		
    initial shift1 = init1 << 2;
    initial shift2 = init1 <<< 2;

    initial shift3 = init2 >> 2;
    initial shift4 = init2 >>> 2;


	reg signed [15:0] x, y;
    wire signed [15:0] sum_signed;
    wire [15:0] sum_unsigned;
    
	initial x = 16'h7FF0;
	initial y = 16'h11;

    assign sum_signed = x + y;   // 2's complement addition
    assign sum_unsigned = x + y; // unsigned addition

    reg signed [11:0] val = 12'h801;
    reg signed [11:0] val2 = 12'h701;
    wire signed [11:0] abs_val1, abs_val2;

	assign abs_val1 = val[11] ? ~val + 1'b1 : val;
	assign abs_val2 = val2[11] ? ~val2 + 1'b1 : val2;

	assign arith_negation_out = invert ? -arith_negation_in : arith_negation_in;

endmodule // signed_arithmetic



module signed_shift(input clk, reset, output reg signed [15:0] shift_signed, input signed [15:0] shift_init);  
                
    always @ (posedge clk) begin
        if (reset) begin
            shift_signed   <= shift_init; 
        end    
        else begin
            shift_signed   <=  shift_signed >>> 1;
        end    
    end

endmodule // signed_shift


module unsigned_shift(input clk, reset, output reg [15:0] shift_unsigned, input [15:0] shift_init );  
                
    always @ (posedge clk) begin
        if (reset) begin
            shift_unsigned   <= shift_init; 
        end    
        else begin
            shift_unsigned   <=  shift_unsigned >>> 1;
        end    
    end

endmodule // unsigned_shift



module signed_compare(input clk, reset, input signed [15:0] comp_a, comp_b, output reg signed  comp_signed);  
                
    always @ (posedge clk) begin
        if (reset) begin
            comp_signed   <= 1'b0; 
        end    
        else begin
            comp_signed   <=  comp_a > comp_b;
        end    
    end

endmodule // signed_compare


module unsigned_compare(input clk, reset, input [15:0] comp_a, comp_b, output reg signed  comp_unsigned);  
                
    always @ (posedge clk) begin
        if (reset) begin
            comp_unsigned   <= 1'b0; 
        end    
        else begin
            comp_unsigned   <=  comp_a > comp_b;
        end    
    end

endmodule // unsigned_compare
