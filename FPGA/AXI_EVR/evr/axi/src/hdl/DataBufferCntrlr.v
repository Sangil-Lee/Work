`timescale 1ns / 1ps
//////////////////////////////////////////////////////////////////////////////////
// Company: 
// Engineer: 
// 
// Create Date:    08:26:53 06/17/2010 
// Design Name: 
// Module Name:    DataBufferCntrlr 
// Project Name: 
// Target Devices: 
// Tool versions: 
// Description: 
//
// Dependencies: 
//
// Revision: 
// Revision 0.01 - File Created
// Additional Comments: 
//
//////////////////////////////////////////////////////////////////////////////////
module DataBufferCntrlr(
    input 			RxClock,
	 input			SysClock,
    input 			Reset,
    input 			PacketStart,
    input 			PacketStop,
    input  [7:0] 	DataIn,
    input  [10:0] Address,
	 input 			ReadEna,
    output reg		PacketReady,
	 output reg [11:0] PacketSize,
    output [7:0] 	DataOut,
    output reg		PacketFault
    );
	
	reg RxActive; 
	reg [11:0] RxAddress;
	reg [11:0] TimeoutCounter;
	reg [15:0] CheckSumIn;
	reg [15:0] myCheckSum;

	always @ (posedge RxClock)
	begin
		if (PacketStart == 1'b1) RxActive <= 1'b1;
		else if (PacketStop == 1'b1) RxActive <= 1'b0;
		else if (RxAddress >= 2048)  RxActive <= 1'b0; // time out
		else RxActive <= RxActive;
	end
	
	always @ (posedge RxClock)
	begin
		if (RxActive == 1'b1) RxAddress <= RxAddress + 1;
		else RxAddress <= 12'd0;
	end
	
   parameter IDLE  = 4'b0001;
   parameter LTCH1 = 4'b0010;
   parameter LTCH2 = 4'b0100;
   parameter STRB  = 4'b1000;

   (* FSM_ENCODING="ONE-HOT", SAFE_IMPLEMENTATION="NO" *) reg [3:0] state = IDLE;

   always@(posedge RxClock)
      if (Reset) begin
         state <= IDLE;
      end
      else
         (* FULL_CASE, PARALLEL_CASE *)case (state)
            IDLE : begin
               if (PacketStop == 1'b1)
                  state <= LTCH1;
               else
                  state <= IDLE;
            end
            LTCH1 :  state <= LTCH2;
            LTCH2 : state <= STRB;
            STRB  : begin
               if (ReadEna == 1'b1)
                  state <= IDLE;
               else
                  state <= STRB;
					end
         endcase

	always @ (posedge RxClock)
	begin
		if (state == LTCH1) PacketSize <= (RxAddress + 1);
		else PacketSize <= PacketSize;
	end
			
	always @ (posedge RxClock)
	begin
		if (state == LTCH1) CheckSumIn[15:8] <= DataIn;
		else if (state == LTCH2) CheckSumIn[7:0] <= DataIn;
		else CheckSumIn <= CheckSumIn;
	end
	
	//////////////////// CHECKSUM EXERCISES BEGIN HERE ////////////////////////
	always @ (posedge RxClock) begin
		if (RxActive == 1'b1) begin
			if (RxAddress[0] == 1'b0) myCheckSum <= (myCheckSum + DataIn);
			else myCheckSum <= myCheckSum; 
			end
		else if (state == LTCH1) myCheckSum <= 16'hffff - myCheckSum + 16'h3C; // 0x3C, the end of character signifier gets added
																									  // to the checksum, so need to add it to the 1's compliment as 
																									  // shown here
		else if (state == IDLE) myCheckSum <= 16'd0;
		else myCheckSum <= myCheckSum;
	end
	/////////////////////// CHECKSUM EXERCISES END HERE ///////////////////////////////////////////

	
	// test checksum
	always @ (posedge RxClock)
	begin
		if ((state == STRB) && (myCheckSum == CheckSumIn)) PacketReady <= 1'b1;
		else if ((state == STRB) && (myCheckSum != CheckSumIn)) PacketFault <= 1'b1;
		else 
			begin
				PacketReady <= 1'b0;
				PacketFault <= 1'b0;
			end
	end
		
		
PacketMem myPacketMem (
	.clka(RxClock),
	.wea(RxActive && (RxAddress[0] == 1'b0)), // Bus [0 : 0] 
	.addra(RxAddress[11:1]), // Bus [10 : 0] 
	.dina(DataIn), // Bus [7 : 0] 
	.clkb(SysClock),
	.addrb(Address), // Bus [10 : 0] 
	.doutb(DataOut)); // Bus [7 : 0] 

endmodule
