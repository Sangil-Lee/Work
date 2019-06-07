#!../../bin/linux-x86_64/nidaq

## You may have to change nidaq to something else
## everywhere it appears in this file

< envPaths

cd "${TOP}"

## Register all support components
dbLoadDatabase "dbd/nidaq.dbd"
nidaq_registerRecordDeviceDriver pdbbase

nidaqAsynEpicsConfigure("nidaqmx","${TOP}/iocBoot/${IOC}/NIDAQRegister.reg", "PXI1Slot5/ai0")

## Load record instances
#dbLoadRecords("db/xxx.db","user=wjeongHost")

dbLoadTemplate("${TOP}/iocBoot/${IOC}/nidaqmx.sub")

cd "${TOP}/iocBoot/${IOC}"
iocInit

## Start any sequence programs
#seq sncxxx,"user=wjeongHost"
