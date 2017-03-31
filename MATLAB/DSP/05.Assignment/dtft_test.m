%w = -2*pi:0.01:2*pi;
w = -pi:0.01:pi;
n = 0:1:100;
w0 = 2;
x = exp(n .* (1i*w0)); % x = exp(jw0n)

X = dtft(x, n, w);

disp(X)

subplot(2,1,1); plot(n, x); title('signal = exp(jw0n)');
subplot(2,1,2); plot(w, X); title('DTFT');
grid;
