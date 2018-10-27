#!../../bin/linux-arm/timing

## You may have to change timing to something else
## everywhere it appears in this file

< envPaths
< /mnt/sdcard/envDev

epicsEnvSet("EPICS_CA_MAX_ARRAY_BYTES","6553500")

cd "${TOP}"

## Register all support components
dbLoadDatabase "dbd/timing.dbd"
timing_registerRecordDeviceDriver pdbbase

timingAsynEpicsConfigure("timing", 1000, 0, 10, "${TOP}/iocBoot/${IOC}/TimingRegister.reg","${DEV_NAME}",${DEV_NUM})

## Load record instances
dbLoadRecords("db/timeStamp.db","SYS=${DEV_SYS},SUBSYS=${DEV_SUBSYS},DEV=${DEV_NAME}${DEV_NUM}")
dbLoadRecords("db/fanoutBit.vdb","SYS=${DEV_SYS},SUBSYS=${DEV_SUBSYS},DEV=${DEV_NAME}${DEV_NUM}")
dbLoadRecords("db/fanoutShort.vdb","SYS=${DEV_SYS},SUBSYS=${DEV_SUBSYS},DEV=${DEV_NAME}${DEV_NUM}")
dbLoadRecords("db/strToValuesub.vdb","SYS=${DEV_SYS},SUBSYS=${DEV_SUBSYS},DEV=${DEV_NAME}${DEV_NUM}")
dbLoadTemplate("${TS_LOCAL}/timing.sub")
asynSetTraceIOMask("timing", 0, 0x2)

cd "${TOP}/iocBoot/${IOC}"
iocInit

## Start any sequence programs
#seq sncxxx,"user=ctrluserHost"


## For Setup Clock
#epicsThreadSleep(5)
#dbpf SCL3:TS:EVG:EVGSet 1
