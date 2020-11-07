# -*- coding: utf-8 -*-
"""
Created on Fri Nov  6 21:09:36 2020

@author: ctrluser
"""
"""import matplotlib.pyplot as plt"""

import numpy as np
from scipy.optimize import curve_fit
from epics import PV
import time

bval = 0

def func(x, a, b, c):
    return a*np.exp(-(x-b)**2/(2*c**2))

def onChanges(pvname=None, value=None, char_value=None, **kw):
    global bval
    bval = value
    return;

mypv = PV('LEBT-DIAG:WS002:BValue')
mypv.add_callback(onChanges)

t0=time.time()

"""while time.time() - t0 < 5.0:"""
while True:
   bvalue = bval
   if bvalue:
       x = np.linspace(-10, 10, 100)
       y = func(x, 2, 0, 2)
       np.random.seed(int(time.time()))
       yn = y+0.2*np.random.normal(size=len(x))
       popt, pcov = curve_fit(func, x, yn)
       print(popt)
       wavepv = PV('LEBT-DIAG:WS002:WValue')
       wavepv.put(x, wait=True)
   time.sleep(1)
   
print('Done')

    