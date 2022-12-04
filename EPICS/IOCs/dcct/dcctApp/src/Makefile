TOP=../..

include $(TOP)/configure/CONFIG
#----------------------------------------
#  ADD MACRO DEFINITIONS AFTER THIS LINE
#=============================
# Build an IOC support library
LIBRARY_IOC += dmm7510Support

# Link locally-provided code into the support library,
# rather than directly into the IOC application.
# This is required for Windows DLL builds.
dmm7510Support_SRCS += initTrace.c

dmm7510Support_LIBS += $(EPICS_BASE_IOC_LIBS)

#=============================
# Build the IOC application

PROD_IOC = dcct
# dcct.dbd will be created and installed
DBD += dcct.dbd

Common_DBDs += base.dbd
Common_DBDs += system.dbd

Common_SRCs +=

ifneq ($(ASYN),)
Common_DBDs += asyn.dbd
Common_DBDs += drvAsynIPPort.dbd
Common_DBDs += drvAsynSerialPort.dbd
Common_LIBs += asyn
endif

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

ifneq ($(CALC),)
Common_DBDs += calcSupport.dbd
Common_LIBs += calc
endif

#ifneq ($(AUTOSAVE),)
#Common_DBDs += asSupport.dbd
#Common_LIBs += autosave
#endif

#ifneq ($(devIocStats),)
#Common_DBDs += devIocStats.dbd
#Common_LIBs += devIocStats
#endif

#ifneq ($(RECCASTER),)
#Common_DBDs += reccaster.dbd
#Common_LIBs += reccaster
#endif

ifneq ($(SNCSEQ),)
# sncExample_SNCFLAGS += +r
# Common_DBDs += sncExample.dbd
# A .stt sequence program is *not* pre-processed:
# Common_SRCs += sncExample.stt
    # Build sncDMM7510 into dmm7510Support
    sncDMM7510_SNCFLAGS += +r
    dcct_DBD += sncDMM7510.dbd
    dmm7510Support_SRCS += sncDMM7510.stt
    dmm7510Support_LIBS += seq pv
    dcct_LIBS += seq pv

    # Build sncDMM7510Program as a standalone program
    PROD_HOST += sncDMM7510Program
    sncDMM7510Program_SNCFLAGS += +m
    sncDMM7510Program_SRCS += sncDMM7510Program.st
    sncDMM7510Program_LIBS += seq pv
    sncDMM7510Program_LIBS += $(EPICS_BASE_HOST_LIBS)
    
    # Build sncDCCT into dmm7510Support
    sncDCCT_SNCFLAGS += +r
    dcct_DBD += sncDCCT.dbd
    dmm7510Support_SRCS += sncDCCT.stt
    dmm7510Support_LIBS += seq pv
    dcct_LIBS += seq pv

    # Build sncDCCTProgram as a standalone program
    PROD_HOST += sncDCCTProgram
    sncDCCTProgram_SNCFLAGS += +m
    sncDCCTProgram_SRCS += sncDCCTProgram.st
    sncDCCTProgram_LIBS += seq pv
    sncDCCTProgram_LIBS += $(EPICS_BASE_HOST_LIBS)

Common_LIBs += seq pv
endif

ifneq ($(STREAM),)
Common_DBDs += stream.dbd
Common_LIBs += stream
endif

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
#dcct_SYS_LIBS += uldaq
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
dcct_DBD  += $(Common_DBDs)
dcct_LIBS += $(Common_LIBs)
dcct_LIBS += dmm7510Support
dcct_SRCS += $(Common_SRCs)
dcct_SRCS += dcct_registerRecordDeviceDriver.cpp

# System Libraries
dcct_SYS_LIBS +=
dcct_SYS_LIBS_Linux +=

# Build the main IOC entry point on workstation OSs.
dcct_SRCS_DEFAULT += dcctMain.cpp
dcct_SRCS_vxWorks += -nil-

dcct_OBJS_vxWorks += $(EPICS_BASE_BIN)/vxComLibrary
dcct_LIBS += $(EPICS_BASE_IOC_LIBS)

#===========================

include $(TOP)/configure/RULES
#----------------------------------------
#  ADD RULES AFTER THIS LINE

