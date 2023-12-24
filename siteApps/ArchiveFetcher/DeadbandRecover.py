"""
Created on Sat Mar 11 13:10:43 2023
@author: silee
"""

import sys
import numpy as np
from epics import PV

if len(sys.argv) != 2:
    print(sys.argv[0] + " deadband_initial_value")
    sys.exit()

dbdval = float(sys.argv[1])

Vs = 100 #Source Signal

Sigmas = [2.5, 5, 10, 15, 20]   #1 Sigma(%)
#Deadbands = [5, 10, 20, 40, 60, 80, 100, 110, 120, 140] #Deadband Rate(%)
#Deadbands = [0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0] #Deadband Rate(%)
Deadbands = [dbdval]*10 #Deadband Rate(%)
print(Deadbands)

#DeadbandMat = np.zeros((len(Sigmas), len(Deadbands)) #Deadband Matrix for ADEL, MDEL
DeadbandMat = np.zeros((len(Deadbands), len(Sigmas))) #Deadband Matrix for ADEL, MDEL
#print(DeadbandMat.shape)

preName = "ctrlslab-mdelsmooSim:"
postName = ":NoiseAI0Filter"

Sigmas = np.array(Sigmas)/100 # 1Sigma %
Deadbands = np.array(Deadbands)/100 #Deadband(%) for Noise Value


## Setting Deadband(ADEL, MDEL) for 50 PVs
for col, sigma in enumerate(Sigmas):
    for row, deadband in enumerate(Deadbands):
        DeadbandMat[row][col] = round(Vs*sigma*deadband, 6)
        pvname = preName+str(col+1)+postName+str(row+1)
        mdel_pv = PV(pvname+".MDEL")
        adel_pv = PV(pvname+".ADEL")
        mdel_pv.put(DeadbandMat[row][col], wait=True)
        adel_pv.put(DeadbandMat[row][col], wait=True)
        print(col, row, pvname+".MDEL", pvname+".ADEL", DeadbandMat[row][col])
        
print(DeadbandMat)
