module simple_dflop (clk, in_1, out_1);
	input clk, in_1;
	output out_1;

	//기존값을 유지해야 하므로, 내부 변수로 register를 쓴다.
	reg	out_1;

	always @(posedge clk)
	begin
		out_1 <= in_1;
	end

endmodule
