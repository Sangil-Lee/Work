TOP=../..

include $(TOP)/configure/CONFIG
#----------------------------------------
#  ADD MACRO DEFINITIONS AFTER THIS LINE
#=============================

#=============================
# Build the IOC application

PROD_IOC = mbboLong
# mbboLong.dbd will be created and installed
DBD += mbboLong.dbd

# mbboLong.dbd will be made up from these files:
mbboLong_DBD += base.dbd

# Include dbd files from all support applications:
#mbboLong_DBD += xxx.dbd

# Add all the support libraries needed by this IOC
#mbboLong_LIBS += xxx

# mbboLong_registerRecordDeviceDriver.cpp derives from mbboLong.dbd
mbboLong_SRCS += mbboLong_registerRecordDeviceDriver.cpp

# Build the main IOC entry point on workstation OSs.
mbboLong_SRCS_DEFAULT += mbboLongMain.cpp
mbboLong_SRCS_vxWorks += -nil-

# Add support from base/src/vxWorks if needed
#mbboLong_OBJS_vxWorks += $(EPICS_BASE_BIN)/vxComLibrary

# Finally link to the EPICS Base libraries
mbboLong_LIBS += $(EPICS_BASE_IOC_LIBS)

#===========================

include $(TOP)/configure/RULES
#----------------------------------------
#  ADD RULES AFTER THIS LINE

