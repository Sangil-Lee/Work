module edge_wait;

reg enable, clk, trigger;

always @(posedge enable)
begin
	trigger = 0;
	repeat(5) begin
		@(posedge clk);
	end
	trigger = 1;
end

initial begin
	$dumpfile("./edge_wait.vcd");
	$dumpvars(0,edge_wait);
	$monitor("Time:%d, Clk:%b, Enable:%b, Trigger:%b",$time,clk,enable,trigger);
	clk = 0;
	enable = 0;
	#5	enable = 1;
	#1	enable = 0;
	#10 enable = 1;
	#1 enable = 0;
	#10 $finish;
end

always
#1 clk = ~clk;

endmodule
