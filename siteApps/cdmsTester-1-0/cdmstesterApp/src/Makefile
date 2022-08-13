TOP=../..

include $(TOP)/configure/CONFIG
#----------------------------------------
#  ADD MACRO DEFINITIONS AFTER THIS LINE
#=============================
USR_CPPFLAGS += -std=c++11 -ggdb
nidaqmx_DIR = /usr/lib/x86_64-linux-gnu

USR_DBDFLAGS  += -I$(RAON_SITELIBS)/dbd

DBD += rtpDev.dbd
DBD += nidaqAsynEpics.dbd

INC			  += drvRtpAsyn.h
USR_INCLUDES  += -I$(RAON_SITELIBS)/include

LIBRARY_IOC   += rtpDev nidaqAsynEpicsSupport 

rtpDev_SRCS += rtpDevManager.cpp rtpSyncManager.cpp
rtpDev_SRCS +=drvRtpAsyn.c
rtpDev_SRCS +=rtpInterpose.c
rtpDev_LIBS += asyn
rtpDeb_LIBS += $(EPICS_BASE_IOC_LIBS)

nidaqAsynEpicsSupport_SRCS += nidaqAsynEpics.cpp nidaqDriver.cpp
nidaqAsynEpicsSupport_LIBS += asyn nidaqmx
nidaqAsynEpicsSupport_LIBS += $(EPICS_BASE_IOC_LIBS)

#=============================
# Build the IOC application

PROD_IOC = cdmstester
# cdmstester.dbd will be created and installed
DBD += cdmstester.dbd

# cdmstester.dbd will be made up from these files:
cdmstester_DBD += base.dbd

# Include dbd files from all support applications:
#cdmstester_DBD += xxx.dbd
cdmstester_DBD += epidRecord.dbd  scalerRecord.dbd 
cdmstester_DBD += aCalcoutRecord.dbd  sCalcoutRecord.dbd  sseqRecord.dbd  swaitRecord.dbd  transformRecord.dbd
cdmstester_DBD += drvAsynIPPort.dbd
cdmstester_DBD += modbusSupport.dbd
cdmstester_DBD += asyn.dbd
cdmstester_DBD += calc.dbd
cdmstester_DBD += calcSupport.dbd
cdmstester_DBD += rtpDev.dbd

# Add all the support libraries needed by this IOC
#cdmstester_LIBS += xxx

# cdmstester_registerRecordDeviceDriver.cpp derives from cdmstester.dbd
cdmstester_SRCS += cdmstester_registerRecordDeviceDriver.cpp

# Build the main IOC entry point on workstation OSs.
cdmstester_SRCS_DEFAULT += cdmstesterMain.cpp
cdmstester_SRCS_vxWorks += -nil-

# Add support from base/src/vxWorks if needed
#cdmstester_OBJS_vxWorks += $(EPICS_BASE_BIN)/vxComLibrary

ifneq ($(SNCSEQ),)
    cdmstester_SNCFLAGS += +r
    #cdmstester_DBD  +=
    #cdmstester_SRCS +=
    cooldown_LIBS += seq pv
endif

# Finally link to the EPICS Base libraries
cdmstester_LIBS += asyn modbus calc std sscan rtpDev nidaqAsynEpicsSupport
cdmstester_LIBS += $(EPICS_BASE_IOC_LIBS)

#===========================

include $(TOP)/configure/RULES
#----------------------------------------
#  ADD RULES AFTER THIS LINE

