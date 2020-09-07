#!../../bin/linux-x86_64/scl3linac

#- You may have to change scl3linac to something else
#- everywhere it appears in this file

< envPaths
< SCLEnv

cd "${TOP}"

## Register all support components
dbLoadDatabase "dbd/scl3linac.dbd"
scl3linac_registerRecordDeviceDriver pdbbase

## Load record instances
#dbLoadRecords("db/xxx.db","user=ctrluser")
dbLoadTemplate("${TOP}/iocBoot/${IOC}/scl3_macro_test.sub", "SYS=${SYS},SUBSYS=${SUBSYS},DEV=${DEV1},SUBDEV=${SUBDEV1}")

dbLoadTemplate("${TOP}/iocBoot/${IOC}/scl3_macro_test.sub", "SYS=${SYS},SUBSYS=${SUBSYS},DEV=${DEV2},SUBDEV=${SUBDEV2}")

cd "${TOP}/iocBoot/${IOC}"
iocInit

## Start any sequence programs
#seq sncxxx,"user=ctrluser"
