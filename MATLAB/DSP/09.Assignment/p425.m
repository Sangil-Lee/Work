clc; close all;
b1 = [1 -1]; nb1 = [0 1]; 
a11 = [1 0 -0.81]; 
na11 = [0 1 2]; a12 = [1 -0.7];
na12 = [0 1]; 
[a1,na1] = conv_m(a11,na11,a12,na12);

b2 = [0.1914 1.62]; nb2 = [0 1]; a2 = [1 0 -0.81]; na2 = [0 1 2];
[bnr1,nbnr1] = conv_m(b1,nb1,a2,na2); 
[bnr2,nbnr2] = conv_m(b2,nb2,a1,na1);

[b,nb] = sigadd(bnr1,nbnr1,bnr2,nbnr2); [a,na] = conv_m(a1,na1,a2,na2);
[R,p,k] = residuez(b,a)

n = [0:20]; x = 0.7.^n; xic = [0.1914 1.62];

yb1 = filter(b1,a11,x,xic);
yb2 = R(1)*((p(1)).^n)+R(3)*((p(3)).^n)+R(5)*((p(5)).^n);

error = max(abs(yb1-yb2))
