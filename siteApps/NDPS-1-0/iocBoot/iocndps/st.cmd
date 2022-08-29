#!../../bin/linux-x86_64/ndps

#- You may have to change ndps to something else
#- everywhere it appears in this file

< envPaths

cd "${TOP}"

## Register all support components
dbLoadDatabase "dbd/ndps.dbd"
ndps_registerRecordDeviceDriver pdbbase

#Siemens PLC
drvAsynIPPortConfigure("NDPS_WSPLC", "192.168.0.1:502", 0, 0, 1)
modbusInterposeConfig("NDPS_WSPLC",	0,5000,0)


## to 0   up 250 bytes(125 word), data block
drvModbusAsynConfigure("NDPSWSAI", "NDPS_WSPLC", 0, 3,  0,   8,  7, 100, "Modicon")  
drvModbusAsynConfigure("NDPSWSAO", "NDPS_WSPLC", 0, 6,  40,  20, 7, 100, "Modicon")
drvModbusAsynConfigure("NDPSWSDI", "NDPS_WSPLC", 0, 3,  110, 2,  7, 100, "Modicon")
drvModbusAsynConfigure("NDPSWSDO", "NDPS_WSPLC", 0, 6,  112, 2,  7, 100, "Modicon")

dbLoadTemplate("db/NDPS_WS.sub")

#Siemens PLC
#drvAsynIPPortConfigure("NDPS_CCSPLC", "192.168.0.1:502", 0, 0, 1)
#modbusInterposeConfig("NDPS_CCSPLC",	0,5000,0)
#drvModbusAsynConfigure("NDPSCCSAI", "NDPS_CCSPLC", 0, 3,  0,   118,  7, 100, "Modicon")  
#drvModbusAsynConfigure("NDPSCCSAO", "NDPS_CCSPLC", 0, 6,  120, 120, 7, 100, "Modicon")
#drvModbusAsynConfigure("NDPSCCSDI", "NDPS_CCSPLC", 0, 3,  240, 5,  7, 100, "Modicon")
#drvModbusAsynConfigure("NDPSCCSDO", "NDPS_CCSPLC", 0, 6,  246, 10,  7, 100, "Modicon")
#dbLoadTemplate("db/NDPS_CCS.sub")

#drvAsynIPPortConfigure("NDPS_VACPLC", "192.168.0.1:502", 0, 0, 1)
#modbusInterposeConfig("NDPS_VACPLC",	0,5000,0)
#drvModbusAsynConfigure("NDPSVACAI", "NDPS_VACPLC", 0, 3,  0,   98,  7, 100, "Modicon")  
#drvModbusAsynConfigure("NDPSVACAO", "NDPS_VACPLC", 0, 6,  100, 98, 7, 100, "Modicon")
#drvModbusAsynConfigure("NDPSVACDI", "NDPS_VACPLC", 0, 3,  200, 15,  7, 100, "Modicon")
#drvModbusAsynConfigure("NDPSVACDO", "NDPS_VACPLC", 0, 6,  216, 15,  7, 100, "Modicon")
#dbLoadTemplate("db/NDPS_VAC.sub")

cd "${TOP}/iocBoot/${IOC}"
iocInit

## Start any sequence programs
#seq sncxxx,"user=ctrluser"
