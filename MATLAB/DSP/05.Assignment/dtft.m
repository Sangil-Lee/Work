%DTFT Matlab function code

function [X]=dtft(x,n,w)
%Computes Discrete-time Fourier Transform
%[X]=dtft(x,n,w)
% X= DTFT values computed at w frequencies
% x= finite durtion sequence over n
% n= sampling position vector
% w= frequency location vector

	tmp = w' *n;
	tmp = -1i * tmp;
	e = exp(tmp);
	X = e* x';
end
