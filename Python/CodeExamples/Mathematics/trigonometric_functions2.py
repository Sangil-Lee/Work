import math as mt
import matplotlib.pyplot as plt

xdata = []
ydata = []


for x in range(0, 10):
    xdata.append(x)
    y = mt.sin(x)
    ydata.append(y)
    

plt.plot(xdata, ydata)
plt.show()