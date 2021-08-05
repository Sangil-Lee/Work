# -*- coding: utf-8 -*-
"""
Created on Thu Jul 29 07:22:45 2021

@author: ctrluser
"""
import time
import sys
sys.path.append('/usr/local/lib/python3.7/dist-packages')
import sympy as sym
from epics import caget, caput
"""
X = sym.Symbol('X')
E1 = 3
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
gx = E1*(X-D1)**0.5*F1**0.5+E2*(X-D2)**0.5*F2**0.5-H*(I-C+X*K/L*(N/M)**0.25)**0.5*J**0.5
"""
paramlist = ['SCL31-CDL01:EBx01-WF7001:Param', 'SCL31-CDL01:VBx02-WF7001:Param', 'SCL31-CDL01:VBx04-WF7001:Param',
'SCL31-CDL01:VBx06-WF7001:Param', 'SCL31-CDL01:VBx08-WF7001:Param', 'SCL31-CDL01:VBx10-WF7001:Param',
'SCL31-CDL01:VBx12-WF7001:Param', 'SCL31-CDL01:VBx14-WF7001:Param', 'SCL31-CDL01:VBx16-WF7001:Param',
'SCL31-CDL01:VBx18-WF7001:Param', 'SCL31-CDL01:VBx20-WF7001:Param', 'SCL31-CDL01:VBx22-WF7001:Param']


paramlist_2= ['SCL32-CDL02:VBx01-WF7001:Param', 'SCL32-CDL02:VBx02-WF7001:Param', 'SCL32-CDL02:VBx03-WF7001:Param',
'SCL32-CDL02:VBx04-WF7001:Param', 'SCL32-CDL02:VBx05-WF7001:Param', 'SCL32-CDL02:VBx06-WF7001:Param',
'SCL32-CDL02:VBx07-WF7001:Param', 'SCL32-CDL02:VBx08-WF7001:Param', 'SCL32-CDL02:VBx09-WF7001:Param',
'SCL32-CDL02:VBx10-WF7001:Param', 'SCL32-CDL02:VBx11-WF7001:Param', 'SCL32-CDL02:VBx12-WF7001:Param',
'SCL32-CDL02:VBx13-WF7001:Param', 'SCL32-CDL03:VBx01-WF7001:Param', 'SCL32-CDL03:VBx02-WF7001:Param',
'SCL32-CDL03:VBx03-WF7001:Param', 'SCL32-CDL03:VBx04-WF7001:Param', 'SCL32-CDL03:VBx05-WF7001:Param',
'SCL32-CDL03:VBx06-WF7001:Param', 'SCL32-CDL03:VBx07-WF7001:Param', 'SCL32-CDL03:VBx08-WF7001:Param',
'SCL32-CDL03:VBx09-WF7001:Param', 'SCL32-CDL03:VBx10-WF7001:Param', 'SCL32-CDL03:VBx11-WF7001:Param',
'SCL32-CDL03:VBx12-WF7001:Param', 'SCL32-CDL03:VBx13-WF7001:Param', 'SCL32-CDL03:VBx14-WF7001:Param',
'SCL32-CDL03:VBx15-WF7001:Param', 'SCL32-CDL03:VBx16-WF7001:Param', 'SCL32-CDL03:VBx17-WF7001:Param',
'SCL32-CDL03:VBx18-WF7001:Param', 'SCL32-CDL03:VBx19-WF7001:Param', 'P2DT-CDL04:VBx01-WF7001:Param']

respv    = 'SCL31-CDL01:QWR-WFIn7001:Result'      # 12 QWR Result Value to Waveform
respv_2  = 'SCL32-CDL23:HWR-WFIn7001:Result'      # 33 QWR Result Value to Waveform

X = sym.Symbol('X')
resval = 0.0
def update():
    while True:
        resvallist = []
        resvallist_2 = []
        for idx, param in enumerate(paramlist):
            #if(idx == 1): break
            wfparam = caget(param)
            fx = sym.Eq(wfparam[0]*(X-wfparam[1])**0.5*wfparam[2]**0.5+wfparam[3]*(X-wfparam[4])**0.5*wfparam[5]**0.5-wfparam[6]*(wfparam[7]-wfparam[8]+X*wfparam[9]/wfparam[10]*(wfparam[11]/wfparam[12])**0.25)**0.5*wfparam[13]**0.5, 0)
            gx = wfparam[0]*(X-wfparam[1])**0.5*wfparam[2]**0.5+wfparam[3]*(X-wfparam[4])**0.5*wfparam[5]**0.5-wfparam[6]*(wfparam[7]-wfparam[8]+X*wfparam[9]/wfparam[10]*(wfparam[11]/wfparam[12])**0.25)**0.5*wfparam[13]**0.5
            try:
                result = sym.solve(fx, X, check=False)
                checkval = round(gx.subs(X, result[0]), 6)
                print('Verification(fx) = ', checkval )
                if(checkval == 0.0): resval = result[0]
                print('VFM Result(B) = ', resval)
                resvallist.append(resval)
            except: print('Solver Exception')
        for idx, param in enumerate(paramlist_2):
            #if(idx == 1): break
            wfparam = caget(param)
            fx = sym.Eq(wfparam[0]*(X-wfparam[1])**0.5*wfparam[2]**0.5-wfparam[3]*(wfparam[4]-wfparam[5]+X*wfparam[6]/wfparam[7]*(wfparam[8]/wfparam[9])**0.25)**0.5*wfparam[10]**0.5, 0)
            gx = wfparam[0]*(X-wfparam[1])**0.5*wfparam[2]**0.5-wfparam[3]*(wfparam[4]-wfparam[5]+X*wfparam[6]/wfparam[7]*(wfparam[8]/wfparam[9])**0.25)**0.5*wfparam[10]**0.5
            try:
                result = sym.solve(fx, X, check=False)
                checkval = round(gx.subs(X, result[0]), 6)
                print('Verification(fx) = ', checkval )
                if(checkval == 0.0): resval = result[0]
                print('VFM Result(B) = ', resval)
                resvallist_2.append(resval)
            except: print('Solver Exception')
        caput(respv, resvallist)
        caput(respv_2, resvallist_2)
        time.sleep(5)


if __name__ == '__main__':
    update()