TOP=../..

include $(TOP)/configure/CONFIG
#----------------------------------------
#  ADD MACRO DEFINITIONS AFTER THIS LINE
#=============================

#=============================
# Build the IOC application

PROD_IOC = phasend
# phasend.dbd will be created and installed
DBD += phasend.dbd

# phasend.dbd will be made up from these files:
phasend_DBD += base.dbd
phasend_DBD += epidRecord.dbd  scalerRecord.dbd 
phasend_DBD += aCalcoutRecord.dbd  sCalcoutRecord.dbd  sseqRecord.dbd  swaitRecord.dbd  transformRecord.dbd

# Include dbd files from all support applications:
#phasend_DBD += xxx.dbd
phasend_DBD += $(ASYN)/dbd/drvAsynIPPort.dbd
phasend_DBD += $(ASYN)/dbd/asyn.dbd
phasend_DBD += $(CALC)/dbd/calc.dbd
phasend_DBD += $(CALC)/dbd/calcSupport.dbd
phasend_DBD += $(SITELIBS)/dbd/dblfilter.dbd

# Add all the support libraries needed by this IOC
#phasend_LIBS += xxx

# phasend_registerRecordDeviceDriver.cpp derives from phasend.dbd
phasend_SRCS += phasend_registerRecordDeviceDriver.cpp

# Build the main IOC entry point on workstation OSs.
phasend_SRCS_DEFAULT += phasendMain.cpp
phasend_SRCS_vxWorks += -nil-

# Add support from base/src/vxWorks if needed
#phasend_OBJS_vxWorks += $(EPICS_BASE_BIN)/vxComLibrary

ifneq ($(SNCSEQ),)
    phasend_SNCFLAGS += +r
    #phasend_DBD  += CDOperationSeq.dbd
    #phasend_DBD  += sncMakeTempWaveform.dbd
    phasend_DBD += sncTempWF.dbd
    phasend_DBD += sncTempEvalWF.dbd
    phasend_DBD += sncPhaseEnd.dbd

    #phasend_SRCS += CDOperationSeq.stt
    #phasend_SRCS += sncMakeTempWaveform.stt
    phasend_SRCS += sncTempWF.stt
    phasend_SRCS += sncTempEvalWF.stt
    phasend_SRCS += sncPhaseEnd.stt

    phasend_LIBS += seq pv
endif
# Finally link to the EPICS Base libraries
phasend_LIBS += asyn calc std sscan iocshExt
phasend_LIBS += $(EPICS_BASE_IOC_LIBS)

#===========================

include $(TOP)/configure/RULES
#----------------------------------------
#  ADD RULES AFTER THIS LINE

