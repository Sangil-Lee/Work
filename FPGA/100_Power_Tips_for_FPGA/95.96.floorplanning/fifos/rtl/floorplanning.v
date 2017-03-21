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


module floorplanning(  input wr_clk,rd_clk,reset,
                       
                       input enable,
                       output [15:0] data_out,
                       
                       output reg io_data_qo, io_enable_qo
                       
                      );  
                
    wire [15:0] fifo_data_in, fifo_data_out;
    wire fifo_full, fifo_empty;
    wire io_data, io_enable;
    
    
    
    fifo_generator_v6_1 fifo (
	    .rst(reset),
	    .wr_clk(wr_clk),
	    .rd_clk(rd_clk),
	    .din(fifo_data_in), 
	    .wr_en(fifo_write),
	    .rd_en(fifo_read),
	    .dout(fifo_data_out), 
	    .full(fifo_full),
	    .empty(fifo_empty));
    
    
    data_src data_src (
        .clk(wr_clk), 
        .reset(reset), 
        .data(fifo_data_in), 
        .enable(enable), 
        .fifo_full(fifo_full), 
        .fifo_write(fifo_write));    
    
    
    
    data_sink data_sink (
        .clk(rd_clk), 
        .reset(reset), 
        .data_in(fifo_data_out), 
        .data_out(data_out), 
        .fifo_empty(fifo_empty), 
        .fifo_read(fifo_read)
        );    
        
        
    bus_controller 
       bus_controller(
        .clk(rd_clk), 
        .reset(reset), 
        .data(io_data), 
        .enable(io_enable));     
        
        
    always @(posedge rd_clk) begin
        if(reset) begin
			io_data_qo   <= 1'b0;
            io_enable_qo <= 1'b0;
		end
        else begin
			io_data_qo   <= io_data;  
            io_enable_qo <= io_enable;
        end
    end
                           
    
        
endmodule // counters


module data_src(input clk,reset,
                output reg [15:0] data,
                input enable,
                
                input fifo_full,
                output fifo_write );  
          
    assign fifo_write = ~fifo_full;
                
    always @(posedge clk) begin
        if(reset) begin
			data <= 16'b1;
		end
        else begin
            data <= enable ? {data[14:0],data[15]} : data;    
        end
    end

endmodule // data_src


module data_sink(input clk,reset,
                 input [15:0] data_in,
                 output reg [15:0] data_out,
                                 
                 input fifo_empty,
                 output fifo_read );  
          
    assign fifo_read = ~fifo_empty;
    
    reg fifo_read_q;
                
    always @(posedge clk) begin
        if(reset) begin
            fifo_read_q <= 1'b0;
			data_out    <= 16'b0;
		end
        else begin
            fifo_read_q <= fifo_read;
            data_out    <= fifo_read_q ? data_in : data_out;    
        end
    end

endmodule // data_sink


// A dummy module that drives data and enable outputs.
// It illustrates the IO floorplanning issues
module bus_controller(input clk, reset    ,
                      output reg data     , 
                      output reg enable   );

    always @(posedge clk) begin
        if(reset) begin
			data   <= 1'b1;
            enable <= 1'b0;
		end
        else begin
			data   <= ~data;
            enable <= ~enable;
        end
    end

endmodule // bus_controller
