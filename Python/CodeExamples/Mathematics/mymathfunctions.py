import math as mt

def calcexpression(x,y):
    
    z = 3*x**2 + mt.sqrt(x**2 + y**2) + mt.exp(mt.log(x))
    return z  
