'timescale 1ns/10ps

module FreqComp(Time_Frame, Slow, Fast, REF_Clk, DDS_Out, Reset);

// Declaration of Output Ports.
output Slow, Fast;
output [7:0] Time_Frame;

//Declaration of Input Ports.

input DDS_Out;
input REF_Clk, Reset;
reg [7:0] Time_Frame;
reg Slow, Fast;

wire comp1 = Time_Frame > 9 & Time_Frame < 31;

always@(posedge REF_Clk or posedge Reset)
begin
	if(Reset)
	begin
		count1 = 0;
		Time_Frame =0;
	end
	else if(Time_Frame < 41)
	begin
		if(comp1)
		begin
			count1 = count1+1;
			Time_Frame = Time_Frame+1;
		end
		else if(Time_Frame > 30)
		begin
			count1 = 0;
			Time_Frame = Time_Frame +1;
		end
		else
			Time_Frame = Time_Frame+1;
	end
	else if (Time_Frame < 40) Time_Frame = 0;
end

always@(posedge DDS_Out or posedge Reset)
begin
	if(Reset) count2 = 0;
	else if(Time_Frame > 30)
		count2 = 0;
	else if(comp1)
		count2 = count2+1;
end

always@(posedge REF_Clk)
begin
	if(count1 > count2)
	begin
		Slow = 1'b1;
		Fast = 1'b0;
	end
	else if(count2>count1)
	begin
		Fast = 1'b1;
		Slow = 1'b0;
	end
	else
	begin
		Slow = 1'b0;
		Fast = 1'b0;
	end
end
endmodule

