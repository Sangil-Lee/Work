`include "full_adder.v"

module full_adder_tb;

reg a, b, cin;
wire sum, cout;

initial
begin
	$dumpfile("./full_adder_tb.vcd");
	$dumpvars(0,full_adder_tb);
end

initial
begin: stimulus
	reg [3:0] invect;
	for(invect=0; invect < 8; invect = invect+1)
		begin
			{a, b, cin} = invect [3:0];
			#10 $display("abcin = %b, cout=%b, sum = %b", {a, b, cin}, cout, sum);
		end
end
	full_adder full_adder(.a(a),.b(b),.cin(cin),.cout(cout),.sum(sum));
endmodule

