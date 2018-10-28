#!../../bin/linux-x86_64/mbboLong

## You may have to change mbboLong to something else
## everywhere it appears in this file

< envPaths

cd "${TOP}"

## Register all support components
dbLoadDatabase "dbd/mbboLong.dbd"
mbboLong_registerRecordDeviceDriver pdbbase

## Load record instances
#dbLoadRecords("db/xxx.db","user=ctrluserHost")
dbLoadRecords("db/mbbo2LongCalc.vdb","SYS=SCL3,SUBSYS=TS,DEV=EVS1")

cd "${TOP}/iocBoot/${IOC}"
iocInit

## Start any sequence programs
#seq sncxxx,"user=ctrluserHost"
