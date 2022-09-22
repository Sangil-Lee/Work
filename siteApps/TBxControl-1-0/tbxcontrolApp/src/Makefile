$(shell ./fortrancompile.sh)
TOP=../..

include $(TOP)/configure/CONFIG
#----------------------------------------
#  ADD MACRO DEFINITIONS AFTER THIS LINE
#=============================

gfortran_DIR=/usr/lib/gcc/x86_64-linux-gnu/10
#=============================
# Build the IOC application

PROD_IOC = tbxcontrol
# tbxcontrol.dbd will be created and installed
DBD += tbxcontrol.dbd

# tbxcontrol.dbd will be made up from these files:
tbxcontrol_DBD += base.dbd
tbxcontrol_DBD += asyn.dbd
tbxcontrol_DBD += drvAsynIPPort.dbd
tbxcontrol_DBD += modbusSupport.dbd
tbxcontrol_DBD += FortranCalcLogic.dbd

# Include dbd files from all support applications:
#tbxcontrol_DBD += xxx.dbd

# Add all the support libraries needed by this IOC
#tbxcontrol_LIBS += xxx

# tbxcontrol_registerRecordDeviceDriver.cpp derives from tbxcontrol.dbd
tbxcontrol_SRCS += tbxcontrol_registerRecordDeviceDriver.cpp
tbxcontrol_SRCS += Dnst_Enthalpy.f
tbxcontrol_SRCS += FortranCalcLogic.cpp

# Build the main IOC entry point on workstation OSs.
tbxcontrol_SRCS_DEFAULT += tbxcontrolMain.cpp
tbxcontrol_SRCS_vxWorks += -nil-

# Add support from base/src/vxWorks if needed
#tbxcontrol_OBJS_vxWorks += $(EPICS_BASE_BIN)/vxComLibrary

# Finally link to the EPICS Base libraries
tbxcontrol_LIBS += asyn modbus gfortran
tbxcontrol_LIBS += $(EPICS_BASE_IOC_LIBS)

#===========================

include $(TOP)/configure/RULES
#----------------------------------------
#  ADD RULES AFTER THIS LINE

