#!../../bin/linux-x86_64/inventory

#- You may have to change inventory to something else
#- everywhere it appears in this file

< envPaths

cd "${TOP}"

## Register all support components
dbLoadDatabase "dbd/inventory.dbd"
inventory_registerRecordDeviceDriver pdbbase

## Load record instances
#dbLoadRecords("db/inventory.db","user=ctrluser")

cd "${TOP}/iocBoot/${IOC}"
iocInit

## Start any sequence programs
#seq sncxxx,"user=ctrluser"
