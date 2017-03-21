TOP=../..

include $(TOP)/configure/CONFIG
#----------------------------------------
#  ADD MACRO DEFINITIONS AFTER THIS LINE
#=============================

#=============================
# Build the IOC application

PROD_IOC = FastProtect
# FastProtect.dbd will be created and installed
DBD += FastProtect.dbd

# FastProtect.dbd will be made up from these files:
FastProtect_DBD += base.dbd

# Include dbd files from all support applications:
FastProtect_DBD += FastProtectSup.dbd

# Add all the support libraries needed by this IOC
FastProtect_LIBS += FastProtectSup
FastProtect_LIBS += asyn

# FastProtect_registerRecordDeviceDriver.cpp derives from FastProtect.dbd
FastProtect_SRCS += FastProtect_registerRecordDeviceDriver.cpp

# Build the main IOC entry point on workstation OSs.
FastProtect_SRCS_DEFAULT += FastProtectMain.cpp
FastProtect_SRCS_vxWorks += -nil-

# Add support from base/src/vxWorks if needed
#FastProtect_OBJS_vxWorks += $(EPICS_BASE_BIN)/vxComLibrary

# Finally link to the EPICS Base libraries
FastProtect_LIBS += $(EPICS_BASE_IOC_LIBS)

#===========================

include $(TOP)/configure/RULES
#----------------------------------------
#  ADD RULES AFTER THIS LINE

