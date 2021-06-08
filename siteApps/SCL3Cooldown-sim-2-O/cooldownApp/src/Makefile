TOP=../..

include $(TOP)/configure/CONFIG
#----------------------------------------
#  ADD MACRO DEFINITIONS AFTER THIS LINE
#=============================

#=============================
# Build the IOC application

PROD_IOC = cooldown
# cooldown.dbd will be created and installed
DBD += cooldown.dbd

# cooldown.dbd will be made up from these files:
cooldown_DBD += base.dbd
cooldown_DBD += epidRecord.dbd  scalerRecord.dbd 
cooldown_DBD += aCalcoutRecord.dbd  sCalcoutRecord.dbd  sseqRecord.dbd  swaitRecord.dbd  transformRecord.dbd

# Include dbd files from all support applications:
#cooldown_DBD += xxx.dbd
cooldown_DBD += $(ASYN)/dbd/drvAsynIPPort.dbd
cooldown_DBD += $(ASYN)/dbd/asyn.dbd
cooldown_DBD += $(CALC)/dbd/calc.dbd
cooldown_DBD += $(CALC)/dbd/calcSupport.dbd
cooldown_DBD += $(SITELIBS)/dbd/dblfilter.dbd

cooldown_DBD += PhaseEndEval.dbd

# Add all the support libraries needed by this IOC
#cooldown_LIBS += xxx

# cooldown_registerRecordDeviceDriver.cpp derives from cooldown.dbd
cooldown_SRCS += cooldown_registerRecordDeviceDriver.cpp
cooldown_SRCS += PhaseEndEval.cpp

# Build the main IOC entry point on workstation OSs.
cooldown_SRCS_DEFAULT += cooldownMain.cpp
cooldown_SRCS_vxWorks += -nil-

# Add support from base/src/vxWorks if needed
#cooldown_OBJS_vxWorks += $(EPICS_BASE_BIN)/vxComLibrary

ifneq ($(SNCSEQ),)
    cooldown_SNCFLAGS += +r
    cooldown_DBD  += CDOperationSeq.dbd
    cooldown_DBD  += CDFanoutTest.dbd
    cooldown_DBD  += CDFanoutLogic.dbd
    cooldown_DBD  += sncMakeTempWaveform.dbd
    cooldown_DBD  += CDOM031.dbd
    cooldown_DBD  += CDOM032.dbd
    cooldown_DBD  += CDOM034.dbd
    cooldown_DBD  += CDOM035.dbd
    cooldown_DBD  += CDOM036-1.dbd
    cooldown_DBD  += CDOM036-2.dbd
    cooldown_DBD  += CDOM036-3.dbd
    cooldown_DBD  += CDOM036-4.dbd
    cooldown_DBD  += CDOM036-5.dbd
    cooldown_DBD  += CDOM036-6.dbd
    cooldown_DBD  += CDOM036-7.dbd
    #cooldown_DBD  += CDOM037.dbd
    #cooldown_DBD  += CDOM038.dbd
    cooldown_DBD  += CDOM041_2.dbd 
    cooldown_DBD  += CDOM043-1.dbd
    cooldown_DBD  += CDOM043-2.dbd
    cooldown_DBD  += CDOM043-3.dbd
    cooldown_DBD  += CDOM043-4.dbd
    cooldown_DBD  += CDOM043-5.dbd
    cooldown_DBD  += CDOM043-6.dbd
    cooldown_DBD  += CDOM044.dbd
    cooldown_DBD  += CDOM121.dbd
    cooldown_DBD  += CDOM122.dbd
    cooldown_DBD  += CDOMCompile.dbd
    #cooldown_DBD  += CDOM141.dbd
    #cooldown_DBD  += CDOM142.dbd
    #cooldown_DBD  += CDOM143.dbd
    #cooldown_DBD  += CDOM144.dbd
    #cooldown_DBD  += CDOM145.dbd
    #cooldown_DBD  += CDOM146.dbd
    #cooldown_DBD  += CDOM147.dbd
    #cooldown_DBD  += CDOM15HWR_1.dbd 
    #cooldown_DBD  += CDOM15HWR_2_3.dbd 
    #cooldown_DBD  += CDOM15HWR_4.dbd 
    #cooldown_DBD  += CDOM15HWR_5.dbd 
    #cooldown_DBD  += CDOM15QWR_1_2_3.dbd 
    #cooldown_DBD  += CDOM15QWR_4.dbd 
    #cooldown_DBD  += CDOM15QWR_5.dbd
    #cooldown_DBD  += CDOM21.dbd
    #cooldown_DBD  += CDOM51_1.dbd
    #cooldown_DBD  += CDOM51_2.dbd
    #cooldown_DBD  += CDOM52.dbd
    #cooldown_DBD  += CDOM53_1_3_4.dbd
    #cooldown_DBD  += CDOM53_5.dbd
    #cooldown_DBD  += CDOM53_6.dbd

    cooldown_SRCS += CDOperationSeq.stt
    cooldown_SRCS += CDFanoutTest.stt
    cooldown_SRCS += CDFanoutLogic.stt
    cooldown_SRCS += sncMakeTempWaveform.stt
    cooldown_SRCS += CDOM031.stt
    cooldown_SRCS += CDOM032.stt
    cooldown_SRCS += CDOM034.stt
    cooldown_SRCS += CDOM035.stt
    cooldown_SRCS += CDOM036-1.stt
    cooldown_SRCS += CDOM036-2.stt
    cooldown_SRCS += CDOM036-3.stt
    cooldown_SRCS += CDOM036-4.stt
    cooldown_SRCS += CDOM036-5.stt
    cooldown_SRCS += CDOM036-6.stt
    cooldown_SRCS += CDOM036-7.stt
    #cooldown_SRCS += CDOM037.stt
    #cooldown_SRCS += CDOM038.stt
    cooldown_SRCS += CDOM041_2.stt 
    cooldown_SRCS += CDOM043-1.stt
    cooldown_SRCS += CDOM043-2.stt
    cooldown_SRCS += CDOM043-3.stt
    cooldown_SRCS += CDOM043-4.stt
    cooldown_SRCS += CDOM043-5.stt
    cooldown_SRCS += CDOM043-6.stt
    cooldown_SRCS += CDOM044.stt
    cooldown_SRCS += CDOM121.stt
    cooldown_SRCS += CDOM122.stt
    cooldown_SRCS += CDOMCompile.stt

    #cooldown_SRCS += CDOM141.stt
    #cooldown_SRCS += CDOM142.stt
    #cooldown_SRCS += CDOM143.stt
    #cooldown_SRCS += CDOM144.stt
    #cooldown_SRCS += CDOM145.stt
    #cooldown_SRCS += CDOM146.stt
    #cooldown_SRCS += CDOM147.stt
    #cooldown_SRCS += CDOM15HWR_1.stt 
    #cooldown_SRCS += CDOM15HWR_2_3.stt 
    #cooldown_SRCS += CDOM15HWR_4.stt 
    #cooldown_SRCS += CDOM15HWR_5.stt 
    #cooldown_SRCS += CDOM15QWR_1_2_3.stt 
    #cooldown_SRCS += CDOM15QWR_4.stt 
    #cooldown_SRCS += CDOM15QWR_5.stt
    #cooldown_SRCS += CDOM21.stt
    #cooldown_SRCS += CDOM51_1.stt
    #cooldown_SRCS += CDOM51_2.stt
    #cooldown_SRCS += CDOM52.stt
    #cooldown_SRCS += CDOM53_1_3_4.stt
    #cooldown_SRCS += CDOM53_5.stt
    #cooldown_SRCS += CDOM53_6.stt

    cooldown_LIBS += seq pv
endif
# Finally link to the EPICS Base libraries
cooldown_LIBS += asyn calc std sscan iocshExt
cooldown_LIBS += $(EPICS_BASE_IOC_LIBS)

#===========================

include $(TOP)/configure/RULES
#----------------------------------------
#  ADD RULES AFTER THIS LINE
