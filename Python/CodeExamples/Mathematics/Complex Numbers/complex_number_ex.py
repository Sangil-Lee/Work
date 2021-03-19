import cmath 

x = 2
y = -3

# converting x and y into complex number using complex()
z = complex(x,y)
print(z.real)
print(z.imag)

print(z.conjugate())

# converting complex number into polar using polar() 
w = cmath.polar(z) 
print (w) 


# converting complex number into rectangular using rect() 
w = cmath.rect(2,3) 
print (w) 