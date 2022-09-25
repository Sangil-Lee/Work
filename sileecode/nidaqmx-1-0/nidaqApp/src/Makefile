TOP=../..

include $(TOP)/configure/CONFIG
#----------------------------------------
#  ADD MACRO DEFINITIONS AFTER THIS LINE
#=============================
DBD += nidaqAsynEpics.dbd

LIBRARY_IOC += nidaqAsynEpicsSupport 

#USR_CPPFLAGS += -std=c++11 -ggdb
#nidaqmx_DIR = /usr/lib/x86_64-linux-gnu
USR_DBDFLAGS  += -I$(RAON_SITELIBS)/dbd

#nidaqAsynEpicsSupport_SRCS += niacq_IntClk.cpp nidaqAsynEpics.cpp nidaqDriver.cpp
nidaqAsynEpicsSupport_SRCS += nidaqAsynEpics.cpp nidaqDriver.cpp
nidaqAsynEpicsSupport_LIBS += asyn nidaqmx
nidaqAsynEpicsSupport_LIBS += $(EPICS_BASE_IOC_LIBS)

#=============================
# Build the IOC application

PROD_IOC = nidaq
# nidaq.dbd will be created and installed
DBD += nidaq.dbd

# nidaq.dbd will be made up from these files:
nidaq_DBD += base.dbd nidaqAsynEpics.dbd

# Include dbd files from all support applications:
#nidaq_DBD += xxx.dbd

# Add all the support libraries needed by this IOC
#nidaq_LIBS += xxx

# nidaq_registerRecordDeviceDriver.cpp derives from nidaq.dbd
nidaq_SRCS += nidaq_registerRecordDeviceDriver.cpp

# Build the main IOC entry point on workstation OSs.
nidaq_SRCS_DEFAULT += nidaqMain.cpp
nidaq_SRCS_vxWorks += -nil-

# Add support from base/src/vxWorks if needed
#nidaq_OBJS_vxWorks += $(EPICS_BASE_BIN)/vxComLibrary

# Finally link to the EPICS Base libraries
nidaq_LIBS += asyn nidaqAsynEpicsSupport
nidaq_LIBS += $(EPICS_BASE_IOC_LIBS)

#===========================

include $(TOP)/configure/RULES
#----------------------------------------
#  ADD RULES AFTER THIS LINE

