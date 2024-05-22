#!../../bin/linux-aarch64/nex1

#- You may have to change nex1 to something else
#- everywhere it appears in this file

< envPaths

cd "${TOP}"

## Register all support components
dbLoadDatabase "dbd/nex1.dbd"
nex1_registerRecordDeviceDriver pdbbase

## Load record instances
#dbLoadRecords("db/nex1.db","user=ctrluser")

cd "${TOP}/iocBoot/${IOC}"
iocInit

## Start any sequence programs
#seq sncxxx,"user=ctrluser"
