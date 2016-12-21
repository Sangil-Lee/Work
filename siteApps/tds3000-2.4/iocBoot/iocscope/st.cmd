#!../../bin/linux-x86_64/scope

< envPaths
< envParams

#
# Site-specific configuration
#
epicsEnvSet(FAMILY, "$(TDS_FAMILY=3000)")   # Choose scope family (3000 or 5000)
epicsEnvSet(ETHER, "$(TDS_ETHERNET=10.1.5.51)") # Choose scope ethernet address
epicsEnvSet(SCOPE, "$(TDS_SCOPE=scopeTest:)") # Choose scope ethernet address

#
# Ensure that we can transfer a complete waveform
#
epicsEnvSet(EPICS_CA_MAX_ARRAY_BYTES, 1000000)

## Register all support components
cd ${TOP}
dbLoadDatabase("../../dbd/scope.dbd")
scope_registerRecordDeviceDriver(pdbbase)

## Load diagnostic record instance
dbLoadRecords("../../db/asynRecord.db","P=$(SCOPE),R=,PORT=L0,ADDR=3,OMAX=0,IMAX=0")

## Load oscilloscope db's if using Auto Print/Re-arm trig SNL program.
dbLoadRecords("../../db/scopeMessage.vdb","scope=$(SCOPE),L=0,A=3,ASG=RF1CTL,rfsys=1")
#dbLoadRecords("../../db/scopeMessage.vdb","scope=$(SCOPE),L=0,A=3,rfsys=1")

## This is the main db. 
dbLoadRecords("../../db/TDSxxxx.vdb","scope=$(SCOPE),L=0,A=3,ASG=RF1CTL,Name=RF1-HV,FAMILY=$(FAMILY)")
#dbLoadRecords("../../db/TDSxxxx.vdb","scope=$(SCOPE),L=0,A=3,Name=RF1-HV,FAMILY=$(FAMILY)")
#dbLoadRecords("../../db/TDS3000.vdb","scope=$(SCOPE),L=0,A=3,ASG=RF1CTL,Name=RF1-HV,FAMILY=$(FAMILY)")

# reboot scopes (Ethernet only)
#TDS3000Reboot 164.54.xxx.xxx
#epicsThreadSleep(60)

## Set up IOC/hardware links -- GPIB/LAN connection
#vxi11Configure("L0", "$(ETHER)", 0, 0.0, "hpib", 0)

## Set up IOC/hardware links -- Direct network connection
vxi11Configure("L0", "$(ETHER)", 0, 0.0, "inst0", 0)

## Set up IOC/hardware links -- Remote serial port
##  (link, host, priority, noAutoConnect, noEosProcessing)
#drvAsynIPPortConfigure("L0", "$(ETHER)", 0, 0, 0)

#asynOctetSetInputEos("L0",0,"\n")
#asynOctetSetOutputEos("L0",0,"\n")

## Set up IOC/hardware links -- Local serial port
##  (link, ttyName, priority, noAutoConnect, noEosProcessing)
#drvAsynSerialPortConfigure("L0", "/dev/ttyS0", 0, 0, 0)
#asynOctetSetInputEos("L0",0,"\n")
#asynOctetSetOutputEos("L0",0,"\n")
#asynSetOption("L0", 0, "baud", "38400")
#asynSetOption("L0", 0, "bits", "8")
#asynSetOption("L0", 0, "parity", "none")
#asynSetOption("L0", 0, "stop", "1")
#asynSetOption("L0", 0, "clocal", "N")
#asynSetOption("L0", 0, "crtscts", "Y")

## Turn on all diagnostic messages to stdout
#asynSetTraceMask("L0", -1, 0x9)
#asynSetTraceIOMask("L0", -1, 0x2)
#asynSetTraceIOTruncateSize("L0", -1, 120)

## Start IOC
cd ${TOP}/iocBoot/${IOC}
iocInit()

## Start any sequence programs
#seq scopeAutoArmPrint,"scope=$(SCOPE)"
