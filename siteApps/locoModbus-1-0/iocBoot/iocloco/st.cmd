#!../../bin/linux-x86_64/loco

#- You may have to change loco to something else
#- everywhere it appears in this file

< envPaths

cd "${TOP}"

## Register all support components
dbLoadDatabase "dbd/loco.dbd"
loco_registerRecordDeviceDriver pdbbase

#Siemens 1200/1500 PLC -> Default Port:502
#drvAsynIPPortConfigure("L1", "192.168.0.3:502", 0, 0, 1)

#Siemens Logo PLC -> Default Port:510
drvAsynIPPortConfigure("L1", "192.168.0.3:510", 0, 0, 1)
modbusInterposeConfig("L1",0,5000,0)

##Siemens ModbusTCP Address Map according to modbus function code.
#drvModbusAsynConfigure("MB_DIn",  	"L1", 0, 3,  0,		10,  0, 100, "Modicon")
#I, Read Discreate Input(DI, 1-24, Address(0~23), Modbus F.C: 2)
drvModbusAsynConfigure("MB_DIn",  	"L1", 0, 2,  0,		23,  0, 100, "Modicon")

#Q, Write Single Coil(Coil Address(8193-8212), Modbus F.C: 5)
drvModbusAsynConfigure("MB_DOut", 	"L1", 0, 5,  8192,  20,  0, 100, "Modicon")

##ModbusTCP DB
#dbLoadTemplate("${TOP}/iocBoot/${IOC}/modbustcp.sub")
dbLoadTemplate("${TOP}/iocBoot/${IOC}/bitfanmobus.sub","HOST=LOGO")
dbLoadRecords("db/StrBitOut.vdb", "HOST=LOGO")

cd "${TOP}/iocBoot/${IOC}"
iocInit

## Start any sequence programs
seq sncLEDSeq,"HOST=LOGO"
