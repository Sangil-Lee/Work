#!../../bin/linux-x86_64/llrf

# Following must be added for many board support packages
#cd <full path to target bin directory>

< envPaths 

cd ${TOP}

dbLoadDatabase "dbd/llrf.dbd"
llrf_registerRecordDeviceDriver(pdbbase) 

#llrfConfig(0, "192.168.1.100", 3600)
<  iocBoot/ioc/llrf_config
dbLoadTemplate("db/db.template")
dbLoadRecords("db/llrfDev.db", "D=$(DEVICE)")
dbLoadRecords("db/limits.db", "D=$(DEVICE)")

cd ${STARTUP}

< save_restore.cmd

iocInit()

create_monitor_set("save_settings.req", "30", "P=$(DEVICE)")
create_monitor_set("save_ipsettings.req", "4", "P=$(DEVICE)")

