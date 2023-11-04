#!../../bin/linux-x86_64/ifmgdaq

#- You may have to change ifmgdaq to something else
#- everywhere it appears in this file

< envPaths

cd "${TOP}"

## Register all support components
dbLoadDatabase "dbd/ifmgdaq.dbd"
ifmgdaq_registerRecordDeviceDriver pdbbase

## Load record instances
#dbLoadRecords("db/ifmgdaq.db","user=ctrluser")

###parameters(portName, register file, pxi_device_prefix, sample_rate, NELM)
nidaqAsynEpicsConfigure("ifLTSDAQ","db/ifLTSMGDAQ_Register.reg", "PXI1", 3200, 1600)

nidaqAsynEpicsConfigure("ifLTSDAQ_2","db/ifLTSMGDAQ_Register_2.reg", "PXI1", 3200, 1600)

dbLoadTemplate("db/ifLTSMG_DAQmx.sub", "PORT=ifLTSDAQ,SYS=IF,SUBSYS=-BL01:,DEV=LTSQT09, NELM=10")

dbLoadTemplate("db/ifLTSMG_DAQmx_2.sub", "PORT=ifLTSDAQ_2,SYS=IF,SUBSYS=-BL01:,DEV=LTSQT10, NELM=10")

cd "${TOP}/iocBoot/${IOC}"
iocInit

## Start any sequence programs
#seq sncxxx,"user=ctrluser"
