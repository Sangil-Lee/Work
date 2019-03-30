`include "counter2.v"
module counter2_tb;

reg clk, reset, enable;
wire [3:0] counter_out;

initial begin
	$display("time\t clk, reset, enable, counter");
	$monitor("%g\t %b, %b, %b, %b", $time, clk, reset, enable, counter_out);
	$dumpfile("./counter2_tb.vcd");
	$dumpvars(0,counter2_tb);
	clk	   = 1;
	reset  = 0;
	enable = 0;

	#5  reset  = 1;
	#10 reset  = 0;
	#5  enable = 1;
	#125 enable = 0;
	#10 $finish;
end

always begin
	#5 clk = ~clk;
end

counter2 count(clk,reset,enable,counter_out);

endmodule
