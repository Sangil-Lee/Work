%Example 3.13
w = [0:1:500]*pi/500;

H = exp(j*w) ./ (exp(j*w) - 0.9*ones(1,501));
magX = abs(H); angX = angle(H);

subplot(2,1,1); plot(w/pi,magX); grid; axis([0,1,0,10])

xlabel('frequency in pi units'); ylabel('|H|');
title('Magnitude Response');

subplot(2,1,2); plot(w/pi,angX/pi); grid
xlabel('frequency in pi units'); ylabel('Phase in pi Radians');
title('Phase Response');
