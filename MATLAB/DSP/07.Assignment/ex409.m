%Example 4.9

b = 1; a = poly([0.9,0.9,-0.9])
[R,p,C ] = residuez(b,a)


[delta, n]=impseq(0,0,7); 

x = filter(b,a,delta)

x = (0.75)*(0.9).^n + (0.5)*n.*(0.9).^n + (0.25)*(-0.9).^n
