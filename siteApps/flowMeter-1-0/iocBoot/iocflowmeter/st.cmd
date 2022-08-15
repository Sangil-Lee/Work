#!../../bin/linux-x86_64/flowmeter

#- You may have to change flowmeter to something else
#- everywhere it appears in this file

< envPaths

cd "${TOP}"

## Register all support components
dbLoadDatabase "dbd/flowmeter.dbd"
flowmeter_registerRecordDeviceDriver pdbbase

drvAsynIPPortConfigure("NDPSFlowMeter", "192.168.1.250:502", 0, 0, 1)
modbusInterposeConfig("NDPSFlowMeter",0,5000,0)


## to 0   up 250 bytes(125 word), data block
drvModbusAsynConfigure("NDPSAI",  "NDPSFlowMeter", 0, 3,  1002,   2, 7, 100, "Modicon")  
drvModbusAsynConfigure("NDPSAI1",  "NDPSFlowMeter", 0, 3,  2002,   2, 7, 100, "Modicon")  
drvModbusAsynConfigure("NDPSAI2",  "NDPSFlowMeter", 0, 3,  3002,   2, 7, 100, "Modicon")  
drvModbusAsynConfigure("NDPSAI3",  "NDPSFlowMeter", 0, 3,  4002,   2, 7, 100, "Modicon")  
drvModbusAsynConfigure("NDPSAI4",  "NDPSFlowMeter", 0, 3,  5002,   2, 7, 100, "Modicon")  
drvModbusAsynConfigure("NDPSAI5",  "NDPSFlowMeter", 0, 3,  6002,   2, 7, 100, "Modicon")  
drvModbusAsynConfigure("NDPSAI6",  "NDPSFlowMeter", 0, 3,  7002,   2, 7, 100, "Modicon")  
drvModbusAsynConfigure("NDPSAI7",  "NDPSFlowMeter", 0, 3,  8002,   2, 7, 100, "Modicon")  

## Real Template
dbLoadTemplate("db/flowMeter.sub")


###Test DB
##dbLoadRecords("db/DataFanout.vdb","PREFIX=NDPS-PS:Flow-CH01")
##dbLoadRecords("db/DataFanout.vdb","PREFIX=NDPS-PS:Flow-CH02")

cd "${TOP}/iocBoot/${IOC}"
iocInit

## Start any sequence programs
#seq sncxxx,"user=ctrluser"
