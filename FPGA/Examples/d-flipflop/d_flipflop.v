module d_ff_nand;

wire Q, Q_Bar;
reg d,clk;

nand u1(X,d,clk);
nand u2(Y,X,clk);
nand u3(Q,Q_Bar,X);
nand u4(Q_Bar,Q,Y);


initial begin
	$monitor("CLK=%b, D=%b, Q=%b, Q_Bar=%b", clk, d, Q, Q_Bar);
	clk = 0;
	d = 0;
	#3 d = 1;
	#3 $finish;
end

always #2 clk = ~clk;

endmodule
