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

module bram_floorplanning(input clk,reset,
                          input  [9:0] rom_addr,
                          output [31:0] data_out);
                
    wire [31:0] rom_dout;
    
    rom rom (
	    .clka(clk),
	    .addra(rom_addr),
	    .douta(rom_dout)); 
    
    data_sink data_sink (
        .clk(clk), 
        .reset(reset), 
        .data_in(rom_dout), 
        .data_out(data_out)); 

endmodule // bram_floorplanning


module data_sink(input clk,reset,
                 input [31:0] data_in,
                 output reg [31:0] data_out);

    reg [31:0] data_in_q;
    
    always @(posedge clk) begin
        if(reset) begin
            data_out   <= 'b0;
			data_in_q  <= 'b0;
		end
        else begin
            data_out   <= data_in_q;
            data_in_q  <= data_in;    
        end
    end

endmodule // data_sink





module bram_floorplanning2(input clk,reset,
                           input  [9:0] rom_addr,
                           output [31:0] data_out);
                
    wire [31:0] rom_dout;
    
    rom_registered rom (
	    .clka(clk),
	    .addra(rom_addr),
	    .douta(rom_dout)); 
    
    data_sink data_sink (
        .clk(clk), 
        .reset(reset), 
        .data_in(rom_dout), 
        .data_out(data_out)); 

endmodule // bram_floorplanning
