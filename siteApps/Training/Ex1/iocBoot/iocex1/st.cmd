#!../../bin/linux-x86_64/iocex1

#- You may have to change iocex1 to something else
#- everywhere it appears in this file

< envPaths

cd "${TOP}"

## Register all support components
dbLoadDatabase "dbd/iocex1.dbd"
iocex1_registerRecordDeviceDriver pdbbase

## Load record instances
#dbLoadRecords("db/xxx.db","user=ctrluser")
dbLoadRecords("db/count.db")
dbLoadRecords("db/sincalc.vdb")
dbLoadRecords("db/subMul.vdb")

cd "${TOP}/iocBoot/${IOC}"
iocInit

## Start any sequence programs
#seq sncxxx,"user=ctrluser"
