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
dbLoadTemplate("db/PressR.sub")
dbLoadTemplate("db/Additional.sub")
dbLoadTemplate("db/FlowR.sub")
dbLoadTemplate("db/LevelR.sub")
dbLoadTemplate("db/TempR.sub")
dbLoadTemplate("db/cooldown03.sub")
#dbLoadTemplate("db/cooldown04.sub")
#dbLoadTemplate("db/cooldown12.sub")
#dbLoadTemplate("db/cooldown14.sub")
#dbLoadTemplate("db/cooldown15.sub")
#dbLoadTemplate("db/cooldown21.sub")
#dbLoadTemplate("db/cooldown51.sub")
#dbLoadTemplate("db/cooldown52.sub")
#dbLoadTemplate("db/cooldown53.sub")
dbLoadTemplate("db/subhePAK.sub")
dbLoadTemplate("db/subHePAKWF.sub")

cd "${TOP}/iocBoot/${IOC}"
iocInit
## Start any sequence programs
#seq sncxxx,"user=ctrluser"
seq sncCooldown03, "SYS=SCL3,SUBSYS=-CD:,DEV=Logic,SUBDEV=-ADD:"

