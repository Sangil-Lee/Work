import numpy as np
import matplotlib.pyplot as plt
from scipy.integrate import *

def f(x) :
	return 3.0*x*x + 1.0

xlo = 2
xhi = 4
I, err=quad(f,xlo,xhi)
print("I =", I)
print("Err =", err)
