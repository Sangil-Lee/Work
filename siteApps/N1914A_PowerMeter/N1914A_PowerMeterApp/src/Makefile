TOP=../..

include $(TOP)/configure/CONFIG
#----------------------------------------
#  ADD MACRO DEFINITIONS AFTER THIS LINE
#=============================

#=============================
# Build the IOC application

PROD_IOC = N1914A_PowerMeter
# N1914A_PowerMeter.dbd will be created and installed
DBD += N1914A_PowerMeter.dbd

# N1914A_PowerMeter.dbd will be made up from these files:
N1914A_PowerMeter_DBD += base.dbd

# Include dbd files from all support applications:
#N1914A_PowerMeter_DBD += xxx.dbd

N1914A_PowerMeter_DBD += stream.dbd
N1914A_PowerMeter_DBD += asyn.dbd
#N1914A_PowerMeter_DBD += drvAsynSerialPort.dbd
N1914A_PowerMeter_DBD += drvAsynIPPort.dbd

# Add all the support libraries needed by this IOC
#N1914A_PowerMeter_LIBS += xxx
N1914A_PowerMeter_LIBS += stream asyn


# N1914A_PowerMeter_registerRecordDeviceDriver.cpp derives from N1914A_PowerMeter.dbd
N1914A_PowerMeter_SRCS += N1914A_PowerMeter_registerRecordDeviceDriver.cpp

# Build the main IOC entry point on workstation OSs.
N1914A_PowerMeter_SRCS_DEFAULT += N1914A_PowerMeterMain.cpp
N1914A_PowerMeter_SRCS_vxWorks += -nil-

# Add support from base/src/vxWorks if needed
#N1914A_PowerMeter_OBJS_vxWorks += $(EPICS_BASE_BIN)/vxComLibrary

# Finally link to the EPICS Base libraries
N1914A_PowerMeter_LIBS += $(EPICS_BASE_IOC_LIBS)

#===========================

include $(TOP)/configure/RULES
#----------------------------------------
#  ADD RULES AFTER THIS LINE

