#!../../bin/linux-x86_64/timing

## You may have to change timing to something else
## everywhere it appears in this file

< envPaths

epicsEnvSet("EPICS_CA_MAX_ARRAY_BYTES","6553500")

cd "${TOP}"

## Register all support components
dbLoadDatabase "dbd/timing.dbd"
timing_registerRecordDeviceDriver pdbbase

timingAsynEpicsConfigure("timing", 2048, 0, 10, "${TOP}/iocBoot/${IOC}/TimingRegister.reg","EVS")

## Load record instances
#dbLoadRecords("db/xxx.db","user=ctrluserHost")
#dbLoadRecords("db/timingAsynDriver.db","SYS=SCL3,SUBSYS=TS,DEV=EVR,PORT=timing,OFFSET=0,TIMEOUT=1,NPOINTS=1000")
dbLoadRecords("db/timeStamp.db","SYS=SCL3,SUBSYS=TS,DEV=EVS1")
dbLoadRecords("db/fanoutBit.vdb","SYS=SCL3,SUBSYS=TS,DEV=EVS1")
dbLoadRecords("db/fanoutShort.vdb","SYS=SCL3,SUBSYS=TS,DEV=EVS1")
dbLoadRecords("db/strToValuesub.vdb","SYS=SCL3,SUBSYS=TS,DEV=EVS1")
dbLoadTemplate("${TOP}/iocBoot/${IOC}/timing.sub")
asynSetTraceIOMask("timing", 0, 0x2)

cd "${TOP}/iocBoot/${IOC}"
iocInit

## Start any sequence programs
#seq sncxxx,"user=ctrluserHost"


## For Setup Clock
#epicsThreadSleep(5)
#dbpf SCL3:TS:EVG:EVGSet 1
