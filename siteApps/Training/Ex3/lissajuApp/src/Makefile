TOP=../..

include $(TOP)/configure/CONFIG
#----------------------------------------
#  ADD MACRO DEFINITIONS AFTER THIS LINE
#=============================

#=============================
# Build the IOC application

PROD_IOC = lissaju
# lissaju.dbd will be created and installed
DBD += lissaju.dbd

# lissaju.dbd will be made up from these files:
lissaju_DBD += base.dbd

# Include dbd files from all support applications:
#lissaju_DBD += xxx.dbd
lissaju_DBD += asubLissaju.dbd

# Add all the support libraries needed by this IOC
#lissaju_LIBS += xxx

# lissaju_registerRecordDeviceDriver.cpp derives from lissaju.dbd
lissaju_SRCS += lissaju_registerRecordDeviceDriver.cpp
lissaju_SRCS += asubLissaju.cpp

# Build the main IOC entry point on workstation OSs.
lissaju_SRCS_DEFAULT += lissajuMain.cpp
lissaju_SRCS_vxWorks += -nil-

# Add support from base/src/vxWorks if needed
#lissaju_OBJS_vxWorks += $(EPICS_BASE_BIN)/vxComLibrary

# Finally link to the EPICS Base libraries
lissaju_LIBS += $(EPICS_BASE_IOC_LIBS)

#===========================

include $(TOP)/configure/RULES
#----------------------------------------
#  ADD RULES AFTER THIS LINE

