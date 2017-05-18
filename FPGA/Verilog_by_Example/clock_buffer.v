module clock_buffer ( clk_in, reset, dat_in, dat_out);
	input clk_in, reset, dat_in;
	output dat_out;

	wire clk;
	reg dat_out;

	BUFG clock_buffer_inst (.I(clk_in), .O(clk));

	always @(posedge clk or posedge reset)
		begin
			if(reset)	dat_out <= 1'b0;
			else			dat_out <= dat_in;
		end

endmodule
