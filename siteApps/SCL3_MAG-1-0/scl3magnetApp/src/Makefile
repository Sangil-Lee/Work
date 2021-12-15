TOP=../..

include $(TOP)/configure/CONFIG
#----------------------------------------
#  ADD MACRO DEFINITIONS AFTER THIS LINE
#=============================

#=============================
# Build the IOC application

PROD_IOC = scl3magnet
# scl3magnet.dbd will be created and installed
DBD += scl3magnet.dbd

# scl3magnet.dbd will be made up from these files:
scl3magnet_DBD += base.dbd
scl3magnet_DBD += epidRecord.dbd  scalerRecord.dbd 
scl3magnet_DBD += aCalcoutRecord.dbd  sCalcoutRecord.dbd  sseqRecord.dbd  swaitRecord.dbd  transformRecord.dbd

# Include dbd files from all support applications:
scl3magnet_DBD += $(ASYN)/dbd/drvAsynIPPort.dbd
scl3magnet_DBD += $(ASYN)/dbd/asyn.dbd
scl3magnet_DBD += $(CALC)/dbd/calc.dbd
scl3magnet_DBD += $(CALC)/dbd/calcSupport.dbd
scl3magnet_DBD += $(STREAM)/dbd/stream.dbd

# Add all the support libraries needed by this IOC
#scl3magnet_LIBS += xxx

# scl3magnet_registerRecordDeviceDriver.cpp derives from scl3magnet.dbd
scl3magnet_SRCS += scl3magnet_registerRecordDeviceDriver.cpp

# Build the main IOC entry point on workstation OSs.
scl3magnet_SRCS_DEFAULT += scl3magnetMain.cpp
scl3magnet_SRCS_vxWorks += -nil-

# Add support from base/src/vxWorks if needed
#scl3magnet_OBJS_vxWorks += $(EPICS_BASE_BIN)/vxComLibrary
ifneq ($(SNCSEQ),)
    scl3magnet_SNCFLAGS += +r
#scl3magnet_DBD  += sncAutoRampup.dbd
#scl3magnet_DBD  += sncAutoRampdown.dbd
#scl3magnet_SRCS += sncAutoRampup.stt
#scl3magnet_SRCS += sncAutoRampdown.stt

    scl3magnet_LIBS += seq pv
endif
# Finally link to the EPICS Base libraries
scl3magnet_LIBS += asyn calc std sscan stream

# Finally link to the EPICS Base libraries
scl3magnet_LIBS += $(EPICS_BASE_IOC_LIBS)

#===========================

include $(TOP)/configure/RULES
#----------------------------------------
#  ADD RULES AFTER THIS LINE

