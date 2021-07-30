# -*- coding: utf-8 -*-
"""
Created on Thu Jul 29 07:22:45 2021

@author: ctrluser
"""
from softioc import softioc, builder
import cothread
import sympy as sym
from cothread.catools import caget, caput, camonitor

builder.SetDeviceName('MY-DEVICE-PREFIX')


# Create some records
ai = builder.aIn('AI', initial_value=5)
E1Val = builder.aIn('E1Val', initial_value=8)
ao = builder.aOut('AO', initial_value=12.45, always_update=True,
                  on_update=lambda v: ai.set(v))
E1Out = builder.aOut('E1Out', initial_value=8, always_update=True,
                  on_update=lambda v: E1Val.set(v))
vfm = builder.aIn('VFM', initial_value=0.0)
#vfm = builder.aOut('VFM', initial_value=0.0, always_update=True, on_update=lambda v: vfm.set(v))

X = sym.Symbol('X')
E1 = E1Val.get()
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

fx = sym.Eq(E1*(X-D1)**0.5*F1**0.5+E2*(X-D2)**0.5*F2**0.5-H*(I-C+X*K/L*(N/M)**0.25)**0.5*J**0.5, 0)

resval = 0.0
try:
    result = sym.solve(fx, X, check=False)
    gx = E1*(X-D1)**0.5*F1**0.5+E2*(X-D2)**0.5*F2**0.5-H*(I-C+X*K/L*(N/M)**0.25)**0.5*J**0.5
    checkval = round(gx.subs(X, result[0]), 6)
    print('Verification(fx) = ', checkval )
    if(checkval == 0.0): resval = result[0]
    print('VFM Result(B) = ', resval)
except: print('Solver Exception')


# Boilerplate get the IOC started
builder.LoadDatabase()
softioc.iocInit()

# Start processes required to be run after iocInit
def update():
    while True:
        ai.set(ai.get() + 1)
        vfm.set(float(resval))
        cothread.Sleep(1)

cothread.Spawn(update)

# Finally leave the IOC running with an interactive shell.
softioc.interactive_ioc(globals())