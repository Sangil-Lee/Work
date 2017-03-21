module standard_mux (in_1, in_2, in_3, out_1);
	input [3:0]	in_1;
	input [3:0]	in_2;
	input in_3;
	output [3:0]	out_1;

	assign out_1 = int_3 ? in_2:in_1;
endmodule
