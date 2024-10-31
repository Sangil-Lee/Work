TOP=../..

include $(TOP)/configure/CONFIG
#----------------------------------------
#  ADD MACRO DEFINITIONS AFTER THIS LINE
#=============================

#=============================
# Build the IOC application

PROD_IOC = ifmotor
# ifmotor.dbd will be created and installed
DBD += ifmotor.dbd

# ifmotor.dbd will be made up from these files:
ifmotor_DBD += base.dbd

# Include dbd files from all support applications:
#ifmotor_DBD += xxx.dbd
ifmotor_DBD += epidRecord.dbd  scalerRecord.dbd 
ifmotor_DBD += aCalcoutRecord.dbd  sCalcoutRecord.dbd  sseqRecord.dbd  swaitRecord.dbd  transformRecord.dbd
ifmotor_DBD += drvAsynIPPort.dbd
ifmotor_DBD += modbusSupport.dbd
ifmotor_DBD += asyn.dbd
ifmotor_DBD += calc.dbd
ifmotor_DBD += calcSupport.dbd
ifmotor_DBD += stream.dbd
ifmotor_DBD += aSubHexStrtoDec.dbd

# Add all the support libraries needed by this IOC
#ifmotor_LIBS += xxx

# ifmotor_registerRecordDeviceDriver.cpp derives from ifmotor.dbd
ifmotor_SRCS += ifmotor_registerRecordDeviceDriver.cpp
ifmotor_SRCS += aSubHexStrtoDec.cpp
# Build the main IOC entry point on workstation OSs.
ifmotor_SRCS_DEFAULT += ifmotorMain.cpp
ifmotor_SRCS_vxWorks += -nil-

# Add support from base/src/vxWorks if needed
#ifmotor_OBJS_vxWorks += $(EPICS_BASE_BIN)/vxComLibrary

ifneq ($(SNCSEQ),)
    ifmotor_SNCFLAGS += +r
    #ifmotor_DBD  +=
    #ifmotor_SRCS +=
    ifmotorIBS += seq pv
endif

# Finally link to the EPICS Base libraries
ifmotor_LIBS += asyn modbus calc std sscan stream
# Finally link to the EPICS Base libraries
ifmotor_LIBS += $(EPICS_BASE_IOC_LIBS)

#===========================

include $(TOP)/configure/RULES
#----------------------------------------
#  ADD RULES AFTER THIS LINE

