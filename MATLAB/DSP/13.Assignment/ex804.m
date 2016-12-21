% Example 8.4: Butterworth Lowpass Analog filter design

Wp = 0.2*pi; Ws = 0.3*pi; Rp = 7; As = 16;
Ripple = 10 ^ (-Rp/20); Attn = 10 ^ (-As/20);

[b,a] = afd_butt(Wp,Ws,Rp,As);

[C,B,A] = sdir2cas(b,a)

[db,mag,pha,w] = freqs_m(b,a,0.5*pi);

%[ha,x,t] = impulse(b,a);



% Plots
subplot(2,2,1); plot(w/pi,mag);title('Magnitude Response')
xlabel('Analog frequency in pi units'); ylabel('|H|'); axis([0,0.5,0,1.1])

subplot(2,2,2); plot(w/pi,db);title('Magnitude in dB')
xlabel('Analog frequency in pi units'); ylabel('decibels'); axis([0,0.5,-30,5])

subplot(2,2,3); plot(w/pi,pha/pi); title('Phase Response')
xlabel('Analog frequency in pi units'); ylabel('radians'); axis([0,0.5,-1,1])

%subplot(2,2,4); plot(t,ha,[0,max(t)],[0,0]); title('Impulse Response')
%xlabel('time in seconds'); ylabel('ha(t)'); axis([0,max(t),min(ha),max(ha)])

