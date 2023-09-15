#!../../bin/linux-x86_64/rntipoff

#- You may have to change rntipoff to something else
#- everywhere it appears in this file

< envPaths

cd "${TOP}"

## Register all support components
dbLoadDatabase "dbd/rntipoff.dbd"
rntipoff_registerRecordDeviceDriver pdbbase

#Siemens 1200/1500 PLC -> Default Port:502
drvAsynIPPortConfigure("RnTipOff", "192.168.10.100:502", 0, 0, 1)
modbusInterposeConfig("RnTipOff",0,5000,0)

#Read Holding Register Address(0-699), Modbus F.C: 3)
###Start Address(0Bit, 173(32bit, 4bytes, 2words))############################
#Read Holding Register BI_Word (DI,	Address(0-699), Modbus F.C: 3)

drvModbusAsynConfigure("DIn","RnTipOff",  0, 3,  0, 10,  0, 100, "Modicon")

#Write BO_Word (DO, Address(0~23), Modbus F.C: 6)
#drvModbusAsynConfigure("DOut","RnTipOff", 0, 6,  0, 10,  0, 100, "Modicon")
drvModbusAsynConfigure("DOut","RnTipOff", 0, 6,  100, 10,  0, 100, "Modicon")


## Load record instances
dbLoadTemplate("db/rntipoff.sub")

cd "${TOP}/iocBoot/${IOC}"
iocInit

## Start any sequence programs
#seq sncxxx,"user=ctrluser"
