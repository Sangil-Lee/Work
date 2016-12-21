% N=500
x = chirp([0:0.001:2],0,2,500);  %freq. sweep from 0-500 over 2 sec.
Fs=1000;     %sampled every 0.001 sec so rate is 1 kHz
nfft = min(256,length(x))
specgram(x,nfft,Fs)
%specgram(x,nfft,Fs,rectwin(nfft))