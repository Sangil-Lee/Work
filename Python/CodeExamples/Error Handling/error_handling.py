try:
    10 * (1/0)
except:
    print("The calculation failed")
    
    
    
try:
  print(x)
except:
  print("x is not defined")
  
  
  


try:
  print(x)
except NameError:
  print("x is not defined")
except:
  print("Something is wrong")
  
  
  
  
x=2

try:
  print(x)
except NameError:
  print("x is not defined")
except:
  print("Something is wrong")
finally:
    print("The Program is finished")