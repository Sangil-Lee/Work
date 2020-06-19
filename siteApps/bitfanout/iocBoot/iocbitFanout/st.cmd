#!../../bin/linux-x86_64/bitFanout

## You may have to change bitFanout to something else
## everywhere it appears in this file

< envPaths

cd "${TOP}"

## Register all support components
dbLoadDatabase "dbd/bitFanout.dbd"
bitFanout_registerRecordDeviceDriver pdbbase

## Load record instances
#dbLoadRecords("db/xxx.db","user=ctrluserHost")
dbLoadRecords("db/mbbiDirectTest.vdb")

cd "${TOP}/iocBoot/${IOC}"
iocInit

## Start any sequence programs
#seq sncxxx,"user=ctrluserHost"
