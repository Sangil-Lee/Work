# -*- coding: utf-8 -*-
"""
Created on Wed Apr  8 12:40:03 2020

@author: ctrluser
"""

import numpy as np
import matplotlib.pyplot as plt

in_array = np.linspace(-np.pi, np.pi, 12) 
out_array = np.tanh(in_array) 
  
print("in_array : ", in_array) 
print("\nout_array : ", out_array) 
  
# red for numpy.tanh() 
plt.plot(in_array, out_array, color = 'red', marker = "o") 
plt.title("numpy.tanh()") 
plt.xlabel("X") 
plt.ylabel("Y") 
plt.show() 