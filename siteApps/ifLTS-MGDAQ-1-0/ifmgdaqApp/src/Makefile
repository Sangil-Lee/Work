TOP=../..

include $(TOP)/configure/CONFIG
#----------------------------------------
#  ADD MACRO DEFINITIONS AFTER THIS LINE
#=============================
INCLUDES += -I/usr/include/eigen3
USR_CXXFLAGS += -Wno-deprecated-declarations -Wno-unused-variable -Wno-stringop-truncation -Wno-stringop-overflow
USR_CPPFLAGS += -std=c++11 -ggdb -D_POSIX_C_SOURCE=200809L

DBD += nicDAQAsynEpics.dbd
LIBRARY_IOC += nicDAQAsynEpicsSupport 
nidaqmx_DIR = /usr/lib/x86_64-linux-gnu

USR_DBDFLAGS  += -I$(RAON_SITELIBS)/dbd
#timeStamp_DIR += $(RAON_SITELIBS)/lib/$(T_A)

nicDAQAsynEpicsSupport_SRCS += nicDAQAsynEpics.cpp
nicDAQAsynEpicsSupport_LIBS += asyn nidaqmx
nicDAQAsynEpicsSupport_LIBS += $(EPICS_BASE_IOC_LIBS)

#=============================
# Build the IOC application

PROD_IOC = ifmgdaq
# ifmgdaq.dbd will be created and installed
DBD += ifmgdaq.dbd

# ifmgdaq.dbd will be made up from these files:
ifmgdaq_DBD += base.dbd
ifmgdaq_DBD += nicDAQAsynEpics.dbd
#ifmgdaq_DBD += asubFFTWave.dbd

# Include dbd files from all support applications:
#ifmgdaq_DBD += xxx.dbd

# Add all the support libraries needed by this IOC
#ifmgdaq_LIBS += xxx

# ifmgdaq_registerRecordDeviceDriver.cpp derives from ifmgdaq.dbd
ifmgdaq_SRCS += ifmgdaq_registerRecordDeviceDriver.cpp
#ifmgdaq_SRCS += asubFFTWave.cpp

# Build the main IOC entry point on workstation OSs.
ifmgdaq_SRCS_DEFAULT += ifmgdaqMain.cpp
ifmgdaq_SRCS_vxWorks += -nil-

# Add support from base/src/vxWorks if needed
#ifmgdaq_OBJS_vxWorks += $(EPICS_BASE_BIN)/vxComLibrary

# Finally link to the EPICS Base libraries
ifmgdaq_LIBS += asyn nicDAQAsynEpicsSupport
ifmgdaq_LIBS += $(EPICS_BASE_IOC_LIBS)

#===========================

include $(TOP)/configure/RULES
#----------------------------------------
#  ADD RULES AFTER THIS LINE

