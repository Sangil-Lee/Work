#!../../bin/windows-x64/pxieioc

#- You may have to change pxieioc to something else
#- everywhere it appears in this file

< envPaths

cd "${TOP}"

## Register all support components
dbLoadDatabase "dbd/pxieioc.dbd"
pxieioc_registerRecordDeviceDriver pdbbase

## Load record instances
#dbLoadRecords("db/pxieioc.db","user=cdms")
dbLoadRecords("db/pxi_long.db")

cd "${TOP}/iocBoot/${IOC}"
iocInit

## Start any sequence programs
#seq sncxxx,"user=cdms"
