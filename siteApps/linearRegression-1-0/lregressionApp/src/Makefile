TOP=../..

include $(TOP)/configure/CONFIG
#----------------------------------------
#  ADD MACRO DEFINITIONS AFTER THIS LINE
#=============================

#=============================
# Build the IOC application

PROD_IOC = lregression
# lregression.dbd will be created and installed
DBD += lregression.dbd

INCLUDES += -I/usr/local/include/eigen3

# lregression.dbd will be made up from these files:
lregression_DBD += base.dbd
lregression_DBD += epidRecord.dbd  scalerRecord.dbd 
lregression_DBD += aCalcoutRecord.dbd  sCalcoutRecord.dbd  sseqRecord.dbd  swaitRecord.dbd  transformRecord.dbd
lregression_DBD += asubLReGression.dbd

# Include dbd files from all support applications:
#lregression_DBD += xxx.dbd
lregression_DBD += $(ASYN)/dbd/drvAsynIPPort.dbd
lregression_DBD += $(ASYN)/dbd/asyn.dbd
lregression_DBD += $(CALC)/dbd/calc.dbd
lregression_DBD += $(CALC)/dbd/calcSupport.dbd
lregression_DBD += $(SITELIBS)/dbd/dblfilter.dbd

# Add all the support libraries needed by this IOC
#lregression_LIBS += xxx

# lregression_registerRecordDeviceDriver.cpp derives from lregression.dbd
lregression_SRCS += lregression_registerRecordDeviceDriver.cpp
lregression_SRCS += asubLReGression.cpp

# Build the main IOC entry point on workstation OSs.
lregression_SRCS_DEFAULT += lregressionMain.cpp
lregression_SRCS_vxWorks += -nil-

# Add support from base/src/vxWorks if needed
#lregression_OBJS_vxWorks += $(EPICS_BASE_BIN)/vxComLibrary

ifneq ($(SNCSEQ),)
    lregression_SNCFLAGS += +r
    lregression_DBD  += sncMakeTempWaveform.dbd
    lregression_SRCS += sncMakeTempWaveform.stt
    lregression_LIBS += seq pv
endif
# Finally link to the EPICS Base libraries
lregression_LIBS += asyn calc std sscan iocshExt
lregression_LIBS += $(EPICS_BASE_IOC_LIBS)

#===========================

include $(TOP)/configure/RULES
#----------------------------------------
#  ADD RULES AFTER THIS LINE

