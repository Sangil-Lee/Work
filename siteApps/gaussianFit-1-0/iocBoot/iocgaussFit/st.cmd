#!../../bin/linux-x86_64/gaussFit

#- You may have to change gaussFit to something else
#- everywhere it appears in this file

< envPaths
< SCLEnv

cd "${TOP}"

## Register all support components
dbLoadDatabase "dbd/gaussFit.dbd"
gaussFit_registerRecordDeviceDriver pdbbase

## Load record instances
#dbLoadRecords("db/xxx.db","user=ctrluser")
dbLoadRecords("db/gaussfitting.vdb","SYS=${SYS},SUBSYS=${SUBSYS},DEV=${DEV},SUBDEV=${SUBDEV}")
#dbLoadRecords("db/wsgaussfitting.vdb","SYS=${SYS},SUBSYS=${SUBSYS},DEV=${DEV},SUBDEV=${SUBDEV}")

#resetGaussWave(100, 1, 0, 0.3, 0.8)

cd "${TOP}/iocBoot/${IOC}"
iocInit

## Start any sequence programs
#seq sncxxx,"user=ctrluser"
