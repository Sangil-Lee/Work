#!../../bin/linux-x86_64/ndpscontrol

#- You may have to change ndpscontrol to something else
#- everywhere it appears in this file

< envPaths

cd "${TOP}"

## Register all support components
dbLoadDatabase "dbd/ndpscontrol.dbd"
ndpscontrol_registerRecordDeviceDriver pdbbase

## Load record instances
dbLoadRecords("db/NDPSControl.db","SYS=NDPS,SUBSYS=-TG002:,DEV=TRHS,SUBDEV=-MOTZ:")

cd "${TOP}/iocBoot/${IOC}"
iocInit

## Start any sequence programs
#seq sncxxx,"user=ctrluser"
seq  sncNDPSControl,"SYS=NDPS,SUBSYS=-TG002:,DEV=TRHS,SUBDEV=-MOTZ:")
