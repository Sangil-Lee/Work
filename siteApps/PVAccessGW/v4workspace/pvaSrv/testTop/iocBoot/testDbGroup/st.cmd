< envPaths

cd ${TOP}

## Register all support components
dbLoadDatabase("dbd/testDbGroup.dbd")
testDbGroup_registerRecordDeviceDriver(pdbbase)

## Load record instances
dbLoadRecords "db/quadruple.db","name=quadruple"
dbLoadRecords "db/bpm.db","name=bpm"

cd ${TOP}/iocBoot/${IOC}
iocInit()

pvaSrvStart
startChannelProviderLocal
dbGroupCreate quadruple.txt
dbGroupCreate bpm.txt
