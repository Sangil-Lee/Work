% Example 7.3

h = [1,1,1]; w = [0:500]*pi/500;
H = freqz(h,1,w);
magH = abs(H); phaH = angle(H);
ampH = ones(1,501)+2*cos(w); angH = -w;

subplot(2,2,1); plot(w/pi,magH); title('Magnitude Response');
xlabel('frequency in pi units'); ylabel('|H|'); grid
axis([0 1 -1.5 3.5]);

subplot(2,2,3); plot(w/pi,phaH/pi); title('Piecewise Linear Phase Response');
xlabel('frequency in pi units'); ylabel('angle in pi units'); grid
axis([0 1 -1 1])

subplot(2,2,2); plot(w/pi,ampH); title('Amplitude Response');
xlabel('frequency in pi units'); ylabel('Hr'); grid
axis([0 1 -1.5 3.5])

subplot(2,2,4); plot(w/pi,angH/pi); title('Linear Phase Response');
xlabel('frequency in pi units'); ylabel('angle in pi units'); grid
axis([0 1 -1 1])
