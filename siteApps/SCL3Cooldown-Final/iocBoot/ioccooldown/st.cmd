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

### for testing.
#dbLoadTemplate("db/PressR.sub")
#dbLoadTemplate("db/FlowR.sub")
#dbLoadTemplate("db/LevelR.sub")
#dbLoadTemplate("db/TempR.sub")

#dbLoadTemplate("db/Additional.sub")
#dbLoadTemplate("db/cooldown03.sub")
#dbLoadTemplate("db/cooldown04.sub")
#dbLoadTemplate("db/cooldown12.sub")
#dbLoadTemplate("db/cooldown14.sub")
#dbLoadTemplate("db/cooldown15.sub")
#dbLoadTemplate("db/cooldown21.sub")
#dbLoadTemplate("db/cooldown51.sub")
#dbLoadTemplate("db/cooldown52.sub")
#dbLoadTemplate("db/cooldown53.sub")
#dbLoadTemplate("db/subhePAK.sub")
#dbLoadTemplate("db/subHePAKWF.sub")


###for testing 0903
dbLoadTemplate("db/Additional.sub")
dbLoadTemplate("db/PressR_1.sub")
#dbLoadTemplate("db/cooldown14_1.sub")
#dbLoadTemplate("db/cooldown14_QWR3.sub")
#dbLoadTemplate("db/cooldown15_QWR3.sub")

#dbLoadTemplate("db/cooldown14_QWR5.sub")
#dbLoadTemplate("db/cooldown15_QWR5.sub")

#dbLoadTemplate("db/cooldown14_QWR7.sub")
#dbLoadTemplate("db/cooldown15_QWR7.sub")

#dbLoadTemplate("db/cooldown14_QWR8.sub")
#dbLoadTemplate("db/cooldown15_QWR8.sub")

dbLoadTemplate("db/cooldown14_QWR9.sub")
dbLoadTemplate("db/cooldown15_QWR9.sub")

dbLoadTemplate("db/TT8001_Target.sub")
dbLoadTemplate("db/TT8404_Min.sub")
dbLoadTemplate("db/TT8403_Min.sub")
dbLoadRecords("db/Linear_regression.vdb")

### for CM1.4 ~ 1.5 Testing
#dbLoadTemplate("db/SkipCyclePVs.sub")

cd "${TOP}/iocBoot/${IOC}"
iocInit
## Start any sequence programs
#seq sncxxx,"user=ctrluser"
#seq sncCooldown03, "SYS=SCL3,SUBSYS=-CD:,DEV=Logic,SUBDEV=-ADD:"
seq sncMakeTempWaveform
#dbpf SCL31-BL01:CM11-CV8401:Valve.INPA "SCL31-BL01:CM11-CV8401:SetManu CPP"
#dbpf SCL31-BL01:CM12-CV8401:Valve.INPA "SCL31-BL01:CM12-CV8401:SetManu CPP"
#dbpf SCL31-CDL01:VBx12-XV7301:Valve.INPA "SCL31-CDL01:VBx12-XV7301:SetManuOpen CPP"


