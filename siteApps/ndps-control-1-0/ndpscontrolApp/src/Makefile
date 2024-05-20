TOP=../..

include $(TOP)/configure/CONFIG
#----------------------------------------
#  ADD MACRO DEFINITIONS AFTER THIS LINE
#=============================

#=============================
# Build the IOC application

PROD_IOC = ndpscontrol
# ndpscontrol.dbd will be created and installed
DBD += ndpscontrol.dbd

# ndpscontrol.dbd will be made up from these files:
ndpscontrol_DBD += base.dbd

# Include dbd files from all support applications:
#ndpscontrol_DBD += xxx.dbd
ndpscontrol_DBD += $(ASYN)/dbd/drvAsynIPPort.dbd
ndpscontrol_DBD += $(ASYN)/dbd/asyn.dbd
ndpscontrol_DBD += $(CALC)/dbd/calc.dbd
ndpscontrol_DBD += $(CALC)/dbd/calcSupport.dbd

# Add all the support libraries needed by this IOC
#ndpscontrol_LIBS += xxx

# ndpscontrol_registerRecordDeviceDriver.cpp derives from ndpscontrol.dbd
ndpscontrol_SRCS += ndpscontrol_registerRecordDeviceDriver.cpp

# Build the main IOC entry point on workstation OSs.
ndpscontrol_SRCS_DEFAULT += ndpscontrolMain.cpp
ndpscontrol_SRCS_vxWorks += -nil-

# Add support from base/src/vxWorks if needed
#ndpscontrol_OBJS_vxWorks += $(EPICS_BASE_BIN)/vxComLibrary
ifneq ($(SNCSEQ),)
    ndpscontrol_SNCFLAGS += +r
    ndpscontrol_DBD  += sncNDPSControl.dbd
    ndpscontrol_DBD += sncNDPSControl_equal.dbd
    ndpscontrol_SRCS += sncNDPSControl.stt
    ndpscontrol_SRCS += sncNDPSControl_equal.stt
    ndpscontrol_LIBS += seq pv
endif

# Finally link to the EPICS Base libraries
ndpscontrol_LIBS += asyn calc std sscan
ndpscontrol_LIBS += $(EPICS_BASE_IOC_LIBS)

#===========================

include $(TOP)/configure/RULES
#----------------------------------------
#  ADD RULES AFTER THIS LINE

