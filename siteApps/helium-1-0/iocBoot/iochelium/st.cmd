#!../../bin/linux-x86_64/helium

## You may have to change helium to something else
## everywhere it appears in this file

< envPaths

cd "${TOP}"

## Register all support components
dbLoadDatabase "dbd/helium.dbd"
helium_registerRecordDeviceDriver pdbbase

#var s7plcDebug	1
#s7plcConfigure (S7PLC:0, 127.0.0.1, 2000, 80, 80, 1, 2000, 100)

#EIP_buffer_limit(500)
#drvEtherIP_init()
#drvEtherIP_define_PLC("ABPLC","127.0.0.1",0)

## Load record instances
#dbLoadRecords("db/abplcAIAO.db","PLC=ABPLC")

## Load record instances
#dbLoadRecords("db/xxx.db","user=ctrluserHost")
dbLoadRecords("db/HeInventory.vdb","SYS=STF02")


cd "${TOP}/iocBoot/${IOC}"
iocInit

## Start any sequence programs
#seq sncxxx,"user=ctrluserHost"
