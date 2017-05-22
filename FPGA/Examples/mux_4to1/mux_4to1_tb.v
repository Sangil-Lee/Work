`include "mux_4to1.v"

module mux_4to1_tb;

reg [3:0] d;
reg [1:0] s;
reg en;
wire z1;

initial
	$monitor($time,"ns, select:s=%b, inputs:d=%b, output:z1=%b",s,d,z1);

initial
	begin
	$dumpfile("./mux_4to1.vcd");
	$dumpvars(0,mux_4to1_tb);
	#0  s[0] = 1'b0; s[1] = 1'b0;
		  d[0] = 1'b0; d[1] = 1'b1; d[2] = 1'b0; d[3] = 1'b1;
		  en=1'b1;
	#10 s[0] = 1'b0; s[1] = 1'b0;
		  d[0] = 1'b1; d[1] = 1'b1; d[2] = 1'b0; d[3] = 1'b1;
		  en=1'b1;
	#10 s[0] = 1'b1; s[1] = 1'b0;
		  d[0] = 1'b1; d[1] = 1'b1; d[2] = 1'b0; d[3] = 1'b1;
		  en=1'b1;
	#10 s[0] = 1'b0; s[1] = 1'b1;
		  d[0] = 1'b1; d[1] = 1'b1; d[2] = 1'b0; d[3] = 1'b1;
		  en=1'b1;
	#10 s[0] = 1'b1; s[1] = 1'b0;
		  d[0] = 1'b1; d[1] = 1'b0; d[2] = 1'b0; d[3] = 1'b1;
		  en=1'b1;
	#10 s[0] = 1'b1; s[1] = 1'b1;
		  d[0] = 1'b1; d[1] = 1'b1; d[2] = 1'b0; d[3] = 1'b1;
		  en=1'b1;
	#10 s[0] = 1'b1; s[1] = 1'b1;
		  d[0] = 1'b1; d[1] = 1'b1; d[2] = 1'b0; d[3] = 1'b0;
		  en=1'b1;
	#10 s[0] = 1'b1; s[1] = 1'b1;
		  d[0] = 1'b1; d[1] = 1'b1; d[2] = 1'b0; d[3] = 1'b0;
		  en=1'b0;
	#10 $finish; 
	end

mux_4to1 inst1(.d(d), .s(s), .en(en), .z1(z1));

endmodule
