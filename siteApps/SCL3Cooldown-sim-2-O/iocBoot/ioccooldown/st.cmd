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

#dbLoadTemplate("${TOP}/iocBoot/${IOC}/cooldownlogic_eval_OM0.3_step1.sub", "SYS=SCL31, SUBSYS=-CDL01:")
#dbLoadTemplate("${TOP}/iocBoot/${IOC}/cooldownlogic_eval_OM0.3_step2.sub", "SYS=SCL31, SUBSYS=-CDL01:")
#dbLoadTemplate("${TOP}/iocBoot/${IOC}/cooldownlogic_eval_OM0.3_step4.sub", "SYS=SCL31, SUBSYS=-CDL01:")
#dbLoadTemplate("${TOP}/iocBoot/${IOC}/cooldownlogic_eval_OM0.3_step5.sub", "SYS=SCL31, SUBSYS=-CDL01:")
#dbLoadTemplate("${TOP}/iocBoot/${IOC}/cooldownlogic_eval_OM0.3_step6-1.sub", "SYS=SCL31, SUBSYS=-CDL01:")
#dbLoadTemplate("${TOP}/iocBoot/${IOC}/cooldownlogic_eval_OM0.3_step6-2.sub", "SYS=SCL31, SUBSYS=-CDL01:")
#dbLoadTemplate("${TOP}/iocBoot/${IOC}/cooldownlogic_eval_OM0.3_step6-3.sub", "SYS=SCL31, SUBSYS=-CDL01:")
#dbLoadTemplate("${TOP}/iocBoot/${IOC}/cooldownlogic_eval_OM0.3_step6-4.sub", "SYS=SCL31, SUBSYS=-CDL01:")
#dbLoadTemplate("${TOP}/iocBoot/${IOC}/cooldownlogic_eval_OM0.3_step6-5.sub", "SYS=SCL31, SUBSYS=-CDL01:")
#dbLoadTemplate("${TOP}/iocBoot/${IOC}/cooldownlogic_eval_OM0.3_step6-6.sub", "SYS=SCL31, SUBSYS=-CDL01:")
#dbLoadTemplate("${TOP}/iocBoot/${IOC}/cooldownlogic_eval_OM0.3_step6-7.sub", "SYS=SCL31, SUBSYS=-CDL01:")
#dbLoadTemplate("${TOP}/iocBoot/${IOC}/cooldownlogic_eval_OM0.4_step1_step2.sub", "SYS=SCL31, SUBSYS=-CDL01:")
#dbLoadTemplate("${TOP}/iocBoot/${IOC}/cooldownlogic_eval_OM0.4_step3-1.sub", "SYS=SCL31, SUBSYS=-CDL01:")
#dbLoadTemplate("${TOP}/iocBoot/${IOC}/cooldownlogic_eval_OM0.4_step3-2.sub", "SYS=SCL31, SUBSYS=-CDL01:")
#dbLoadTemplate("${TOP}/iocBoot/${IOC}/cooldownlogic_eval_OM0.4_step3-3.sub", "SYS=SCL31, SUBSYS=-CDL01:")
#dbLoadTemplate("${TOP}/iocBoot/${IOC}/cooldownlogic_eval_OM0.4_step3-4.sub", "SYS=SCL31, SUBSYS=-CDL01:")
#dbLoadTemplate("${TOP}/iocBoot/${IOC}/cooldownlogic_eval_OM0.4_step3-5.sub", "SYS=SCL31, SUBSYS=-CDL01:")
#dbLoadTemplate("${TOP}/iocBoot/${IOC}/cooldownlogic_eval_OM0.4_step3-6_step4.sub", "SYS=SCL31, SUBSYS=-CDL01:")
#dbLoadTemplate("${TOP}/iocBoot/${IOC}/cooldownlogic_eval_OM1.2_step1.sub", "SYS=SCL31, SUBSYS=-CDL01:")
#dbLoadTemplate("${TOP}/iocBoot/${IOC}/cooldownlogic_eval_OM1.2_step2.sub", "SYS=SCL31, SUBSYS=-CDL01:")
#dbLoadTemplate("${TOP}/iocBoot/${IOC}/cooldownlogic_eval_OM1.4_step1.sub", "SYS=SCL31, SUBSYS=-CDL01:")
#dbLoadTemplate("${TOP}/iocBoot/${IOC}/cooldownlogic_eval_OM1.4_step2.sub", "SYS=SCL31, SUBSYS=-CDL01:")
#dbLoadTemplate("${TOP}/iocBoot/${IOC}/cooldownlogic_eval_OM1.4_step3.sub", "SYS=SCL31, SUBSYS=-CDL01:")
#dbLoadTemplate("${TOP}/iocBoot/${IOC}/cooldownlogic_eval_OM1.4_step4.sub", "SYS=SCL31, SUBSYS=-CDL01:")
#dbLoadTemplate("${TOP}/iocBoot/${IOC}/cooldownlogic_eval_OM1.4_step5.sub", "SYS=SCL31, SUBSYS=-CDL01:")
#dbLoadTemplate("${TOP}/iocBoot/${IOC}/cooldownlogic_eval_OM1.4_step6.sub", "SYS=SCL31, SUBSYS=-CDL01:")
#dbLoadTemplate("${TOP}/iocBoot/${IOC}/cooldownlogic_eval_OM1.4_step7.sub", "SYS=SCL31, SUBSYS=-CDL01:")
#dbLoadTemplate("${TOP}/iocBoot/${IOC}/cooldownlogic_eval_OM1.5_HWR_step1.sub", "SYS=SCL31, SUBSYS=-CDL01:")
#dbLoadTemplate("${TOP}/iocBoot/${IOC}/cooldownlogic_eval_OM1.5_HWR_step2_step3.sub", "SYS=SCL31, SUBSYS=-CDL01:")
#dbLoadTemplate("${TOP}/iocBoot/${IOC}/cooldownlogic_eval_OM1.5_HWR_step4.sub", "SYS=SCL31, SUBSYS=-CDL01:")
#dbLoadTemplate("${TOP}/iocBoot/${IOC}/cooldownlogic_eval_OM1.5_HWR_step5.sub", "SYS=SCL31, SUBSYS=-CDL01:")
#dbLoadTemplate("${TOP}/iocBoot/${IOC}/cooldownlogic_eval_OM1.5_QWR_step1_step2_step3.sub", "SYS=SCL31, SUBSYS=-CDL01:")
#dbLoadTemplate("${TOP}/iocBoot/${IOC}/cooldownlogic_eval_OM1.5_QWR_step4.sub", "SYS=SCL31, SUBSYS=-CDL01:")
#dbLoadTemplate("${TOP}/iocBoot/${IOC}/cooldownlogic_eval_OM1.5_QWR_step5.sub", "SYS=SCL31, SUBSYS=-CDL01:")
#dbLoadTemplate("${TOP}/iocBoot/${IOC}/cooldownlogic_eval_OM2.1_step1_step2.sub", "SYS=SCL31, SUBSYS=-CDL01:")
#dbLoadTemplate("${TOP}/iocBoot/${IOC}/cooldownlogic_eval_OM5.1_step1.sub", "SYS=SCL31, SUBSYS=-CDL01:")
#dbLoadTemplate("${TOP}/iocBoot/${IOC}/cooldownlogic_eval_OM5.1_step2.sub", "SYS=SCL31, SUBSYS=-CDL01:")
#dbLoadTemplate("${TOP}/iocBoot/${IOC}/cooldownlogic_eval_OM5.2.sub", "SYS=SCL31, SUBSYS=-CDL01:")
#dbLoadTemplate("${TOP}/iocBoot/${IOC}/cooldownlogic_eval_OM5.3_step1_step3_step4.sub", "SYS=SCL31, SUBSYS=-CDL01:")
#dbLoadTemplate("${TOP}/iocBoot/${IOC}/cooldownlogic_eval_OM5.3_step5.sub", "SYS=SCL31, SUBSYS=-CDL01:")

dbLoadTemplate("${TOP}/iocBoot/${IOC}/cooldownlogic_eval_OM5.3_step6.sub", "SYS=SCL31, SUBSYS=-CDL01:")

#
#dbLoadTemplate("${TOP}/iocBoot/${IOC}/cooldownlogic_eval_EM5.1_step1.sub", "SYS=SCL31, SUBSYS=-CDL01:")
#dbLoadTemplate("${TOP}/iocBoot/${IOC}/cooldownlogic_eval_EM5.1_step2.sub", "SYS=SCL31, SUBSYS=-CDL01:")
#dbLoadTemplate("${TOP}/iocBoot/${IOC}/cooldownlogic_eval_EM5.1_step3.sub", "SYS=SCL31, SUBSYS=-CDL01:")
#dbLoadTemplate("${TOP}/iocBoot/${IOC}/cooldownlogic_eval_EM5.1_step4.sub", "SYS=SCL31, SUBSYS=-CDL01:")
#
#dbLoadTemplate("${TOP}/iocBoot/${IOC}/cooldownlogic_eval_EM5.2_HWR.sub", "SYS=SCL31, SUBSYS=-CDL01:")
#dbLoadTemplate("${TOP}/iocBoot/${IOC}/cooldownlogic_eval_EM5.2_QWR.sub", "SYS=SCL31, SUBSYS=-CDL01:")
#
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
