TOP=../..

include $(TOP)/configure/CONFIG
#----------------------------------------
#  ADD MACRO DEFINITIONS AFTER THIS LINE
#=============================

#=============================
# Build the IOC application

PROD_IOC = opmode
# opmode.dbd will be created and installed
DBD += opmode.dbd

# opmode.dbd will be made up from these files:
opmode_DBD += base.dbd

# Include dbd files from all support applications:
#opmode_DBD += xxx.dbd

# Add all the support libraries needed by this IOC
#opmode_LIBS += xxx

# opmode_registerRecordDeviceDriver.cpp derives from opmode.dbd
opmode_SRCS += opmode_registerRecordDeviceDriver.cpp

# Build the main IOC entry point on workstation OSs.
opmode_SRCS_DEFAULT += opmodeMain.cpp
opmode_SRCS_vxWorks += -nil-

# Add support from base/src/vxWorks if needed
#opmode_OBJS_vxWorks += $(EPICS_BASE_BIN)/vxComLibrary
ifneq ($(SNCSEQ),)
    opmode_SNCFLAGS += +r
    opmode_DBD  += sncOPModeSeq.dbd
    opmode_SRCS += sncOPModeSeq.stt
    opmode_LIBS += seq pv
endif

# Finally link to the EPICS Base libraries
opmode_LIBS += $(EPICS_BASE_IOC_LIBS)

#===========================

include $(TOP)/configure/RULES
#----------------------------------------
#  ADD RULES AFTER THIS LINE

