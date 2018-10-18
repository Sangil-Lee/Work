TOP=../..

include $(TOP)/configure/CONFIG
#----------------------------------------
#  ADD MACRO DEFINITIONS AFTER THIS LINE
#=============================
# Build the Library

DBD += asynDriverSupport.dbd

LIBRARY_IOC += asynDriverSupport 

CXXFLAGS += -Wno-format -Wno-unused-variable

USR_DBDFLAGS  += -I$(RAON_SITELIBS)/dbd
timeStamp_DIR += $(RAON_SITELIBS)/lib/$(T_A)

asynDriverSupport_SRCS += asynDriverSupport.cpp
asynDriverSupport_LIBS += asyn 
asynDriverSupport_LIBS += $(EPICS_BASE_IOC_LIBS)

#=============================
# Build the IOC application

PROD_IOC = asynDriver
# asynDriver.dbd will be created and installed
DBD += asynDriver.dbd

# asynDriver.dbd will be made up from these files:
asynDriver_DBD += base.dbd
asynDriver_DBD += asynDriverSupport.dbd
#asynDriver_DBD += waveSetupSub.dbd

# Include dbd files from all support applications:
#asynDriver_DBD += xxx.dbd

# Add all the support libraries needed by this IOC
#asynDriver_LIBS += xxx

# asynDriver_registerRecordDeviceDriver.cpp derives from asynDriver.dbd
asynDriver_SRCS += asynDriver_registerRecordDeviceDriver.cpp
#asynDriver_SRCS += waveSetupSub.c

# Build the main IOC entry point on workstation OSs.
asynDriver_SRCS_DEFAULT += asynDriverMain.cpp
asynDriver_SRCS_vxWorks += -nil-

# Add support from base/src/vxWorks if needed
#asynDriver_OBJS_vxWorks += $(EPICS_BASE_BIN)/vxComLibrary

# Finally link to the EPICS Base libraries
asynDriver_LIBS += $(EPICS_BASE_IOC_LIBS)
asynDriver_LIBS += asynDriverSupport 
asynDriver_LIBS += asyn timeStamp

#===========================

include $(TOP)/configure/RULES
#----------------------------------------
#  ADD RULES AFTER THIS LINE

