n=[-10:1:10]; alpha = -0.1+0.3j;
x = exp(alpha*n);
subplot(2,2,1); stem(n, real(x)); title('Real part'); xlabel('n');
subplot(2,2,2); stem(n, imag(x)); title('Imaginary part'); xlabel('n');
subplot(2,2,3); stem(n, abs(x)); title('Magnitude part'); xlabel('n');
subplot(2,2,4); stem(n, (180/pi)*angle(x)); title('Phase part'); xlabel('n');
