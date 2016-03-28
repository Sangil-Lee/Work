`timescale 1ns / 1ps
//////////////////////////////////////////////////////////////////////////////////
// Company: 
// Engineer: 
// 
// Create Date: 01/27/2016 10:38:59 AM
// Design Name: 
// Module Name: testbench_flip
// Project Name: 
// Target Devices: 
// Tool Versions: 
// Description: 
// 
// Dependencies: 
// 
// Revision:
// Revision 0.01 - File Created
// Additional Comments:
// 
//////////////////////////////////////////////////////////////////////////////////

module main;
reg j,k,clk,rst; 
wire q;
jkflop jk(j,k,clk,rst,q);
//Module to generate clock with period 10 time units
initial begin 
 forever begin 
 clk=0;
 #5
 clk=1;
 #5
 clk=0;
 end
end
initial begin
 j=0; k=0; rst=1;
 #4
 j=1; k=1; rst=0;
 #40
 rst=1;
 #10
 j=0; k=1;
 #10
 rst=0;
 #10
 j=1; k=0;
end
endmodule 
