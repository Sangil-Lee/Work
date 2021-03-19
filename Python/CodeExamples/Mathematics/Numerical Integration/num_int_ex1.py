import numpy as np
import matplotlib.pyplot as plt

a = 0
b = 1
N = 10


x = np.linspace(a,b,N+1)
 
y = x**2;

y_right = y[1:]
y_left = y[:-1]


# Trapezoid Rule
dx = (b - a)/N
A = (dx/2) * np.sum(y_right + y_left)

print(A)


plt.plot(x,y)
plt.xlim([0,1]); plt.ylim([0,1]);