TOP=../..

include $(TOP)/configure/CONFIG
#----------------------------------------
#  ADD MACRO DEFINITIONS AFTER THIS LINE
#=============================
USR_CXXFLAGS += -Wno-deprecated-declarations

#=============================
# Build the IOC application

PROD_IOC = valveEval
# valveEval.dbd will be created and installed
DBD += valveEval.dbd

# valveEval.dbd will be made up from these files:
valveEval_DBD += base.dbd
valveEval_DBD += epidRecord.dbd  scalerRecord.dbd 
valveEval_DBD += aCalcoutRecord.dbd  sCalcoutRecord.dbd  sseqRecord.dbd  swaitRecord.dbd  transformRecord.dbd

# Include dbd files from all support applications:
#valveEval_DBD += xxx.dbd
valveEval_DBD += $(ASYN)/dbd/drvAsynIPPort.dbd
valveEval_DBD += $(ASYN)/dbd/asyn.dbd
valveEval_DBD += $(CALC)/dbd/calc.dbd
valveEval_DBD += $(CALC)/dbd/calcSupport.dbd
valveEval_DBD += $(SITELIBS)/dbd/dblfilter.dbd
valveEval_DBD += subXVValveSetpt.dbd


# Add all the support libraries needed by this IOC
#valveEval_LIBS += xxx

# valveEval_registerRecordDeviceDriver.cpp derives from valveEval.dbd
valveEval_SRCS += valveEval_registerRecordDeviceDriver.cpp
valveEval_SRCS += subXVValveSetpt.cpp

# Build the main IOC entry point on workstation OSs.
valveEval_SRCS_DEFAULT += valveEvalMain.cpp
valveEval_SRCS_vxWorks += -nil-

# Add support from base/src/vxWorks if needed
#valveEval_OBJS_vxWorks += $(EPICS_BASE_BIN)/vxComLibrary
ifneq ($(SNCSEQ),)
    valveEval_SNCFLAGS += +r
	#
    valveEval_DBD  += CDOM031.dbd
	#Cooldown_03.dbd instead of CDOM031~038
    valveEval_DBD  += Cooldown_03.dbd
    #valveEval_DBD  += Cooldown_04.dbd
    valveEval_DBD  += Cooldown_04_2.dbd
    valveEval_DBD  += Cooldown_12.dbd
    valveEval_DBD  += CDOM1Cycle.dbd
    valveEval_DBD  += CDOMAutoRegulate.dbd
    valveEval_DBD  += sncCDOM14_15_Dic.dbd
    valveEval_DBD  += sncCDOM031.dbd
    valveEval_DBD  += sncCooldown03.dbd
    valveEval_DBD  += sncCooldown04.dbd
    valveEval_DBD  += sncCooldown12.dbd

    valveEval_SRCS += CDOM031.stt
	#Cooldown_03.stt instead of CDOM031~038
    valveEval_SRCS += Cooldown_03.stt
    #valveEval_SRCS += Cooldown_04.stt
    valveEval_SRCS += Cooldown_04_2.stt
    valveEval_SRCS += Cooldown_12.stt
    valveEval_SRCS += CDOM1Cycle.stt
    valveEval_SRCS += CDOMAutoRegulate.stt
    valveEval_SRCS += sncCDOM14_15_Dic.stt
    valveEval_SRCS += sncCDOM031.stt
    valveEval_SRCS += sncCooldown03.stt
    valveEval_SRCS += sncCooldown04.stt
    valveEval_SRCS += sncCooldown12.stt

    valveEval_LIBS += seq pv
endif

# Finally link to the EPICS Base libraries
valveEval_LIBS += asyn calc std sscan iocshExt
valveEval_LIBS += $(EPICS_BASE_IOC_LIBS)

#===========================

include $(TOP)/configure/RULES
#----------------------------------------
#  ADD RULES AFTER THIS LINE

