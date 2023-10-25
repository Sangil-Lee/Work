#!../../bin/linux-x86_64/ifmgdaq

#- You may have to change ifmgdaq to something else
#- everywhere it appears in this file

< envPaths

cd "${TOP}"

## Register all support components
dbLoadDatabase "dbd/ifmgdaq.dbd"
ifmgdaq_registerRecordDeviceDriver pdbbase

## Load record instances
#dbLoadRecords("db/ifmgdaq.db","user=ctrluser")

cd "${TOP}/iocBoot/${IOC}"
iocInit

## Start any sequence programs
#seq sncxxx,"user=ctrluser"
