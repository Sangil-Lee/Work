function [y,H]=conv_tp(h,x)
% Linear Convolution using Toeplitz Matrix
%-----------------------------------------
% [y,H]=conv_tp(h,x)

% y = Output sequence in column vector form
% H = Toeplitz matrix corrensponding to sequence h so that y = Hx
% h = Impulse response sequence in column vector


Nx = length(x); Ny = length(h);

hc = [h; zeros(Nx-1, 1)];
hr = [h(1); zeros(1,Nx-1)];

H = toeplitz(hc,hr);
y=H*x;

