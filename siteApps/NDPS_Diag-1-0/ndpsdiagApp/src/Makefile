TOP=../..

include $(TOP)/configure/CONFIG
#----------------------------------------
#  ADD MACRO DEFINITIONS AFTER THIS LINE
#=============================

#=============================
# Build the IOC application

PROD_IOC = ndpsdiag
# ndpsdiag.dbd will be created and installed
DBD += ndpsdiag.dbd

# ndpsdiag.dbd will be made up from these files:
ndpsdiag_DBD += base.dbd
ndpsdiag_DBD += base.dbd
ndpsdiag_DBD += asyn.dbd
ndpsdiag_DBD += drvAsynIPPort.dbd
ndpsdiag_DBD += calc.dbd sCalcoutRecord.dbd
ndpsdiag_DBD += stream.dbd
ndpsdiag_DBD += modbusSupport.dbd

# Include dbd files from all support applications:
#ndpsdiag_DBD += xxx.dbd

# Add all the support libraries needed by this IOC
#ndpsdiag_LIBS += xxx

# ndpsdiag_registerRecordDeviceDriver.cpp derives from ndpsdiag.dbd
ndpsdiag_SRCS += ndpsdiag_registerRecordDeviceDriver.cpp

# Build the main IOC entry point on workstation OSs.
ndpsdiag_SRCS_DEFAULT += ndpsdiagMain.cpp
ndpsdiag_SRCS_vxWorks += -nil-

# Add support from base/src/vxWorks if needed
#ndpsdiag_OBJS_vxWorks += $(EPICS_BASE_BIN)/vxComLibrary

# Finally link to the EPICS Base libraries
ndpsdiag_LIBS += asyn calc stream modbus
ndpsdiag_LIBS += $(EPICS_BASE_IOC_LIBS)

#===========================

include $(TOP)/configure/RULES
#----------------------------------------
#  ADD RULES AFTER THIS LINE

