t=0:0.01:1; xt=zeros(1,length(t));

for k=1:3
	xt = xt + (1/k)*sin(2*pi*k*t);
end
