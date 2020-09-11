#!../../bin/linux-arm/miti

## You may have to change miti to something else
## everywhere it appears in this file

< envPaths

cd "${TOP}"

## Register all support components
dbLoadDatabase "dbd/miti.dbd"
miti_registerRecordDeviceDriver pdbbase

mitiFPSAsynEpicsConfigure("MITI","${TOP}/iocBoot/${IOC}/MITI.reg")

## Load record instances
#dbLoadRecords("db/xxx.db","user=ctrluserHost")
dbLoadRecords("db/timeStamp.db","SYS=Ctrl,SUBSYS=MPS,DEV=FPS-MN01")
dbLoadTemplate("${TOP}/iocBoot/${IOC}/mitiDB.sub")

cd "${TOP}/iocBoot/${IOC}"
iocInit

## Start any sequence programs
#seq sncxxx,"user=ctrluserHost"
