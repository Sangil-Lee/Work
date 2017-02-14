% Example 5.22

Nmax = 2048; fft_time=zeros(1,Nmax);

for n=1:1:Nmax
   %disp(n);
   %x=ones(1,n);
   x=rand(1,n);
   t=clock;fft(x);fft_time(n)=etime(clock,t);
end

n=[1:1:Nmax];
plot(n,fft_time,'.');

%save fft_time.mat fft_time n;

%load fft_time.mat;
%top = ceil(max(fft_time));
%subplot
%clf
%plot(n,fft_time,'.');
axis([0,2500,0,50])
xlabel('N');ylabel('Time in Sec.')
title('FFT execution times')
%text(2100,top,'o(N*N)')
%text(2100,top/2,'o(N*N/2)')
%text(2100,top/3,'o(N*N/3)')
%text(2100,top/4,'o(N*N/4)')
%text(2100,1,'o(N*logN)')

