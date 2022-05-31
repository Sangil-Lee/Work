TOP=../..

include $(TOP)/configure/CONFIG
#----------------------------------------
#  ADD MACRO DEFINITIONS AFTER THIS LINE
#=============================

#=============================
# Build the IOC application

PROD_IOC = glalarm
# glalarm.dbd will be created and installed
DBD += glalarm.dbd

# glalarm.dbd will be made up from these files:
glalarm_DBD += base.dbd

# Include dbd files from all support applications:
#glalarm_DBD += xxx.dbd

# Add all the support libraries needed by this IOC
#glalarm_LIBS += xxx

# glalarm_registerRecordDeviceDriver.cpp derives from glalarm.dbd
glalarm_SRCS += glalarm_registerRecordDeviceDriver.cpp

# Build the main IOC entry point on workstation OSs.
glalarm_SRCS_DEFAULT += glalarmMain.cpp
glalarm_SRCS_vxWorks += -nil-

# Add support from base/src/vxWorks if needed
#glalarm_OBJS_vxWorks += $(EPICS_BASE_BIN)/vxComLibrary

# Finally link to the EPICS Base libraries
glalarm_LIBS += $(EPICS_BASE_IOC_LIBS)

#===========================

include $(TOP)/configure/RULES
#----------------------------------------
#  ADD RULES AFTER THIS LINE

