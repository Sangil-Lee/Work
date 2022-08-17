TOP=../..

include $(TOP)/configure/CONFIG
#----------------------------------------
#  ADD MACRO DEFINITIONS AFTER THIS LINE
#=============================
INCLUDES += -I/usr/include/eigen3
USR_CXXFLAGS += -Wno-deprecated-declarations -Wno-unused-variable -Wno-stringop-truncation -Wno-stringop-overflow

DBD += nicDAQAsynEpics.dbd

LIBRARY_IOC += nicDAQAsynEpicsSupport 

USR_CPPFLAGS += -std=c++11 -ggdb -D_POSIX_C_SOURCE=200809L

nidaqmx_DIR = /usr/lib/x86_64-linux-gnu

USR_DBDFLAGS  += -I$(RAON_SITELIBS)/dbd
#timeStamp_DIR += $(RAON_SITELIBS)/lib/$(T_A)

nicDAQAsynEpicsSupport_SRCS += nicDAQAsynEpics.cpp
nicDAQAsynEpicsSupport_LIBS += asyn nidaqmx
nicDAQAsynEpicsSupport_LIBS += $(EPICS_BASE_IOC_LIBS)

#=============================
# Build the IOC application

PROD_IOC = dpcryo
# dpcryo.dbd will be created and installed
DBD += dpcryo.dbd

# dpcryo.dbd will be made up from these files:
dpcryo_DBD += base.dbd
dpcryo_DBD += nicDAQAsynEpics.dbd
dpcryo_DBD += asubFFTWave.dbd

# Include dbd files from all support applications:
#dpcryo_DBD += xxx.dbd

# Add all the support libraries needed by this IOC
#dpcryo_LIBS += xxx

# dpcryo_registerRecordDeviceDriver.cpp derives from dpcryo.dbd
dpcryo_SRCS += dpcryo_registerRecordDeviceDriver.cpp
dpcryo_SRCS += asubFFTWave.cpp

# Build the main IOC entry point on workstation OSs.
dpcryo_SRCS_DEFAULT += dpcryoMain.cpp
dpcryo_SRCS_vxWorks += -nil-

# Add support from base/src/vxWorks if needed
#dpcryo_OBJS_vxWorks += $(EPICS_BASE_BIN)/vxComLibrary

# Finally link to the EPICS Base libraries
dpcryo_LIBS += asyn nicDAQAsynEpicsSupport
dpcryo_LIBS += $(EPICS_BASE_IOC_LIBS)

#===========================

include $(TOP)/configure/RULES
#----------------------------------------
#  ADD RULES AFTER THIS LINE

