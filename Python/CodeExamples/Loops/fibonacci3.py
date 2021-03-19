import numpy as np
  

N = 10

fib = np.zeros(N)

fib[0] = 0
fib[1] = 1

for k in range(N-2):
   fib[k+2] = fib[k+1] +fib[k]


print(fib)
