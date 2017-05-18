module single_port_mem(clk,reset,data_io,address,wr_en,rd);

	input clk,reset;
	inout [15:0] data_io;
	input [9:0] address;
	input wr_en,rd;

	reg [15:0] memory[0:1023];
	reg [15:0] dat_out;
	reg				 rd_dl;

	always @(posedge clk)
		begin
			if(wr_en)
				memory[address] <= data_io;
			dat_out <= memory[address];
			rd_dl <= rd;
		end

		assign data_io = rd_dl ? data_out:16'bz;

endmodule
