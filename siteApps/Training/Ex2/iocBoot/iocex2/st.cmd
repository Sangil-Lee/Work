#!../../bin/linux-x86_64/iocex2

#- You may have to change iocex2 to something else
#- everywhere it appears in this file

< envPaths

cd "${TOP}"

## Register all support components
dbLoadDatabase "dbd/iocex2.dbd"
iocex2_registerRecordDeviceDriver pdbbase

## Load record instances
#dbLoadRecords("db/xxx.db","user=ctrluser")
dbLoadRecords("db/Ex2.vdb","P=NIDAQ")
dbLoadRecords("db/bitfanout.vdb","P=TEST")

cd "${TOP}/iocBoot/${IOC}"
iocInit

## Start any sequence programs
#seq sncxxx,"user=ctrluser"
seq sncLightControl, "P=NIDAQ"
