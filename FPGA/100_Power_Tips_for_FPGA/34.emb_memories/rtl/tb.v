`timescale 1ns / 100ps

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

module tb;

	// Inputs
	reg clk;
	reg reset;
	reg [15:0] mem1_din;
	reg [15:0] mem2_din;
	reg [9:0] mem1_addr;
	reg [9:0] mem2_addr;

	reg [15:0] mem1_wen;
	reg [15:0] mem2_wen;

	// Outputs
	wire [15:0] mem1_dout;
	wire [15:0] mem2_dout;    


	memories uut (
		.clk(clk), 
		.reset(reset), 
		.mem1_din(mem1_din), 
		.mem2_din(mem2_din), 
		.mem1_addr(mem1_addr), 
		.mem2_addr(mem2_addr), 
		.mem1_wen(mem1_wen),
		.mem2_wen(mem2_wen), 
		.mem1_dout(mem1_dout),
		.mem2_dout(mem2_dout)        
	);

	initial begin
		// Initialize Inputs
		reset = 1;
		mem1_din = 0;
		mem2_din = 0;
		mem1_addr = 0;
		mem2_addr = 0;
		mem1_wen = 0;
		mem2_wen = 0;

        
		#100;
        reset = 0;
        
        mem1_write(1,16'h1234,16'hffff);
        mem1_read(1,16'h1234);
    
        mem1_write(1,16'h1235,16'h000f);
        mem1_read(1,16'h1235);
        
        mem1_write(2,16'h1234,16'hf000);
        mem1_read(2,16'h1000);

        mem1_write(2,16'h1234,16'h0f00);
        mem1_read(2,16'h1200);
        
        #1000;
        
        mem2_write(1,16'h1234,16'hffff);
        mem2_read(1,16'h1234);

        mem2_write(1,16'h1235,16'h000f);
        mem2_read(1,16'h1235);
        
        mem2_write(2,16'h1234,16'hf000);
        mem2_read(2,16'h1000);

        mem2_write(2,16'h1234,16'h0f00);
        mem2_read(2,16'h1200);
        
        #1000;
        
        $finish;
	end
    
    initial begin
		clk = 0;
    
        forever #5 clk = ~clk; 
    end
      
      
task mem1_write;
    input [9:0]  addr;
    input [15:0] data;
    input [15:0] we;
        
    begin    
        @(posedge clk); #1;
        mem1_wen  = we;
        mem1_addr = addr;
        mem1_din  = data;
       
        @(posedge clk); #1;
        mem1_wen = 0;
        mem1_addr = 0;
        mem1_din = 0;        
    end    
endtask        
 
 
task mem1_read;
    input [9:0]  addr;
    input [15:0] expected_data;
        
    begin    
        @(posedge clk); #1;
        mem1_addr = addr;
       
        @(posedge clk); #1;
        mem1_addr = 0;
        
        
        if(expected_data !== mem1_dout)
            $display("mem1 read error data=%x expected=%x",mem1_dout,expected_data);
        else
            $display("mem1 read data=%x",mem1_dout);
                
    end    
endtask        
   
   
task mem2_write;
    input [9:0]  addr;
    input [15:0] data;
    input [15:0] we;
        
    begin    
        @(posedge clk); #1;
        mem2_wen  = we;
        mem2_addr = addr;
        mem2_din  = data;
       
        @(posedge clk); #1;
        mem2_wen = 0;
        mem2_addr = 0;
        mem2_din = 0;        
    end    
endtask        
 
 
task mem2_read;
    input [9:0]  addr;
    input [15:0] expected_data;
        
    begin    
        @(posedge clk); #1;
        mem2_addr = addr;
       
        @(posedge clk); #1;
        mem2_addr = 0;
        
        
        if(expected_data !== mem2_dout)
            $display("mem2 read error data=%x expected=%x",mem2_dout,expected_data);
        else
            $display("mem2 read data=%x",mem2_dout);
                
    end    
endtask        
      
endmodule

