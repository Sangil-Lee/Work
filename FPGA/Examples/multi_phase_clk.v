//Using multiple edges of the same clock


module multi_phase_clk(a,b,f,clk)
	input a, b, clk;
	output f;

	reg f, t;

	always @(posedge clk)
	begin
		f <= t & b;
	end

	always @(negedge clk)
	begin
		t <= a | b;
	end
endmodule
