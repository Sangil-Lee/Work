TOP=../..

include $(TOP)/configure/CONFIG
#----------------------------------------
#  ADD MACRO DEFINITIONS AFTER THIS LINE
#=============================

#=============================
# Build the IOC application

PROD_IOC = cryoready
# cryoready.dbd will be created and installed
DBD += cryoready.dbd

# cryoready.dbd will be made up from these files:
cryoready_DBD += base.dbd

# Include dbd files from all support applications:
cryoready_DBD += epidRecord.dbd  scalerRecord.dbd 
cryoready_DBD += aCalcoutRecord.dbd  sCalcoutRecord.dbd  sseqRecord.dbd  swaitRecord.dbd  transformRecord.dbd

# Include dbd files from all support applications:
cryoready_DBD += $(ASYN)/dbd/drvAsynIPPort.dbd
cryoready_DBD += $(ASYN)/dbd/asyn.dbd
cryoready_DBD += $(CALC)/dbd/calc.dbd
cryoready_DBD += $(CALC)/dbd/calcSupport.dbd

# Add all the support libraries needed by this IOC
#cryoready_LIBS += xxx

# cryoready_registerRecordDeviceDriver.cpp derives from cryoready.dbd
cryoready_SRCS += cryoready_registerRecordDeviceDriver.cpp

# Build the main IOC entry point on workstation OSs.
cryoready_SRCS_DEFAULT += cryoreadyMain.cpp
cryoready_SRCS_vxWorks += -nil-

# Add support from base/src/vxWorks if needed
#cryoready_OBJS_vxWorks += $(EPICS_BASE_BIN)/vxComLibrary
ifneq ($(SNCSEQ),)
    cryoready_SNCFLAGS += +r
    cryoready_DBD  +=  sncMakeCryoReady.dbd
    cryoready_SRCS +=  sncMakeCryoReady.stt
    cryoready_LIBS += seq pv
endif

# Finally link to the EPICS Base libraries
cryoready_LIBS += asyn calc std sscan 
cryoready_LIBS += $(EPICS_BASE_IOC_LIBS)

#===========================

include $(TOP)/configure/RULES
#----------------------------------------
#  ADD RULES AFTER THIS LINE

