t=0:0.01:1; N=length(t); xt=zeros(1,N);

for n = 1:N
	temp = 0;
		for k=1:3
			temp = temp + (1/k)*sin(2*pi*k*t(n));
		end
	xt(n) = temp;
end
