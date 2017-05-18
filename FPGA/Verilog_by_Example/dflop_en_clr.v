module dflop_en_clr(clk, reset, enable, clear, in_1, out_1);
	input clk, reset, enable, clear,in_1;
	output out_1;

	reg out_1;

	always @(posedge clk or posedge reset)
	begin
		if(reset)
			out_1 <= 1'b0;
		else if (clear == 1'b0)
			out_1 <= 1'b0;
		else if (enable == 1'b1)
			out_1 <= in_1;
	end

endmodule
