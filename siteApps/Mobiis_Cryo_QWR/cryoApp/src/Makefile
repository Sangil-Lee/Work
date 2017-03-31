TOP=../..

include $(TOP)/configure/CONFIG
#----------------------------------------
#  ADD MACRO DEFINITIONS AFTER THIS LINE
#=============================

#=============================
# Build the IOC application

PROD_IOC = cryo
# cryo.dbd will be created and installed
DBD += cryo.dbd

# cryo.dbd will be made up from these files:
cryo_DBD += base.dbd
cryo_DBD += devLakeShore.dbd
cryo_DBD += drvAsynIPPort.dbd
cryo_DBD += ether_ip.dbd
cryo_DBD += seqCryo.dbd

# Include dbd files from all support applications:
#cryo_DBD += xxx.dbd

# Add all the support libraries needed by this IOC
#cryo_LIBS += xxx

# cryo_registerRecordDeviceDriver.cpp derives from cryo.dbd
cryo_SRCS += cryo_registerRecordDeviceDriver.cpp
cryo_SRCS += seqCryo.st

# Build the main IOC entry point on workstation OSs.
cryo_SRCS_DEFAULT += cryoMain.cpp
cryo_SRCS_vxWorks += -nil-

# Add support from base/src/vxWorks if needed
#cryo_OBJS_vxWorks += $(EPICS_BASE_BIN)/vxComLibrary

# Finally link to the EPICS Base libraries
cryo_LIBS += asyn
cryo_LIBS += devLakeShore
cryo_LIBS += ether_ip
cryo_LIBS += seq pv
#cryo_LIBS += autosave
cryo_LIBS += $(EPICS_BASE_IOC_LIBS)

#===========================

include $(TOP)/configure/RULES
#----------------------------------------
#  ADD RULES AFTER THIS LINE

