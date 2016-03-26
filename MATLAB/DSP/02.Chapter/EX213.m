n=[0:50];
x = cos(0.04*pi*n) + 0.2*randn(size(n));
subplot(2,2,3);
stem(n,x); title('Sequence in Problem 2.1c');
xlabel('n'); ylabel('x(n)');
