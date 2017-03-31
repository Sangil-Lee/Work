TOP=../..

include $(TOP)/configure/CONFIG
#----------------------------------------
#  ADD MACRO DEFINITIONS AFTER THIS LINE
#=============================

#=============================
# build an ioc application

PROD_IOC = scope
# <name>.dbd will be created from <name>Include.dbd
DBD += scope.dbd


# <name>_registerRecordDeviceDriver.cpp will be created from <name>.dbd
scope_SRCS += scope_registerRecordDeviceDriver.cpp
scope_SRCS_DEFAULT += scopeMain.cpp
scope_SRCS_vxWorks += -nil-

#The following adds support from base/src/vxWorks
scope_OBJS_vxWorks += $(EPICS_BASE_BIN)/vxComLibrary

#add a definition for each support application used by this application
scope_LIBS += devTDS3000
scope_LIBS += asyn
SYS_PROD_LIBS_cygwin32 += rpc


scope_SRCS += TDS3000autoArmPrt.st
scope_LIBS += seq pv
scope_LIBS += $(EPICS_BASE_IOC_LIBS)

#===========================

include $(TOP)/configure/RULES
#----------------------------------------
#  ADD RULES AFTER THIS LINE

