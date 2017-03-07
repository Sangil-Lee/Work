#!../../bin/linux-x86_64/e3632a

## You may have to change e3632a to something else
## everywhere it appears in this file

< envPaths

cd "${TOP}"

epicsEnvSet "STREAM_PROTOCOL_PATH" "../../proto"

## Register all support components
dbLoadDatabase "dbd/e3632a.dbd"
e3632a_registerRecordDeviceDriver pdbbase

drvAsynIPPortConfigure("L0", "10.1.5.88:4003", 0, 0, 0)
#asynOctetSetInputEos("L0",0,"\n")
#asynOctetSetOutputEos("L0",0,"\n")

## Load record instances
#dbLoadRecords("db/xxx.db","user=ctrluserHost")
dbLoadRecords("db/E3632A.db","SYS=MEBT,SUBSYS=QM,DEV=PS,P=L0")

cd "${TOP}/iocBoot/${IOC}"
iocInit

## Start any sequence programs
#seq sncxxx,"user=ctrluserHost"
