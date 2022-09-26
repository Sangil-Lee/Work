#!../../bin/linux-x86_64/pxieioc

#- You may have to change pxieioc to something else
#- everywhere it appears in this file

< envPaths

cd "${TOP}"

## Register all support components
dbLoadDatabase "dbd/pxieioc.dbd"
pxieioc_registerRecordDeviceDriver pdbbase

nidaqDIConfigure("nidaqmx", "PXI1Slot3")
nidaqDICreateChannel("port0")

## Load record instances
#dbLoadRecords("db/pxieioc.db","user=cdms")
dbLoadRecords("db/pxi_long.db")
dbLoadRecords("db/pxi_longin_bitfanout.vdb", "P=CDMS-Tester:PXI-DI")
dbLoadRecords("db/pxi_longout_bitfanout.vdb", "P=CDMS-Tester:PXI-DO")

cd "${TOP}/iocBoot/${IOC}"
iocInit

## Start any sequence programs
#seq sncxxx,"user=cdms"
