#!../../bin/linux-x86_64/gbintstat

#- You may have to change gbintstat to something else
#- everywhere it appears in this file

< envPaths

cd "${TOP}"

## Register all support components
dbLoadDatabase "dbd/gbintstat.dbd"
gbintstat_registerRecordDeviceDriver pdbbase

## Load record instances
#dbLoadRecords("db/xxx.db","user=ctrluser")
#dbLoadTemplate("db/GBIntStat.sub, SYS=SCL3,SUBSYS=-BL01:,DEV=CM01,SUB=-8101:,SIGNAL=IntStat,SUBDEV=PT,NUM=1")
##Example
#dbLoadTemplate("db/GBIntStat.sub")

##Real
dbLoadTemplate("db/GBIntStat_CMs.sub")
dbLoadTemplate("db/GBIntStat_VBx.sub")
dbLoadTemplate("db/GBIntStat_EBx.sub")
dbLoadTemplate("db/GBCPStat.sub")

##After 0725
dbLoadTemplate("db/GBCURStat_Each_stat.sub")

##Before 0721
#dbLoadTemplate("db/GBCURStat.sub")

##Test
#dbLoadRecords("db/BitWiseTest.vdb")
#dbLoadRecords("db/GBCURStat.vdb", "SYS=SCL3, SUBSYS=-BL01:, DEV=VBx02, SUBDEV=-CV8401:")

cd "${TOP}/iocBoot/${IOC}"
iocInit

## Start any sequence programs
#seq sncxxx,"user=ctrluser"
