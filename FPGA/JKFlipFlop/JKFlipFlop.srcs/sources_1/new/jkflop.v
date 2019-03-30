`timescale 1ns / 1ps
//////////////////////////////////////////////////////////////////////////////////
// Company: 
// Engineer: 
// 
// Create Date: 01/27/2016 10:37:24 AM
// Design Name: 
// Module Name: jkflop
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
`define TICK #2

module jkflop(
    input j,
    input k,
    input clk,
    input rst,
    output q
    );
 reg q; 
    always @(posedge clk)begin
     if(j==1 & k==1 & rst==0)begin
     q <=`TICK ~q; //Toggles
     end
     else if(j==1 & k==0 & rst==0)begin
     q <= `TICK 1; //Set
     end
     else if(j==0 & k==1)begin
     q <= `TICK 0; //Cleared
     end
    end
    always @(posedge rst)begin
     q <= 0; //The reset normally has negligible delay and hence ignored.
    end 
   
    
endmodule
