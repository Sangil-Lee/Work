module tri_state_buf;

reg data_in, enable;
wire pad;

assign pad = (enable)?data_in:1'bz;

initial begin
	$dumpfile("./tri_state_buf.vcd");
	$dumpvars(0,tri_state_buf);
	$monitor("Enable:%b, Data:%b, PAD:%b",enable,data_in,pad);

	#1 enable  = 0;
	#1 data_in = 1;
	#1 enable  = 1;
	#1 data_in = 0;
	#1 enable  = 0;
	#1 $finish;
end

endmodule
