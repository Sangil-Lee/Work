import numpy as np
import matplotlib.pyplot as plt


xstart = -2
xstop = 2.1
increment = 0.1
x = np.arange(xstart,xstop,increment)

y = x**2

plt.plot(x,y)
 

xstart = -2
xstop = 3
increment = 1
x = np.arange(xstart,xstop,increment)
 
y = x**2;

plt.plot(x,y, '-o')