module bus_breakout(in_1, in_2, out_1);
	input  in_1;
	input  in_2;
	output out_1;

	assign out_1 =	{ in_2[3:2],
		(in_1[3] & in_2[1]),
		(in_1[2] & in_2[0]),
		in_1[1:0]
	};
endmodule
