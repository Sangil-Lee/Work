'timescale 1ns/10ps

module Controllog ic(FTW, REF_Clk, Reset, Time_Frame, Slow, Fast);

//Declaration of I nput and Output Ports
input REF_Clk, Reset;
input Slow, Fast;
input [7:0] Time_Frame;
output [15:0] FTW;


//Declaration Registers
reg [15:0] FTW
reg [14:0] Add_Gain, Sub_Gain;
reg flag1, flag2;

always@(posedge REF_Clk or posedge Reset)
begin
	if(Reset)
	begin
		FTW = 16'h0000;
		Add_Gain = 15'h8000;
		Sub_Gain = 15'h4000;
		flag1 = 1'b0;
		flag2 = 1'b0;
	end
	else if(Time_Frame == 30)
	begin
		if(Slow)
		begin
			FTW = FTW + Add_Gain;
			flag2 = 1'b1;
			if(flag1)
			begin
				Sub_Gain = Sub_Gain >> 2;
				flag1 = 1'b0;
			end
		else Sub_Gain = Sub_Gain;
		end
		else if(Fast)
		begin
			FTW = FTW - Sub_Gain;
			flag1 = 1'b1;
			if(flag2)
			begin
				Add_Gain = Add_Gain >> 2;
				flag2 = 1'b0;
			end
			else Add_Gain = Add_Gain;
		end
	end
	else FTW = FTW;
end

endmodule

