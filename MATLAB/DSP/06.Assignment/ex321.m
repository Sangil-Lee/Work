% Example 3.21


ts = 0.0002; Fs = 1/ts; n = -25:1:25; nTs = n*ts;
x = exp(-1000*abs(nTs));

dt = 0.00005;
t = -0.005:dt:0.005;

xa = x * sinc(Fs*(ones(length(nTs),1)*t-nTs'*ones(1,length(t))));

error = max(abs(xa - exp(-1000*abs(t))))

subplot(2,1,2);plot(t*1000,xa);
xlabel('t in msec.'); ylabel('xa(t)')
title('Reconstructed Signal from x1(n) using sinc function'); hold on
stem(n*ts*1000,x); hold off
