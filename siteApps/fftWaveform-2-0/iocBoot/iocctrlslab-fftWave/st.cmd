#!../../bin/linux-x86_64/fftWave
#-
< envPaths
#-
cd "${TOP}"
#-
#-epicsEnvSet("EPICS_CA_ADDR_LIST","xxx.xxx.xxx.255")
#-epicsEnvSet("EPICS_CA_AUTO_ADDR_LIST","YES")
#-
epicsEnvSet("DB_TOP",               "$(TOP)/db")
epicsEnvSet("STREAM_PROTOCOL_PATH", "$(DB_TOP)")
epicsEnvSet("IOCSH_LOCAL_TOP",      "$(TOP)/iocsh")
epicsEnvSet("IOCSH_TOP",            "$(EPICS_BASE)/../modules/iocsh/iocsh")
#-
epicsEnvSet("ENGINEER",  "ctrluser")
epicsEnvSet("LOCATION",  "")
epicsEnvSet("WIKI", "")
#- 
epicsEnvSet("IOCNAME", "ctrlslab-fftWave")
epicsEnvSet("IOC", "iocctrlslab-fftWave")
#-
epicsEnvSet("P", "ctrlslab")
epicsEnvSet("R", "-fftWave:")

dbLoadDatabase "dbd/fftWave.dbd"
fftWave_registerRecordDeviceDriver pdbbase
var fftDebug 0

#-
#- The following termination defintion should be in st.cmd or .iocsh. 
#- Mostly, it should be .iocsh file. Please don't use them within .proto file
#-
#- <0x0d> \r
#= <0x0a> \n
#- asynOctetSetInputEos($(PORT), 0, "\r")
#- asynOctetSetOutputEos($(PORT), 0, "\r")

#-
#- iocshLoad("$(IOCSH_TOP)/als_default.iocsh")
#- iocshLoad("$(IOCSH_TOP)/iocLog.iocsh",    "IOCNAME=$(IOCNAME), LOG_INET=$(LOG_DEST), LOG_INET_PORT=$(LOG_PORT)")
#-# Load record instances
#- iocshLoad("$(IOCSH_TOP)/iocStats.iocsh",  "IOCNAME=$(IOCNAME), DATABASE_TOP=$(DB_TOP)")
#- iocshLoad("$(IOCSH_TOP)/reccaster.iocsh", "IOCNAME=$(IOCNAME), DATABASE_TOP=$(DB_TOP)")
#- iocshLoad("$(IOCSH_TOP)/caPutLog.iocsh",  "IOCNAME=$(IOCNAME), LOG_INET=$(LOG_DEST), LOG_INET_PORT=$(LOG_PORT)")
#- iocshLoad("$(IOCSH_TOP)/autosave.iocsh", "AS_TOP=$(TOP),IOCNAME=$(IOCNAME),DATABASE_TOP=$(DB_TOP),SEQ_PERIOD=60")

#- access control list
#-asSetFilename("$(DB_TOP)/access_securityctrlslab-fftWave.acf")

cd "${TOP}/iocBoot/${IOC}"

#epicsEnvSet("PORT1",      "AABBCCDD")
#epicsEnvSet("PORT1_IP",   "xxx.xxx.xxx.xxx")
#epicsEnvSet("PORT1_PORT", "xxxx")

### Python Test
var fftPyVerify 1
iocshLoad("$(IOCSH_LOCAL_TOP)/fftWave_Python.iocsh", "P=$(P),R=$(R),DATABASE_TOP=$(DB_TOP),NELM=2048,TIME_N=1024,FREQ_N=513")

## ASUB Test
#iocshLoad("$(IOCSH_LOCAL_TOP)/fftWave.iocsh", "P=$(P),R=$(R),DATABASE_TOP=$(DB_TOP),NELM=1024,TIME_N=1024,FREQ_N=513")

iocInit

#- epicsEnvShow > /vxboot/PVenv/${IOC}.softioc
#- dbl > /vxboot/PVnames/${IOC}
#- iocshLoad("$(IOCSH_TOP)/after_iocInit.iocsh", "IOC=$(IOC),TRAGET_TOP=/vxboot")

## Start any sequence programs
#seq sncxxx,"user=ctrluser"
