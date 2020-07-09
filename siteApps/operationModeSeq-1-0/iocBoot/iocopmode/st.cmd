#!../../bin/linux-x86_64/opmode

#- You may have to change opmode to something else
#- everywhere it appears in this file

< envPaths

cd "${TOP}"

## Register all support components
dbLoadDatabase "dbd/opmode.dbd"
opmode_registerRecordDeviceDriver pdbbase

## Load record instances
#dbLoadRecords("db/xxx.db","user=ctrluser")
dbLoadRecords("db/dbOPModeSeq.vdb","SYS=SCL3,SUBSYS=-CM01:,DEV=CM,SUBDEV=:")

cd "${TOP}/iocBoot/${IOC}"
iocInit

## Start any sequence programs
seq sncOPModeSeq,"SYS=SCL3,SUBSYS=-CM01:,DEV=CM,SUBDEV=:"
