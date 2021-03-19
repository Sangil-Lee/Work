import numpy as np


A = np.array([[0, 1], 
              [-2, -3]])

B = np.array([[1, 0], 
              [3, -2]])

C = A + B
print(C)


C = A.dot(B)
print(C)

C = np.dot(A,B)
print(C)


C =  np.mat(A) * np.mat(B)
print(C)


C = A.transpose()
print(C)


