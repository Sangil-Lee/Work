"""
Created on Sat Mar 11 13:10:43 2023
@author: silee
"""

import sys
import numpy as np
from epics import PV

prefix = "System:"

## Setting Deadband(ADEL, MDEL) for 50 PVs
for idx in range(0,11):
    pvname = prefix+str(idx)
    ##print (pvname)

    hihi = PV(pvname+".HIHI")
    high = PV(pvname+".HIGH")
    low  = PV(pvname+".LOW")
    lolo = PV(pvname+".LOLO")
    hihi.put(65, wait=True)
    high.put(55, wait=True)
    low.put(-10, wait=True)
    lolo.put(-20, wait=True)
        
print ("Successful Alarm Limit Settings...")
