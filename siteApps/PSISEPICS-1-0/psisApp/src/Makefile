TOP=../..

include $(TOP)/configure/CONFIG
#----------------------------------------
#  ADD MACRO DEFINITIONS AFTER THIS LINE
#=============================

#=============================
# Build the IOC application

PROD_IOC = psis
# psis.dbd will be created and installed
DBD += psis.dbd

# psis.dbd will be made up from these files:
psis_DBD += base.dbd

# Include dbd files from all support applications:
psis_DBD += asyn.dbd
psis_DBD += drvAsynIPPort.dbd
psis_DBD += modbusSupport.dbd

# Add all the support libraries needed by this IOC
#psis_LIBS += xxx

# psis_registerRecordDeviceDriver.cpp derives from psis.dbd
psis_SRCS += psis_registerRecordDeviceDriver.cpp

# Build the main IOC entry point on workstation OSs.
psis_SRCS_DEFAULT += psisMain.cpp
psis_SRCS_vxWorks += -nil-

# Add support from base/src/vxWorks if needed
#psis_OBJS_vxWorks += $(EPICS_BASE_BIN)/vxComLibrary

# Finally link to the EPICS Base libraries
psis_LIBS += asyn modbus
psis_LIBS += $(EPICS_BASE_IOC_LIBS)

#===========================

include $(TOP)/configure/RULES
#----------------------------------------
#  ADD RULES AFTER THIS LINE

