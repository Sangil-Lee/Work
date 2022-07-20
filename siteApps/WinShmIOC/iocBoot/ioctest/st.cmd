#!../../bin/windows-x64-mingw/ioctest

#- You may have to change ioctest to something else
#- everywhere it appears in this file

< envPaths

cd "${TOP}"

## Register all support components
dbLoadDatabase "dbd/ioctest.dbd"
ioctest_registerRecordDeviceDriver pdbbase

## Load record instances
dbLoadTemplate "db/user.substitutions"
dbLoadRecords "db/ioctestVersion.db", "user=silee"
dbLoadRecords "db/dbSubExample.db", "user=silee"

#- Set this to see messages from mySub
#var mySubDebug 1

#- Run this to trace the stages of iocInit
#traceIocInit

cd "${TOP}/iocBoot/${IOC}"
iocInit

## Start any sequence programs
#seq sncExample, "user=silee"
