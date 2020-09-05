#!../../bin/linux-x86_64/scl3test

#- You may have to change scl3test to something else
#- everywhere it appears in this file

< envPaths
epicsEnvSet("SYS",		"SRF1")
epicsEnvSet("SUBSYS",	"-QWR01:")
epicsEnvSet("DEV1","CM")
epicsEnvSet("DEV2","CTL")
epicsEnvSet("SUBDEV1","-Valve:")
epicsEnvSet("SUBDEV2","-Press:")

cd "${TOP}"

## Register all support components
dbLoadDatabase "dbd/scl3test.dbd"
scl3test_registerRecordDeviceDriver pdbbase

## Load record instances
#dbLoadRecords("db/xxx.db","user=ctrluser")
#dbLoadRecords("db/test.vdb")
dbLoadTemplate("${TOP}/iocBoot/${IOC}/macro_test.sub","SYS=${SYS}, SUBSYS=${SUBSYS}, DEV=${DEV1}, SUBDEV=${SUBDEV1}")
dbLoadTemplate("${TOP}/iocBoot/${IOC}/macro_test.sub","SYS=${SYS}, SUBSYS=${SUBSYS}, DEV=${DEV2}, SUBDEV=${SUBDEV2}")

#{SRF1-QWR01:CM-,  InCh0,		Open,   Close,    "1 second"}
cd "${TOP}/iocBoot/${IOC}"
iocInit

## Start any sequence programs
#seq sncxxx,"user=ctrluser"
