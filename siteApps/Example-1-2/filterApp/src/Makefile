TOP=../..

include $(TOP)/configure/CONFIG
#----------------------------------------
#  ADD MACRO DEFINITIONS AFTER THIS LINE
#=============================

#=============================
# Build the IOC application

PROD_IOC = filter
# filter.dbd will be created and installed
DBD += filter.dbd

# filter.dbd will be made up from these files:
filter_DBD += base.dbd

# Include dbd files from all support applications:
#filter_DBD += xxx.dbd

# Add all the support libraries needed by this IOC
#filter_LIBS += xxx

# filter_registerRecordDeviceDriver.cpp derives from filter.dbd
filter_SRCS += filter_registerRecordDeviceDriver.cpp

# Build the main IOC entry point on workstation OSs.
filter_SRCS_DEFAULT += filterMain.cpp
filter_SRCS_vxWorks += -nil-

# Add support from base/src/vxWorks if needed
#filter_OBJS_vxWorks += $(EPICS_BASE_BIN)/vxComLibrary

# Finally link to the EPICS Base libraries
filter_LIBS += $(EPICS_BASE_IOC_LIBS)

#===========================

include $(TOP)/configure/RULES
#----------------------------------------
#  ADD RULES AFTER THIS LINE

