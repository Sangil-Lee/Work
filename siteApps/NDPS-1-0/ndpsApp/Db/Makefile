TOP=../..

include $(TOP)/configure/CONFIG
#----------------------------------------
#  ADD MACRO DEFINITIONS AFTER THIS LINE
#=============================

#=============================
# Build the IOC application

PROD_IOC = ndps
# ndps.dbd will be created and installed
DBD += ndps.dbd

# ndps.dbd will be made up from these files:
ndps_DBD += base.dbd
ndps_DBD += epidRecord.dbd  scalerRecord.dbd 
ndps_DBD += aCalcoutRecord.dbd  sCalcoutRecord.dbd  sseqRecord.dbd  swaitRecord.dbd  transformRecord.dbd

# Include dbd files from all support applications:
ndps_DBD += $(ASYN)/dbd/drvAsynIPPort.dbd
ndps_DBD += $(ASYN)/dbd/asyn.dbd
ndps_DBD += $(CALC)/dbd/calc.dbd
ndps_DBD += $(CALC)/dbd/calcSupport.dbd

# Include dbd files from all support applications:
#ndps_DBD += xxx.dbd

# Add all the support libraries needed by this IOC
#ndps_LIBS += xxx

# ndps_registerRecordDeviceDriver.cpp derives from ndps.dbd
ndps_SRCS += ndps_registerRecordDeviceDriver.cpp

# Build the main IOC entry point on workstation OSs.
ndps_SRCS_DEFAULT += ndpsMain.cpp
ndps_SRCS_vxWorks += -nil-

ifneq ($(SNCSEQ),)
    ndps_SNCFLAGS += +r
#ndps_DBD += 
#ndps_SRCS += 

    ndps_LIBS += seq pv
    ndps_LIBS += $(EPICS_BASE_HOST_LIBS)
endif

# Add support from base/src/vxWorks if needed
#ndps_OBJS_vxWorks += $(EPICS_BASE_BIN)/vxComLibrary

# Finally link to the EPICS Base libraries
ndps_LIBS += asyn calc std sscan 
ndps_LIBS += $(EPICS_BASE_IOC_LIBS)

#===========================

include $(TOP)/configure/RULES
#----------------------------------------
#  ADD RULES AFTER THIS LINE

