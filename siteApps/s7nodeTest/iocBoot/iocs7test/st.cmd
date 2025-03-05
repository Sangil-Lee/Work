#!../../bin/linux-x86_64/s7test

#- You may have to change s7test to something else
#- everywhere it appears in this file

< envPaths

cd "${TOP}"

## Register all support components
dbLoadDatabase "dbd/s7test.dbd"
s7test_registerRecordDeviceDriver pdbbase

s7nodaveConfigureIsoTcpPort("s7PLC","192.168.1.1", 0, 0, 0)
s7nodaveConfigurePollGroup("s7PLC","1s", 1.0, 0)

## Load record instances
#dbLoadRecords("db/s7test.db","user=ctrluser")
dbLoadRecords("db/s7nodave_ex.db","P=TEST")

cd "${TOP}/iocBoot/${IOC}"
iocInit

## Start any sequence programs
#seq sncxxx,"user=ctrluser"
