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
nidaqAsynEpicsConfigure("ifLTSDAQ","db/ifLTSMGDAQ_Register.reg", "PXI1", 100, 50)

dbLoadTemplate("db/ifLTSMG_DAQmx.sub", "SYS=IF,SUBSYS=-BL01:,DEV=LTSQT01, NELM=50")

cd "${TOP}/iocBoot/${IOC}"
iocInit

## Start any sequence programs
#seq sncxxx,"user=ctrluser"
