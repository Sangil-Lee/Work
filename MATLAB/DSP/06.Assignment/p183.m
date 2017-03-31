%(c) Reconstruction of the analog signal ya(t) from the samples x(n) using the spline interpolation (for θ = 0, π/6, π/4, π/3, π/2. Matlab script:
clc; close all; set(0,'defaultfigurepaperposition',[0,0,6,7]);
Hf_1 = figure; set(Hf_1,'NumberTitle','off','Name','P18-3');
Ts = 0.05; Fs = 1/Ts; Dt = 0.001; t = 0:Dt:1; n = [0:20]; nTs = n*Ts;
theta1 = 0; x1 = cos(20*pi*nTs+theta1); y_a1 = spline(nTs,x1,t);
subplot(5,1,1); plot(t,y_a1,'LineWidth',1.5); axis([0 1 -1.2 1.2]); hold on;
plot(nTs,x1,'o'); xlabel('t in sec');
title('Spline Interpolation for theta = 0');
ylabel('Amplitude');
theta2 = pi/6; x2 = cos(20*pi*nTs+theta2); y_a2 = spline(nTs,x2,t);
subplot(5,1,2); plot(t,y_a2,'LineWidth',1.5); hold on; axis([0 1 -1.2 1.2]);
plot(nTs,x2,'o'); xlabel('t in sec');
title('Spline Interpolation for theta = \pi/6');
ylabel('Amplitude');
theta3 = pi/4; x3 = cos(20*pi*nTs+theta3); y_a3 = spline(nTs,x3,t);
subplot(5,1,3); plot(t,y_a3,'LineWidth',1.5); hold on; axis([0 1 -1.2 1.2]);
plot(nTs,x3,'o'); xlabel('t in sec');
title('Spline Interpolation for theta = \pi/3');
ylabel('Amplitude');
theta4 = pi/3; x4 = cos(20*pi*nTs+theta4); y_a4 = spline(nTs,x4,t);
subplot(5,1,4); plot(t,y_a4,'LineWidth',1.5); axis([0 1 -1.2 1.2]); hold on;
plot(nTs,x4,'o'); ylabel('Amplitude');
title('Spline Interpolation for theta = \pi');
xlabel('t in sec');
theta5 = pi/2; x5 = cos(20*pi*nTs+theta5); y_a5 = spline(nTs,x5,t);
subplot(5,1,5); plot(t,y_a5,'LineWidth',1.5); axis([0 1 -1.2 1.2]); hold on;
plot(nTs,x5,'o'); ylabel('Amplitude');
title('Spline Interpolation for theta = \pi/2');
xlabel('t in sec');


% (d) When a sinusoidal signal is sampled at f = 2 samples per cycle as is the case in this problem, then the resulting samples x(n) has the amplitude that depends on the phase of the signal. In particular note that this amplitude is given by cos(θ ). Thus the amplitude of the reconstructed signal y(t) is also equal to cos(θ ).
