TOP=../..

include $(TOP)/configure/CONFIG
#----------------------------------------
#  ADD MACRO DEFINITIONS AFTER THIS LINE
#=============================

#=============================
# Build the IOC application

PROD_IOC = iocex2
# iocex2.dbd will be created and installed
DBD += iocex2.dbd

# iocex2.dbd will be made up from these files:
iocex2_DBD += base.dbd

# Include dbd files from all support applications:
#iocex2_DBD += xxx.dbd

# Add all the support libraries needed by this IOC
#iocex2_LIBS += xxx

# iocex2_registerRecordDeviceDriver.cpp derives from iocex2.dbd
iocex2_SRCS += iocex2_registerRecordDeviceDriver.cpp

# Build the main IOC entry point on workstation OSs.
iocex2_SRCS_DEFAULT += iocex2Main.cpp
iocex2_SRCS_vxWorks += -nil-

# Add support from base/src/vxWorks if needed
#iocex2_OBJS_vxWorks += $(EPICS_BASE_BIN)/vxComLibrary

# Finally link to the EPICS Base libraries
iocex2_LIBS += $(EPICS_BASE_IOC_LIBS)

#===========================

include $(TOP)/configure/RULES
#----------------------------------------
#  ADD RULES AFTER THIS LINE

