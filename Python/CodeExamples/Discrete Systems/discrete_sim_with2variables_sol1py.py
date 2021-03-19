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
x1k = 1
x2k = 1
N = int((Tstop-Tstart)/Ts) # Simulation length
datax1 = []
datax2 = []
datax1.append(x1k)
datax2.append(x2k)

 
# Simulation
for k in range(N):   
   x1k1 = x1k - Ts * x2k
   x2k1 = x2k + Ts * x1k
   
   x1k = x1k1
   x2k = x2k1
   datax1.append(x1k1)
   datax2.append(x2k1)

# Plot the Simulation Results
t = np.arange(Tstart,Tstop+Ts,Ts)

plt.plot(t,datax1, t, datax2)
plt.title('Simulation with 2 Variables')
plt.xlabel('t [s]')
plt.ylabel('x')
plt.grid()
plt.axis([-1, 1, -1.5, 1.5])
plt.show()