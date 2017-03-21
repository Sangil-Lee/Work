%(b) Reconstruction of the analog signal ya(t) from the samples x(n) using the sinc interpolation (for θ = 0, π/6, π/4, π/3, π/2. Matlab script:
clc; close all; set(0,'defaultfigurepaperposition',[0,0,6,7]);
Hf_1 = figure; set(Hf_1,'NumberTitle','off','Name','p18-2');
Ts = 0.05; Fs = 1/Ts; Dt = 0.001; t = 0:Dt:1; n = [0:20]; nTs = n*Ts;
theta1 = 0; x1 = cos(20*pi*nTs+theta1);
y_a1 = x1*sinc(Fs*(ones(length(n),1)*t-nTs'*ones(1,length(t))));
subplot(5,1,1); plot(t,y_a1,'LineWidth',1.5); hold on;
plot(nTs,x1,'o'); axis([0 1 -1.2 1.2]); xlabel('t in sec');
title('Sinc Interpolation for \theta = 0');
ylabel('Amplitude');
theta2 = pi/6; x2 = cos(20*pi*nTs+theta2);
y_a2 = x2*sinc(Fs*(ones(length(n),1)*t-nTs'*ones(1,length(t))));
subplot(5,1,2); plot(t,y_a2,'LineWidth',1.5); hold on; axis([0 1 -1.2 1.2])
plot(nTs,x2,'o'); xlabel('t in sec');
title('Sinc Interpolation for \theta = \pi/6');
ylabel('Amplitude');
theta3 = pi/4; x3 = cos(20*pi*nTs+theta3);
y_a3 = x3*sinc(Fs*(ones(length(n),1)*t-nTs'*ones(1,length(t))));
subplot(5,1,3); plot(t,y_a3,'LineWidth',1.5); hold on; axis([0 1 -1.2 1.2])
plot(nTs,x3,'o'); xlabel('t in sec');
title('Sinc Interpolation for \theta = \pi/4');
ylabel('Amplitude');
theta4 = pi/3; x4 = cos(20*pi*nTs+theta4);
y_a4 = x4*sinc(Fs*(ones(length(n),1)*t-nTs'*ones(1,length(t))));
subplot(5,1,4); plot(t,y_a4,'LineWidth',1.5); axis([0 1 -1.2 1.2]); hold on;
plot(nTs,x4,'o'); xlabel('t in sec');
title('Sinc Interpolation for \theta = \pi/3');
ylabel('Amplitude');
theta5 = pi/2; x5 = cos(20*pi*nTs+theta5);
y_a5 = x5*sinc(Fs*(ones(length(n),1)*t-nTs'*ones(1,length(t))));
subplot(5,1,5); plot(t,y_a5,'LineWidth',1.5); axis([0 1 -1.2 1.2]); hold on;
plot(nTs,x5,'o'); xlabel('t in sec');
title('Sinc Interpolation for \theta = \pi/3');
ylabel('Amplitude');
