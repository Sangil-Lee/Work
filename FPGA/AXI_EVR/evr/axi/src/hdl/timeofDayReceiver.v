`timescale 1ns / 1ps
//////////////////////////////////////////////////////////////////////////////////
// Company: 			BNL
// Engineer: 			J. DeLong
// 
// Create Date:    	09:49:15 03/16/2010 
// Design Name: 
// Module Name:    	timeofDayReceiver 
// Project Name: 		embedded event receiver
// Target Devices: 	FX70T
// Tool versions: 	ISE 12.1
// Description: 		This subdesign receives time stamp control events to set the
//							time of day from the GPS locked NTP server. The offset to the
//							time of day is incremented at the system clock rate.
//
// Dependencies: 
//
// Revision: 
// Revision 0.01 - File Created
// Additional Comments: 
//
//////////////////////////////////////////////////////////////////////////////////
module timeofDayReceiver(
    input 					Clock,
    input 					Reset,
    input 		[7:0] 	EventStream,
    output reg	[63:0] 	TimeStamp,
	 output reg [31:0] Seconds,
	 output reg [31:0] Offset,
	 output reg [4:0]  Position,
	 output eventClock
	);


	reg [31:0] offsetReg;
	reg [31:0] secondsReg;
	reg clear;
	reg clear_reg;
	
	// the time of day is updated by a serial stream of 0 events (0x70) and 1 events
	// (0x71). This code implements a pointer into the time of day register and writes
	// the data into that position. On receibt of the latch event (0x7d) the data is
	// moved to the output register and the pointer is cleared. The offset is cleared
	// on event 0x7d then incremented on the input clock edge.
	always @ (posedge Clock)
	begin
		if (EventStream == 8'h7d)  Position <= 5'b11111;
		else if ((EventStream == 8'h70) || (EventStream == 8'h71)) Position <= (Position - 1);
		else Position <= Position;
	end
	
	always @ (posedge Clock)
	begin
		if (Reset || clear_reg) Seconds <= 32'd0;		
		else if (EventStream == 8'h70) Seconds[Position] <= 1'b0;
		else if (EventStream == 8'h71) Seconds[Position] <= 1'b1;
		else Seconds <= Seconds;
	end
	
	always @ (posedge Clock)
	begin
		if (EventStream == 8'h7d) secondsReg <= Seconds;
		else secondsReg <= secondsReg;
	end

	
	always @ (posedge Clock)
	begin
		if (EventStream == 8'h7d) Offset <= 32'd0;
		else Offset <= (Offset + 1);
	end
	
	always @ (posedge Clock)
	begin
		if (EventStream == 8'h7d) clear <= 1'b1;
		else clear <= 1'b0;
		clear_reg <= clear;
	end
			
	always @ (posedge Clock)
	begin
		if (Reset) TimeStamp <= 64'd0;
		else  TimeStamp <= {secondsReg,Offset};
	end

	assign eventClock = ((EventStream == 8'h70) || (EventStream == 8'h71) || (EventStream == 8'h7D));
endmodule
