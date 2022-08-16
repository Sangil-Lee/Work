#!../../bin/linux-x86_64/tbxcontrol

#- You may have to change tbxcontrol to something else
#- everywhere it appears in this file

< envPaths

cd "${TOP}"

## Register all support components
dbLoadDatabase "dbd/tbxcontrol.dbd"
tbxcontrol_registerRecordDeviceDriver pdbbase

#Siemens PLC
drvAsynIPPortConfigure("TBX_PLC", "192.168.140.171:502", 0, 0, 1)
modbusInterposeConfig("TBX_PLC",0,5000,0)


## to 0   up 250 bytes(125 word), data block
drvModbusAsynConfigure("TBXAI",   "TBX_PLC", 0, 3,  0,   125, 7, 100, "Modicon")  
## to 251 up 500 bytes(125 word), data block
drvModbusAsynConfigure("TBXAI_1", "TBX_PLC", 0, 3,  126, 125, 7, 100, "Modicon")  
## to 252 up 750 bytes(125 word), data block
drvModbusAsynConfigure("TBXAI_2", "TBX_PLC", 0, 3,  252, 125, 7, 100, "Modicon")  


#Spare
#drvModbusAsynConfigure("SPARE", "TBX_PLC",   0, 3,  378,  20, 7, 100, "Modicon")  


drvModbusAsynConfigure("TBXAO",   "TBX_PLC", 0, 6,  400, 125, 7, 100, "Modicon")
drvModbusAsynConfigure("TBXAO_1", "TBX_PLC", 0, 6,  526, 25,  7, 100, "Modicon")
drvModbusAsynConfigure("TBXAO_2", "TBX_PLC", 0, 6,  608, 125,  7, 100, "Modicon")
drvModbusAsynConfigure("TBXAO_3", "TBX_PLC", 0, 6,  734, 125,  7, 100, "Modicon")
drvModbusAsynConfigure("TBXAO_4", "TBX_PLC", 0, 6,  860, 100,  7, 100, "Modicon")


#drvModbusAsynConfigure("TBXDI", "TBX_PLC", 0, 5,  550,  26, 7, 100, "Modicon")
drvModbusAsynConfigure("TBXDI", "TBX_PLC", 0, 3,  550,  26, 7, 100, "Modicon")
drvModbusAsynConfigure("TBXDO", "TBX_PLC", 0, 6,  576,  22, 7, 100, "Modicon")

## Load record instances
#dbLoadTemplate("db/TBxcontrol.sub")
#dbLoadTemplate("db/TBxcontrol_test.sub")

dbLoadTemplate("db/TBxcontrol_2.sub")

cd "${TOP}/iocBoot/${IOC}"
iocInit

## Start any sequence programs
#seq sncxxx,"user=ctrluser"
