#!../../bin/linux-x86_64/lregression

#- You may have to change lregression to something else
#- everywhere it appears in this file

< envPaths

cd "${TOP}"

## Register all support components
dbLoadDatabase "dbd/lregression.dbd"
lregression_registerRecordDeviceDriver pdbbase

## Load record instances
#dbLoadRecords("db/xxx.db","user=ctrluser")
dbLoadTemplate("${TOP}/iocBoot/${IOC}/scl3_seq_simulate.sub")
dbLoadRecords("db/linear_regression.vdb")


cd "${TOP}/iocBoot/${IOC}"
iocInit

## Start any sequence programs
#seq sncxxx,"user=ctrluser"
seq sncMakeTempWaveform
