
import numpy as np

a = np.array([1,2,3,4])
b = np.array([11,12,13,14])
c=a+b
print(c)

#multi dimension

a=np.array([[1,2,3],[4,5,6],[7,8,9]])
b=np.array([[11,12,13],[14,15,16],[17,18,19]])
c=a+b
d=a*b

print(c)
print(d)

#convert Array to Matrix
a=np.matrix(a)
b=np.matrix(b)

d=a*b
print(d)

#Matrix Addition
e = a+b
print(e)

print('Martrix Transpose \n', np.transpose(a))
