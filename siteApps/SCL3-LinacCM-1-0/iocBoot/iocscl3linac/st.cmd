#!../../bin/linux-x86_64/scl3linac

#- You may have to change scl3linac to something else
#- everywhere it appears in this file

< envPaths
< SCLEnv

cd "${TOP}"

## Register all support components
dbLoadDatabase "dbd/scl3linac.dbd"
scl3linac_registerRecordDeviceDriver pdbbase

#Siemens 1200/1500 PLC -> Default Port:502
#drvAsynIPPortConfigure("SCL3", "192.168.0.3:502", 0, 0, 1)
#modbusInterposeConfig("SCL3",0,5000,0)

##Siemens ModbusTCP Address Map according to modbus function code.

#Read Holding Register Address(0-699), Modbus F.C: 5)
###Start Address(0Bit, 173(32bit, 4bytes, 2words))############################
#drvModbusAsynConfigure("SCL3DB_AIn", "SCL3", 0, 3,  0,  175,  0, 100, "Modicon")

#Write Single Holding Register Address(700Bit, 175(32bit, 4bytes, 2words)), Modbus F.C: 15,16)
###Start Address(700Bit, 175(32bit, 4bytes, 2words))############################
#drvModbusAsynConfigure("SCL3DB_AOut","SCL3", 0, 16,  175, 325,  0, 100, "Modicon")

#drvModbusAsynConfigure("SCL3DB_DIn","SCL3", 0, 3,  500, 14,  0, 100, "Modicon")

#Write Discreate Input(DI, 1-24, Address(0~23), Modbus F.C: 2)
#drvModbusAsynConfigure("SCL3DB_DOut","SCL3", 0, 6,  514, 11,  0, 100, "Modicon")

## Load record instances
#dbLoadRecords("db/xxx.db","user=ctrluser")
dbLoadTemplate("${TOP}/iocBoot/${IOC}/scl3_macro_test.sub", "SYS=${SYS},SUBSYS=${SUBSYS},DEV=${DEV1},SUBDEV=${SUBDEV1}")

dbLoadTemplate("${TOP}/iocBoot/${IOC}/scl3_macro_test.sub", "SYS=${SYS},SUBSYS=${SUBSYS},DEV=${DEV2},SUBDEV=${SUBDEV2}")


cd "${TOP}/iocBoot/${IOC}"
iocInit

## Start any sequence programs
#seq sncxxx,"user=ctrluser"
