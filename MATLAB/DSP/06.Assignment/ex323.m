% Example 3.23

ts = 0.0002; n = -25:1:25; nTs = n*ts;
x = exp(-1000*abs(nTs));

subplot(2,1,1); stairs(nTs*1000,x);
xlabel('t in msec.'); ylabel('xa(t)')
title('Reconstructed Signal from x1(n) using zero-order-hold'); hold on
stem(n*ts*1000,x); hold off

subplot(2,1,2); plot(nTs*1000,x);
xlabel('t in msec.'); ylabel('xa(t)')
title('Reconstructed Signal from x1(n) using first-order-hold'); hold on
stem(n*ts*1000,x); hold off
