TOP=../..

include $(TOP)/configure/CONFIG
#----------------------------------------
#  ADD MACRO DEFINITIONS AFTER THIS LINE
#=============================

#=============================
# Build the IOC application

PROD_IOC = s7Modbus
# s7Modbus.dbd will be created and installed
DBD += $(MODBUS)/dbd/modbusSupport.dbd
DBD += $(ASYN)/dbd/drvAsynIPPort.dbd
DBD += $(ASYN)/dbd/asyn.dbd
DBD += s7Modbus.dbd

# s7Modbus.dbd will be made up from these files:
s7Modbus_DBD += base.dbd
s7Modbus_DBD += $(TOP)/dbd/modbusSupport.dbd
s7Modbus_DBD += $(TOP)/dbd/drvAsynIPPort.dbd
s7Modbus_DBD += $(TOP)/dbd/asyn.dbd

# Include dbd files from all support applications:
#s7Modbus_DBD += xxx.dbd

# Add all the support libraries needed by this IOC
#s7Modbus_LIBS += xxx
s7Modbus_LIBS += asyn modbus

# s7Modbus_registerRecordDeviceDriver.cpp derives from s7Modbus.dbd
s7Modbus_SRCS += s7Modbus_registerRecordDeviceDriver.cpp

# Build the main IOC entry point on workstation OSs.
s7Modbus_SRCS_DEFAULT += s7ModbusMain.cpp
s7Modbus_SRCS_vxWorks += -nil-

# Add support from base/src/vxWorks if needed
#s7Modbus_OBJS_vxWorks += $(EPICS_BASE_BIN)/vxComLibrary

# Finally link to the EPICS Base libraries
s7Modbus_LIBS += $(EPICS_BASE_IOC_LIBS)

#===========================

include $(TOP)/configure/RULES
#----------------------------------------
#  ADD RULES AFTER THIS LINE

