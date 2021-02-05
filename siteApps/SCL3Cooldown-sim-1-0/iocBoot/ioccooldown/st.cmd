#!../../bin/linux-x86_64/cooldown

#- You may have to change cooldown to something else
#- everywhere it appears in this file

< envPaths

cd "${TOP}"

## Register all support components
dbLoadDatabase "dbd/cooldown.dbd"
cooldown_registerRecordDeviceDriver pdbbase

## Load record instances
#dbLoadRecords("db/xxx.db","user=ctrluser")
#dbLoadRecords("db/check_condition_press.vdb")
#dbLoadRecords("db/check_condition_heline_circulation.vdb", "SYS=SCL31, SUBSYS=CDL01, DEV=VBx02")
#dbLoadTemplate("${TOP}/iocBoot/${IOC}/scl3_seq_simulate.sub")
dbLoadTemplate("${TOP}/iocBoot/${IOC}/cooldownlogic.sub", "SYS=SCL31, SUBSYS=-CDL01:")
#dbLoadRecords("db/pressure_eval.vdb", "SYS=SCL31,SUBSYS=-CDL01:, DEV=VBx02, SUBDEV=-PT7301:")
#dbLoadRecords("db/PressEval.vdb", "SYS=SCL31,SUBSYS=-CDL01:, DEV=VBx02, SUBDEV=-PT7301:")
#dbLoadRecords("db/PressEval2.vdb", "SYS=SCL31,SUBSYS=-CDL01:, DEV=VBx02, SUBDEV=-PT7301:")
#dbLoadTemplate("${TOP}/iocBoot/${IOC}/presseval.sub", "SYS=SCL31, SUBSYS=-CDL01:")

cd "${TOP}/iocBoot/${IOC}"
iocInit

## Start any sequence programs
#seq sncxxx,"user=ctrluser"
