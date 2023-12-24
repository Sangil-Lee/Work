"""
Created on Sat Mar 11 13:10:43 2023
@author: silee
"""

import sys
import numpy as np
from epics import PV

init_opt = len(sys.argv)

HIHI = 80. #Source Signal
HIGH = 60. #Source Signal
LOW  = 40. #Source Signal
LOLO = 20. #Source Signal


Sigmas = [2.5, 5, 10, 15, 20]   #1 Sigma(%)
preName = "ctrlslab-mdelsmooSim:"
postName = ":NoiseAI0AFilter"

sig_name = preName+'AddNoise.B'
pv_signal = PV(sig_name)

#print(pv_signal.get())
#sys.exit(0)

sig_val = pv_signal.get()
if sig_val >= 80:
    testmod = 1 
elif sig_val >= 60 and sig_val < 80:
    testmod = 2 
elif sig_val >= 40 and sig_val < 60:
    testmod = 3 
else:
    testmod = 4 
print(testmod)

if testmod == 1:
    hh_limit = HIHI
    hh_sever = 'MAJOR'
    h_limit = l_limit = ll_limit = 0
    h_sever = l_sever = ll_sever = 'NO_ALARM'
elif testmod == 2:
    h_limit = HIGH
    h_sever = 'MINOR'
    hh_limit = l_limit = ll_limit = 0
    hh_sever = l_sever = ll_sever = 'NO_ALARM'
elif testmod == 3:
    l_limit = LOW
    l_sever = 'MINOR'
    hh_limit = h_limit = ll_limit = 0
    hh_sever = h_sever = ll_sever = 'NO_ALARM'
else:
    ll_limit = LOLO
    ll_sever = 'MAJOR'
    hh_limit = h_limit = l_limit = 0
    hh_sever = h_sever = l_sever = 'NO_ALARM'

## Setting Deadband(ADEL, MDEL) for 50 PVs
for col, sigma in enumerate(Sigmas):
    for row in range(0,11):
        pvname = preName+str(col+1)+postName+str(row)
        print(pvname)

        pv_hh_limit = PV(pvname+".HIHI")
        pv_hh_sever = PV(pvname+".HHSV")

        pv_h_limit  = PV(pvname+".HIGH")
        pv_h_sever  = PV(pvname+".HSV")

        pv_l_limit  = PV(pvname+".LOW")
        pv_l_sever  = PV(pvname+".LSV")

        pv_ll_limit = PV(pvname+".LOLO")
        pv_ll_sever = PV(pvname+".LLSV")

        pv_hh_limit.put(hh_limit, wait=True)
        pv_hh_sever.put(hh_sever, wait=True)

        pv_h_limit.put(h_limit, wait=True)
        pv_h_sever.put(h_sever, wait=True)

        pv_l_limit.put(l_limit, wait=True)
        pv_l_sever.put(l_sever, wait=True)

        pv_ll_limit.put(ll_limit, wait=True)
        pv_ll_sever.put(ll_sever, wait=True)

        msg = ('HIHI:{hihi},HIGH:{high},LOW:{low},LOLO:{lolo}').format(hihi=hh_limit,high=h_limit, low=l_limit, lolo=ll_limit)
        print(msg)

