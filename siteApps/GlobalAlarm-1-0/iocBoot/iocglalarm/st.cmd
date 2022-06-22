#!../../bin/linux-x86_64/glalarm

#- You may have to change glalarm to something else
#- everywhere it appears in this file

< envPaths

cd "${TOP}"

## Register all support components
dbLoadDatabase "dbd/glalarm.dbd"
glalarm_registerRecordDeviceDriver pdbbase

## Load record instances
#dbLoadRecords("db/xxx.db","user=ctrluser")
dbLoadRecords("db/globalAlarm.vdb","SYS=SCL3, SUBSYS=-ALL:, DEV=IntWF")
dbLoadTemplate("db/sim_pv.sub")

cd "${TOP}/iocBoot/${IOC}"
iocInit

## Start any sequence programs
seq sncGLInterlockWF, "SYS=SCL3, SUBSYS=CDL01, SUBSYS2=CDL02, SUBSYS3=CDL03"
