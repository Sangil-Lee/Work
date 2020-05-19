# -*- coding: utf-8 -*-
"""
Created on Wed Apr  8 13:09:48 2020

@author: ctrluser
"""

import numpy as np
import matplotlib.pylab as plt

# x = np.linspace(-np.pi, np.pi, 201)
x = np.linspace(-800, 800, 1000)
plt.plot(x, 2*np.tanh(1*(x-x*0.996)))
plt.xlabel('x')
plt.grid()
plt.ylabel('tanh(x)')
plt.axis('tight')
plt.show()