module mux_4to1(d, s, en, z1);

input [3:0] d;
input [1:0] s;
input en;
output z1;

not inst1(net1, s[0]), inst2(net2, s[1]);

and inst3(net3, d[0], net1, net2, en),
		inst4(net4, d[1], s[0], net2, en),
		inst5(net5, d[2], net1, s[1], en),
		inst6(net6, d[3], s[0], s[1], en);

or  inst7(z1, net3, net4, net5, net6);

endmodule
