#!../../bin/linux-x86_64/dcct
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
epicsEnvSet("IOCNAME", "alsu-dcct")
epicsEnvSet("IOC", "iocalsu-dcct")
#-
epicsEnvSet("P", "HostA:")
epicsEnvSet("R", "Desk:")

dbLoadDatabase "dbd/dcct.dbd"
dcct_registerRecordDeviceDriver pdbbase

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
#-asSetFilename("$(DB_TOP)/access_securityalsu-dcct.acf")

cd "${TOP}/iocBoot/${IOC}"

epicsEnvSet("PORT1",      "AABBCCDD")
epicsEnvSet("PORT1_IP",   "xxx.xxx.xxx.xxx")
epicsEnvSet("PORT1_PORT", "xxxx")
iocshLoad("$(IOCSH_LOCAL_TOP)/dcct.iocsh", "P=$(P),R=$(R),DATABASE_TOP=$(DB_TOP),PORT=$(PORT1),IPADDR=$(PORT1_IP),IPPORT=$(PORT1_PORT)")



iocInit

#- epicsEnvShow > /vxboot/PVenv/${IOC}.softioc
#- dbl > /vxboot/PVnames/${IOC}
#- iocshLoad("$(IOCSH_TOP)/after_iocInit.iocsh", "IOC=$(IOC),TRAGET_TOP=/vxboot")

## Start any sequence programs
#seq sncxxx,"user=ctrluser"
