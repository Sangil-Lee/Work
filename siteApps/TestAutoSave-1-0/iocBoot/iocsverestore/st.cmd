#!../../bin/linux-x86_64/iocsverestore

#- You may have to change iocsverestore to something else
#- everywhere it appears in this file

< envPaths

cd "${TOP}"

## Register all support components
dbLoadDatabase "dbd/iocsverestore.dbd"
iocsverestore_registerRecordDeviceDriver pdbbase

## Load record instances
dbLoadTemplate "db/user.substitutions"
dbLoadRecords "db/iocsverestoreVersion.db", "user=ctrluser"
dbLoadRecords "db/dbSubExample.db", "user=ctrluser"

< "${TOP}/iocBoot/${IOC}"/save_restore.cmd

#save_restoreSet_status_prefix("save_restore:")
#set_savefile_path("$(TOP)/iocBoot/$(IOC)", "save_restore")
#set_requestfile_path("$(TOP)/iocBoot/$(IOC)", "save_restore")
##set_pass0_restoreFile("save_restore.sav", "user=ctrluser")
#save_restoreSet_NumSeqFiles(3) 
#save_restoreSet_SeqPeriodInSeconds(600) 
#set_pass1_restoreFile("save_restore.sav", "user=ctrluser")

#- Set this to see messages from mySub
#-var mySubDebug 1

#- Run this to trace the stages of iocInit
#-traceIocInit

cd "${TOP}/iocBoot/${IOC}"
iocInit
#makeAutosaveFileFromDbInfo("save_restore.req", "autosaveFields") 
create_monitor_set("save_restore.req", 1, "user=ctrluser")

## Start any sequence programs
#seq sncExample, "user=ctrluser"
