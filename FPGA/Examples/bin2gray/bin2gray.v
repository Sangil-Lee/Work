module bin2gray(x1,x2,x3,x4,z1,z2,z3,z4);

input  x1,x2,x3,x4;
output z1,z2,z3,z4;

//wire z1,z2,z3,z4;

buf Buff (z1, x1);
xor E_Or (z2, x1, x2);
xor E_Or1(z3, x2, x3);
xor E_Or2(z4, x3, x4);

endmodule
