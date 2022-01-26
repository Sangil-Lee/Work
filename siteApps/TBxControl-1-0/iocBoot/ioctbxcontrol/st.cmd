#!../../bin/linux-x86_64/tbxcontrol

#- You may have to change tbxcontrol to something else
#- everywhere it appears in this file

< envPaths

cd "${TOP}"

## Register all support components
dbLoadDatabase "dbd/tbxcontrol.dbd"
tbxcontrol_registerRecordDeviceDriver pdbbase

#Siemens PLC
#drvAsynIPPortConfigure("TBX_PLC", "192.168.100.11:502", 0, 0, 1)
#modbusInterposeConfig("TBX_PLC",0,5000,0)


#drvModbusAsynConfigure("TBXAI", "TBX_PLC", 0, 5,   0,   100, 7, 100, "Modicon")
#drvModbusAsynConfigure("TBXAO", "TBX_PLC", 0, 16,  400, 100, 7, 100, "Modicon")


#drvModbusAsynConfigure("TBXDI", "TBX_PLC", 0, 3,   550,  27, 7, 100, "Modicon")
#drvModbusAsynConfigure("TBXDO", "TBX_PLC", 0, 16,  576,  25, 7, 100, "Modicon")

## Load record instances
#dbLoadRecords("db/xxx.db","user=ctrluser")

dbLoadTemplate("db/TBxcontrol.sub")

cd "${TOP}/iocBoot/${IOC}"
iocInit

## Start any sequence programs
#seq sncxxx,"user=ctrluser"
