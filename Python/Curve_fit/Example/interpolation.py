import numpy as np
import matplotlib.pyplot as plt
from scipy.interpolate import *


x_given = np.linspace(0,10,10)
y_given = np.cos(x_given**2.0/8.0)

xx = np.linspace(0,10,1000)
yy = np.cos(xx**2.0/8.0)

x_i = np.linspace(0,10,100)

#--- Linear interpolation---
f_linear = interp1d(x_given, y_given)
y_il = f_linear(x_i)

#--- Spline interpolation---
f_spline = interp1d(x_given, y_given, kind='cubic')
y_is = f_spline(x_i)

print('Y_SData:', y_is)

#plt.plot(x_given, y_given, 'o', label='given data')
#plt.plot(xx, yy, ':', label='perfect')
plt.plot(x_given, y_given, 'o')
plt.plot(x_i, y_il, '-')
plt.plot(x_i, y_is, '--')
plt.plot(xx, yy, ':')

plt.ylabel('y')
plt.xlabel('x')
plt.legend(['data', 'linear', 'spline', 'perfect'], loc='best')

plt.show()
