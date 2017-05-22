module full_adder(a, b, cin, cout, sum);

input a, b, cin;
output cout, sum;

xor E_Or (net1, a, b);
and And  (net2, a, b);
xor E_Or1(sum, net1, cin);
and And2 (net4, net1, cin);
or  Or   (cout, net4, net2);

endmodule
