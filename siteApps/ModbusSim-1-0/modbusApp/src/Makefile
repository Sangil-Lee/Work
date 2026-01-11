TOP=../..

include $(TOP)/configure/CONFIG
#----------------------------------------
#  ADD MACRO DEFINITIONS AFTER THIS LINE
#=============================

#=============================
# Build the IOC application

PROD_IOC = modbus
# modbus.dbd will be created and installed
DBD += modbus.dbd

# modbus.dbd will be made up from these files:
modbus_DBD += base.dbd
modbus_DBD += asyn.dbd
modbus_DBD += drvAsynIPPort.dbd
modbus_DBD += modbusSupport.dbd

# Include dbd files from all support applications:
#modbus_DBD += xxx.dbd

# Add all the support libraries needed by this IOC
#modbus_LIBS += xxx

# modbus_registerRecordDeviceDriver.cpp derives from modbus.dbd
modbus_SRCS += modbus_registerRecordDeviceDriver.cpp

# Build the main IOC entry point on workstation OSs.
modbus_SRCS_DEFAULT += modbusMain.cpp
modbus_SRCS_vxWorks += -nil-

# Add support from base/src/vxWorks if needed
#modbus_OBJS_vxWorks += $(EPICS_BASE_BIN)/vxComLibrary

# Finally link to the EPICS Base libraries
modbus_LIBS += asyn modbus
modbus_LIBS += $(EPICS_BASE_IOC_LIBS)

#===========================

include $(TOP)/configure/RULES
#----------------------------------------
#  ADD RULES AFTER THIS LINE

