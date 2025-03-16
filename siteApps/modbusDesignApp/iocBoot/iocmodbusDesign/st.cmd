#!../../bin/linux-x86_64/modbusDesign

#- You may have to change modbusDesign to something else
#- everywhere it appears in this file

< envPaths

cd "${TOP}"

## Register all support components
dbLoadDatabase "dbd/modbusDesign.dbd"
modbusDesign_registerRecordDeviceDriver pdbbase
drvAsynIPPortConfigure("MBDESIGN", "127.0.0.1:502", 0, 0, 1)
modbusInterposeConfig("MBDESIGN",0,5000,0)

drvModbusAsynConfigure("LONGIN", "MBDESIGN", 1, 3,  0,   10,  7, 100, "Modicon")  

## Load record instances
#dbLoadRecords("db/modbusDesign.db","user=ctrluser")
dbLoadTemplate("db/longinDesign.sub")

cd "${TOP}/iocBoot/${IOC}"
iocInit

## Start any sequence programs
#seq sncxxx,"user=ctrluser"
