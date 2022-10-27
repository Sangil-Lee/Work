#!../../bin/linux-x86_64/ndpsdiag

#- You may have to change ndpsdiag to something else
#- everywhere it appears in this file

< envPaths

cd "${TOP}"

## Register all support components
dbLoadDatabase "dbd/ndpsdiag.dbd"
ndpsdiag_registerRecordDeviceDriver pdbbase

#drvAsynIPPortConfigure("ndpsdiag", "192.168.0.1:502", 0, 0, 1)
#modbusInterposeConfig ("ndpsdiag", 0, 5000, 0)

#Analog, ndpsdiag001
#drvModbusAsynConfigure("NDPSDiagAI",   "ndpsdiag"   0,  3,   0,   10,  7, 100, "Modicon")
#drvModbusAsynConfigure("NDPSDiagAO",   "ndpsdiag",  0,  6,   50,  20,  7, 100, "Modicon")

#Bool, ndpsdiag001
#drvModbusAsynConfigure("NDPSDiagBI",   "ndpsdiag",  0,  3,   110,   2,  0, 100, "Modicon")
#drvModbusAsynConfigure("NDPSDiagBO",   "ndpsdiag",  0,  6,   112,   2,  0, 100, "Modicon")

##NDPS BeamDiag 
#dbLoadTemplate("db/ndpsdiag001.sub")

cd "${TOP}/iocBoot/${IOC}"
iocInit

## Start any sequence programs
#seq sncxxx,"user=ctrluser"
