#!../../bin/linux-x86_64/timeStamp

## You may have to change timeStamp to something else
## everywhere it appears in this file

< envPaths

cd ${TOP}

## Register all support components
dbLoadDatabase "dbd/timeStamp.dbd"
timeStamp_registerRecordDeviceDriver pdbbase

## Load record instances
#dbLoadRecords("db/timeStamp.vdb","SYS=EXTR6")
dbLoadRecords("db/timeStamp.db","SYS=Ctrl,SUBSYS=TFc,DEV=NET")


cd ${TOP}/iocBoot/${IOC}
iocInit

## Start any sequence programs
#seq sncxxx,"user=sileeHost"
