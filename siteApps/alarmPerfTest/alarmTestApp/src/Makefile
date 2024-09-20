TOP=../..

include $(TOP)/configure/CONFIG
#----------------------------------------
#  ADD MACRO DEFINITIONS AFTER THIS LINE
#=============================

#=============================
# Build the IOC application

PROD_IOC = alarmTest
# alarmTest.dbd will be created and installed
DBD += alarmTest.dbd

Common_DBDs += base.dbd
Common_DBDs += system.dbd
Common_DBDs += subDiskCheck.dbd

Common_SRCs += subDiskCheck.cpp

#ifneq ($(ASYN),)
#Common_DBDs += asyn.dbd
#Common_DBDs += drvAsynIPPort.dbd
#Common_DBDs += drvAsynSerialPort.dbd
#Common_LIBs += asyn
#endif

#ifneq ($(MODBUS),)
#Common_DBDs += modbusSupport.dbd
#Common_LIBs += modbus
#endif

#ifneq ($(SNMP),)
#Common_DBDs += devSnmp.dbd
#Common_LIBs += devSnmp
#endif

#ifneq ($(STD),)
#Common_DBDs += stdSupport.dbd
#Common_LIBs += std
#endif

#ifneq ($(CALC),)
#Common_DBDs += calcSupport.dbd
#Common_LIBs += calc
#endif

#ifneq ($(AUTOSAVE),)
#Common_DBDs += asSupport.dbd
#Common_LIBs += autosave
#endif

ifneq ($(devIocStats),)
Common_DBDs += devIocStats.dbd
Common_LIBs += devIocStats
endif

#ifneq ($(RECCASTER),)
#Common_DBDs += reccaster.dbd
#Common_LIBs += reccaster
#endif

#ifneq ($(SNCSEQ),)
## sncExample_SNCFLAGS += +r
## Common_DBDs += sncExample.dbd
## A .stt sequence program is *not* pre-processed:
## Common_SRCs += sncExample.stt
#
# Common_LIBs += seq pv
#endif

#ifneq ($(STREAM),)
#Common_DBDs += stream.dbd
#Common_LIBs += stream
#endif

#ifneq ($(RETOOLS),)
#Common_DBDs += retools.dbd
#Common_LIBs += retools
#endif

#ifneq ($(CAPUTLOG),)
#Common_DBDs += caPutLog.dbd
#Common_LIBs += caPutLog
#endif

#ifneq ($(MEASCOMP),)
#Common_DBDs += measCompSupport.dbd
#Common_LIBs += measComp
#alarmTest_SYS_LIBS += uldaq
#endif

#ifneq ($(SSCAN),)
#Common_DBDs += sscanSupport.dbd
#Common_LIBs += sscan
#endif

#ifneq ($(BUSY),)
#Common_DBDs += busySupport.dbd
#Common_LIBs += busy
#endif

#ifneq ($(SCALER),)
#Common_DBDs += scalerSupport.dbd
#Common_LIBs += scaler
#endif

#ifneq ($(MCA),)
#Common_DBDs += mcaSupport.dbd
#Common_LIBs += mca
#endif

#ifneq ($(ETHER_IP),)
#Common_DBDs += ether_ip.dbd
#Common_LIBs += ether_ip
#endif

# Include dbd files from all support applications:
alarmTest_DBD  += $(Common_DBDs)
alarmTest_LIBS += $(Common_LIBs)
alarmTest_SRCS += $(Common_SRCs)
alarmTest_SRCS += alarmTest_registerRecordDeviceDriver.cpp

# System Libraries
alarmTest_SYS_LIBS +=
alarmTest_SYS_LIBS_Linux +=

# Build the main IOC entry point on workstation OSs.
alarmTest_SRCS_DEFAULT += alarmTestMain.cpp
alarmTest_SRCS_vxWorks += -nil-

alarmTest_OBJS_vxWorks += $(EPICS_BASE_BIN)/vxComLibrary
alarmTest_LIBS += $(EPICS_BASE_IOC_LIBS)

#===========================

include $(TOP)/configure/RULES
#----------------------------------------
#  ADD RULES AFTER THIS LINE

