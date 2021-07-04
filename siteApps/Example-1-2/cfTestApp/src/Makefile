TOP=../..

include $(TOP)/configure/CONFIG
#----------------------------------------
#  ADD MACRO DEFINITIONS AFTER THIS LINE
#=============================

#=============================
# Build the IOC application

USR_INCLUDES  += -I$(RAON_SITELIBS)/include
USR_DBDFLAGS  += -I$(RAON_SITELIBS)/dbd

reccaster_DIR += $(RAON_SITELIBS)/lib/$(T_A)

PROD_IOC = cfTest
# cfTest.dbd will be created and installed
DBD += cfTest.dbd

# cfTest.dbd will be made up from these files:
cfTest_DBD += base.dbd

# Include dbd files from all support applications:
#cfTest_DBD += xxx.dbd
cfTest_DBD += reccaster.dbd

# Add all the support libraries needed by this IOC
#cfTest_LIBS += xxx
cfTest_LIBS += reccaster

# cfTest_registerRecordDeviceDriver.cpp derives from cfTest.dbd
cfTest_SRCS += cfTest_registerRecordDeviceDriver.cpp

# Build the main IOC entry point on workstation OSs.
cfTest_SRCS_DEFAULT += cfTestMain.cpp
cfTest_SRCS_vxWorks += -nil-

# Add support from base/src/vxWorks if needed
#cfTest_OBJS_vxWorks += $(EPICS_BASE_BIN)/vxComLibrary

# Finally link to the EPICS Base libraries
cfTest_LIBS += $(EPICS_BASE_IOC_LIBS)

#===========================

include $(TOP)/configure/RULES
#----------------------------------------
#  ADD RULES AFTER THIS LINE

