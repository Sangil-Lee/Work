TOP=../..

include $(TOP)/configure/CONFIG
#----------------------------------------
#  ADD MACRO DEFINITIONS AFTER THIS LINE
#=============================

#=============================
# Build the IOC application

PROD_IOC = iocex1
# iocex1.dbd will be created and installed
DBD += iocex1.dbd

# iocex1.dbd will be made up from these files:
iocex1_DBD += base.dbd

# Include dbd files from all support applications:
#iocex1_DBD += xxx.dbd
iocex1_DBD += subMul.dbd

# Add all the support libraries needed by this IOC
#iocex1_LIBS += xxx

# iocex1_registerRecordDeviceDriver.cpp derives from iocex1.dbd
iocex1_SRCS += iocex1_registerRecordDeviceDriver.cpp
iocex1_SRCS += subMul.cpp

# Build the main IOC entry point on workstation OSs.
iocex1_SRCS_DEFAULT += iocex1Main.cpp
iocex1_SRCS_vxWorks += -nil-

# Add support from base/src/vxWorks if needed
#iocex1_OBJS_vxWorks += $(EPICS_BASE_BIN)/vxComLibrary

# Finally link to the EPICS Base libraries
iocex1_LIBS += $(EPICS_BASE_IOC_LIBS)

#===========================

include $(TOP)/configure/RULES
#----------------------------------------
#  ADD RULES AFTER THIS LINE

