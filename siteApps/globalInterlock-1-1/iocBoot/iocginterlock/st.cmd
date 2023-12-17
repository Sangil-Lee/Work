#!../../bin/linux-x86_64/ginterlock

#- You may have to change ginterlock to something else
#- everywhere it appears in this file

< envPaths

cd "${TOP}"

## Register all support components
dbLoadDatabase "dbd/ginterlock.dbd"
ginterlock_registerRecordDeviceDriver pdbbase

## Load record instances
#dbLoadRecords("db/xxx.db","user=ctrluser")
dbLoadTemplate("db/biSim.sub")
dbLoadTemplate("db/aiSim.sub")

###dbLoadTemplate("db/GInt.sub")

dbLoadTemplate("db/globalIntWF.sub")
dbLoadTemplate("db/globalInt_PT7503.sub")
dbLoadTemplate("db/globalInt_PT7301.sub")
dbLoadTemplate("db/globalInt_PT7503PT7307.sub")
dbLoadTemplate("db/globalInt_PT7503CV.sub")
dbLoadTemplate("db/globalInt_PT7301CV.sub")

cd "${TOP}/iocBoot/${IOC}"
iocInit

## Start any sequence programs
#seq sncxxx,"user=ctrluser"
seq sncQWRGInt
