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
#drvAsynIPPortConfigure ("SCL3_QM1",    "192.168.84.6:4001", 0, 0, 0)
#drvAsynIPPortConfigure ("SCL3_QM1_ST", "192.168.84.6:4002", 0, 0, 0)

## According to SCL3 Quadrupole Maganet Type
## Load record instances
#dbLoadTemplate("${TOP}/iocBoot/${IOC}/danfysik_SW.sub", "SYS=SCL31, SUBSYS=-QWR01:, DEV=QMAD01, SUBDEV=-PSU01:, port=SCL3_QM1")
#dbLoadTemplate("${TOP}/iocBoot/${IOC}/danfysik_SW.sub")

##Toyotech P.S
#dbLoadRecords("db/toyotech.db", "SYS=MEBT,SUBSYS=QM5,DEV=ToyoTech, port=SCL3_QM1_ST")

cd "${TOP}/iocBoot/${IOC}"
iocInit

## Start any sequence programs
#seq sncxxx,"user=ctrluser"
