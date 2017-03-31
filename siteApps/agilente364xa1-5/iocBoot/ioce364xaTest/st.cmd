#!../../bin/linux-x86_64/e364xaTest

< envPaths

epicsEnvSet(ETHER,"10.1.5.88:4003")
epicsEnvSet(P,"e364xachk:")
epicsEnvSet(R,"")

cd ${TOP}

## Register all support components
dbLoadDatabase("dbd/e364xaTest.dbd",0,0)
e364xaTest_registerRecordDeviceDriver(pdbbase)

## Set up IOC/hardware links -- Remote serial port
##  (link, host, priority, noAutoConnect, noEosProcessing)
drvAsynIPPortConfigure("L0", "$(ETHER)", 0, 0, 0)
asynOctetSetInputEos("L0",0,"\n\r")
asynOctetSetOutputEos("L0",0,"\n")

## Set up IOC/hardware links -- Local serial port
##  (link, ttyName, priority, noAutoConnect, noEosProcessing)
#drvAsynSerialPortConfigure("L0", "$(TTY)", 0, 0, 0)
#asynOctetSetInputEos("L0",0,"\n")
#asynOctetSetOutputEos("L0",0,"\n")
#asynSetOption("L0", 0, "baud", "9600")
#asynSetOption("L0", 0, "bits", "8")
#asynSetOption("L0", 0, "parity", "none")
#asynSetOption("L0", 0, "stop", "2")
#asynSetOption("L0", 0, "clocal", "Y")
#asynSetOption("L0", 0, "crtscts", "Y")

## Turn on all driver I/O messages
asynSetTraceMask("L0", -1, 0x9)
asynSetTraceIOMask("L0", -1, 0x2)

## Load record instances
dbLoadRecords("db/deve364xa.db","P=$(P),R=$(R),L=0,A=-1")
dbLoadRecords("db/asynRecord.db","P=$(P),R=asyn,PORT=L0,ADDR=-1,OMAX=80,IMAX=80")

cd ${TOP}/iocBoot/${IOC}
iocInit()

epicsThreadSleep(2)
dbpf e364xachk:GetVoltage.PROC 1
epicsThreadSleep(2)
dbpf e364xachk:GetCurrent.PROC 1

##When someone switch Remote to Local, Retry Remote Command as follows:
#dbpf e364xachk:Init.PROC 1
