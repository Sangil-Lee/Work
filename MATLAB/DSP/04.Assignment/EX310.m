%Example 3.10

n = -5:10; x = rand(1,length(n)) + j*rand(1,length(n));
k = -100:100; w = (pi/100)*k;        
X = x * (exp(-j*pi/100)).^(n'*k);    

y = conj(x);                         
Y = y * (exp(-j*pi/100)).^(n'*k);    

Y_check = conj(fliplr(X));           
error = max(abs(Y-Y_check))          

