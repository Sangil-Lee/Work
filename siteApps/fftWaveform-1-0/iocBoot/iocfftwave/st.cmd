#!../../bin/linux-x86_64/fftwave

#- You may have to change fftwave to something else
#- everywhere it appears in this file

< envPaths

cd "${TOP}"

## Register all support components
dbLoadDatabase "dbd/fftwave.dbd"
fftwave_registerRecordDeviceDriver pdbbase

## Load record instances
#dbLoadRecords("db/xxx.db","user=ctrluser")

dbLoadRecords("db/fftwave.vdb","SYS=SCL3, DEV=Cooldown")

cd "${TOP}/iocBoot/${IOC}"
iocInit

## Start any sequence programs
#seq sncxxx,"user=ctrluser"
