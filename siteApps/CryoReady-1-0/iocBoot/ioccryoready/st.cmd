#!../../bin/linux-x86_64/cryoready

#- You may have to change cryoready to something else
#- everywhere it appears in this file

< envPaths

cd "${TOP}"

## Register all support components
dbLoadDatabase "dbd/cryoready.dbd"
cryoready_registerRecordDeviceDriver pdbbase

## Load record instances
#dbLoadRecords("db/xxx.db","user=ctrluser")
dbLoadTemplate("db/CryoReadyPVs.sub")
dbLoadRecords("db/CryoReady.db", "SYS=SCL31, SUBSYS=-Cryo:, DEV=QWR, SUBDEV=":", NELM=22")
dbLoadRecords("db/CryoReady.db", "SYS=SCL32, SUBSYS=-Cryo:, DEV=HWR, SUBDEV=":", NELM=32")

cd "${TOP}/iocBoot/${IOC}"
iocInit

## Start any sequence programs
#seq sncxxx,"user=ctrluser"
