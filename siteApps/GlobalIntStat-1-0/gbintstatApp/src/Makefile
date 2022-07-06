TOP=../..

include $(TOP)/configure/CONFIG
#----------------------------------------
#  ADD MACRO DEFINITIONS AFTER THIS LINE
#=============================

#=============================
# Build the IOC application

PROD_IOC = gbintstat
# gbintstat.dbd will be created and installed
DBD += gbintstat.dbd

# gbintstat.dbd will be made up from these files:
gbintstat_DBD += base.dbd

# Include dbd files from all support applications:
#gbintstat_DBD += xxx.dbd

# Add all the support libraries needed by this IOC
#gbintstat_LIBS += xxx

# gbintstat_registerRecordDeviceDriver.cpp derives from gbintstat.dbd
gbintstat_SRCS += gbintstat_registerRecordDeviceDriver.cpp

# Build the main IOC entry point on workstation OSs.
gbintstat_SRCS_DEFAULT += gbintstatMain.cpp
gbintstat_SRCS_vxWorks += -nil-

# Add support from base/src/vxWorks if needed
#gbintstat_OBJS_vxWorks += $(EPICS_BASE_BIN)/vxComLibrary

# Finally link to the EPICS Base libraries
gbintstat_LIBS += $(EPICS_BASE_IOC_LIBS)

#===========================

include $(TOP)/configure/RULES
#----------------------------------------
#  ADD RULES AFTER THIS LINE

