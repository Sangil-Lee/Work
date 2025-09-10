TOP=../..

include $(TOP)/configure/CONFIG
#----------------------------------------
#  ADD MACRO DEFINITIONS AFTER THIS LINE
#=============================

#=============================
# Build the IOC application

PROD_IOC = radondetect
# radondetect.dbd will be created and installed
DBD += radondetect.dbd

# radondetect.dbd will be made up from these files:
radondetect_DBD += base.dbd

# Include dbd files from all support applications:
#radondetect_DBD += xxx.dbd

# Add all the support libraries needed by this IOC
#radondetect_LIBS += xxx

# radondetect_registerRecordDeviceDriver.cpp derives from radondetect.dbd
radondetect_SRCS += radondetect_registerRecordDeviceDriver.cpp

# Build the main IOC entry point on workstation OSs.
radondetect_SRCS_DEFAULT += radondetectMain.cpp
radondetect_SRCS_vxWorks += -nil-

# Add support from base/src/vxWorks if needed
#radondetect_OBJS_vxWorks += $(EPICS_BASE_BIN)/vxComLibrary

# Finally link to the EPICS Base libraries
radondetect_LIBS += $(EPICS_BASE_IOC_LIBS)

#===========================

include $(TOP)/configure/RULES
#----------------------------------------
#  ADD RULES AFTER THIS LINE

