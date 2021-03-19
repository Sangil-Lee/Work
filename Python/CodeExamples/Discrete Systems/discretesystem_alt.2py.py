# Simulation of discrete model
import numpy as np
import matplotlib.pyplot as plt

# Model Parameters
a = 0.25
b = 2
 
# Simulation Parameters
Ts = 0.1 # Sampling Time
Tstop = 30 # End of Simulation Time
uk = 1 # Step Response
N = int(Tstop/Ts) # Simulation length
x = np.zeros(N+2) # Initialization the x vector
x[0] = 0

 
# Simulation
for k in range(N+1):
   x[k+1] = (1 - a*Ts) * x[k]  + Ts * b * uk

 
# Plot the Simulation Results
t = np.arange(0,Tstop+2*Ts,Ts) #Create the Time Series

plt.plot(t,x)

# Formatting the appearance of the Plot
plt.title('Simulation of dxdt = -ax + bu')
plt.xlabel('t [s]')
plt.ylabel('x')
plt.grid()
plt.axis([0, 30, 0, 8])
plt.show()
