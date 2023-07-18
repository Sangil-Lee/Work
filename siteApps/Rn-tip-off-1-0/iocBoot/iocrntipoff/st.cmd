#!../../bin/linux-x86_64/rntipoff

#- You may have to change rntipoff to something else
#- everywhere it appears in this file

< envPaths

cd "${TOP}"

## Register all support components
dbLoadDatabase "dbd/rntipoff.dbd"
rntipoff_registerRecordDeviceDriver pdbbase

#Siemens 1200/1500 PLC -> Default Port:502
#drvAsynIPPortConfigure("RnTipOff", "192.168.0.3:502", 0, 0, 1)
#modbusInterposeConfig("RnTipOff",0,5000,0)

#Read Holding Register Address(0-699), Modbus F.C: 3)
###Start Address(0Bit, 173(32bit, 4bytes, 2words))############################
#drvModbusAsynConfigure("AIn", "RnTipOff", 0,  3,    0, 175,  0, 100, "Modicon")

#Write Single Holding Register Address(700Bit, 175(32bit, 4bytes, 2words)), Modbus F.C: 15,16)
###Start Address(700Bit, 175(32bit, 4bytes, 2words))###########################
#drvModbusAsynConfigure("AOut","RnTipOff", 0, 16,  175, 325,  0, 100, "Modicon")


#Read Holding Register BI_Word (DI,	Address(0-699), Modbus F.C: 3)
#drvModbusAsynConfigure("DIn","RnTipOff",  0, 3,  500, 2,  0, 100, "Modicon")

#Write BO_Word (DO, Address(0~23), Modbus F.C: 6)
#drvModbusAsynConfigure("DOut","RnTipOff", 0, 6,  514, 2,  0, 100, "Modicon")


## Load record instances
#dbLoadRecords("db/rntipoff.db", "user=ctrluser")
dbLoadTemplate("db/rntipoff.sub")

cd "${TOP}/iocBoot/${IOC}"
iocInit

## Start any sequence programs
#seq sncxxx,"user=ctrluser"
