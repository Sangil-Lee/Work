#!../../bin/linux-x86_64/testioc

#- You may have to change testioc to something else
#- everywhere it appears in this file

< envPaths

cd "${TOP}"

## Register all support components
dbLoadDatabase "dbd/testioc.dbd"
testioc_registerRecordDeviceDriver pdbbase

## Load record instances
dbLoadTemplate "db/user.substitutions"
dbLoadRecords "db/testiocVersion.db", "user=ctrluser"
dbLoadRecords "db/dbSubExample.db", "user=ctrluser"

dbLoadTemplate "db/simulation.sub"

#- Set this to see messages from mySub
#-var mySubDebug 1

#- Run this to trace the stages of iocInit
#-traceIocInit

cd "${TOP}/iocBoot/${IOC}"
iocInit

## Start any sequence programs
#seq sncExample, "user=ctrluser"
