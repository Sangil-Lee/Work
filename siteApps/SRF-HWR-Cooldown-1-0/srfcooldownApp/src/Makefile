TOP=../..

include $(TOP)/configure/CONFIG
#----------------------------------------
#  ADD MACRO DEFINITIONS AFTER THIS LINE
#=============================

#=============================
# Build the IOC application

PROD_IOC = srfcooldown
# srfcooldown.dbd will be created and installed
DBD += srfcooldown.dbd

# srfcooldown.dbd will be made up from these files:
srfcooldown_DBD += base.dbd
srfcooldown_DBD += epidRecord.dbd  scalerRecord.dbd 
srfcooldown_DBD += aCalcoutRecord.dbd  sCalcoutRecord.dbd  sseqRecord.dbd  swaitRecord.dbd  transformRecord.dbd

# Include dbd files from all support applications:
#srfcooldown_DBD += xxx.dbd
srfcooldown_DBD += $(ASYN)/dbd/drvAsynIPPort.dbd
srfcooldown_DBD += $(ASYN)/dbd/asyn.dbd
srfcooldown_DBD += $(CALC)/dbd/calc.dbd
srfcooldown_DBD += $(CALC)/dbd/calcSupport.dbd

srfcooldown_DBD += PhaseEndEval.dbd

# Add all the support libraries needed by this IOC
#srfcooldown_LIBS += xxx

# srfcooldown_registerRecordDeviceDriver.cpp derives from srfcooldown.dbd
srfcooldown_SRCS += srfcooldown_registerRecordDeviceDriver.cpp
srfcooldown_SRCS += PhaseEndEval.cpp

# Build the main IOC entry point on workstation OSs.
srfcooldown_SRCS_DEFAULT += srfcooldownMain.cpp
srfcooldown_SRCS_vxWorks += -nil-

ifneq ($(SNCSEQ),)
    srfcooldown_SNCFLAGS += +r
    srfcooldown_DBD  += sncSRFCooldown.dbd
    srfcooldown_SRCS += sncSRFCooldown.stt
    srfcooldown_LIBS += seq pv
endif

# Finally link to the EPICS Base libraries
#srfcooldown_LIBS += asyn calc std sscan iocshExt
srfcooldown_LIBS += asyn calc std sscan
srfcooldown_LIBS += $(EPICS_BASE_IOC_LIBS)

#===========================

include $(TOP)/configure/RULES
#----------------------------------------
#  ADD RULES AFTER THIS LINE

