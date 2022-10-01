#!../../bin/windows-x64/pxieioc

#- You may have to change pxieioc to something else
#- everywhere it appears in this file

< envPaths

cd "${TOP}"

## Register all support components
dbLoadDatabase "dbd/pxieioc.dbd"
pxieioc_registerRecordDeviceDriver pdbbase

#### Tested RTP-Protocol########
#drvSyncRTPConfigure("RTPDevice", "192.168.1.50:50199 TCP", 0, 0)
#dbLoadTemplate("db/cdmsTester_RTP.sub");

##ModbusTCP
#drvAsynIPPortConfigure("CDMS_RTP", "192.168.1.100:502", 0, 0, 1)
#modbusInterposeConfig("CDMS_RTP",0,5000,0)

## to 0   up 250 bytes(125 word), data block
###Holding Register
#drvModbusAsynConfigure("RTPAI", "CDMS_RTP", 0, 3,  100,  20, 7, 100, "Modicon")  

###Read Coil, (FCode = 01)
#drvModbusAsynConfigure("RTPDI", "CDMS_RTP", 0, 1,  100,  4, 7, 100, "Modicon")

###Write Coil, (FCode = 05)
#drvModbusAsynConfigure("RTPDO", "CDMS_RTP", 0, 5,  100,  4, 7, 100, "Modicon")

###Input Register(FCode = 04, CPU, Power Supply, AI, DI, DO)
#drvModbusAsynConfigure("RTPAI_CPU", "CDMS_RTP", 0, 4,  200,  10, 7, 100, "Modicon")  
#drvModbusAsynConfigure("RTPAI_PS", "CDMS_RTP",  0, 4,  216,  10, 7, 100, "Modicon")  
#drvModbusAsynConfigure("RTPAI_AI", "CDMS_RTP",  0, 4,  232,  10, 7, 100, "Modicon")  


#dbLoadTemplate("db/rtp_modbus.sub");

nidaqDIConfigure("nidaqmx", "PXI2Slot3")
nidaqDOCreateChannel("PXI2Slot3", "port4")
nidaqDOCreateChannel("PXI2Slot3", "port5")
nidaqDOCreateChannel("PXI2Slot3", "port6")
nidaqDOCreateChannel("PXI2Slot3", "port7")
nidaqDICreateChannel("PXI2Slot3", "port0")
nidaqDICreateChannel("PXI2Slot3", "port1")
nidaqDICreateChannel("PXI2Slot3", "port2")
nidaqDICreateChannel("PXI2Slot3", "port3")

nidaqAOCreateChannel("PXI2Slot2", "ao0")
nidaqAOCreateChannel("PXI2Slot2", "ao1")
nidaqAOCreateChannel("PXI2Slot2", "ao2")
nidaqAOCreateChannel("PXI2Slot2", "ao3")
nidaqAOCreateChannel("PXI2Slot2", "ao4")
nidaqAOCreateChannel("PXI2Slot2", "ao5")
nidaqAOCreateChannel("PXI2Slot2", "ao6")
nidaqAOCreateChannel("PXI2Slot2", "ao7")

nidaqTaskStart()

## Load record instances
#dbLoadRecords("db/pxi_long.db")
#dbLoadRecords("db/pxi_longin_bitfanout.vdb", "P=CDMS-Tester:PXI-DI")
#dbLoadRecords("db/pxi_longout_bitfanout.vdb", "P=CDMS-Tester:PXI-DO")

##PXI6514 Digital Input/Output
dbLoadRecords("db/pxi_mbbi_bitfanout.vdb", "P=CDMS-Tester:PXI-DI")
###dbLoadRecords("db/pxi_mbbo_bitfanout.vdb", "P=CDMS-Tester:PXI-DO")

##PXI4322 Analog Output
dbLoadTemplate("db/pxi_ao.sub")

##RTP Digital Input/Output
dbLoadRecords("db/rtp_mbbi_bitfanout.vdb", "P=CDMS-Tester:RTP-DI")
dbLoadRecords("db/rtp_mbbo_bitfanout.vdb", "P=CDMS-Tester:RTP-DO")

dbLoadRecords("db/pxi_add.vdb")

cd "${TOP}/iocBoot/${IOC}"
iocInit

## Start any sequence programs
#seq sncxxx,"user=cdms"
