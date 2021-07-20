#!../../bin/linux-x86_64/valveEval

#- You may have to change valveEval to something else
#- everywhere it appears in this file

< envPaths

cd "${TOP}"

## Register all support components
dbLoadDatabase "dbd/valveEval.dbd"
valveEval_registerRecordDeviceDriver pdbbase

## Load record instances
#dbLoadRecords("db/xxx.db","user=ctrluser")
dbLoadRecords("db/OM031AdditionalPV.vdb")
dbLoadTemplate("${TOP}/iocBoot/${IOC}/cdom141_6.sub")
dbLoadTemplate("${TOP}/iocBoot/${IOC}/cooldownlogic_eval_OM0.3_step1.sub", "SYS=SCL31, SUBSYS=-CDL01:")
dbLoadRecords("db/XVSetPoint.vdb", "SYS=SCL31, SUBSYS=-Test:, DEV=XVValve, SUBDEV=-SetPt:")

cd "${TOP}/iocBoot/${IOC}"
iocInit

## Start any sequence programs
#seq sncxxx,"user=ctrluser"
seq CDOM031Logic
#seq CDOMAutoRegulate
