TOP=../..

include $(TOP)/configure/CONFIG
#----------------------------------------
#  ADD MACRO DEFINITIONS AFTER THIS LINE
#=============================

#=============================
# Build the IOC application

PROD_IOC = kobramagnet
# kobramagnet.dbd will be created and installed
DBD += kobramagnet.dbd

# kobramagnet.dbd will be made up from these files:
kobramagnet_DBD += base.dbd
kobramagnet_DBD += epidRecord.dbd  scalerRecord.dbd 
kobramagnet_DBD += aCalcoutRecord.dbd  sCalcoutRecord.dbd  sseqRecord.dbd  swaitRecord.dbd  transformRecord.dbd

# Include dbd files from all support applications:
kobramagnet_DBD += $(ASYN)/dbd/drvAsynIPPort.dbd
kobramagnet_DBD += $(ASYN)/dbd/asyn.dbd
kobramagnet_DBD += $(CALC)/dbd/calc.dbd
kobramagnet_DBD += $(CALC)/dbd/calcSupport.dbd

# Include dbd files from all support applications:
#kobramagnet_DBD += xxx.dbd

# Add all the support libraries needed by this IOC
#kobramagnet_LIBS += xxx

# kobramagnet_registerRecordDeviceDriver.cpp derives from kobramagnet.dbd
kobramagnet_SRCS += kobramagnet_registerRecordDeviceDriver.cpp

# Build the main IOC entry point on workstation OSs.
kobramagnet_SRCS_DEFAULT += kobramagnetMain.cpp
kobramagnet_SRCS_vxWorks += -nil-

# Add support from base/src/vxWorks if needed
#kobramagnet_OBJS_vxWorks += $(EPICS_BASE_BIN)/vxComLibrary
ifneq ($(SNCSEQ),)
    kobramagnet_SNCFLAGS += +r
    #kobramagnet_DBD  += CDOperationSeq.dbd
    #kobramagnet_DBD  += CDFanoutTest.dbd
    #kobramagnet_DBD  += CDFanoutLogic.dbd
    #kobramagnet_DBD  += sncMakeTempWaveform.dbd

    #kobramagnet_SRCS += CDOperationSeq.stt
    #kobramagnet_SRCS += CDFanoutTest.stt
    #kobramagnet_SRCS += CDFanoutLogic.stt
    #kobramagnet_SRCS += sncMakeTempWaveform.stt

    kobramagnet_LIBS += seq pv
endif
# Finally link to the EPICS Base libraries
kobramagnet_LIBS += asyn calc std sscan
kobramagnet_LIBS += $(EPICS_BASE_IOC_LIBS)

#===========================

include $(TOP)/configure/RULES
#----------------------------------------
#  ADD RULES AFTER THIS LINE

