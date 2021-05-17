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
#dbLoadRecords("db/testwaveform.vdb") #for. seq sncMakeTempWaveform

#dbLoadTemplate("${TOP}/iocBoot/${IOC}/cooldownlogic.sub", "SYS=SCL31, SUBSYS=-CDL01:, DEV=VBx02, SUBDEV=-CV7301:")

#dbLoadTemplate("${TOP}/iocBoot/${IOC}/cooldownlogic_eval_QWR.sub", "SYS=SCL31, SUBSYS=-CDL01:")
#dbLoadTemplate("${TOP}/iocBoot/${IOC}/cooldownlogic_eval_HWRA.sub", "SYS=SCL31, SUBSYS=-CDL01:")
#dbLoadTemplate("${TOP}/iocBoot/${IOC}/cooldownlogic_eval_HWRB.sub", "SYS=SCL31, SUBSYS=-CDL01:")
#dbLoadTemplate("${TOP}/iocBoot/${IOC}/cooldownlogic_eval_P2DT.sub", "SYS=SCL31, SUBSYS=-CDL01:")
#dbLoadRecords("db/TestCountPV.vdb")

#dbLoadRecords("db/CDFanoutTest.vdb","SYS=SCL3, SUBSYS=-CoolDown:")
#dbLoadRecords("db/fanout_db_field.vdb","SYS=SCL3, SUBSYS=-CoolDown:")
#dbLoadRecords("db/TempEval.vdb")

#dbLoadTemplate("${TOP}/iocBoot/${IOC}/cooldownlogic_eval_OM0_3_step1.sub", "SYS=SCL31, SUBSYS=-CDL01:")
dbLoadTemplate("${TOP}/iocBoot/${IOC}/cooldownlogic_eval_OM0.3_step1.sub", "SYS=SCL31, SUBSYS=-CDL01:")
#dbLoadTemplate("${TOP}/iocBoot/${IOC}/cooldownlogic_eval_OM0.3_step2.sub", "SYS=SCL31, SUBSYS=-CDL01:")
#dbLoadTemplate("${TOP}/iocBoot/${IOC}/cooldownlogic_eval_OM0.3_step3.sub", "SYS=SCL31, SUBSYS=-CDL01:")
#dbLoadTemplate("${TOP}/iocBoot/${IOC}/cooldownlogic_eval_OM0.3_step4.sub", "SYS=SCL31, SUBSYS=-CDL01:")
#dbLoadTemplate("${TOP}/iocBoot/${IOC}/cooldownlogic_eval_OM0.3_step5.sub", "SYS=SCL31, SUBSYS=-CDL01:")
#dbLoadTemplate("${TOP}/iocBoot/${IOC}/cooldownlogic_eval_OM0.3_step6_1.sub", "SYS=SCL31, SUBSYS=-CDL01:")
#dbLoadTemplate("${TOP}/iocBoot/${IOC}/cooldownlogic_eval_OM0.3_step6_2.sub", "SYS=SCL31, SUBSYS=-CDL01:")
#dbLoadTemplate("${TOP}/iocBoot/${IOC}/cooldownlogic_eval_OM0.3_step6_3.sub", "SYS=SCL31, SUBSYS=-CDL01:")
#dbLoadTemplate("${TOP}/iocBoot/${IOC}/cooldownlogic_eval_OM0.3_step6_4.sub", "SYS=SCL31, SUBSYS=-CDL01:")
#dbLoadTemplate("${TOP}/iocBoot/${IOC}/cooldownlogic_eval_OM0.3_step6_5.sub", "SYS=SCL31, SUBSYS=-CDL01:")
#dbLoadTemplate("${TOP}/iocBoot/${IOC}/cooldownlogic_eval_OM0.3_step6_6.sub", "SYS=SCL31, SUBSYS=-CDL01:")
#dbLoadTemplate("${TOP}/iocBoot/${IOC}/cooldownlogic_eval_HWRA.sub", "SYS=SCL31, SUBSYS=-CDL01:")
#dbLoadTemplate("${TOP}/iocBoot/${IOC}/cooldownlogic_eval_HWRB.sub", "SYS=SCL31, SUBSYS=-CDL01:")
#dbLoadTemplate("${TOP}/iocBoot/${IOC}/cooldownlogic_eval_P2DT.sub", "SYS=SCL31, SUBSYS=-CDL01:")

cd "${TOP}/iocBoot/${IOC}"
iocInit

## Start any sequence programs
#seq CDOperationSeq
#seq CDFanoutTest
#seq sncMakeTempWaveform
#seq CDFanoutLogic
