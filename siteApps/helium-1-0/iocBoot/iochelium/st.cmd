#!../../bin/linux-x86_64/helium

## You may have to change helium to something else
## everywhere it appears in this file

< envPaths

cd "${TOP}"

## Register all support components
dbLoadDatabase "dbd/helium.dbd"
helium_registerRecordDeviceDriver pdbbase

var s7plcDebug	1

#s7plc default port(2000)
abplc default port(0xAF12=44818)

#It works
EIP_buffer_limit(500)
drvEtherIP_init()

drvEtherIP_define_PLC("ABPLC","192.168.11.2",0)
s7plcConfigure ("S7PLC", "192.168.11.50", 2000, 240, 0, 1, 2000, 100)

#dbLoadRecords("db/S7.db","SYS=STF02")
#dbLoadRecords("db/S7_One.db","SYS=STF02")
#dbLoadRecords("db/S7_Three.db","SYS=STF02")

## Load record instances
#dbLoadRecords("db/HeInventory.vdb","SYS=STF02")

## ABPLC 2 Inputs(Float), SiemensPLC 3 Inputs(Float)
dbLoadRecords("db/HeInventory_1sec.vdb","SYS=STF02")

cd "${TOP}/iocBoot/${IOC}"
iocInit


