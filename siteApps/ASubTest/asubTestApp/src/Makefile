TOP=../..

include $(TOP)/configure/CONFIG
#----------------------------------------
#  ADD MACRO DEFINITIONS AFTER THIS LINE
#=============================

#=============================
# Build the IOC application

PROD_IOC = asubTest
# asubTest.dbd will be created and installed
DBD += asubTest.dbd

# asubTest.dbd will be made up from these files:
asubTest_DBD += base.dbd

# Include dbd files from all support applications:
#asubTest_DBD += xxx.dbd
asubTest_DBD += ASubTest.dbd
asubTest_DBD += ASubConv.dbd

# Add all the support libraries needed by this IOC
#asubTest_LIBS += xxx

# asubTest_registerRecordDeviceDriver.cpp derives from asubTest.dbd
asubTest_SRCS += asubTest_registerRecordDeviceDriver.cpp
asubTest_SRCS += ASubTest.cpp
asubTest_SRCS += ASubConv.cpp

# Build the main IOC entry point on workstation OSs.
asubTest_SRCS_DEFAULT += asubTestMain.cpp
asubTest_SRCS_vxWorks += -nil-

# Add support from base/src/vxWorks if needed
#asubTest_OBJS_vxWorks += $(EPICS_BASE_BIN)/vxComLibrary

# Finally link to the EPICS Base libraries
asubTest_LIBS += $(EPICS_BASE_IOC_LIBS)

#===========================

include $(TOP)/configure/RULES
#----------------------------------------
#  ADD RULES AFTER THIS LINE

