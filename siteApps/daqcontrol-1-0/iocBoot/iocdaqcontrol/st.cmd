#!../../bin/linux-x86_64/daqcontrol

## You may have to change daqcontrol to something else
## everywhere it appears in this file

< envPaths

cd "${TOP}"

## Register all support components
dbLoadDatabase "dbd/daqcontrol.dbd"
daqcontrol_registerRecordDeviceDriver pdbbase

daqControlAsynEpicsConfigure("daqcontrol","${TOP}/iocBoot/${IOC}/DAQRegister.reg",0)

## Load record instances
#dbLoadRecords("db/xxx.db","user=ctrluserHost")

dbLoadTemplate("${TOP}/iocBoot/${IOC}/DAQ.sub")

cd "${TOP}/iocBoot/${IOC}"
iocInit

## Start any sequence programs
#seq sncxxx,"user=ctrluserHost"
