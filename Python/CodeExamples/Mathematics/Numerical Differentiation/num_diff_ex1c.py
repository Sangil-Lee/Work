import numpy as np
import matplotlib.pyplot as plt


xstart = -2
xstop = 2.1
increment = 0.1
x = np.arange(xstart,xstop,increment)
 
y = x**2;


# Exact/Analytical Solution

dydx_exact = 2*x

print(dydx_exact)

plt.plot(x, dydx_exact, 'o-')



# Exact/Analytical Solution

dydx_num = np.diff(y) / np.diff(x);

print(dydx_num)

xstart = -2
xstop = 2

x = np.arange(xstart,xstop,increment)


plt.plot(x, dydx_num, 'o-')