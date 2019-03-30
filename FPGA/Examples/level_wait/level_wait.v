module level_wait;

reg mem_read, data_ready;
reg[7:0] data_bus, data;

always @(mem_read or data_bus or data_ready)
begin
	data = 0;
	while(mem_read == 1'b1) begin
		wait (data_ready == 1) #1 data=data_bus;
	end
end

initial begin
	$dumpfile("./level_wait.vcd");
	$dumpvars(0,level_wait);

	$monitor("%d Read = %b, Ready = %b, Data=%b", $time, mem_read, data_ready, data);

	data_bus = 0;
	mem_read = 0;
	data_ready = 0;
	#10 data_bus   = 8'hDE;
	#10 mem_read   = 1;
	#20 data_ready = 1;
	//#1  mem_read   = 1;
	#1  data_ready = 0;
	#10 data_bus   = 8'hAD;
	//#10  mem_read   = 1;
	#20  data_ready = 1;
	//#1   mem_read   = 1;
	#1   data_ready = 0;
	#10  $finish;
end
endmodule
