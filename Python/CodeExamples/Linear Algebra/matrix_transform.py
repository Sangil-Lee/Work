import numpy as np


A = np.matrix([[0, 1], 
              [-2, -3]])

At = np.transpose(A)

print (At)



B = np.matrix([[1, 0, 4], 
              [3, -2, 8]])

Bt = np.transpose(B)

print (Bt)



C = np.matrix([[1, 4,], 
               [2, -3,],
              [-6, -2]])

Ct = np.transpose(C)

print (Ct)