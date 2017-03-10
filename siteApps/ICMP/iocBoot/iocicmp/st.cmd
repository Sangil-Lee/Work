#!/usr/local/bin/icmp

## You may have to change icmp to something else
## everywhere it appears in this file

## Root sticky bit set
##sudo install -o root -g root -m u=rxs,g=rx,o=x -t /usr/local/bin icmp

< envPaths

cd "${TOP}"

## Register all support components
dbLoadDatabase "dbd/icmp.dbd"
icmp_registerRecordDeviceDriver pdbbase

icmpOPingInit("ping_list","eth0",0);

## Load record instances
dbLoadRecords("db/icmpex.vdb","SYS=PING")

cd "${TOP}/iocBoot/${IOC}"
iocInit

## Start any sequence programs
#seq sncxxx,"user=ctrluserHost"
