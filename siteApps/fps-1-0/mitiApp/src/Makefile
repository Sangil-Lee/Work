TOP=../..

include $(TOP)/configure/CONFIG
#----------------------------------------
#  ADD MACRO DEFINITIONS AFTER THIS LINE
#=============================
# Build the Library

DBD += mitiFPSAsynEpics.dbd

LIBRARY_IOC += mitiFPSAsynEpicsSupport 

USR_DBDFLAGS  += -I$(RAON_SITELIBS)/dbd
timeStamp_DIR += $(RAON_SITELIBS)/lib/$(T_A)

#timingAsynEpicsSupport_SRCS += ts2iic.c ts2ip.c ts2api.c
#timingAsynEpicsSupport_SRCS += ts2iic.cpp ts2api.cpp ts2ip.cpp
mitiFPSAsynEpicsSupport_SRCS += mitiFPSAsynEpics.cpp mitiFPSDriver.cpp
mitiFPSAsynEpicsSupport_LIBS += asyn 
mitiFPSAsynEpicsSupport_LIBS += $(EPICS_BASE_IOC_LIBS)

#=============================
# Build the IOC application

PROD_IOC = miti
# miti.dbd will be created and installed
DBD += miti.dbd

# miti.dbd will be made up from these files:
miti_DBD += base.dbd
miti_DBD += mitiFPSAsynEpics.dbd

# Include dbd files from all support applications:
#miti_DBD += xxx.dbd

# Add all the support libraries needed by this IOC
#miti_LIBS += xxx

# miti_registerRecordDeviceDriver.cpp derives from miti.dbd
miti_SRCS += miti_registerRecordDeviceDriver.cpp
miti_LIBS += mitiFPSAsynEpicsSupport
miti_LIBS += asyn timeStamp

# Build the main IOC entry point on workstation OSs.
miti_SRCS_DEFAULT += mitiMain.cpp
miti_SRCS_vxWorks += -nil-

# Add support from base/src/vxWorks if needed
#miti_OBJS_vxWorks += $(EPICS_BASE_BIN)/vxComLibrary

# Finally link to the EPICS Base libraries
miti_LIBS += $(EPICS_BASE_IOC_LIBS)

#===========================

include $(TOP)/configure/RULES
#----------------------------------------
#  ADD RULES AFTER THIS LINE

