< envPaths

cd ${TOP}

## Register all support components
dbLoadDatabase("dbd/simpleDbPv.dbd")
simpleDbPv_registerRecordDeviceDriver(pdbbase)

## Load record instances
dbLoadRecords("db/test.db")

cd ${TOP}/iocBoot/${IOC}
iocInit()

startPVAServer
