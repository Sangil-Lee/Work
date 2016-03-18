n=0:1:40;
x=sin(0.1*pi*n);
Hs=stem(n,x,'b','filled');
set(Hs,'markersize',4);
plot(t, x, 'b');
xlabel('n'); ylabel('x(n)');
title ('Stem Plot of sin(0.2 pi n)');
