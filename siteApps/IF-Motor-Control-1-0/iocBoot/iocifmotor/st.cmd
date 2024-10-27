#!../../bin/linux-x86_64/ifmotor

#- You may have to change ifmotor to something else
#- everywhere it appears in this file

< envPaths

cd "${TOP}"

epicsEnvSet "STREAM_PROTOCOL_PATH" "../../proto"

## Register all support components
dbLoadDatabase "dbd/ifmotor.dbd"
ifmotor_registerRecordDeviceDriver pdbbase

drvAsynIPPortConfigure ("IF-Motor-1", "192.168.:4001", 0, 0, 0)

## According to KoBRA Maganet Type
## Load record instances
##dbLoadTemplate("${TOP}/iocBoot/${IOC}/if-motor-1.sub", "DEVICE=IF-Motor:,   port=IF-Motor-1")
#dbLoadTemplate("db/ifMotor.template", "SYS=IF, SUBSYS=-Target:, DEV=Sec1, SUBDEV=-Motor:, EEPROM_ADDR=E4, port=IF-Motor-1")
dbLoadRecords("db/ifMotor.db", "SYS=IF, SUBSYS=-Target:, DEV=Sec1, SUBDEV=-Motor:, EEPROM_ADDR=E4, port=IF-Motor-1")


cd "${TOP}/iocBoot/${IOC}"
iocInit

## Start any sequence programs
#seq sncxxx,"user=ctrluser"
