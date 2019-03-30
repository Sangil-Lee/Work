module simple_dp_mem(clk, reset, dat_in, wr_adr, wr_en, dat_out, rd_adr);
	input clk, reset, wr_en;
	input [15:0]  dat_in;
	input [9:0]   rd_adr, wr_adr;
	output [15:0] dat_out;

	reg [15:0] memory[0:1023];
	reg [15:0] dat_out;

	always @(posedge clk)
	begin
		if(wr_en)
			memory[wr_adr] <= dat_in;
		dat_out <= memory[rd_adr];
	end
endmodule
