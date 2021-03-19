import numpy as np
from scipy.optimize import curve_fit
import matplotlib.pyplot as plt

x = [0, 1, 2, 3, 4, 5]
y = [15, 10, 9, 6, 2, 0]


def linear_model(x, a, b, c, d, e):
    return a * x**4 + b * x**3 + c * x**2 + d * x + e

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
e = popt[4]

ymodel = a * xmodel**4 + b * xmodel**3 + c * xmodel**2 + d * xmodel + e

plt.plot(xmodel,ymodel, 'g')