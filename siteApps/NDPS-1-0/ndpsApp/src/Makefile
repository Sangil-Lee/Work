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

# Include dbd files from all support applications:
#ndps_DBD += xxx.dbd
ndps_DBD += asyn.dbd
ndps_DBD += drvAsynIPPort.dbd
ndps_DBD += modbusSupport.dbd
ndps_DBD += motorSupport.dbd
ndps_DBD += phytronSupport.dbd

# Add all the support libraries needed by this IOC
#ndps_LIBS += xxx

# ndps_registerRecordDeviceDriver.cpp derives from ndps.dbd
ndps_SRCS += ndps_registerRecordDeviceDriver.cpp

# Build the main IOC entry point on workstation OSs.
ndps_SRCS_DEFAULT += ndpsMain.cpp
ndps_SRCS_vxWorks += -nil-

# Add support from base/src/vxWorks if needed
#ndps_OBJS_vxWorks += $(EPICS_BASE_BIN)/vxComLibrary

ifneq ($(SNCSEQ),)
    ndps_SNCFLAGS += +r
    #ndps_DBD  +=
    #ndps_SRCS +=
    ndpsIBS += seq pv
endif

# Finally link to the EPICS Base libraries
ndps_LIBS += asyn modbus motor phytronAxisMotor
ndps_LIBS += $(EPICS_BASE_IOC_LIBS)

#===========================

include $(TOP)/configure/RULES
#----------------------------------------
#  ADD RULES AFTER THIS LINE

