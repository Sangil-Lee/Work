TOP=../..

include $(TOP)/configure/CONFIG
#----------------------------------------
#  ADD MACRO DEFINITIONS AFTER THIS LINE
#=============================

#=============================
# Build the IOC application

PROD_IOC = autoscan
# autoscan.dbd will be created and installed
DBD += autoscan.dbd

# autoscan.dbd will be made up from these files:
autoscan_DBD += base.dbd

# Include dbd files from all support applications:
#autoscan_DBD += xxx.dbd
autoscan_DBD += stream.dbd
autoscan_DBD += asyn.dbd
autoscan_DBD += drvAsynIPPort.dbd
autoscan_DBD += drvAsynSerialPort.dbd

# Add all the support libraries needed by this IOC
#autoscan_LIBS += xxx
autoscan_LIBS += asyn
autoscan_LIBS += stream

# autoscan_registerRecordDeviceDriver.cpp derives from autoscan.dbd
autoscan_SRCS += autoscan_registerRecordDeviceDriver.cpp

# Build the main IOC entry point on workstation OSs.
autoscan_SRCS_DEFAULT += autoscanMain.cpp
autoscan_SRCS_vxWorks += -nil-

# Add support from base/src/vxWorks if needed
#autoscan_OBJS_vxWorks += $(EPICS_BASE_BIN)/vxComLibrary
ifneq ($(SNCSEQ),)
    # This builds sncExample as a component of tr1
    autoscan_SNCFLAGS += +r
    autoscan_DBD  += sncAutoScan.dbd
    autoscan_SRCS += sncAutoScan.stt
    autoscan_LIBS += seq pv
endif

# Finally link to the EPICS Base libraries
autoscan_LIBS += $(EPICS_BASE_IOC_LIBS)

#===========================

include $(TOP)/configure/RULES
#----------------------------------------
#  ADD RULES AFTER THIS LINE

