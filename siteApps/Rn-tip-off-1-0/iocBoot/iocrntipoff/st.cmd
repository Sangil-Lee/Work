#!../../bin/linux-x86_64/rntipoff

#- You may have to change rntipoff to something else
#- everywhere it appears in this file

< envPaths

cd "${TOP}"

## Register all support components
dbLoadDatabase "dbd/rntipoff.dbd"
rntipoff_registerRecordDeviceDriver pdbbase

## Load record instances
#dbLoadRecords("db/rntipoff.db","user=ctrluser")

cd "${TOP}/iocBoot/${IOC}"
iocInit

## Start any sequence programs
#seq sncxxx,"user=ctrluser"
