#!../../bin/linux-arm/scl3magnet

#- You may have to change scl3magnet to something else
#- everywhere it appears in this file

< envPaths

cd "${TOP}"

epicsEnvSet "STREAM_PROTOCOL_PATH" "../../proto"

## Register all support components
dbLoadDatabase "dbd/scl3magnet.dbd"
scl3magnet_registerRecordDeviceDriver pdbbase


######SCL3 Quadrupole Magnet Power Supply######
#drvAsynIPPortConfigure ("SCL3_QM1", "192.168.84.6:4001", 0, 0, 0)

## According to SCL3 Quadrupole Maganet Type
## Load record instances
#dbLoadTemplate("${TOP}/iocBoot/${IOC}/danfysik_SW.sub", "DEVICE=KOBRA-MAG:Q9-PS:,   port=SCL3_QM1")

## According to KoBRA Maganet Type
#dbLoadRecords("db/dbAutoRampup.db", "DEVICE=KOBRA-MAG:Q9-PS")

## According to KoBRA Maganet Type
#dbLoadRecords("db/MGField2Cur_Q9.vdb",   "DEVICE=KOBRA-MAG:Q9-PS")

cd "${TOP}/iocBoot/${IOC}"
iocInit

## Start any sequence programs
#seq sncxxx,"user=ctrluser"
