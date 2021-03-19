import numpy as np
from scipy.optimize import curve_fit
import matplotlib.pyplot as plt

x = [0, 1, 2, 3, 4, 5]
y = [15, 10, 9, 6, 2, 0]


def linear_model(x, a, b, c, d, e, f):
    return a * x**5 + b * x**4 + c * x**3 + d * x**2 + e * x + f

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
f = popt[5]

ymodel = a * xmodel**5 + b * xmodel**4 + c * xmodel**3 + d * xmodel**2 + e * xmodel + f

plt.plot(xmodel,ymodel, 'm')