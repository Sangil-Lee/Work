TOP=../..

include $(TOP)/configure/CONFIG
#----------------------------------------
#  ADD MACRO DEFINITIONS AFTER THIS LINE
#=============================

#=============================
# Build the IOC application

PROD_IOC = aliasTr
# aliasTr.dbd will be created and installed
DBD += aliasTr.dbd

# aliasTr.dbd will be made up from these files:
aliasTr_DBD += base.dbd

# Include dbd files from all support applications:
#aliasTr_DBD += xxx.dbd

# Add all the support libraries needed by this IOC
#aliasTr_LIBS += xxx

# aliasTr_registerRecordDeviceDriver.cpp derives from aliasTr.dbd
aliasTr_SRCS += aliasTr_registerRecordDeviceDriver.cpp

# Build the main IOC entry point on workstation OSs.
aliasTr_SRCS_DEFAULT += aliasTrMain.cpp
aliasTr_SRCS_vxWorks += -nil-

# Add support from base/src/vxWorks if needed
#aliasTr_OBJS_vxWorks += $(EPICS_BASE_BIN)/vxComLibrary

# Finally link to the EPICS Base libraries
aliasTr_LIBS += $(EPICS_BASE_IOC_LIBS)

#===========================

include $(TOP)/configure/RULES
#----------------------------------------
#  ADD RULES AFTER THIS LINE

