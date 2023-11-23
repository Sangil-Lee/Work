#!../../bin/linux-aarch64/kslitmotor

#- You may have to change kslitmotor to something else
#- everywhere it appears in this file

< envPaths

cd "${TOP}"

## Register all support components
dbLoadDatabase "dbd/kslitmotor.dbd"
kslitmotor_registerRecordDeviceDriver pdbbase

## Load record instances
#dbLoadRecords("db/kslitmotor.db","user=dietpi")

#LS Step Motor Controller
drvAsynIPPortConfigure("SLITPLC", "192.168.0.2:502", 0, 0, 1)
modbusInterposeConfig("SLITPLC",   0,5000,0)

drvModbusAsynConfigure("KoBRAAI", "IFPLC", 0, 6,  0,   32,  7, 100, "Modicon")
drvModbusAsynConfigure("KoBRAAO", "IFPLC", 0, 6,  128, 122, 7, 100, "Modicon")
drvModbusAsynConfigure("KoBRADI", "IFPLC", 0, 3,  256, 4,   7, 100, "Modicon")
drvModbusAsynConfigure("KoBRADO", "IFPLC", 0, 6,  260, 4,   7, 100, "Modicon")
dbLoadTemplate("db/KoBRASlit.sub")

cd "${TOP}/iocBoot/${IOC}"
iocInit

## Start any sequence programs
#seq sncxxx,"user=dietpi"
