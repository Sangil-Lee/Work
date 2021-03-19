import numpy as np
from scipy.integrate import odeint
import matplotlib.pyplot as plt

# Initialization
tstart = -1
tstop = 1
increment = 0.1

# Initial condition
z0 = [1,1]


t = np.arange(tstart,tstop+1,increment)


# Function that returns dx/dt
def mydiff(z, t):
    dxdt = -z[1] 
    dydt = z[0]
    
    dzdt = [dxdt,dydt]
    return dzdt


# Solve ODE
z = odeint(mydiff, z0, t)
print(z)

x = z[:,0]
y = z[:,1]


# Plot the Results
plt.plot(t,x)
plt.plot(t,y)
plt.title('Plotting Differential Equations Solution')
plt.xlabel('t')
plt.ylabel('z(t)')
plt.grid()
plt.axis([-1, 1, -1.5, 1.5])
plt.show()