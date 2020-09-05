TOP=../..

include $(TOP)/configure/CONFIG
#----------------------------------------
#  ADD MACRO DEFINITIONS AFTER THIS LINE
#=============================

#=============================
# Build the IOC application

PROD_IOC = scl3test
# scl3test.dbd will be created and installed
DBD += scl3test.dbd

# scl3test.dbd will be made up from these files:
scl3test_DBD += base.dbd

# Include dbd files from all support applications:
#scl3test_DBD += xxx.dbd

# Add all the support libraries needed by this IOC
#scl3test_LIBS += xxx

# scl3test_registerRecordDeviceDriver.cpp derives from scl3test.dbd
scl3test_SRCS += scl3test_registerRecordDeviceDriver.cpp

# Build the main IOC entry point on workstation OSs.
scl3test_SRCS_DEFAULT += scl3testMain.cpp
scl3test_SRCS_vxWorks += -nil-

# Add support from base/src/vxWorks if needed
#scl3test_OBJS_vxWorks += $(EPICS_BASE_BIN)/vxComLibrary

# Finally link to the EPICS Base libraries
scl3test_LIBS += $(EPICS_BASE_IOC_LIBS)

#===========================

include $(TOP)/configure/RULES
#----------------------------------------
#  ADD RULES AFTER THIS LINE

