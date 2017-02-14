%Problme 4.7-4

b = 0.5*[2 -cos(0.4*pi)]; a = [1 -cos(0.4*pi) 0.25];
[delta,n1] = impseq(0,0,7); 
xb1 = filter(b,a,delta); 
xb1 = fliplr(xb1);

[u,n2] = stepseq(-7,-7,0); 
xb2 = ((2.^n2).*cos(0.4*pi*n2)).*u;
error = max(abs(xb1-xb2))
