//Using more than one clocks in a module

module multiple_clk(clk1, clk2, a, b, c, f1, f2)
	input clk1, clk2, a, b, c;
	output f1, f2;

	reg f1, f2;

	always @(posedge clk1)
	begin
		f1 <= a & b;
	end

	always @(negedge clk2)
	begin
		f2 <= b ^ c;
	end
endmodule
