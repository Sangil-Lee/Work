import numpy as np
import matplotlib.pyplot as plt


# Parameters
T = 5
a = -1/T

x0 = 1 
t = 0

tstart = 0
tstop = 25
increment = 1
N = 25

#t = np.arange(tstart,tstop+1,increment) #Alternative Approach
t = np.linspace(tstart, tstop, N)

x = np.exp(a*t) * x0


# Plot the Results
plt.plot(t,x)
plt.title('Plotting Differential Equation Solution')
plt.xlabel('t')
plt.ylabel('x(t)')
plt.grid()
plt.axis([0, 25, 0, 1])
plt.show()