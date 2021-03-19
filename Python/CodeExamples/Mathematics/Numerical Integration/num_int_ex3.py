import numpy as np

a = 0
b = 1
N = 10
dx = (b - a)/N

x = np.linspace(a,b,N+1)
 
y = x**2;

I = np.trapz(y,x,dx)

print(I)