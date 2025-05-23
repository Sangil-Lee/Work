module ring_counter_modi2 (clk, init, count)
	input clk, init; output[7:0] count;
	reg[7:0] count;

	always @(posedge clk)
	begin
		if(init)
			count = 8'b10000000;
		else begin
			count = {count[6:0],count[7]};
		end
	end
endmodule
