#!../../bin/linux-x86_64/psis

#- You may have to change psis to something else
#- everywhere it appears in this file

< envPaths

cd "${TOP}"

## Register all support components
dbLoadDatabase "dbd/psis.dbd"
psis_registerRecordDeviceDriver pdbbase

drvAsynIPPortConfigure("PSIS", "192.168.1.101:502", 0, 0, 1)
modbusInterposeConfig ("PSIS", 0, 5000, 0)

## Load record instances
#drvModbusAsynConfigure("PSISBI", "PSIS",  0,  4,   30515,   2,  0, 100, "Modicon")
drvModbusAsynConfigure("PSISBI", "PSIS",  0,  4,   514,   2,  0, 100, "Modicon")

##PSIS
dbLoadTemplate("db/psisPVs.sub")

cd "${TOP}/iocBoot/${IOC}"
iocInit

## Start any sequence programs
#seq sncxxx,"user=ctrluser"
