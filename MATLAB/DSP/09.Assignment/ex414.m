%Example 4.14

format long;

b = [1,0,0]; a = [1, -3/2,1/2];
n = [0:7]; x = (1/4).^n; 
xic = [1,-2];

y1 = filter(b,a,x,xic)

y2 =  (1/3)*(1/4).^n+(1/2).^n+(2/3)*ones(1,8)

Y = [4,10]; xic = filtic(b,a,Y)

