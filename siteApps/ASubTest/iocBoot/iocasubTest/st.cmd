#!../../bin/linux-x86_64/asubTest

#- You may have to change asubTest to something else
#- everywhere it appears in this file

< envPaths

cd "${TOP}"

## Register all support components
dbLoadDatabase "dbd/asubTest.dbd"
asubTest_registerRecordDeviceDriver pdbbase

## Load record instances
#dbLoadRecords("db/xxx.db","user=ctrluser")
dbLoadRecords("db/asubTest.vdb")
dbLoadRecords("db/asubStr.vdb")

cd "${TOP}/iocBoot/${IOC}"
iocInit

## Start any sequence programs
#seq sncxxx,"user=ctrluser"
