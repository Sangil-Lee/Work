TOP=../..

include $(TOP)/configure/CONFIG
#----------------------------------------
#  ADD MACRO DEFINITIONS AFTER THIS LINE
#=============================

#=============================
# Build the IOC application

PROD_IOC = plantsim
# plantsim.dbd will be created and installed
DBD += plantsim.dbd

# plantsim.dbd will be made up from these files:
plantsim_DBD += base.dbd

# Include dbd files from all support applications:
#plantsim_DBD += xxx.dbd

# Add all the support libraries needed by this IOC
#plantsim_LIBS += xxx

# plantsim_registerRecordDeviceDriver.cpp derives from plantsim.dbd
plantsim_SRCS += plantsim_registerRecordDeviceDriver.cpp

# Build the main IOC entry point on workstation OSs.
plantsim_SRCS_DEFAULT += plantsimMain.cpp
plantsim_SRCS_vxWorks += -nil-

# Add support from base/src/vxWorks if needed
#plantsim_OBJS_vxWorks += $(EPICS_BASE_BIN)/vxComLibrary

# Finally link to the EPICS Base libraries
plantsim_LIBS += $(EPICS_BASE_IOC_LIBS)

#===========================

include $(TOP)/configure/RULES
#----------------------------------------
#  ADD RULES AFTER THIS LINE

