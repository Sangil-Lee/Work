import numpy as np

p = np.poly1d([1,0,0,2])
print(p)

dpdx = np.polyder(p)
print(dpdx)