% N = 1024
n=linspace(0, 1, 1024);s1=sin(2*pi*80*n);s2=sin(2*pi*160*n);
s=[s1 zeros(1, 128) s2 zeros(1, 128) s1+s2];  
Fs=2;     %sampled every 0.001 sec so rate is 1 kHz
nfft = min(256,length(n))
specgram(s,nfft,Fs,rectwin(nfft))
