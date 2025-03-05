TOP=../..

include $(TOP)/configure/CONFIG
#----------------------------------------
#  ADD MACRO DEFINITIONS AFTER THIS LINE
#=============================

#=============================
# Build the IOC application

PROD_IOC = s7test
# s7test.dbd will be created and installed
DBD += s7test.dbd

# s7test.dbd will be made up from these files:
s7test_DBD += base.dbd
s7test_DBD += s7nodave.dbd 

# Include dbd files from all support applications:
#s7test_DBD += xxx.dbd

# Add all the support libraries needed by this IOC
#s7test_LIBS += xxx

# s7test_registerRecordDeviceDriver.cpp derives from s7test.dbd
s7test_SRCS += s7test_registerRecordDeviceDriver.cpp

# Build the main IOC entry point on workstation OSs.
s7test_SRCS_DEFAULT += s7testMain.cpp
s7test_SRCS_vxWorks += -nil-

# Add support from base/src/vxWorks if needed
#s7test_OBJS_vxWorks += $(EPICS_BASE_BIN)/vxComLibrary

# Finally link to the EPICS Base libraries
s7test_LIBS += s7nodave
s7test_LIBS += $(EPICS_BASE_IOC_LIBS)

#===========================

include $(TOP)/configure/RULES
#----------------------------------------
#  ADD RULES AFTER THIS LINE

