TOP=../..

include $(TOP)/configure/CONFIG
#----------------------------------------
#  ADD MACRO DEFINITIONS AFTER THIS LINE
#=============================

#=============================
# Build the IOC application

PROD_IOC = linear
# linear.dbd will be created and installed
DBD += linear.dbd

# linear.dbd will be made up from these files:
linear_DBD += base.dbd

# Include dbd files from all support applications:
#linear_DBD += xxx.dbd

# Add all the support libraries needed by this IOC
#linear_LIBS += xxx

# linear_registerRecordDeviceDriver.cpp derives from linear.dbd
linear_SRCS += linear_registerRecordDeviceDriver.cpp

# Build the main IOC entry point on workstation OSs.
linear_SRCS_DEFAULT += linearMain.cpp
linear_SRCS_vxWorks += -nil-

# Add support from base/src/vxWorks if needed
#linear_OBJS_vxWorks += $(EPICS_BASE_BIN)/vxComLibrary

# Finally link to the EPICS Base libraries
linear_LIBS += $(EPICS_BASE_IOC_LIBS)

#===========================

include $(TOP)/configure/RULES
#----------------------------------------
#  ADD RULES AFTER THIS LINE

