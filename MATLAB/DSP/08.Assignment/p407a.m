%Problem 4.7-1
b1 = [0 2 3]; a1 = [1]; 
[delta,n] = impseq(0,0,4);
xb1 = filter(b1,a1,delta); 
xb1 = fliplr(xb1); 
n1 = -fliplr(n);
b2 = [1 -2 -3]; 
a2 = [1]; 
xb2 = filter(b2,a2,delta); 
n2 = n;
[xa1,na1] = sigadd(xb1,n1,xb2,n2); 
xa2 = [0 0 3 2 1 -2 -3 0 0];

error = max(abs(xa1-xa2))
