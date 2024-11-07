#!../../bin/linux-x86_64/rtp

#- You may have to change rtp to something else
#- everywhere it appears in this file

< envPaths

cd "${TOP}"

## Register all support components
dbLoadDatabase "dbd/rtp.dbd"
rtp_registerRecordDeviceDriver pdbbase

drvSyncRTPConfigure("RTPDevice", "192.168.10.50:50199 TCP", 0, 0)


## Load record instances
#dbLoadRecords("db/rtp.db","user=ctrluser")
dbLoadRecords("db/RTPTest.vdb","user=ctrluser")


cd "${TOP}/iocBoot/${IOC}"
iocInit

## Start any sequence programs
#seq sncxxx,"user=ctrluser"
