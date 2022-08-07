#!../../bin/linux-x86_64/cdmstester

#- You may have to change cdmstester to something else
#- everywhere it appears in this file

< envPaths

cd "${TOP}"

## Register all support components
dbLoadDatabase "dbd/cdmstester.dbd"
cdmstester_registerRecordDeviceDriver pdbbase

#drvSyncRTPConfigure("RTPDevice", "89.89.80.30:50199 TCP", 0, 0)

#dbLoadRecords("db/RTPOneTest.vdb")
#dbLoadRecords("db/RTPTest.vdb")

dbLoadRecords("db/CDMSTester.db", "PREFIX=CDMSTester")


##ModbusTCP
#drvAsynIPPortConfigure("CDMS_RTP", "89.89.80.230:502", 0, 0, 1)
#modbusInterposeConfig("CDMS_RTP",0,5000,0)

## to 0   up 250 bytes(125 word), data block
#drvModbusAsynConfigure("CDMSAI",   "CDMS_RTP", 0, 3,  0,   125, 7, 100, "Modicon")  
#drvModbusAsynConfigure("CDMSAO",   "CDMS_RTP", 0, 6,  400, 125, 7, 100, "Modicon")

#drvModbusAsynConfigure("CDMSDI", "CDMS_RTP", 0, 3,  550,  26, 7, 100, "Modicon")
#drvModbusAsynConfigure("CDMSDO", "CDMS_RTP", 0, 6,  576,  22, 7, 100, "Modicon")

#dbLoadTemplate("db/cdmsTester_Modbus.sub");

cd "${TOP}/iocBoot/${IOC}"
iocInit

## Start any sequence programs
#seq sncxxx,"user=ctrluser"
