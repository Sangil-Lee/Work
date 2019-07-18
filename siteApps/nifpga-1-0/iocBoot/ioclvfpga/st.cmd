#!../../bin/linux-x86_64/lvfpga

## You may have to change lvfpga to something else
## everywhere it appears in this file

< envPaths

epicsEnvSet("EPICS_CA_SERVER_PORT","10000")

cd "${TOP}"

## Register all support components
dbLoadDatabase "dbd/lvfpga.dbd"
lvfpga_registerRecordDeviceDriver pdbbase

lvfpgaAsynEpicsConfigure("lvfpga","${TOP}/iocBoot/${IOC}/LVFPGARegister.reg","${TOP}/iocBoot/${IOC}/NiFpga_FPGA_1.lvbitx", "RIO0")

## Load record instances
#dbLoadRecords("db/xxx.db","user=lvuserHost")

dbLoadTemplate("${TOP}/iocBoot/${IOC}/laview_fpga.sub")

cd "${TOP}/iocBoot/${IOC}"
iocInit

## Start any sequence programs
#seq sncxxx,"user=lvuserHost"
