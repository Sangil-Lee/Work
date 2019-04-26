#!../../bin/linux-x86_64/autoscan

## You may have to change autoscan to something else
## everywhere it appears in this file

< envPaths

cd "${TOP}"

## Register all support components
dbLoadDatabase "dbd/autoscan.dbd"
autoscan_registerRecordDeviceDriver pdbbase

## Load record instances
#dbLoadRecords("db/xxx.db","user=ctrluserHost")
dbLoadRecords("db/dbAutoScan.db","SYS=MEBT,SUBSYS=QM3,DEV=Danfysik")

cd "${TOP}/iocBoot/${IOC}"
iocInit

## Start any sequence programs
seq sncAutoScan,"SYS=MEBT,SUBSYS=QM3,DEV=Danfysik,HISET=FinVal,LOSET=InitVal,TARGET=MEBT:HWRGAUSS"
