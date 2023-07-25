#!../../bin/linux-x86_64/aliasTr

#- You may have to change aliasTr to something else
#- everywhere it appears in this file

< envPaths

cd "${TOP}"

## Register all support components
dbLoadDatabase "dbd/aliasTr.dbd"
aliasTr_registerRecordDeviceDriver pdbbase

## Load record instances
#dbLoadRecords("db/aliasTr.db","user=ctrluser")
dbLoadRecords("db/aiRecordTest.db")
dbLoadTemplate("db/aiRecordLoad.sub")

cd "${TOP}/iocBoot/${IOC}"
iocInit

## Start any sequence programs
#seq sncxxx,"user=ctrluser"
