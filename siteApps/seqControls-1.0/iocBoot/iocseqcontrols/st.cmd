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
#dbLoadRecords("db/alarmDemo.db","P=ctrlslab:")
#dbLoadRecords("db/analogDemo.db","P=ctrlslab:")
#dbLoadRecords("db/analogDemo2.db","P=ctrlslab:")
#dbLoadRecords("db/analogDemo3.db","P=ctrlslab:")
dbLoadTemplate("db/sim.sub")

cd "${TOP}/iocBoot/${IOC}"
iocInit

## Start any sequence programs
#seq sncxxx,"user=ctrluser"
#seq SeqControls,"Group1PVs=ctrlslab:A_Alarm_100,Group2PVs=ctrlslab:A_Alarm_200"
#seq SeqControls,"GrpFirstPVs=ctrlslab:A_Alarm_100, GrpSecondPVs=ctrlslab:A_Alarm_200, GrpThirdPVs=ctrlslab:A_Alarm_300"
#seq SeqControlsSCL3,"C_PV=ctrlslab:A_Alarm_100"
#seq SeqControlsSCL3QWR,"C_PV=ctrlslab:A_Alarm_200"
seq SeqVBxSetCP,"GrpVBxA=SCL31-CDL01:VBx99-TT7502:SetCP, GrpVBxB=SCL31-CDL01:VBx99-PT7503:SetCP, GrpVBxC=SCL31-CDL01:VBx99-PT7501:SetCP, GrpVBxD=SCL31-CDL99:VBx99-PT7501:SetCP, GrpVBxE=SCL31-CDL01:VBx99-PT7502:SetCP, GrpVBxF=SCL31-CDL99:VBx99-PT7502:SetCP, GrpVBxG=SCL31-CDL01:VBx99-PT7401:SetCP, GrpVBxH=SCL31-CDL99:VBx99-PT7401:SetCP, GrpVBxI=SCL31-CDL01:VBx99-PT7301:SetCP, GrpVBxJ=SCL31-CDL99:VBx99-PT7301:SetCP, GrpVBxK=SCL31-CDL01:VBx99-TT7501:SetCP, GrpVBxL=SCL31-CDL99:VBx99-TT7501:SetCP, GrpVBxM=SCL31-CDL99:VBx99-PT7201:SetCP"
