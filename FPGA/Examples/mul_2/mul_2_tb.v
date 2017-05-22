`include "mul_2.v"

module mul_2_tb;

reg [1:0] a, b;
wire [3:0] p;

initial
begin
	$dumpfile("./mul_2_tb.vcd");
	$dumpvars(0,mul_2_tb);
end

initial
begin: stimulus
	reg [4:0] invect;
	for ( invect = 0; invect < 16; invect = invect+1)
	begin
		{a, b} = invect [4:0];
		#10 $display( "a=%b, b=%b, p=%b(%d)", a, b, p, p);
	end
end

mul_2 mul_2_inst(.a(a), .b(b), .p(p));

endmodule
