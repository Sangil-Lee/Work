`timescale 1ns / 1ps
//////////////////////////////////////////////////////////////////////////////////
// Company: 
// Engineer: 
// 
// Create Date: 2020/11/30 07:12:06
// Design Name: 
// Module Name: SWLed
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


module SWLed(input GCLK,
    input [7:0] Btn,
    output [7:0] Led
    );
    
 
//assign Led = Btn;
reg [7:0] LedVal;
always@(posedge GCLK)
begin
    if(GCLK) begin
       LedVal <= Btn; 
    end
end
 
 assign Led = LedVal;   
ila_0 debug(.clk(GCLK), .probe0(Btn), .probe1(Led));

endmodule
