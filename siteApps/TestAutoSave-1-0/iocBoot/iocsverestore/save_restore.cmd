save_restoreSet_status_prefix("save_restore:")
set_savefile_path("$(TOP)/iocBoot/$(IOC)", "save_restore")
set_requestfile_path("$(TOP)/iocBoot/$(IOC)", "save_restore")
#set_pass0_restoreFile("save_restore.sav", "user=ctrluser")
save_restoreSet_NumSeqFiles(3) 
save_restoreSet_SeqPeriodInSeconds(600) 
set_pass1_restoreFile("save_restore.sav", "user=ctrluser")

