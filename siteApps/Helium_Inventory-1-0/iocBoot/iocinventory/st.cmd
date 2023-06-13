#!../../bin/linux-x86_64/inventory

#- You may have to change inventory to something else
#- everywhere it appears in this file

< envPaths

cd "${TOP}"

## Register all support components
dbLoadDatabase "dbd/inventory.dbd"
inventory_registerRecordDeviceDriver pdbbase

epicsEnvSet("SIGNAL","MassFlow")

## Load record instances
dbLoadTemplate("db/Reservoir.sub","SIGNAL=$(SIGNAL)")
dbLoadTemplate("db/TBx_Line.sub")
dbLoadTemplate("db/Simulation.sub")
dbLoadRecords("db/noisewaveform.db","P=Helium,R=-Sim:,NELM=102,Vs=50")
dbLoadRecords("db/SCL3_TBx_Invetory_total.vdb","SYS=SCL3, SUBSYS=-TBx:, DEV=Helium, SUBDEV=-Inventory:, NELM=96")

cd "${TOP}/iocBoot/${IOC}"
iocInit

## Start any sequence programs
#seq sncxxx,"user=ctrluser"
seq sncMakeAIDynamic,"P=Helium, R=-Sim:"
seq sncMakeTotalInventory,"SYS=SCL3, SUBSYS=-TBx:, DEV=Helium, SUBDEV=-Inventory:"
