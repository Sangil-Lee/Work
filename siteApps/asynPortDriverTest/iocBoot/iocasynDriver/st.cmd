#!../../bin/linux-x86_64/asynDriver

## You may have to change asynDriver to something else
## everywhere it appears in this file

< envPaths

cd "${TOP}"

## Register all support components
dbLoadDatabase "dbd/asynDriver.dbd"
asynDriver_registerRecordDeviceDriver pdbbase

asynDriverSupportConfigure("AsynDriver","${TOP}/iocBoot/${IOC}/asyndriver.reg")

## Load record instances
#dbLoadRecords("db/xxx.db","user=ctrluserHost")
dbLoadRecords("db/wfTest.vdb")
#dbLoadRecords("db/waveformSetup.vdb")

dbLoadTemplate("${TOP}/iocBoot/${IOC}/asyndriver.sub")

cd "${TOP}/iocBoot/${IOC}"
iocInit

## Start any sequence programs
#seq sncxxx,"user=ctrluserHost"
