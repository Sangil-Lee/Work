%Example 4.11-b

b = [1,0]; a = [1, -0.9];
%[H, w]= freqz(b,a, 100);
%magH=abs(H); phaH = angle(H);

%[H,w]=freqz(b,a,200,'whole');
%magH=abs(H(1:101)); phaH=angle(H(1:101));

w = [0:1:100]*pi/100; H = freqz(b,a,w);

subplot(2,1,1); plot(w/pi,magH); grid;
xlabel('frequency in pi units'); ylabel('Magnitude');
title('Magnitude Response');

subplot(2,1,2); plot(w/pi,phaH/pi); grid;
xlabel('frequency in pi units'); ylabel('Phase in pi units');
title('Phase Response');
