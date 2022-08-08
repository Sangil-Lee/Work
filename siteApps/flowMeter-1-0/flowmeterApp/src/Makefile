TOP=../..

include $(TOP)/configure/CONFIG
#----------------------------------------
#  ADD MACRO DEFINITIONS AFTER THIS LINE
#=============================

#=============================
# Build the IOC application

PROD_IOC = flowmeter
# flowmeter.dbd will be created and installed
DBD += flowmeter.dbd

# flowmeter.dbd will be made up from these files:
flowmeter_DBD += base.dbd
flowmeter_DBD += asyn.dbd
flowmeter_DBD += drvAsynIPPort.dbd
flowmeter_DBD += modbusSupport.dbd

# Include dbd files from all support applications:
#flowmeter_DBD += xxx.dbd

# Add all the support libraries needed by this IOC
#flowmeter_LIBS += xxx

# flowmeter_registerRecordDeviceDriver.cpp derives from flowmeter.dbd
flowmeter_SRCS += flowmeter_registerRecordDeviceDriver.cpp

# Build the main IOC entry point on workstation OSs.
flowmeter_SRCS_DEFAULT += flowmeterMain.cpp
flowmeter_SRCS_vxWorks += -nil-

# Add support from base/src/vxWorks if needed
#flowmeter_OBJS_vxWorks += $(EPICS_BASE_BIN)/vxComLibrary

# Finally link to the EPICS Base libraries
flowmeter_LIBS += asyn modbus
flowmeter_LIBS += $(EPICS_BASE_IOC_LIBS)

#===========================

include $(TOP)/configure/RULES
#----------------------------------------
#  ADD RULES AFTER THIS LINE

