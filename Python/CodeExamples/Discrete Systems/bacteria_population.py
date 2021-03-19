# Simulation of Bacteria Population
import numpy as np
import matplotlib.pyplot as plt

# Model Parameters
b = 1
p = 0.5
 
# Simulation Parameters
Ts = 0.01
Tstop = 1
xk = 100
N = int(Tstop/Ts) # Simulation length
data = []
data.append(xk)

 
# Simulation
for k in range(N):   
   xk1 = xk + Ts* (b * xk - p * xk**2);
   xk = xk1
   data.append(xk1)

# Plot the Simulation Results
t = np.arange(0,Tstop+Ts,Ts)

plt.plot(t,data)
plt.title('Simulation of Bacteria Population')
plt.xlabel('t [s]')
plt.ylabel('x')
plt.grid()
#plt.axis([0, 1, 0, 100])
plt.xlim(0, 1)
plt.ylim(0, 100)  
plt.show()