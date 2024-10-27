####Matlab Code##########

fs = 2000;    % Sampling frequency  
t =0:1/fs:5;  % Time 
F1 = 5;       % Lowest frequency
F2 = 17;      % Mid frequency
F3 = 55;      % Highest frequecy    
X1 = 0.25*sin(2*pi*t*F3)+randn(size(t))*.5;
X2 = 3*sin(2*pi*t*F1)+cos(2*pi*t*F2)+randn(size(t))*.5+X1;
yyaxis left
N = 2^nextpow2(length(X1));
Y1 = fft(X1, N);               % FFT of the signal x1(t)
Y2 = fft(X2, N);               % FFT of the signal x2(t)
freq = fs*(0:(N/2))/N;         % Freq values
Yp1 = abs(Y1/N);               % Absolute value or magnitude of the signal spectrum
Yp2 = abs(Y2/N);             
yyaxis left
plot(freq,Yp1(1:N/2+1)) 
ylabel('Magnitude of signal = |Y1(f)|')
yyaxis right
plot(freq,Yp2(1:N/2+1))
title('FFT analysis of a signal')
xlabel('Frequency, [Hz]')
ylabel('Magnitude of signal = |Y2(f)|')
grid on
xlim([0, 65])
