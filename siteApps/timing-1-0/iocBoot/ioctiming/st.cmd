#!../../bin/linux-x86_64/timing

## You may have to change timing to something else
## everywhere it appears in this file

< envPaths

cd "${TOP}"

## Register all support components
dbLoadDatabase "dbd/timing.dbd"
timing_registerRecordDeviceDriver pdbbase

timingAsynEpicsConfigure("timing", 1000, 0, 0, "timing")

## Load record instances
#dbLoadRecords("db/xxx.db","user=ctrluserHost")
dbLoadRecords("db/timingAsynDriver.db","SYS=SCL3,SUBSYS=TS,DEV=EVG,PORT=timing,ADDR=0,TIMEOUT=1,NPOINTS=1000")

asynSetTraceIOMask("timing", 0, 0x2)

cd "${TOP}/iocBoot/${IOC}"
iocInit

## Start any sequence programs
#seq sncxxx,"user=ctrluserHost"
