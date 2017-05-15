module counter_tb;
	
	reg reset = 0;
	initial begin
		#17 reset = 1;
		#11 reset = 0;
		#29 reset = 1;
		#11 reset = 0;
		#100 $stop;
		//#100 $finish;
	end

	reg clk = 0;
	always #5 clk = !clk;

	wire [7:0] value;
	counter c1(.out(value), .clk(clk), .reset(reset));

	initial
	begin
		$dumpfile("./counter_tb.vcd");
		$dumpvars(0,counter_tb);

		$monitor("At Time %t, value = %h (%0d)", $time, value, value);
	end
endmodule
