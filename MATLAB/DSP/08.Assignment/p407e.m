%Problem 4.7-5

b = [1 -3]; a = [1 -9 27 -27]; 
[delta,n1] = impseq(0,0,7);
xb1 = filter(b,a,delta);
[u,n2] = stepseq(0,0,7); 
xb2 = ((n2+1).*(3.^n2)).*u;

error = max(abs(xb1-xb2))
