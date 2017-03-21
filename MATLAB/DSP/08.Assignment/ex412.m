%Example 4.12

b = [0,1,1]; a = [1, -0.9, 0.81];
[R,p,C]=residuez(b,a)

Mp = (abs(p))'
Ap = (angle(p))'/pi
