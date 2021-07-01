#!../../bin/linux-x86_64/phasend

#- You may have to change phasend to something else
#- everywhere it appears in this file

< envPaths

cd "${TOP}"

## Register all support components
dbLoadDatabase "dbd/phasend.dbd"
phasend_registerRecordDeviceDriver pdbbase

## Load record instances
#dbLoadRecords("db/xxx.db","user=ctrluser")
dbLoadRecords("db/sncTempWF.vdb")
dbLoadRecords("db/aiOutLinkTest.vdb")
dbLoadTemplate("db/sncTempWF.sub","SYS=SCL3,SUBSYS=:, DEV=WFTest, SUBDEV=:")
dbLoadTemplate("${TOP}/iocBoot/${IOC}/scl3_seq_simulate.sub")

cd "${TOP}/iocBoot/${IOC}"
iocInit
## Start any sequence programs
#seq sncxxx,"user=ctrluser"
seq sncTempWF,"SYS=SCL3,SUBSYS=:, DEV=WFTest, SUBDEV=:"

