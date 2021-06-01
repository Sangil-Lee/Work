TOP=../..

include $(TOP)/configure/CONFIG
#----------------------------------------
#  ADD MACRO DEFINITIONS AFTER THIS LINE
#=============================

#=============================
# Build the IOC application

PROD_IOC = kobra
# kobra.dbd will be created and installed
DBD += kobra.dbd

# kobra.dbd will be made up from these files:
kobra_DBD += base.dbd

# Include dbd files from all support applications:
#kobra_DBD += xxx.dbd

# Add all the support libraries needed by this IOC
#kobra_LIBS += xxx

# kobra_registerRecordDeviceDriver.cpp derives from kobra.dbd
kobra_SRCS += kobra_registerRecordDeviceDriver.cpp

# Build the main IOC entry point on workstation OSs.
kobra_SRCS_DEFAULT += kobraMain.cpp
kobra_SRCS_vxWorks += -nil-

# Add support from base/src/vxWorks if needed
#kobra_OBJS_vxWorks += $(EPICS_BASE_BIN)/vxComLibrary

ifneq ($(SNCSEQ),)
   kobra_SNCFLAGS += +r
   kobra_SNCFLAGS += +c
   kobra_DBD  += sncAutoRampup.dbd
   kobra_DBD  += sncAutoRampdown.dbd
   kobra_SRCS += sncAutoRampup.stt
   kobra_SRCS += sncAutoRampdown.stt

   kobra_LIBS += seq pv
endif

# Finally link to the EPICS Base libraries
kobra_LIBS += $(EPICS_BASE_IOC_LIBS)

#===========================

include $(TOP)/configure/RULES
#----------------------------------------
#  ADD RULES AFTER THIS LINE

