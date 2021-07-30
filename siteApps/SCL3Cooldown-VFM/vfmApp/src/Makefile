TOP=../..

include $(TOP)/configure/CONFIG
#----------------------------------------
#  ADD MACRO DEFINITIONS AFTER THIS LINE
#=============================

#=============================
# Build the IOC application

PROD_IOC = vfm
# vfm.dbd will be created and installed
DBD += vfm.dbd

# vfm.dbd will be made up from these files:
vfm_DBD += base.dbd

# Include dbd files from all support applications:
#vfm_DBD += xxx.dbd

# Add all the support libraries needed by this IOC
#vfm_LIBS += xxx

# vfm_registerRecordDeviceDriver.cpp derives from vfm.dbd
vfm_SRCS += vfm_registerRecordDeviceDriver.cpp

# Build the main IOC entry point on workstation OSs.
vfm_SRCS_DEFAULT += vfmMain.cpp
vfm_SRCS_vxWorks += -nil-

# Add support from base/src/vxWorks if needed
#vfm_OBJS_vxWorks += $(EPICS_BASE_BIN)/vxComLibrary

# Finally link to the EPICS Base libraries
vfm_LIBS += $(EPICS_BASE_IOC_LIBS)

#===========================

include $(TOP)/configure/RULES
#----------------------------------------
#  ADD RULES AFTER THIS LINE

