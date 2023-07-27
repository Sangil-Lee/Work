TOP=../..

include $(TOP)/configure/CONFIG
#----------------------------------------
#  ADD MACRO DEFINITIONS AFTER THIS LINE
#=============================

#=============================
# Build the IOC application

PROD_IOC = iftarget
# iftarget.dbd will be created and installed
DBD += iftarget.dbd

# iftarget.dbd will be made up from these files:
iftarget_DBD += base.dbd

# Include dbd files from all support applications:
#iftarget_DBD += xxx.dbd
iftarget_DBD += asyn.dbd
iftarget_DBD += drvAsynIPPort.dbd
iftarget_DBD += modbusSupport.dbd
iftarget_DBD += motorSupport.dbd
iftarget_DBD += phytronSupport.dbd

# Add all the support libraries needed by this IOC
#iftarget_LIBS += xxx

# iftarget_registerRecordDeviceDriver.cpp derives from iftarget.dbd
iftarget_SRCS += iftarget_registerRecordDeviceDriver.cpp

# Build the main IOC entry point on workstation OSs.
iftarget_SRCS_DEFAULT += iftargetMain.cpp
iftarget_SRCS_vxWorks += -nil-

# Add support from base/src/vxWorks if needed
#iftarget_OBJS_vxWorks += $(EPICS_BASE_BIN)/vxComLibrary
ifneq ($(SNCSEQ),)
    iftarget_SNCFLAGS += +r
    #iftarget_DBD  +=
    #iftarget_SRCS +=
    iftargetLIBS += seq pv
endif

# Finally link to the EPICS Base libraries
iftarget_LIBS += asyn modbus motor phytronAxisMotor
iftarget_LIBS += $(EPICS_BASE_IOC_LIBS)

#===========================

include $(TOP)/configure/RULES
#----------------------------------------
#  ADD RULES AFTER THIS LINE

