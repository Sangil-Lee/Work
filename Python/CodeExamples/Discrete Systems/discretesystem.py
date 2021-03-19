# Simulation of discrete model
import numpy as np
import matplotlib.pyplot as plt

# Model Parameters
a = 0.25
b = 2
 
# Simulation Parameters
Ts = 0.1
Tstop = 30
uk = 1 # Step Response
xk = 0
N = int(Tstop/Ts) # Simulation length
data = []
data.append(xk)

 
# Simulation
for k in range(N):
   xk1 = (1 - a*Ts) * xk  + Ts * b * uk
   xk = xk1
   data.append(xk1)

 
# Plot the Simulation Results
t = np.arange(0,Tstop+Ts,Ts)

plt.plot(t,data)

# Formatting the appearance of the Plot
plt.title('Simulation of dxdt = -ax + bu')
plt.xlabel('t [s]')
plt.ylabel('x')
plt.grid()
plt.axis([0, 30, 0, 8])
plt.show()
