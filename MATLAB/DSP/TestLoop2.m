t=0:0.01:1; xt1=zeros(1,length(t));

for k=1:3
	xt1 = xt1 + (1/k)*sin(2*pi*k*t);
end
