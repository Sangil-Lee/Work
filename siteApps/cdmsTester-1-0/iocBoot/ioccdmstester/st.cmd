#!../../bin/linux-x86_64/cdmstester

#- You may have to change cdmstester to something else
#- everywhere it appears in this file

< envPaths

cd "${TOP}"

## Register all support components
dbLoadDatabase "dbd/cdmstester.dbd"
cdmstester_registerRecordDeviceDriver pdbbase

#### Tested RTP-Protocol########
#drvSyncRTPConfigure("RTPDevice", "192.168.1.50:50199 TCP", 0, 0)
#dbLoadRecords("db/RTPOneTest.vdb")
#dbLoadRecords("db/RTPTest.vdb")

dbLoadTemplate("db/cdmsTester_RTP.sub");

##ModbusTCP
#drvAsynIPPortConfigure("CDMS_RTP", "192.168.1.61:502", 0, 0, 1)
#drvAsynIPPortConfigure("CDMS_RTP", "192.168.1.50:50199", 0, 0, 1)
#modbusInterposeConfig("CDMS_RTP",0,5000,0)

## to 0   up 250 bytes(125 word), data block
#drvModbusAsynConfigure("CDMSAI",   "CDMS_RTP", 0, 3,  0,   100, 7, 100, "Modicon")  
#drvModbusAsynConfigure("CDMSAI",   "CDMS_RTP", 0, 3,  40000,   100, 7, 100, "Modicon")  
#drvModbusAsynConfigure("CDMSAO",   "CDMS_RTP", 0, 6,  400, 125, 7, 100, "Modicon")
#drvModbusAsynConfigure("CDMSDI", "CDMS_RTP", 0, 3,  550,  26, 7, 100, "Modicon")
#drvModbusAsynConfigure("CDMSDO", "CDMS_RTP", 0, 6,  576,  22, 7, 100, "Modicon")
#dbLoadTemplate("db/cdmsTester_Modbus.sub");


##NI-DAQ, PXI Interface
#nidaqAsynEpicsConfigure("nidaqmx","${TOP}/iocBoot/${IOC}/NIDAQRegister.reg", "PXI1Slot5/ai0")

## Load record instances
#dbLoadRecords("db/subArray.db","SYS=SRF1,SUBSYS=QWR01,DEV=CM,SUBDEV=P30")
#dbLoadRecords("db/subArray.db","SYS=SRF2,SUBSYS=QWR01,DEV=CM,SUBDEV=P30")
#dbLoadTemplate("${TOP}/iocBoot/${IOC}/nidaqmx.sub")

##Soft Pvs
dbLoadRecords("db/CDMSTester.db", "PREFIX=CDMSTester")

cd "${TOP}/iocBoot/${IOC}"
iocInit

## Start any sequence programs
#seq sncxxx,"user=ctrluser"
