"""
Created on Sat Mar 11 13:10:43 2023
@author: silee
"""

import sys
import numpy as np
from epics import PV

init_opt = len(sys.argv)

Signal = 40 #Source Signal

Sigmas = [2.5, 5, 10, 15, 20]   #1 Sigma(%)
Hyst  = [5, 10, 20, 40, 60, 5, 10, 20, 40, 60] #Alarm Hysterisis Rate(%)
AFTC  = [0.0, 0.0, 0.0, 0.0, 0.0, 0.5, 1.0, 1.5, 2.0, 2.5] #AFTC Constant

if init_opt == 2:
    dbdval = float(sys.argv[1])
    Hyst = [dbdval]*10 #Deadband initialization

HYSTAFTCMat = np.zeros((len(Hyst), len(Sigmas))) #Deadband Matrix for ADEL, MDEL

preName = "ctrlslab-mdelsmooSim:"
postName = ":NoiseAI0AFilter"

Sigmas = np.array(Sigmas)/100 # 1Sigma %
Hyst = np.array(Hyst)/100 #Deadband(%) for Noise Value


## Setting Deadband(ADEL, MDEL) for 50 PVs
for col, sigma in enumerate(Sigmas):
    for row, hyst in enumerate(Hyst):
        if row < 5 :
            HYSTAFTCMat[row][col] = round(Signal*sigma*hyst, 6)
            pvname = preName+str(col+1)+postName+str(row+1)
            hyst_pv = PV(pvname+".HYST")
            hyst_pv.put(HYSTAFTCMat[row][col], wait=True)
            print(col, row, pvname+".HYST", HYSTAFTCMat[row][col])
        else :
            HYSTAFTCMat[row][col] = round(Signal*sigma*hyst, 6)
            pvname = preName+str(col+1)+postName+str(row+1)
            hyst_pv = PV(pvname+".HYST")
            aftc_pv = PV(pvname+".AFTC")
            hyst_pv.put(HYSTAFTCMat[row][col], wait=True)
            aftc_pv.put(AFTC[row], wait=True)
            print(col, row, pvname+".HYST", HYSTAFTCMat[row][col], pvname+".AFTC", AFTC[row])
        
print(HYSTAFTCMat)
