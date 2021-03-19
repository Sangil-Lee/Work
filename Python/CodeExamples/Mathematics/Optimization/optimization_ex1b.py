from scipy import optimize

xmin = -5
xmax = 5


def y(x):
    return x**2 + 2*x + 1
 
    
x_min = optimize.fminbound(y, xmin, xmax)

print(x_min) 