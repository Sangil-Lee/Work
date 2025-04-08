#!../../bin/linux-x86_64/seqcontrols

#- You may have to change seqcontrols to something else
#- everywhere it appears in this file

< envPaths

cd "${TOP}"

## Register all support components
dbLoadDatabase "dbd/seqcontrols.dbd"
seqcontrols_registerRecordDeviceDriver pdbbase

## Load record instances
#dbLoadRecords("db/seqcontrols.db","user=ctrluser")

cd "${TOP}/iocBoot/${IOC}"
iocInit

## Start any sequence programs
#seq sncxxx,"user=ctrluser"
