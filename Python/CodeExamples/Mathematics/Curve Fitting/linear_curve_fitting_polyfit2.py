import numpy as np
import matplotlib.pyplot as plt

# Original Data
x = [0, 1, 2, 3, 4, 5]
y = [15, 10, 9, 6, 2, 0]

plt.plot(x,y, 'ok')

# x values for model
xstart = -1
xstop = 6
increment = 0.1
xmodel = np.arange(xstart,xstop,increment)


startorder = 1
endorder = 5

for model_order in range(startorder, endorder, 1):

    # Finding the Model
    p = np.polyfit(x, y, model_order)
    
    print(p)
    
    # Plot the Model
    ymodel = np.polyval(p, xmodel) 
    
    plt.plot(xmodel,ymodel)