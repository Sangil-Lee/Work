TOP=../..

include $(TOP)/configure/CONFIG
#----------------------------------------
#  ADD MACRO DEFINITIONS AFTER THIS LINE
#=============================

#=============================
# Build the IOC application

PROD_IOC = alarm
# alarm.dbd will be created and installed
DBD += alarm.dbd

# alarm.dbd will be made up from these files:
alarm_DBD += base.dbd

# Include dbd files from all support applications:
#alarm_DBD += xxx.dbd

# Add all the support libraries needed by this IOC
#alarm_LIBS += xxx

# alarm_registerRecordDeviceDriver.cpp derives from alarm.dbd
alarm_SRCS += alarm_registerRecordDeviceDriver.cpp

# Build the main IOC entry point on workstation OSs.
alarm_SRCS_DEFAULT += alarmMain.cpp
alarm_SRCS_vxWorks += -nil-

# Add support from base/src/vxWorks if needed
#alarm_OBJS_vxWorks += $(EPICS_BASE_BIN)/vxComLibrary

# Finally link to the EPICS Base libraries
alarm_LIBS += $(EPICS_BASE_IOC_LIBS)

#===========================

include $(TOP)/configure/RULES
#----------------------------------------
#  ADD RULES AFTER THIS LINE

