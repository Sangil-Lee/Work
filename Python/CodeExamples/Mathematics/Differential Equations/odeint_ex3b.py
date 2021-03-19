import numpy as np
from scipy.integrate import odeint
import matplotlib.pyplot as plt

# Initialization
tstart = -1
tstop = 1
increment = 0.1

# Initial condition
x_init = [1,1]


t = np.arange(tstart,tstop+1,increment)


# Function that returns dx/dt
def mydiff(x, t):
    dx1dt = -x[1] 
    dx2dt = x[0]
    
    dxdt = [dx1dt, dx2dt]
    return dxdt


# Solve ODE
x = odeint(mydiff, x_init, t)
print(x)

x1 = x[:,0]
x2 = x[:,1]


# Plot the Results
plt.plot(t,x1)
plt.plot(t,x2)
plt.title('Plotting Differential Equations Solution')
plt.xlabel('t')
plt.ylabel('x(t)')
plt.grid()
plt.axis([-1, 1, -1.5, 1.5])
plt.show()