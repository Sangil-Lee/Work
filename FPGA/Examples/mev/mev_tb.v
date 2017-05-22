`include "mev.v"

module mev_tb;

reg x1,x2,x3,x4;
wire z1;

initial
begin: apply_test
	reg [4:0] invect;
	$dumpfile("./mev_tb.vcd");
	$dumpvars(0,mev_tb);
	for (invect = 0; invect < 16; invect = invect+1)
		begin
			{x1, x2, x3, x4} = invect [4:0];
			#10 $display ("x1x2x3x4=%b, z1=%b", {x1,x2,x3,x4}, z1);
		end
end
mev inst1(.x1(x1),.x2(x2),.x3(x3), .x4(x4), .z1(z1));
endmodule
