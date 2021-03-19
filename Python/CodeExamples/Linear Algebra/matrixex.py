import numpy as np


A = np.matrix([[0, 1], 
              [-2, -3]])

B = np.matrix([[1, 0], 
              [3, -2]])


C = A + B
print(C)


C = A - B
print(C)


C = A * B
print(C)

