#!../../bin/linux-x86_64/radondetect

#- You may have to change radondetect to something else
#- everywhere it appears in this file

< envPaths

cd "${TOP}"

## Register all support components
dbLoadDatabase "dbd/radondetect.dbd"
radondetect_registerRecordDeviceDriver pdbbase

## Load record instances
#dbLoadRecords("db/radondetect.db","user=ctrluser")

cd "${TOP}/iocBoot/${IOC}"
iocInit

## Start any sequence programs
#seq sncxxx,"user=ctrluser"
