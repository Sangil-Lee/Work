#!../../bin/linux-x86_64/phasend

#- You may have to change phasend to something else
#- everywhere it appears in this file

< envPaths

cd "${TOP}"

## Register all support components
dbLoadDatabase "dbd/phasend.dbd"
phasend_registerRecordDeviceDriver pdbbase

## Load record instances
#dbLoadRecords("db/xxx.db","user=ctrluser")
dbLoadRecords("db/aiOutLinkTest.vdb")
#dbLoadRecords("db/sncTempWF.vdb")
#dbLoadTemplate("db/sncTempWF.sub","SYS=SCL3,SUBSYS=:, DEV=Cooldown, SUBDEV=:")

dbLoadRecords("db/sncTempEvalWF.vdb")
dbLoadTemplate("db/sncTempEvalWF.sub","SYS=SCL3,SUBSYS=:, DEV=Cooldown, SUBDEV=-Temp:")

dbLoadRecords("db/sncPressEvalWF.vdb")
dbLoadTemplate("db/sncPressEvalWF.sub","SYS=SCL3,SUBSYS=:, DEV=Cooldown, SUBDEV=-Press:")

#dbLoadRecords("db/phaseEndCond_TempMon.vdb","SYS=SCL3,SUBSYS=:, DEV=Cooldown, SUBDEV=:, COUNT=5")
dbLoadRecords("db/PhaseEnd.vdb","SYS=SCL3,SUBSYS=:, DEV=Cooldown, SUBDEV=:, COUNT=5")
#dbLoadRecords("db/PhaseEnd.vdb","SYS=SCL3,SUBSYS=:, DEV=Cooldown, SUBDEV=:, COUNT=20")

dbLoadTemplate("db/scl3_seq_simulate_temp.sub")
dbLoadTemplate("db/scl3_seq_simulate_press.sub")
dbLoadTemplate("db/scl3_seq_simulate_press_others.sub")


#dbLoadTemplate("db/scl3_seq_simulate.sub")
#dbLoadTemplate("${TOP}/iocBoot/${IOC}/scl3_seq_simulate.sub")
#dbLoadRecords("db/Test.vdb")

cd "${TOP}/iocBoot/${IOC}"
iocInit
## Start any sequence programs
#seq sncxxx,"user=ctrluser"
#seq sncTempWF,"SYS=SCL3,SUBSYS=:, DEV=Cooldown, SUBDEV=:"
seq sncTempEvalWF,"SYS=SCL3,SUBSYS=:, DEV=Cooldown, SUBDEV=-Temp:"
seq sncPressEvalWF,"SYS=SCL3,SUBSYS=:, DEV=Cooldown, SUBDEV=-Press:"
seq sncPhaseEnd,"SYS=SCL3,SUBSYS=:, DEV=Cooldown, SUBDEV=:"

