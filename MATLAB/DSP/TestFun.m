function xt = TestSumFun(t, ck)

K = length(ck); k = 1:K;
ck = ck(:)';  t = t(:)';

xt= ck*sin(2*pi*k'*t);
