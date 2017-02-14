TOP=../..

include $(TOP)/configure/CONFIG
#----------------------------------------
#  ADD MACRO DEFINITIONS AFTER THIS LINE
#=============================

#=============================
# Build the IOC application

PROD_IOC = siemens
# siemens.dbd will be created and installed
DBD += $(MODBUS)/dbd/modbusSupport.dbd
DBD += $(ASYN)/dbd/drvAsynIPPort.dbd
DBD += $(ASYN)/dbd/asyn.dbd
DBD += siemens.dbd

# siemens.dbd will be made up from these files:
siemens_DBD += base.dbd
siemens_DBD += $(MODBUS)/dbd/modbusSupport.dbd
siemens_DBD += $(ASYN)/dbd/drvAsynIPPort.dbd
siemens_DBD += $(ASYN)/dbd/asyn.dbd

# Include dbd files from all support applications:
#siemens_DBD += xxx.dbd

# Add all the support libraries needed by this IOC
#siemens_LIBS += xxx
siemens_LIBS += asyn modbus

# siemens_registerRecordDeviceDriver.cpp derives from siemens.dbd
siemens_SRCS += siemens_registerRecordDeviceDriver.cpp

# Build the main IOC entry point on workstation OSs.
siemens_SRCS_DEFAULT += siemensMain.cpp
siemens_SRCS_vxWorks += -nil-

# Add support from base/src/vxWorks if needed
#siemens_OBJS_vxWorks += $(EPICS_BASE_BIN)/vxComLibrary

# Finally link to the EPICS Base libraries
siemens_LIBS += $(EPICS_BASE_IOC_LIBS)

#===========================

include $(TOP)/configure/RULES
#----------------------------------------
#  ADD RULES AFTER THIS LINE

