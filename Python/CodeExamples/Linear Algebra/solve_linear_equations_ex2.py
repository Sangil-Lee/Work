import numpy as np

A = np.array([[1, 2], 
              [3, 4],
              [7, 8]])

b = np.array([[5],
              [6],
              [9]])

#x = np.linalg.solve(A, b) #Not working because inverse(A) does not exists


x = np.linalg.lstsq(A, b, rcond=None)[0]

print(x)