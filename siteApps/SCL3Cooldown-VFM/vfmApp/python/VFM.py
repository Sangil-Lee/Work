# -*- coding: utf-8 -*-
"""
Created on Thu Jul 29 07:22:45 2021

@author: ctrluser
"""
import sympy as sym
import math
X = sym.Symbol('X')
E1 = 8
#D1 = 6
D1 = 2
F1 = 5
E2 = 3
D2 = 2
#D2 = 2
F2 = 5
H = 2
I = 7
#C = 40
C = 5
K = 2
L = 4
N = 4
M = 8
J = 7


#E1,D1,F1 = 8,27,5
#E2,D2,F2 = 3,2,5
#H,I,C,K,L,N,M,J = 2,7,5,2,4,5,8,7

fx = sym.Eq(E1*(X-D1)**0.5*F1**0.5+E2*(X-D2)**0.5*F2**0.5-H*(I-C+X*K/L*(N/M)**0.25)**0.5*J**0.5, 0)

result = sym.solve(fx, X, check=False)
print('VFM Result(B) = ', result)

gx = E1*(X-D1)**0.5*F1**0.5+E2*(X-D2)**0.5*F2**0.5-H*(I-C+X*K/L*(N/M)**0.25)**0.5*J**0.5
print('Verification(fx) = ', round(gx.subs(X, result[0]), 6) )
