module full_dp_mem ( reset, clk_a, dat_in_a, address_a, dat_out_a, wr_a, clk_b, dat_in_b, address_b, dat_out_b, wr_b);
 input reset, clk_a;
 input [15:0] dat_in_a;
 input [9:0] address_a;
 output [15:0] dat_out_a;

 input wr_a, clk_b;
 input [15:0] dat_in_b;
 input [9:0] address_b;
 output [15:0] dat_out_b;

 input wr_b;

 reg [15:0] memory[0:1023];
 reg [15:0] dat_out_a;
 reg [15:0] dat_out_b;

 // ------ Design implementation -----
 // Port a
 //
 always @( posedge clk_a )
 begin
	dat_out_a <= memory[address_a];
	if (wr_a)
		begin
		dat_out_a <= dat_in_a;
		memory[address_a] <= dat_in_a;
		end
 end
 // Port b
 //
 always @( posedge clk_b )
 begin
		dat_out_b <= memory[address_b]; 
		if (wr_b)
		begin
			dat_out_b <= dat_in_b;
			memory[address_b] <= dat_in_b;
		end
 end
endmodule 
///////////////////////////////////////////////////
