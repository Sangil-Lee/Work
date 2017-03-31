% Example 3.22
ts = 0.001; Fs = 1/ts; n = -5:1:5; nTs = n*ts;
x = exp(-1000*abs(nTs));

dt = 0.00005;
t = -0.005:dt:0.005;

xa = x * sinc(Fs*(ones(length(nTs),1)*t-nTs'*ones(1,length(t))));
error = max(abs(xa - exp(-1000*abs(t))))

subplot(2,1,2);plot(t*1000,xa);
xlabel('t in msec.'); ylabel('xa(t)')
title('Reconstructed Signal from x2(n) using sinc function'); hold on
stem(n*ts*1000,x); hold off

