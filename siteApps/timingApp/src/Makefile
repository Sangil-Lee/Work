TOP=../..

include $(TOP)/configure/CONFIG
#----------------------------------------
#  ADD MACRO DEFINITIONS AFTER THIS LINE
#=============================

DBD += timingAsynEpics.dbd

LIBRARY_IOC += timingAsynEpicsSupport 

timingAsynEpicsSupport_SRCS += timingAsynEpics.cpp 
timingAsynEpicsSupport_LIBS += asyn 
timingAsynEpicsSupport_LIBS += $(EPICS_BASE_IOC_LIBS)

#=============================
# Build the IOC application

PROD_IOC = timing
# timing.dbd will be created and installed
DBD += timing.dbd

# timing.dbd will be made up from these files:
timing_DBD += base.dbd
timing_DBD += timingAsynEpics.dbd

# Include dbd files from all support applications:
#timing_DBD += xxx.dbd

# Add all the support libraries needed by this IOC
#timing_LIBS += xxx

# timing_registerRecordDeviceDriver.cpp derives from timing.dbd
timing_SRCS += timing_registerRecordDeviceDriver.cpp
timing_LIBS += timingAsynEpicsSupport 
timing_LIBS += asyn 

# Build the main IOC entry point on workstation OSs.
timing_SRCS_DEFAULT += timingMain.cpp
timing_SRCS_vxWorks += -nil-

# Finally link to the EPICS Base libraries
timing_LIBS += $(EPICS_BASE_IOC_LIBS)

#===========================

include $(TOP)/configure/RULES
#----------------------------------------
#  ADD RULES AFTER THIS LINE

