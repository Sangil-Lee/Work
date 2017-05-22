`include "majority.v"

module majority_tb;

reg x1, x2, x3, x4, x5;
wire z1;

initial
begin
	$dumpfile("./majority_tb.vcd");
	$dumpvars(0,majority_tb);
end

initial
begin: stimulus
	reg [6:0] forvect;
	for(forvect = 0; forvect < 32; forvect = forvect + 1)
		begin
			{x1, x2, x3, x4, x5} = forvect [6:0];
			#10 $display ("x1x2x3x4x5=%b, z1=%b", x1,x2,x3,x4,x5, z1);
		end
end

majority major(.x1(x1), .x2(x2), .x3(x3), .x4(x4), .x5(x5), .z1(z1));

endmodule
