#!../../bin/linux-x86_64/kobramagnet

#- You may have to change kobramagnet to something else
#- everywhere it appears in this file

< envPaths

cd "${TOP}"

epicsEnvSet "STREAM_PROTOCOL_PATH" "../../proto"

## Register all support components
dbLoadDatabase "dbd/kobramagnet.dbd"
kobramagnet_registerRecordDeviceDriver pdbbase

######Danfysik System 9700######
## MOXA, Port Configuration="115200, Data=8bit, Stopbit=1bit, No Parity bit, No Flow Control"

#drvAsynIPPortConfigure("SYS9700", "192.168.0.??:4001", 0, 0, 0)	#MOXA port to Danfysik9700 MPS

#asynOctetSetInputEos   "SYS9700", 0, "\n\r"
#asynOctetSetOutputEos  "SYS9700", 0, "\r"

# "\r\n" is only valid for the simulator
# , because we want to use Telnet connection also.
#asynOctetSetOutputEos("SYS9700", 0, "\r\n")

## Load record instances
#dbLoadTemplate("db/danfysik.substitutions", "DEVICE=${SYS}${SUBSYS}${DEV}{SUBDEV}")

cd "${TOP}/iocBoot/${IOC}"
iocInit

## Start any sequence programs
#seq sncxxx,"user=ctrluser"
