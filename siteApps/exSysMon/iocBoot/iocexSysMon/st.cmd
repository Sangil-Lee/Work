#!../../bin/linux-x86_64/exSysMon

## You may have to change exSysMon to something else
## everywhere it appears in this file

< envPaths

cd ${TOP}

## Register all support components
dbLoadDatabase "dbd/exSysMon.dbd"
exSysMon_registerRecordDeviceDriver pdbbase

## Load record instances
#dbLoadRecords("db/xxx.db","user=sileeHost")
## Naming Rule
dbLoadRecords("db/dbSysMon.db","SYS=SYS,SUBSYS=-SUBSYS:,DEV=DEV,SUBDEV=-SUBDEV")
dbLoadRecords("db/dbIfstat.vdb","SYS=SYS,SUBSYS=-SUBSYS:,DEV=DEV,SUBDEV=-SUBDEV")
dbLoadRecords("db/dbSysTimeStamp.vdb","SYS=SYS,SUBSYS=-SUBSYS:,DEV=DEV,SUBDEV=-SUBDEV")

cd ${TOP}/iocBoot/${IOC}
iocInit

## Start any sequence programs
#seq sncxxx,"user=sileeHost"
