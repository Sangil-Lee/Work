from scipy import integrate

a = 0
b = 1

def y(x):
    return x**2

I = integrate.quad(y, a, b)

print(I)