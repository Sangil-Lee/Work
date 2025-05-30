import numpy as np
from scipy.optimize import curve_fit
import matplotlib.pyplot as plt

x = [0, 1, 2, 3, 4, 5]
y = [15, 10, 9, 6, 2, 0]


def linear_model(x, a, b, c, d):
    return a * x**3 + b * x**2 + c * x + d

popt, pcov = curve_fit(linear_model, x, y)

print(popt)

plt.plot(x,y, 'ok')


xstart = -1
xstop = 6
increment = 0.1
xmodel = np.arange(xstart,xstop,increment)

a = popt[0]
b = popt[1]
c = popt[2]
d = popt[3]

ymodel = a * xmodel**3 + b * xmodel**2 + c * xmodel + d

plt.plot(xmodel,ymodel, 'r')