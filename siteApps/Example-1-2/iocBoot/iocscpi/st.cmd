#!../../bin/linux-x86_64/scpi

## You may have to change scpi to something else
## everywhere it appears in this file

< envPaths

cd "${TOP}"

## Register all support components
dbLoadDatabase "dbd/scpi.dbd"
scpi_registerRecordDeviceDriver pdbbase

drvAsynIPPortConfigure("port1", "192.168.1.140:4016", 0, 0, 0)

## Load record instances
dbLoadRecords("db/scpi.db", "SYS=Ex,SUBSYS=SCPI,DEV=POW,P=port1")

cd "${TOP}/iocBoot/${IOC}"
iocInit

## Start any sequence programs
#seq sncxxx,"user=scwookHost"
