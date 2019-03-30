#!../../bin/linux-x86_64/cryo

## You may have to change cryo to something else
## everywhere it appears in this file

< envPaths

cd ${TOP}

## Register all support components
dbLoadDatabase "dbd/cryo.dbd"
cryo_registerRecordDeviceDriver pdbbase

#dbLoadDatabase "dbd/eipIoc.dbd"
#dbLoadDatabase "dbd/ether_ip.dbd"
#eipIoc_registerRecordDeviceDriver pdbbase

# Initialize EtherIP driver, define PLCs
EIP_buffer_limit(450)
drvEtherIP_init()

EIP_verbosity(7)

# drvEtherIP_define_PLC <name>, <ip_addr>, <slot>
# The driver/device uses the <name> to indentify the PLC.
# 
# <ip_addr> can be an IP address in dot-notation
# or a name about which the IOC knows.
# The IP address gets us to the ENET interface.
# To get to the PLC itself, we need the slot that
# it resides in. The first, left-most slot in the
# ControlLogix crate is slot 0.

drvEtherIP_define_PLC("plc1", "192.168.0.2", 0)

cd ${TOP}/iocBoot/${IOC}

dbLoadRecords("$(TOP)/db/qwr.db","SYS=SCL01-CM:CM001")

iocInit

## Start any sequence programs
#seq seqCryo,"sysname=SCL32-CM:CM01"
