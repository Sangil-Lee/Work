import numpy as np
import matplotlib.pyplot as plt
from scipy.optimize import *

# if -> f(x)= a*exp(-bx) + c

#Define the function with parameters
def f(x, a, b, c) :
	return a*np.exp(-b*x)+c


#---- Set some given data
x_given = np.linspace(0,4,50)
y_given = f(x_given, 2.5, 1.3, 0.5) + 0.2*np.random.normal(size=len(x_given))

#---- Do curve fit
params, extras = curve_fit(f, x_given, y_given)

print('a=%g, b=%g, c=%g' %(params[0], params[1], params[2]))

plt.plot(x_given, y_given, 'o')
plt.plot(x_given, f(x_given, params[0], params[1], params[2]))
plt.ylabel('y')
plt.xlabel('x')
plt.legend(['data', 'exp_curve_fit'], loc='best')

plt.show()
