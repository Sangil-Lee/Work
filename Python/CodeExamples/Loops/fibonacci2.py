N = 10

fib = [0, 1]


for k in range(N-2):
   fib_next =  fib[k+1] +fib[k]
   fib.append(fib_next)
    
print(fib)