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
dbLoadRecords("db/alarmDemo.db","P=ctrlslab:")
dbLoadRecords("db/analogDemo.db","P=ctrlslab:")
dbLoadRecords("db/analogDemo2.db","P=ctrlslab:")
dbLoadRecords("db/analogDemo3.db","P=ctrlslab:")

cd "${TOP}/iocBoot/${IOC}"
iocInit

## Start any sequence programs
#seq sncxxx,"user=ctrluser"
#seq SeqControls,"Group1PVs=ctrlslab:A_Alarm_100,Group2PVs=ctrlslab:A_Alarm_200"
seq SeqControls,"GrpFirstPVs=ctrlslab:A_Alarm_100, GrpSecondPVs=ctrlslab:A_Alarm_200, GrpThirdPVs=ctrlslab:A_Alarm_300"
#seq SeqControlsSCL3,"C_PV=ctrlslab:A_Alarm_100"
#seq SeqControlsSCL3QWR,"C_PV=ctrlslab:A_Alarm_200"
