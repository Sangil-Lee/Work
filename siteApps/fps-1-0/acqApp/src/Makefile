TOP=../..

include $(TOP)/configure/CONFIG
#----------------------------------------
#  ADD MACRO DEFINITIONS AFTER THIS LINE
#=============================
# Build the Library

DBD += acqFPSAsynEpics.dbd

LIBRARY_IOC += acqFPSAsynEpicsSupport 

USR_DBDFLAGS  += -I$(RAON_SITELIBS)/dbd
timeStamp_DIR += $(RAON_SITELIBS)/lib/$(T_A)

#timingAsynEpicsSupport_SRCS += ts2iic.c ts2ip.c ts2api.c
#timingAsynEpicsSupport_SRCS += ts2iic.cpp ts2api.cpp ts2ip.cpp
acqFPSAsynEpicsSupport_SRCS += acqFPSAsynEpics.cpp acqFPSDriver.cpp
acqFPSAsynEpicsSupport_LIBS += asyn 
acqFPSAsynEpicsSupport_LIBS += $(EPICS_BASE_IOC_LIBS)

#=============================
# Build the IOC application

PROD_IOC = acq
# acq.dbd will be created and installed
DBD += acq.dbd

# acq.dbd will be made up from these files:
acq_DBD += base.dbd
acq_DBD += acqFPSAsynEpics.dbd

# Include dbd files from all support applications:
#acq_DBD += xxx.dbd

# Add all the support libraries needed by this IOC
#acq_LIBS += xxx

# acq_registerRecordDeviceDriver.cpp derives from acq.dbd
acq_SRCS += acq_registerRecordDeviceDriver.cpp

# Build the main IOC entry point on workstation OSs.
acq_SRCS_DEFAULT += acqMain.cpp
acq_SRCS_vxWorks += -nil-

# Add support from base/src/vxWorks if needed
#acq_OBJS_vxWorks += $(EPICS_BASE_BIN)/vxComLibrary

# Finally link to the EPICS Base libraries
acq_LIBS += acqFPSAsynEpicsSupport
acq_LIBS += asyn timeStamp
acq_LIBS += $(EPICS_BASE_IOC_LIBS)

#===========================

include $(TOP)/configure/RULES
#----------------------------------------
#  ADD RULES AFTER THIS LINE

