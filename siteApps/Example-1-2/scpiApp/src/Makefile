TOP=../..

include $(TOP)/configure/CONFIG
#----------------------------------------
#  ADD MACRO DEFINITIONS AFTER THIS LINE
#=============================

#=============================
# Build the IOC application

PROD_IOC = scpi
# scpi.dbd will be created and installed
DBD += scpi.dbd

# scpi.dbd will be made up from these files:
scpi_DBD += base.dbd

# Include dbd files from all support applications:
#scpi_DBD += xxx.dbd
scpi_DBD += stream.dbd
scpi_DBD += drvAsynIPPort.dbd

# Add all the support libraries needed by this IOC
#scpi_LIBS += xxx
scpi_LIBS += asyn
scpi_LIBS += stream

# scpi_registerRecordDeviceDriver.cpp derives from scpi.dbd
scpi_SRCS += scpi_registerRecordDeviceDriver.cpp

# Build the main IOC entry point on workstation OSs.
scpi_SRCS_DEFAULT += scpiMain.cpp
scpi_SRCS_vxWorks += -nil-

# Add support from base/src/vxWorks if needed
#scpi_OBJS_vxWorks += $(EPICS_BASE_BIN)/vxComLibrary

# Finally link to the EPICS Base libraries
scpi_LIBS += $(EPICS_BASE_IOC_LIBS)

#===========================

include $(TOP)/configure/RULES
#----------------------------------------
#  ADD RULES AFTER THIS LINE

