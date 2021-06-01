#!../../bin/linux-arm/kobramagnet
#- You may have to change kobramagnet to something else
#- everywhere it appears in this file

< envPaths

cd "${TOP}"

epicsEnvSet "STREAM_PROTOCOL_PATH" "../../proto"

## Register all support components
dbLoadDatabase "dbd/kobramagnet.dbd"
kobramagnet_registerRecordDeviceDriver pdbbase

######Danfysik System 9700 Example######
## MOXA, Port Configuration="RFC2217, 115200, Data=8bit, Stopbit=1bit, No Parity bit, No Flow Control"
#drvAsynIPPortConfigure("SYS9700", "192.168.131.124:4001", 0, 0, 0)	#MOXA port to Danfysik9700 MPS

######KoBRA Magnet Power Supply######
drvAsynIPPortConfigure ("Q9", "192.168.131.125:4003", 0, 0, 0)
drvAsynIPPortConfigure("Q10", "192.168.131.125:4004", 0, 0, 0)

## According to KoBRA Maganet Type
## Load record instances
dbLoadTemplate("${TOP}/iocBoot/${IOC}/danfysik_SW.sub", "DEVICE=KOBRA-MAG:Q9-PS:,   port=Q9")
dbLoadTemplate("${TOP}/iocBoot/${IOC}/danfysik_SW.sub", "DEVICE=KOBRA-MAG:Q10-PS:,  port=Q10")

## According to KoBRA Maganet Type
dbLoadRecords("db/dbAutoRampup.db", "DEVICE=KOBRA-MAG:Q9-PS")
dbLoadRecords("db/dbAutoRampup.db", "DEVICE=KOBRA-MAG:Q10-PS")

## According to KoBRA Maganet Type
dbLoadRecords("db/MGField2Cur_Q9.vdb",   "DEVICE=KOBRA-MAG:Q9-PS")
dbLoadRecords("db/MGField2Cur_Q10P.vdb", "DEVICE=KOBRA-MAG:Q10-PS")


cd "${TOP}/iocBoot/${IOC}"
iocInit

## Start any sequence programs
#seq sncxxx,"user=ctrluser"

seq sncAutoRampup,"DEVICE=KOBRA-MAG:Q9-PS,  TARGET=KOBRA-MAG:Q9-PS:SetCurrent"
seq sncAutoRampup,"DEVICE=KOBRA-MAG:Q10-PS, TARGET=KOBRA-MAG:Q10-PS:SetCurrent"
