% P17-3 Spline Interpolation: x_a(t) = cos(20*pi*t); 0 <= t <= 1;
% T_s = 0.01 sec,0.05 sec and 0.1 sec;
clc; close all; set(0,'defaultfigurepaperposition',[0,0,6,4]);
Hf_1 = figure; set(Hf_1,'NumberTitle','off','Name','P17-3');
%
Ts1 = 0.01; Fs1 = 1/Ts1; n1 = [0:100]; nTs1 = n1*Ts1;
x1 = cos(20*pi*nTs1); Dt = 0.001; t = 0:Dt:1; xa1 = spline(nTs1,x1,t);
subplot(3,1,1); plot(t,xa1,'LineWidth',1.5); axis([0 1 -1.2 1.2]);
xlabel('t in sec'); ylabel('y_a(t)');
title(['Spline Interpolation: T_s = 0.01 sec']);
%
Ts2 = 0.05; Fs2 = 1/Ts2; n2 = [0:20]; nTs2 = n2*Ts2;
x2 = cos(20*pi*nTs2); Dt = 0.001; t = 0:Dt:1; xa2 = spline(nTs2,x2,t);
subplot(3,1,2); plot(t,xa2,'LineWidth',1.5); axis([0 1 -1.2 1.2]);
xlabel('t in sec'); ylabel('y_a(t)');
title(['Spline Interpolation: T_s = 0.05 sec']); grid;


Ts3 = 0.1; Fs3 = 1/Ts3; n3 = [0:10]; nTs3 = n3*Ts3; x3 = cos(20*pi*nTs3);
Dt = 0.001; t = 0:Dt:1; xa3 = spline(nTs3,x3,t);
subplot(3,1,3); plot(t,xa3,'LineWidth',1.5); axis([0 1 -1.2 1.2]);
xlabel('t in sec'); ylabel('y_a(t)');
title(['Spline Interpolation: T_s = 0.1 sec']); grid;


%4. Comments: From the plots in Figures it is clear that reconstructions from samples at Ts = 0.01 and 0.05 depict the original frequency (excluding end effects) but reconstructions for Ts = 0.1 show the original frequency aliased to zero. Furthermore, the cubic spline interpolation is a better reconstruction than the sinc interpolation, that is, the sinc interpolation is more susceptible to boundary effect.
