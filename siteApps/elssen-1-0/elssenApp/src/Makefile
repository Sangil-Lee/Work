TOP=../..

include $(TOP)/configure/CONFIG
#----------------------------------------
#  ADD MACRO DEFINITIONS AFTER THIS LINE
#=============================

#=============================
# Build the IOC application

PROD_IOC = elssen
# elssen.dbd will be created and installed
DBD += elssen.dbd

# elssen.dbd will be made up from these files:
elssen_DBD += base.dbd

# Include dbd files from all support applications:
#elssen_DBD += xxx.dbd

# Add all the support libraries needed by this IOC
#elssen_LIBS += xxx

# elssen_registerRecordDeviceDriver.cpp derives from elssen.dbd
elssen_SRCS += elssen_registerRecordDeviceDriver.cpp

# Build the main IOC entry point on workstation OSs.
elssen_SRCS_DEFAULT += elssenMain.cpp
elssen_SRCS_vxWorks += -nil-

# Add support from base/src/vxWorks if needed
#elssen_OBJS_vxWorks += $(EPICS_BASE_BIN)/vxComLibrary

# Finally link to the EPICS Base libraries
elssen_LIBS += $(EPICS_BASE_IOC_LIBS)

#===========================

include $(TOP)/configure/RULES
#----------------------------------------
#  ADD RULES AFTER THIS LINE

