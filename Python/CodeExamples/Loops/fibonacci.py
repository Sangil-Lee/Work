N = 10

fib1 = 0
fib2 = 1

print(fib1)
print(fib2)

for k in range(N-2):
   fib_next = fib2 +fib1
   fib1 = fib2 
   fib2 = fib_next
   print(fib_next)
