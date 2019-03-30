TOP=../..

include $(TOP)/configure/CONFIG
#----------------------------------------
#  ADD MACRO DEFINITIONS AFTER THIS LINE
#=============================

#==================================================
# Build an IOC support library
#


DBD += testDbGroup.dbd


#=============================
# build an ioc application

PROD_IOC += testDbGroup

# <name>_registerRecordDeviceDriver.cpp will be created from <name>.dbd
testDbGroup_SRCS += testDbGroup_registerRecordDeviceDriver.cpp
testDbGroup_SRCS_DEFAULT += testDbGroupMain.cpp
testDbGroup_SRCS_vxWorks += -nil-


# The following adds support from base/src/vxWorks
testDbGroup_OBJS_vxWorks += $(EPICS_BASE_BIN)/vxComLibrary

testDbGroup_LIBS += pvaSrv pvIOC pvAccess pvData pvMB
testDbGroup_LIBS += testDbPvSupport
testDbGroup_LIBS += $(EPICS_BASE_IOC_LIBS)

#===========================

include $(TOP)/configure/RULES
#----------------------------------------
#  ADD RULES AFTER THIS LINE

