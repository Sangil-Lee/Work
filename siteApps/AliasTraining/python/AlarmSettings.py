"""
Created on Sat Mar 11 13:10:43 2023
@author: silee
"""

import sys
import numpy as np
from epics import PV

pvlist = ["Test-User:Rack01-Ch01:Temp", "Test-User:Rack01-Ch03:Temp", "Test-User:Rack01-Ch04:Temp", "Test-User:Rack02-Ch05:Temp", "Exp-User:Rack02-Ch08:Temp",
    "Exp-User:Rack02-Ch00:Temp", "Exp-User:Rack03-Ch01:Temp", "Exp-User:Rack03-Ch04:Temp", "Exp-User:Rack03-Ch21:Temp", "Exp-User:Rack04-Ch33:Temp",
    "Exp-User:Rack04-Ch99:Temp"]

## Setting Deadband(ADEL, MDEL) for 50 PVs
for idx, pvname in enumerate(pvlist):
    hihi = PV(pvname+".HIHI")
    high = PV(pvname+".HIGH")
    low  = PV(pvname+".LOW")
    lolo = PV(pvname+".LOLO")
    hihi.put(95, wait=True)
    high.put(85, wait=True)
    low.put(20, wait=True)
    lolo.put(10, wait=True)
    
print("Successful Changed Alarm Setpoints...")
