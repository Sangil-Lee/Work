module tr_gates;

reg data_enable_low, in;
wire data_bus1,data_bus2, out1, out2;

bufif0 U1(data_bus1, in, data_enable_low);
bufif1 U2(data_bus2, in, data_enable_low);
buf U3(out1, in);
not U4(out2, in);

initial begin
	$monitor("data_bus1=%b, data_bus2=%b, in=%b, data_enable=%b, out1=%b, out2=%b",data_bus1,data_bus2, in, data_enable_low,out1,out2);
	data_enable_low = 0;
	in = 0;

	#4 data_enable_low = 1;
	#8 $finish;
end


always #2 in = ~in;

endmodule
