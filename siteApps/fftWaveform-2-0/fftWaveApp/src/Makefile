TOP=../..

include $(TOP)/configure/CONFIG
#----------------------------------------
#  ADD MACRO DEFINITIONS AFTER THIS LINE
#=============================
INCLUDES += -I$(TOP)/include/eigen3
USR_CXXFLAGS += -Wno-deprecated-declarations -Wno-unused-variable
#=============================
# Build the IOC application

PROD_IOC = fftWave
# fftWave.dbd will be created and installed
DBD += fftWave.dbd

Common_DBDs += base.dbd
Common_DBDs += system.dbd
Common_DBDs += asubFFTWave.dbd 
Common_DBDs += asubFFTWaveW3.dbd 

Common_SRCs += asubFFTWave.cpp 
Common_SRCs += asubFFTWaveW3.cpp 

ifneq ($(FFTW),)
Common_DBDs += fftw.dbd
Common_LIBs += fftwSup
endif

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

ifneq ($(STD),)
Common_DBDs += stdSupport.dbd
Common_LIBs += std
endif

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
#fftWave_SYS_LIBS += uldaq
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
fftWave_DBD  += $(Common_DBDs)
fftWave_LIBS += $(Common_LIBs)
fftWave_SRCS += $(Common_SRCs)
fftWave_SRCS += fftWave_registerRecordDeviceDriver.cpp

# System Libraries
fftWave_SYS_LIBS +=
fftWave_SYS_LIBS_Linux +=

# Build the main IOC entry point on workstation OSs.
fftWave_SRCS_DEFAULT += fftWaveMain.cpp
fftWave_SRCS_vxWorks += -nil-

fftWave_OBJS_vxWorks += $(EPICS_BASE_BIN)/vxComLibrary
fftWave_LIBS += $(EPICS_BASE_IOC_LIBS)

#===========================

include $(TOP)/configure/RULES
#----------------------------------------
#  ADD RULES AFTER THIS LINE

