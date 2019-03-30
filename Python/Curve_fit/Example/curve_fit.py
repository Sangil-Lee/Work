import numpy as np
import matplotlib.pyplot as plt
from scipy.interpolate import *


x_given = np.array([0., 1., 2., 3., 4., 5.])
y_given = np.array([0, 0.8, 0.9, 0.1, -0.8, -1.0])

x_p = np.linspace(-2.0, 6.0, 100)
#--- Poliminal interpolation---
p3  = np.polyfit(x_given, y_given, 3)
p2  = np.polyfit(x_given, y_given, 2)
y_p3 = np.polyval(p3, x_p)
y_p2 = np.polyval(p2, x_p)

plt.plot(x_given, y_given, 'o')
plt.plot(x_p, y_p3, '-')
plt.plot(x_p, y_p2, '--')
plt.ylabel('y')
plt.xlabel('x')
plt.legend(['data', 'polyfit3', 'polyfit2'], loc='best')

plt.show()
