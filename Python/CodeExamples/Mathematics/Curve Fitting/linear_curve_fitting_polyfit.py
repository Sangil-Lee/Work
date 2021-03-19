import numpy as np
import matplotlib.pyplot as plt

# Original Data
x = [0, 1, 2, 3, 4, 5]
y = [15, 10, 9, 6, 2, 0]

plt.plot(x,y, 'ok')


# Finding the Model
model_order = 3

p = np.polyfit(x, y, model_order)

print(p)


# Plot the Model
xstart = -1
xstop = 6
increment = 0.1
xmodel = np.arange(xstart,xstop,increment)

ymodel = np.polyval(p, xmodel) 

plt.plot(xmodel,ymodel, 'r')