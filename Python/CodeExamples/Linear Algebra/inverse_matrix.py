import numpy as np
import numpy.linalg as la

A = np.matrix([[1, 2], 
              [3, 4]])


Ainv = la.inv(A)

print(Ainv)





Adet = la.det(A)

print(Adet)