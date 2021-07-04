#!../../bin/linux-x86_64/cfTest

## You may have to change cfTest to something else
## everywhere it appears in this file

< envPaths

cd "${TOP}"

## Register all support components
dbLoadDatabase "dbd/cfTest.dbd"
cfTest_registerRecordDeviceDriver pdbbase

var(reccastTimeout, 5.0)
var(reccastMaxHoldoff, 5.0)

## Load record instances
#dbLoadRecords("db/xxx.db","user=ctrluserHost")
dbLoadRecords("db/cfTest.db","P=scwook")

cd "${TOP}/iocBoot/${IOC}"
iocInit

## Start any sequence programs
#seq sncxxx,"user=ctrluserHost"
