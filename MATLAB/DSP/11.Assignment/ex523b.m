% Example 5.23

load times.txt -ascii;
conv_time=times(1,:);
hsconv_time=times(2,:);
n = 1:150; subplot(1,1,1); %set(gca,'LineWidth',10);
plot(n(25:150),conv_time(25:150),n(25:150),hsconv_time(25:150))
axis([0,180,0,0.4]);
set(gca,'XTickMode','manual','XTick',[25;50;75;100;125;150]);
set(gca,'YTickMode','manual','YTick',[0.05:0.05:0.35]); %grid
text(150,0.17,'highspeed')
text(150,0.155,'convolution')
text(150,0.34,'convolution')
xlabel('sequence length N'); ylabel('time in seconds');
title('Comparison of convolution times');

conv_time = zeros(1,150); fft_time = zeros(1,150);
for N = 1:150
    tc = 0; tf=0;
    L = 2*N-1; nu = round((log10(L)/log10(2))+0.45); L = 2^nu;
    for I=1:100
       h = randn(1,N);
       x = rand(1,N);
      t0 = clock; y1 = conv(h,x); t1=etime(clock,t0);
      tc = tc+t1;
      t0 = clock; y2 = ifft(fft(h,L).*fft(x,L)); t2=etime(clock,t0);
      tf = tf+t2;
    end
    conv_time(N)=tc/100;
    fft_time(N)=tf/100;
end

n = 1:150; subplot(1,1,1);
plot(n(25:150),conv_time(25:150),n(25:150),fft_time(25:150))
save times.txt conv_time fft_time -ascii -tabs
