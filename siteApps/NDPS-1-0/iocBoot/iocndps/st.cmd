#!../../bin/linux-x86_64/ndps

#- You may have to change ndps to something else
#- everywhere it appears in this file

< envPaths

cd "${TOP}"

## Register all support components
dbLoadDatabase "dbd/ndps.dbd"
ndps_registerRecordDeviceDriver pdbbase

###Phytron phymotion controller
drvAsynIPPortConfigure("NDPSPhytron", "192.168.1.101:22222", 0, 0, 1)

###phytronCreateController (phytronPort, asynPort, movingPollPeriod, idlePollPeriod, timeout)
phytronCreateController("PhyMotion_1", "NDPSPhytron", 100, 100, 1000)

###phytronCreateAxis(phytronPort, module, axis)
phytronCreateAxis("PhyMotion_1",  1, 1)
phytronCreateAxis("PhyMotion_1", 1, 2)
#phytronCreateAxis("PhyMotion_1", 1, 3)
phytronCreateAxis("PhyMotion_1", 1, 4)

###################################
#Phytron Motor DB
dbLoadTemplate("db/phytronMotor.sub")

###################################

###################################
#Siemens PLC 
#drvAsynIPPortConfigure("NDPS_WSPLC", "192.168.0.1:502", 0, 0, 1)
#modbusInterposeConfig("NDPS_WSPLC", 0,5000,0)

## Wire Scanner PLC(3)
#drvModbusAsynConfigure("NDPSWSAI", "NDPS_WSPLC", 0, 3,  0,   8,  7, 100, "Modicon")  
#drvModbusAsynConfigure("NDPSWSAO", "NDPS_WSPLC", 0, 6,  40,  20, 7, 100, "Modicon")
#drvModbusAsynConfigure("NDPSWSDI", "NDPS_WSPLC", 0, 3,  110, 2,  7, 100, "Modicon")
#drvModbusAsynConfigure("NDPSWSDO", "NDPS_WSPLC", 0, 6,  112, 2,  7, 100, "Modicon")
#dbLoadTemplate("db/NDPS_WS.sub")

#Siemens CCS PLC 
drvAsynIPPortConfigure("NDPS_CCSPLC", "192.168.1.11:502", 0, 0, 1)
modbusInterposeConfig("NDPS_CCSPLC",   0,5000,0)
drvModbusAsynConfigure("NDPSCCSAI", "NDPS_CCSPLC", 0, 3,  0,   118,  7, 100, "Modicon")  
drvModbusAsynConfigure("NDPSCCSAO", "NDPS_CCSPLC", 0, 6,  120, 120, 7, 100, "Modicon")
drvModbusAsynConfigure("NDPSCCSDI", "NDPS_CCSPLC", 0, 3,  240, 5,  7, 100, "Modicon")
drvModbusAsynConfigure("NDPSCCSDO", "NDPS_CCSPLC", 0, 6,  246, 10,  7, 100, "Modicon")
dbLoadTemplate("db/NDPS_CCS2_New.sub")

#Siemens Vacuum PLC 
drvAsynIPPortConfigure("NDPS_VACPLC", "192.168.1.1:502", 0, 0, 1)
modbusInterposeConfig("NDPS_VACPLC",   0,5000,0)
drvModbusAsynConfigure("NDPSVACAI", "NDPS_VACPLC", 0, 3,  0,   98,  7, 100, "Modicon")  
drvModbusAsynConfigure("NDPSVACAO", "NDPS_VACPLC", 0, 6,  100, 98, 7, 100, "Modicon")
drvModbusAsynConfigure("NDPSVACDI", "NDPS_VACPLC", 0, 3,  200, 15,  7, 100, "Modicon")
drvModbusAsynConfigure("NDPSVACDO", "NDPS_VACPLC", 0, 6,  216, 15,  7, 100, "Modicon")
dbLoadTemplate("db/NDPS_VAC2_New.sub")

###################################


###ModbusTCP to FlowMeter
##drvAsynIPPortConfigure("NDPSFlowMeter", "192.168.1.250:502", 0, 0, 1)
##modbusInterposeConfig("NDPSFlowMeter",0,5000,0)

## to 0   up 250 bytes(125 word), data block
##drvModbusAsynConfigure("NDPSAI",  "NDPSFlowMeter", 0, 3,  1002,   2, 7, 100, "Modicon")  
##drvModbusAsynConfigure("NDPSAI1",  "NDPSFlowMeter", 0, 3,  2002,   2, 7, 100, "Modicon")  
##drvModbusAsynConfigure("NDPSAI2",  "NDPSFlowMeter", 0, 3,  3002,   2, 7, 100, "Modicon")  
##drvModbusAsynConfigure("NDPSAI3",  "NDPSFlowMeter", 0, 3,  4002,   2, 7, 100, "Modicon")  
##drvModbusAsynConfigure("NDPSAI4",  "NDPSFlowMeter", 0, 3,  5002,   2, 7, 100, "Modicon")  
##drvModbusAsynConfigure("NDPSAI5",  "NDPSFlowMeter", 0, 3,  6002,   2, 7, 100, "Modicon")  
##drvModbusAsynConfigure("NDPSAI6",  "NDPSFlowMeter", 0, 3,  7002,   2, 7, 100, "Modicon")  
##drvModbusAsynConfigure("NDPSAI7",  "NDPSFlowMeter", 0, 3,  8002,   2, 7, 100, "Modicon")  

## Real Template
##dbLoadTemplate("db/flowMeter.sub")

cd "${TOP}/iocBoot/${IOC}"
iocInit

## Start any sequence programs
#seq sncxxx,"user=ctrluser"
