#!../../bin/linux-x86_64/ex7

#- You may have to change ex7 to something else
#- everywhere it appears in this file

< envPaths

cd "${TOP}"

## Register all support components
dbLoadDatabase "dbd/ex7.dbd"
ex7_registerRecordDeviceDriver pdbbase

## Load record instances
dbLoadTemplate "db/user.substitutions"
dbLoadRecords "db/ex7Version.db", "user=ctrluser"
dbLoadRecords "db/dbSubExample.db", "user=ctrluser"

#- Set this to see messages from mySub
#var mySubDebug 1

#- Run this to trace the stages of iocInit
#traceIocInit
PVARecord
powerSupplyRecord PVRpowerSupply

cd "${TOP}/iocBoot/${IOC}"
iocInit

## Start any sequence programs
#seq sncExample, "user=ctrluser"
