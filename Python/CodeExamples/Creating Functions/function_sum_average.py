def stat(x):
    
    totalsum = 0
    
    #Find the Sum of all the numbers
    for x in data:
      totalsum = totalsum + x
      
    
    #Find the Mean or Average of all the numbers
    
    N = len(data)
    
    mean = totalsum/N
    
    
    return totalsum, mean



data = [1, 5, 6, 3, 12, 3]


totalsum, mean = stat(data)

print(totalsum, mean)