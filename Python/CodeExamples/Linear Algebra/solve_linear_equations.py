import numpy as np
import numpy.linalg as la

A = np.array([[1, 2], 
              [3, 4]])

b = np.array([[5], 
              [6]])

Ainv = la.inv(A)

x = Ainv.dot(b)

print(x)


x = np.linalg.solve(A, b)


print(x)
