TOP=../..

include $(TOP)/configure/CONFIG
#----------------------------------------
#  ADD MACRO DEFINITIONS AFTER THIS LINE
#=============================

#=============================
# Build the IOC application

PROD_IOC = gbintstat
# gbintstat.dbd will be created and installed
DBD += gbintstat.dbd

# gbintstat.dbd will be made up from these files:
gbintstat_DBD += base.dbd

# Include dbd files from all support applications:
gbintstat_DBD += epidRecord.dbd  scalerRecord.dbd
gbintstat_DBD += aCalcoutRecord.dbd  sCalcoutRecord.dbd  sseqRecord.dbd  swaitRecord.dbd  transformRecord.dbd

# Include dbd files from all support applications:
gbintstat_DBD += $(ASYN)/dbd/drvAsynIPPort.dbd
gbintstat_DBD += $(ASYN)/dbd/asyn.dbd
gbintstat_DBD += $(CALC)/dbd/calc.dbd
gbintstat_DBD += $(CALC)/dbd/calcSupport.dbd


# Add all the support libraries needed by this IOC
#gbintstat_LIBS += xxx

# gbintstat_registerRecordDeviceDriver.cpp derives from gbintstat.dbd
gbintstat_SRCS += gbintstat_registerRecordDeviceDriver.cpp

# Build the main IOC entry point on workstation OSs.
gbintstat_SRCS_DEFAULT += gbintstatMain.cpp
gbintstat_SRCS_vxWorks += -nil-

# Add support from base/src/vxWorks if needed
#gbintstat_OBJS_vxWorks += $(EPICS_BASE_BIN)/vxComLibrary
ifneq ($(SNCSEQ),)
    gbintstat_SNCFLAGS += +r
    gbintstat_DBD  +=  sncMakeCryoReady.dbd
    gbintstat_SRCS +=  sncMakeCryoReady.stt
    gbintstat_LIBS += seq pv
endif


# Finally link to the EPICS Base libraries
gbintstat_LIBS += asyn calc std sscan 
gbintstat_LIBS += $(EPICS_BASE_IOC_LIBS)

#===========================

include $(TOP)/configure/RULES
#----------------------------------------
#  ADD RULES AFTER THIS LINE

