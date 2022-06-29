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
#dbLoadRecords("db/globalAlarm.vdb","SYS=SCL3, SUBSYS=-ALL:, DEV=IntWF")
#dbLoadRecords("db/globalAlarm_test.vdb","SYS=SCL3, SUBSYS=-ALL:, DEV=IntWF")

#dbLoadRecords("db/gl_Interlock_XV7502_test.vdb","SYS=SCL3, SUBSYS=-ALL:, DEV=IntWF")
#dbLoadRecords("db/gl_Interlock_XV7301_test.vdb","SYS=SCL3, SUBSYS=-ALL:, DEV=IntWF")

dbLoadRecords("db/gl_Interlock_XV7502.vdb","SYS=SCL3, SUBSYS=-ALL:, DEV=IntWF")
dbLoadRecords("db/gl_Interlock_XV7301.vdb","SYS=SCL3, SUBSYS=-ALL:, DEV=IntWF")

dbLoadTemplate("db/setmanuopen.sub")
dbLoadTemplate("db/xv7502_xv7301_manuint.sub")

##Selt Simulation
#dbLoadTemplate("db/sim_pv.sub")
#dbLoadTemplate("db/sim_pv_flnk.sub")

cd "${TOP}/iocBoot/${IOC}"
iocInit

## Start any sequence programs
####Simulation
#seq sncGLInterlockWF, "SYS=SCL3, SUBSYS=CDL01, SUBSYS2=CDL02, SUBSYS3=CDL03"

###Real Global Interlock(XV7502, XV7301, QWR)
seq sncGLInterlockWF, "SYS=SCL31,SYS2=SCL32, SUBSYS=CDL01, SUBSYS2=CDL02, SUBSYS3=CDL03"
