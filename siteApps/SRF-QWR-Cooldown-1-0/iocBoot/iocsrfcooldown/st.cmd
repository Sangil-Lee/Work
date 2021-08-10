#!../../bin/linux-x86_64/srfcooldown

#- You may have to change srfcooldown to something else
#- everywhere it appears in this file

< envPaths

cd "${TOP}"

## Register all support components
dbLoadDatabase "dbd/srfcooldown.dbd"
srfcooldown_registerRecordDeviceDriver pdbbase

## Load record instances
#dbLoadRecords("db/xxx.db","user=ctrluser")
dbLoadRecords("db/RefValue.vdb","SYS=SRF,SUBSYS=-TF:,DEV=Cooldown,SUBDEV=-RefValue:")

cd "${TOP}/iocBoot/${IOC}"
iocInit

## Start any sequence programs
#seq sncxxx,"user=ctrluser"
