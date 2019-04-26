#!../../bin/linux-arm/timing

## You may have to change timing to something else
## everywhere it appears in this file

< envPaths

cd "${TOP}"

## Register all support components
dbLoadDatabase "dbd/timing.dbd"
timing_registerRecordDeviceDriver pdbbase

timingAsynEpicsConfigure("timing", 1000, 0, 10, "${TOP}/iocBoot/${IOC}/TimingRegister.reg","EVR")

## Load record instances
#dbLoadRecords("db/xxx.db","user=ctrluserHost")
#dbLoadRecords("db/timingAsynDriver.db","SYS=SCL3,SUBSYS=TS,DEV=EVR,PORT=timing,OFFSET=0,TIMEOUT=1,NPOINTS=1000")
#dbLoadTemplate("${TOP}/iocBoot/${IOC}/timing.sub")
dbLoadTemplate("${TOP}/iocBoot/${IOC}/timing_evr_2.sub")
asynSetTraceIOMask("timing", 0, 0x2)

cd "${TOP}/iocBoot/${IOC}"
iocInit

## Start any sequence programs
#seq sncxxx,"user=ctrluserHost"


## For Setup Clock
#epicsThreadSleep(5)
#dbpf SCL3:TS:EVG:EVGSet 1
