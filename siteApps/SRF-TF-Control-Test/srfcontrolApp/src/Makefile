TOP=../..

include $(TOP)/configure/CONFIG
#----------------------------------------
#  ADD MACRO DEFINITIONS AFTER THIS LINE
#=============================

#=============================
# Build the IOC application

PROD_IOC = srfcontrol
# srfcontrol.dbd will be created and installed
DBD += srfcontrol.dbd

# srfcontrol.dbd will be made up from these files:
srfcontrol_DBD += base.dbd

# Include dbd files from all support applications:
#srfcontrol_DBD += xxx.dbd

# Add all the support libraries needed by this IOC
#srfcontrol_LIBS += xxx

# srfcontrol_registerRecordDeviceDriver.cpp derives from srfcontrol.dbd
srfcontrol_SRCS += srfcontrol_registerRecordDeviceDriver.cpp

# Build the main IOC entry point on workstation OSs.
srfcontrol_SRCS_DEFAULT += srfcontrolMain.cpp
srfcontrol_SRCS_vxWorks += -nil-
ifneq ($(SNCSEQ),)
    srfcontrol_SNCFLAGS += +r
    srfcontrol_DBD  += sncPulse.dbd
    srfcontrol_SRCS += sncPulse.stt
    srfcontrol_LIBS += seq pv
endif

# Add support from base/src/vxWorks if needed
#srfcontrol_OBJS_vxWorks += $(EPICS_BASE_BIN)/vxComLibrary

# Finally link to the EPICS Base libraries
srfcontrol_LIBS += $(EPICS_BASE_IOC_LIBS)

#===========================

include $(TOP)/configure/RULES
#----------------------------------------
#  ADD RULES AFTER THIS LINE

