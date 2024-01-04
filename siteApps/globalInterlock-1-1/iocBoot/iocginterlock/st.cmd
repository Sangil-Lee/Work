#!../../bin/linux-x86_64/ginterlock

#- You may have to change ginterlock to something else
#- everywhere it appears in this file

< envPaths

epicsEnvSet("EPICS_CA_ADDR_LIST", "192.168.80.44")
epicsEnvSet("EPICS_CA_AUTO_ADDR_LIST", "no")

cd "${TOP}"

## Register all support components
dbLoadDatabase "dbd/ginterlock.dbd"
ginterlock_registerRecordDeviceDriver pdbbase

## Load record instances
#dbLoadRecords("db/xxx.db","user=ctrluser")
#dbLoadTemplate("db/biSim.sub")
#dbLoadTemplate("db/aiSim.sub")
###dbLoadTemplate("db/GInt.sub")

###QWR
##DB Tested
dbLoadTemplate("db/globalIntWF.sub")
dbLoadTemplate("db/globalInt_PT7503.sub")
dbLoadTemplate("db/globalInt_PT7301.sub")
dbLoadTemplate("db/globalInt_PT7503PT7307.sub")
dbLoadTemplate("db/globalInt_PT7503CV.sub")
dbLoadTemplate("db/globalInt_PT7301CV.sub")
dbLoadTemplate("db/globalInt_PT7301PT7307.sub")
#
#####HWR    
##DB Tested
dbLoadTemplate("db/globalInt_PT7502PT7201_HWR.sub")    
dbLoadTemplate("db/globalInt_PT7502_HWR.sub")    
dbLoadTemplate("db/globalInt_PT7502CV_HWR.sub")    
dbLoadTemplate("db/globalInt_PT7201CV_HWR.sub")    
dbLoadTemplate("db/globalInt_PT7201_HWR.sub")
#
###    
#####SSR    
##DB Tested
dbLoadTemplate("db/globalInt_PT7502PT7201_SSR.sub")    
dbLoadTemplate("db/globalInt_PT7502CV_SSR.sub")    
dbLoadTemplate("db/globalInt_PT7502_SSR.sub")    
dbLoadTemplate("db/globalInt_PT7201CV_SSR.sub")    
dbLoadTemplate("db/globalInt_PT7201_SSR.sub")    
###    
#
#####LTS    
##DB Tested
dbLoadTemplate("db/globalInt_PT7502PT7301_LTS.sub")    
dbLoadTemplate("db/globalInt_PT7502CV_LTS.sub")    
dbLoadTemplate("db/globalInt_PT7502_LTS.sub")    
dbLoadTemplate("db/globalInt_PT7301CV_LTS.sub")    
dbLoadTemplate("db/globalInt_PT7301_LTS.sub")    
##    
#####EBx    
##DB Tested
dbLoadTemplate("db/globalInt_EBx.sub")

#
####TBx    
##DB Tested
dbLoadTemplate("db/globalInt_TBx.sub")    
dbLoadTemplate("db/globalInt_TBx_WF.sub")

####OpOnStat
##DB Tested
dbLoadTemplate("db/globalInt_OpOnStat.sub")    

####Vacuum
##DB Tested
#dbLoadRecords("db/globalInt_VBxVacSCL3SCL2.db")
#dbLoadRecords("db/globalInt_VBxVacSCL2SCL2.db")
##New 12.29
dbLoadTemplate("db/globalInt_Vacuum.sub")    
dbLoadTemplate("db/globalInt_SCLVac.sub")    

#2023.12.06 New GInterlock PVs
##DB Tested
dbLoadRecords("db/Alias_IntStat.db")
dbLoadRecords("db/Alias_SetInt.db")

cd "${TOP}/iocBoot/${IOC}"
iocInit

## Start any sequence programs
#seq sncxxx,"user=ctrluser"
seq sncGLInterlockWF
seq sncTBxGIntWF
#seq sncQWRGInt -> sncQWRGInt_PROD
