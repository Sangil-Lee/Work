%Problem 4.15

b = [3/4 5/4 1 1 5/4 3/4]; 
a = [1 0];
n = 0:200; 
x = sin(pi*n/2)+5*cos(pi*n); 
y = filter(b,a,x);

subplot(2,1,1); Hs = stem(n,x); set(Hs,'markersize',2); axis([-2 202 -7 6]);
xlabel('n'); ylabel('x(n)');
title('x(n) = sin(\pi \times n / 2)+5 \times cos(\pi \times n)');

subplot(2,1,2); Hs = stem(n,y); set(Hs,'markersize',2); axis([-2 202 -2 4]);
xlabel('n'); ylabel('y(n)');
title('Output sequence after filtering');
