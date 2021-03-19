import numpy as np
import matplotlib.pyplot as plt

xmin = -5
xmax = 5
dx = 0.1

N = int((xmax - xmin)/dx)

x = np.linspace(xmin, xmax, N+1)
#x = np.arange(xmin, xmax+dx, dx) #Alternative implementation
 
y = x**2 + 2*x + 1;


plt.plot(x,y)
plt.xlim([xmin,xmax])