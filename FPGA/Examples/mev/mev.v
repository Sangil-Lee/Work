module mev(x1, x2, x3, x4, z1);

input x1,x2,x3,x4;
output z1;

and inst1 (net1, ~x1, ~x3, x4);
and inst2 (net2, ~x1, x2);
and inst3 (net3, x1, ~x2);
or  inst4 (z1, net1, net2, net3);

endmodule
