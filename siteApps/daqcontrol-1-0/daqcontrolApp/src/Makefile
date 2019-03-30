TOP=../..

include $(TOP)/configure/CONFIG
#----------------------------------------
#  ADD MACRO DEFINITIONS AFTER THIS LINE
#=============================

DBD += daqControlAsynEpics.dbd

LIBRARY_IOC += daqControlAsynEpicsSupport 

USR_DBDFLAGS  += -I$(RAON_SITELIBS)/dbd
CXXFLAGS += -std=c++11 -Wno-unused-but-set-variable -Wno-format

timeStamp_DIR += $(RAON_SITELIBS)/lib/$(T_A)

daqControlAsynEpicsSupport_SRCS += daqControlAsynEpics.cpp daqControlDriver.cpp
daqControlAsynEpicsSupport_LIBS += asyn 
daqControlAsynEpicsSupport_LIBS += $(EPICS_BASE_IOC_LIBS)

#=============================
# Build the IOC application

PROD_IOC = daqcontrol
# daqcontrol.dbd will be created and installed
DBD += daqcontrol.dbd

# daqcontrol.dbd will be made up from these files:
daqcontrol_DBD += base.dbd
daqcontrol_DBD += daqControlAsynEpics.dbd timestampRecord.dbd
daqcontrol_DBD += timestampDevSoft.dbd

# Include dbd files from all support applications:
#daqcontrol_DBD += xxx.dbd

# Add all the support libraries needed by this IOC
#daqcontrol_LIBS += xxx
daqcontrol_LIBS += daqControlAsynEpicsSupport
daqcontrol_LIBS += asyn timeStamp

# daqcontrol_registerRecordDeviceDriver.cpp derives from daqcontrol.dbd
daqcontrol_SRCS += daqcontrol_registerRecordDeviceDriver.cpp

# Build the main IOC entry point on workstation OSs.
daqcontrol_SRCS_DEFAULT += daqcontrolMain.cpp
daqcontrol_SRCS_vxWorks += -nil-

# Add support from base/src/vxWorks if needed
#daqcontrol_OBJS_vxWorks += $(EPICS_BASE_BIN)/vxComLibrary

# Finally link to the EPICS Base libraries
daqcontrol_LIBS += $(EPICS_BASE_IOC_LIBS)

#===========================

include $(TOP)/configure/RULES
#----------------------------------------
#  ADD RULES AFTER THIS LINE

