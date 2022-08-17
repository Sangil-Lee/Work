#!../../bin/linux-x86_64/dpcryo

#- You may have to change dpcryo to something else
#- everywhere it appears in this file

< envPaths

cd "${TOP}"

## Register all support components
dbLoadDatabase "dbd/dpcryo.dbd"
dpcryo_registerRecordDeviceDriver pdbbase

nicDAQAsynEpicsConfigure("nicDAQ","db/NIcDAQRegister.reg", "cDAQ9185-1FF3DAD", 2000, 1000)

## Load record instances
#dbLoadRecords("db/xxx.db","user=ctrluser")

dbLoadTemplate("db/nicDAQmx.sub")

cd "${TOP}/iocBoot/${IOC}"
iocInit

## Start any sequence programs
#seq sncxxx,"user=ctrluser"
