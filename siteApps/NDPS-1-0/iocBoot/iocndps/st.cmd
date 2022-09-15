#!../../bin/linux-x86_64/ndps

#- You may have to change ndps to something else
#- everywhere it appears in this file

< envPaths

cd "${TOP}"

## Register all support components
dbLoadDatabase "dbd/ndps.dbd"
ndps_registerRecordDeviceDriver pdbbase

###Phytron phymotion controller
drvAsynIPPortConfigure("NDPSPhytron", "192.168.1.100:22222", 0, 0, 1)

###phytronCreateController (phytronPort, asynPort, movingPollPeriod, idlePollPeriod, timeout)
phytronCreateController("PhyMotion_1", "NDPSPhytron", 100, 100, 1000)

###phytronCreateAxis(phytronPort, module, axis)
phytronCreateAxis("PhyMotion_1", 1, 1)
phytronCreateAxis("PhyMotion_1", 2, 1)


###ModbusTCP to Siemens PLC
#drvAsynIPPortConfigure("NDPSSiemensPLC", "192.168.1.210:502", 0, 0, 1)
#modbusInterposeConfig("NDPSSiemensPLC",0,5000,0)
##drvModbusAsynConfigure("NDPSPLC_AI",  "NDPSSiemensPLC", 0, 3,  ??,   2, 7, 100, "Modicon")  
##drvModbusAsynConfigure("NDPSPLC_AI",  "NDPSSiemensPLC", 0, 6,  ??,   2, 7, 100, "Modicon")  



###ModbusTCP to FlowMeter
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

cd "${TOP}/iocBoot/${IOC}"
iocInit

## Start any sequence programs
#seq sncxxx,"user=ctrluser"
