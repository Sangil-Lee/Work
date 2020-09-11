#!../../bin/linux-arm/acq

## You may have to change acq to something else
## everywhere it appears in this file

< envPaths

cd "${TOP}"

## Register all support components
dbLoadDatabase "dbd/acq.dbd"
acq_registerRecordDeviceDriver pdbbase

acqFPSAsynEpicsConfigure("ACQ","${TOP}/iocBoot/${IOC}/ACQ.reg")

## Load record instances
#dbLoadRecords("db/xxx.db","user=ctrluserHost")
dbLoadRecords("db/timeStamp.db","SYS=Ctrl,SUBSYS=MPS,DEV=FPS-AN66")
dbLoadRecords("db/acq_PMSEN_calc.db","SYS=Ctrl,SUBSYS=MPS,DEV=FPS-AN,LINK=6,NODE=6")
dbLoadTemplate("${TOP}/iocBoot/${IOC}/acqDB.sub")

cd "${TOP}/iocBoot/${IOC}"
iocInit

## Start any sequence programs
#seq sncxxx,"user=ctrluserHost"
