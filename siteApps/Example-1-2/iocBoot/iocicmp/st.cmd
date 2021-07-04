#!../../bin/linux-x86_64/icmp

## You may have to change icmp to something else
## everywhere it appears in this file
## Root sticky bit set
##sudo install -o root -g root -m u=rxs,g=rx,o=x -t /usr/local/bin icmp


< envPaths

cd "${TOP}"

## Register all support components
dbLoadDatabase "dbd/icmp.dbd"
icmp_registerRecordDeviceDriver pdbbase

var epicsDebugLevel 0
icmpOPingInit("${TOP}/iocBoot/${IOC}/ping_list","eth0",3);

## Load record instances
#dbLoadRecords("db/icmpex.vdb","AREA=MUNJI")
dbLoadRecords("db/icmpkt.vdb","AREA=KT")

cd "${TOP}/iocBoot/${IOC}"
iocInit

## Start any sequence programs
#seq sncxxx,"user=ctrluserHost"
