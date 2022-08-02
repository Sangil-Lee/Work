#!../../bin/linux-x86_64/cdmstester

#- You may have to change cdmstester to something else
#- everywhere it appears in this file

< envPaths

cd "${TOP}"

## Register all support components
dbLoadDatabase "dbd/cdmstester.dbd"
cdmstester_registerRecordDeviceDriver pdbbase

#drvSyncRTPConfigure("RTPDevice", "89.89.80.30:50199 TCP", 0, 0)


#dbLoadRecords("db/RTPOneTest.vdb")
#dbLoadRecords("db/RTPTest.vdb")

dbLoadRecords("db/CDMSTester.db", "PREFIX=CDMSTester")

cd "${TOP}/iocBoot/${IOC}"
iocInit

## Start any sequence programs
#seq sncxxx,"user=ctrluser"
