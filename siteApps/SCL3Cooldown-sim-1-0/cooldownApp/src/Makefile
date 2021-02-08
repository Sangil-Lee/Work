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
cooldown_DBD += readBack.dbd
cooldown_DBD += epidRecord.dbd  scalerRecord.dbd 
cooldown_DBD += aCalcoutRecord.dbd  sCalcoutRecord.dbd  sseqRecord.dbd  swaitRecord.dbd  transformRecord.dbd

# Include dbd files from all support applications:
#cooldown_DBD += xxx.dbd
cooldown_DBD += $(ASYN)/dbd/drvAsynIPPort.dbd
cooldown_DBD += $(ASYN)/dbd/asyn.dbd
cooldown_DBD += $(CALC)/dbd/calc.dbd
cooldown_DBD += $(CALC)/dbd/calcSupport.dbd

# Add all the support libraries needed by this IOC
#cooldown_LIBS += xxx

# cooldown_registerRecordDeviceDriver.cpp derives from cooldown.dbd
cooldown_SRCS += cooldown_registerRecordDeviceDriver.cpp
cooldown_SRCS += readBack.cpp

# Build the main IOC entry point on workstation OSs.
cooldown_SRCS_DEFAULT += cooldownMain.cpp
cooldown_SRCS_vxWorks += -nil-

# Add support from base/src/vxWorks if needed
#cooldown_OBJS_vxWorks += $(EPICS_BASE_BIN)/vxComLibrary

ifneq ($(SNCSEQ),)
    cooldown_SNCFLAGS += +r
    #cooldown_DBD  += 
    #cooldown_SRCS += 
    cooldown_LIBS += seq pv
endif
# Finally link to the EPICS Base libraries
cooldown_LIBS += asyn calc std sscan
cooldown_LIBS += $(EPICS_BASE_IOC_LIBS)

#===========================

include $(TOP)/configure/RULES
#----------------------------------------
#  ADD RULES AFTER THIS LINE

