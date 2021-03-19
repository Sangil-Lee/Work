import numpy as np
import matplotlib.pyplot as plt

x = [1, 2, 3]
y = [3, 2, 0]

x_new = 2.5

y_new = np.interp(2.5, x, y)

print("New Interpolated Value:")
print(y_new)

plt.plot(x,y,'o-')
plt.show()