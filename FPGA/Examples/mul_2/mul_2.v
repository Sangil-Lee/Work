module mul_2(a, b, p);

input  [1:0] a, b;
output [3:0] p;

and inst1 (p[3], a[1], a[0], b[1], b[0]);
and inst2 (net2, a[1], b[1], ~a[0]),
	  inst3 (net3, a[1], b[1], ~b[0]);
or  inst4 (p[2], net2, net3);

and inst5 (net5, a[0], b[1], ~a[1]),
		inst6 (net6, a[0], b[1], ~b[0]),
		inst7 (net7, a[1], b[0], ~b[1]),
		inst8 (net8, a[1], b[0], ~a[0]);

or  inst9 (p[1], net5, net6, net7, net8);

and inst10 (p[0], a[0], b[0]);

endmodule
