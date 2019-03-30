TOP=../..

include $(TOP)/configure/CONFIG
#----------------------------------------
#  ADD MACRO DEFINITIONS AFTER THIS LINE
#=============================

#==================================================
# Build an IOC support library
#

DBDINC += byteRecord
DBDINC += shortRecord
DBDINC += floatRecord
DBDINC += ubyteRecord
DBDINC += ushortRecord
DBDINC += ulongRecord
DBDINC += bigstringinRecord
DBDINC += waitRecord

DBD += testDbPv.dbd


LIBRARY_IOC += testDbPvSupport
testDbPvSupport_SRCS += byteRecord.c
testDbPvSupport_SRCS += shortRecord.c
testDbPvSupport_SRCS += floatRecord.c
testDbPvSupport_SRCS += ubyteRecord.c
testDbPvSupport_SRCS += ushortRecord.c
testDbPvSupport_SRCS += ulongRecord.c
testDbPvSupport_SRCS += bigstringinRecord.c
testDbPvSupport_SRCS += waitRecord.c
testDbPvSupport_SRCS += testDbPv.cpp
testDbPvSupport_LIBS = pvAccess pvData $(MBLIB) $(EPICS_BASE_IOC_LIBS)

#=============================
# Build an IOC application

PROD_IOC += testDbPv

# <name>_registerRecordDeviceDriver.cpp will be created from <name>.dbd
testDbPv_SRCS += testDbPv_registerRecordDeviceDriver.cpp
testDbPv_SRCS_DEFAULT += testDbPvMain.cpp
testDbPv_SRCS_vxWorks += -nil-

# The following adds support from base/src/vxWorks
testDbPv_OBJS_vxWorks += $(EPICS_BASE_BIN)/vxComLibrary

testDbPv_LIBS += testDbPvSupport
testDbPv_LIBS += pvaSrv pvAccess pvData $(MBLIB)
testDbPv_LIBS += $(EPICS_BASE_IOC_LIBS)

#===========================

include $(TOP)/configure/RULES
#----------------------------------------
#  ADD RULES AFTER THIS LINE
