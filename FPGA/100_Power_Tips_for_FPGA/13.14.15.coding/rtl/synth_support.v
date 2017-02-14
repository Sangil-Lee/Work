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

`define DLY 1

`celldefine

module synth_support(input clk, reset, 
                     input [1:0] sel,
                     output reg data_out,
                     input  [1:0] test_in,
                     output reg test_out,
                     
                     output reg blocking, non_blocking, 
                     
                     output nmos_out,
                     input nmos_data,nmos_control);

    always @(*) begin
        data_out = (sel[0] === 1'b0) ? ^test_in : 1'b1;                  
    end // always
         
    initial begin
        data_out = 0;
    end
 
    always @(posedge clk) begin
        test_out <= #`DLY ^test_in;
    end 
    
    nmos_switch n1(.out(nmos_out), .data(nmos_data), .control(nmos_control));                       

    // mixing blocking and non-blocking statements
    /*
    always @(*) begin
        blocking = ^test_in;
        non_blocking <= |test_in; 
    end 
    */
    always @(posedge clk) begin
        if(reset) begin
            blocking     =  0;
            non_blocking <= 0;
        end
        else begin
            blocking     = ^test_in;
            non_blocking <= |test_in; 
        end    
    end 
    
                    
endmodule // synth_support


module nmos_switch (output out, input data, control);

    assign out = control ? data : 1'bz;
    
endmodule


`endcelldefine
