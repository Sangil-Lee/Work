%Example 4.15a

b=[1,1,1]/3; a=[1, -0.95, 0.9025];

Y = [-2,-3]; X = [1,1]; xic = filtic(b,a,Y,X)

bxplus = [1, -0.5]; axplus = [1,-1,1];
ayplus = conv(a,axplus)
byplus = conv(b,bxplus) + conv(xic, axplus)

[R,p,C]= residuez(byplus, ayplus)

Mp = abs(p), Ap=angle(p)/pi

