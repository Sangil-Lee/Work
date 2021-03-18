TOP=../..

include $(TOP)/configure/CONFIG
#----------------------------------------
#  ADD MACRO DEFINITIONS AFTER THIS LINE
#=============================

INCLUDES += -I/usr/local/include/eigen3
USR_CXXFLAGS += -Wno-deprecated-declarations -Wno-unused-variable

#=============================
# Build the IOC application

PROD_IOC = fftwave
# fftwave.dbd will be created and installed
DBD += fftwave.dbd

# fftwave.dbd will be made up from these files:
fftwave_DBD += base.dbd
fftwave_DBD += epidRecord.dbd  scalerRecord.dbd 
fftwave_DBD += aCalcoutRecord.dbd  sCalcoutRecord.dbd  sseqRecord.dbd  swaitRecord.dbd  transformRecord.dbd
fftwave_DBD += asubSin2FFT.dbd 

# Include dbd files from all support applications:
#fftwave_DBD += xxx.dbd
fftwave_DBD += $(ASYN)/dbd/drvAsynIPPort.dbd
fftwave_DBD += $(ASYN)/dbd/asyn.dbd
fftwave_DBD += $(CALC)/dbd/calc.dbd
fftwave_DBD += $(CALC)/dbd/calcSupport.dbd
fftwave_DBD += $(SITELIBS)/dbd/dblfilter.dbd

# Add all the support libraries needed by this IOC
#fftwave_LIBS += xxx

# fftwave_registerRecordDeviceDriver.cpp derives from fftwave.dbd
fftwave_SRCS += fftwave_registerRecordDeviceDriver.cpp
fftwave_SRCS += asubSin2FFT.cpp

# Build the main IOC entry point on workstation OSs.
fftwave_SRCS_DEFAULT += fftwaveMain.cpp
fftwave_SRCS_vxWorks += -nil-

# Add support from base/src/vxWorks if needed
#fftwave_OBJS_vxWorks += $(EPICS_BASE_BIN)/vxComLibrary
ifneq ($(SNCSEQ),)
    fftwave_SNCFLAGS += +r
#	fftwave_DBD  += sncMakeTempWaveform.dbd
#   fftwave_SRCS += sncMakeTempWaveform.stt

    fftwave_LIBS += seq pv
endif

# Finally link to the EPICS Base libraries
fftwave_LIBS += asyn calc std sscan iocshExt
fftwave_LIBS += $(EPICS_BASE_IOC_LIBS)

#===========================

include $(TOP)/configure/RULES
#----------------------------------------
#  ADD RULES AFTER THIS LINE

