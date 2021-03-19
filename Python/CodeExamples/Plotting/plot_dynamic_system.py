import math as mt
import numpy as np
import matplotlib.pyplot as plt


# Model Parameters
T = 5
a = -1/T

# Simulation Parameters
x0 = 1 
t = 0

tstart = 0
tstop = 25

increment = 1

x = []
x = np.zeros(tstop+1)

t = np.arange(tstart,tstop+1,increment)


# Define the Function
for k in range(tstop):
    x[k] = mt.exp(a*t[k]) * x0


# Plot the Simulation Results
plt.plot(t,x)
plt.title('Simulation of Dynamic System')
plt.xlabel('t')
plt.ylabel('x')
plt.grid()
plt.axis([0, 25, 0, 1])
plt.show()