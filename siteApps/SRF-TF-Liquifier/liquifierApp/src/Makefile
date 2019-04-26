TOP=../..

include $(TOP)/configure/CONFIG
#----------------------------------------
#  ADD MACRO DEFINITIONS AFTER THIS LINE
#=============================

#=============================
# Build the IOC application

PROD_IOC = liquifier
# liquifier.dbd will be created and installed
DBD += liquifier.dbd

# liquifier.dbd will be made up from these files:
liquifier_DBD += base.dbd
liquifier_DBD += drvAsynIPPort.dbd
liquifier_DBD += stream.dbd
liquifier_DBD += modbusSupport.dbd
liquifier_DBD += asyn.dbd

# Include dbd files from all support applications:
#liquifier_DBD += xxx.dbd

# Add all the support libraries needed by this IOC
#liquifier_LIBS += xxx

# liquifier_registerRecordDeviceDriver.cpp derives from liquifier.dbd
liquifier_SRCS += liquifier_registerRecordDeviceDriver.cpp

# Build the main IOC entry point on workstation OSs.
liquifier_SRCS_DEFAULT += liquifierMain.cpp
liquifier_SRCS_vxWorks += -nil-

# Add support from base/src/vxWorks if needed
#liquifier_OBJS_vxWorks += $(EPICS_BASE_BIN)/vxComLibrary

# Finally link to the EPICS Base libraries
liquifier_LIBS += asyn stream modbus
liquifier_LIBS += $(EPICS_BASE_IOC_LIBS)

#===========================

include $(TOP)/configure/RULES
#----------------------------------------
#  ADD RULES AFTER THIS LINE

