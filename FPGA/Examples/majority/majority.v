module majority(x1, x2, x3, x4, x5, z1);

input x1, x2, x3, x4, x5;
output z1;

wire z1;

and inst1(net1, x3, x4, x5),
		inst2(net2, x2, x3, x5),
		inst3(net3, x1, x3, x5),
		inst4(net4, x2, x4, x5),
		inst5(net5, x1, x4, x5),
		inst6(net6, x1, x2, x5),
		inst7(net7, x1, x2, x4),
		inst8(net8, x2, x3, x4),
		inst9(net9, x1, x3, x4);

or  inst10(z1, net1, net2, net3, net4, net5, net6, net7, net8, net9);

endmodule
