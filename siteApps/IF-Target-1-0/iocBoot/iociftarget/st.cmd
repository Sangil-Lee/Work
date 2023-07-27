#!../../bin/linux-x86_64/iftarget

#- You may have to change iftarget to something else
#- everywhere it appears in this file

< envPaths

cd "${TOP}"

## Register all support components
dbLoadDatabase "dbd/iftarget.dbd"
iftarget_registerRecordDeviceDriver pdbbase

###Phytron phymotion controller
drvAsynIPPortConfigure("IFPhytron", "192.168.1.101:22222", 0, 0, 1)

###phytronCreateController (phytronPort, asynPort, movingPollPeriod, idlePollPeriod, timeout)
phytronCreateController("IFPhy", "IFPhytron", 100, 100, 1000)

###phytronCreateAxis(phytronPort, module, axis)
phytronCreateAxis("IFPhy", 1, 1)
phytronCreateAxis("IFPhy", 1, 2)

###################################
#Phytron Motor DB
dbLoadTemplate("db/phytronMotor.sub")
###################################

#Siemens CCS PLC 
#drvAsynIPPortConfigure("IFPLC", "192.168.1.11:502", 0, 0, 1)
#modbusInterposeConfig("IFPLC",   0,5000,0)

#drvModbusAsynConfigure("IFCCSAI", "IFPLC", 0, 3,  0,   118,  7, 100, "Modicon")  
#drvModbusAsynConfigure("IFCCSAO", "IFPLC", 0, 6,  120, 120, 7, 100, "Modicon")
#drvModbusAsynConfigure("IFCCSDI", "IFPLC", 0, 3,  240, 5,  7, 100, "Modicon")
#drvModbusAsynConfigure("IFCCSDO", "IFPLC", 0, 6,  246, 10,  7, 100, "Modicon")
#dbLoadTemplate("db/IFTarget.sub")


###ModbusTCP to FlowMeter
##drvAsynIPPortConfigure("IFFlowMeter", "192.168.1.250:502", 0, 0, 1)
##modbusInterposeConfig("IFFlowMeter",0,5000,0)

## to 0   up 250 bytes(125 word), data block
##drvModbusAsynConfigure("IFAI",   "IFFlowMeter", 0, 3,  1002,   2, 7, 100, "Modicon")  
##drvModbusAsynConfigure("IFAI1",  "IFFlowMeter", 0, 3,  2002,   2, 7, 100, "Modicon")  
##drvModbusAsynConfigure("IFAI2",  "IFFlowMeter", 0, 3,  3002,   2, 7, 100, "Modicon")  
##drvModbusAsynConfigure("IFAI3",  "IFFlowMeter", 0, 3,  4002,   2, 7, 100, "Modicon")  
##drvModbusAsynConfigure("IFAI4",  "IFFlowMeter", 0, 3,  5002,   2, 7, 100, "Modicon")  
##drvModbusAsynConfigure("IFAI5",  "IFFlowMeter", 0, 3,  6002,   2, 7, 100, "Modicon")  
##drvModbusAsynConfigure("IFAI6",  "IFFlowMeter", 0, 3,  7002,   2, 7, 100, "Modicon")  
##drvModbusAsynConfigure("IFAI7",  "IFFlowMeter", 0, 3,  8002,   2, 7, 100, "Modicon")  

## Real Template
##dbLoadTemplate("db/flowMeter.sub")

## Load record instances
#dbLoadRecords("db/iftarget.db","user=ctrluser")

cd "${TOP}/iocBoot/${IOC}"
iocInit

## Start any sequence programs
#seq sncxxx,"user=ctrluser"
