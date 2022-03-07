#!../../bin/linux-x86_64/srfcontrol

#- You may have to change srfcontrol to something else
#- everywhere it appears in this file

< envPaths

cd "${TOP}"

## Register all support components
dbLoadDatabase "dbd/srfcontrol.dbd"
srfcontrol_registerRecordDeviceDriver pdbbase

## Load record instances
#dbLoadRecords("db/xxx.db","user=ctrluser")
dbLoadRecords("db/Heater.vdb","SYS=SRF, SUBSYS=-TF:, DEV=Cryo, SUBDEV=-Ctrl:, HVALUE=Heater-Target:Value")

cd "${TOP}/iocBoot/${IOC}"
iocInit

## Start any sequence programs
#seq sncxxx,"user=ctrluser"
seq sncPulse,"SYS=SRF, SUBSYS=-TF:, DEV=Cryo, SUBDEV=-Ctrl:"
