#!../../bin/linux-x86_64/elssen

#- You may have to change elssen to something else
#- everywhere it appears in this file

< envPaths

cd "${TOP}"

## Register all support components
dbLoadDatabase "dbd/elssen.dbd"
elssen_registerRecordDeviceDriver pdbbase

SerialAsynEpicsConfigure("SerialUSB","${TOP}/iocBoot/${IOC}/serialTemp.reg","/dev/ttyUSB0");

## Load record instances
#dbLoadRecords("db/xxx.db","user=ctrluser")
#dbLoadTemplate("${TOP}/iocBoot/${IOC}/elssen.sub", "Prefix=1214")

dbLoadTemplate("${TOP}/iocBoot/${IOC}/serialTemp.sub")

cd "${TOP}/iocBoot/${IOC}"
iocInit

## Start any sequence programs
#seq sncxxx,"user=ctrluser"
