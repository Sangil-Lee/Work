# Simulation with 2 Variables
import numpy as np
import matplotlib.pyplot as plt

# Model Parameters
b = 1
p = 0.5
 
# Simulation Parameters
Ts = 0.1
Tstart = -1
Tstop = 1
N = int((Tstop-Tstart)/Ts) # Simulation length
x1 = np.zeros(N+2)
x2 = np.zeros(N+2)
x1[0] = 1
x2[0] = 1

 
# Simulation
for k in range(N+1):   
   x1[k+1] = x1[k] - Ts * x2[k]
   x2[k+1] = x2[k] + Ts * x1[k]
   

# Plot the Simulation Results
t = np.arange(Tstart,Tstop+2*Ts,Ts)

plt.plot(t,x1, t, x2)
plt.title('Simulation with 2 Variables')
plt.xlabel('t [s]')
plt.ylabel('x')
plt.grid()
plt.axis([-1, 1, -1.5, 1.5])
plt.show()