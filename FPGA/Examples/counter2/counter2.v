module counter2(clk, reset, enable, counter_out);

input clk, reset, enable;
output [3:0] counter_out;

wire clk, reset, enable;
reg [3:0] counter_out;

always @(posedge clk)
begin: COUNTER

	if(reset == 1'b1) begin
		counter_out <= #1 4'b0000;
	end
	else if(enable == 1'b1) begin
		counter_out <= #1 counter_out + 1;
	end

end

endmodule //counter2
