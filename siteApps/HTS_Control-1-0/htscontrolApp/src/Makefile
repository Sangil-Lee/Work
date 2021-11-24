TOP=../..

include $(TOP)/configure/CONFIG
#----------------------------------------
#  ADD MACRO DEFINITIONS AFTER THIS LINE
#=============================

#=============================
# Build the IOC application

PROD_IOC = htscontrol
# htscontrol.dbd will be created and installed
DBD += htscontrol.dbd

# htscontrol.dbd will be made up from these files:
htscontrol_DBD += base.dbd
htscontrol_DBD += asyn.dbd
htscontrol_DBD += calcSupport.dbd
htscontrol_DBD += drvAsynIPPort.dbd
htscontrol_DBD += stream.dbd
htscontrol_DBD += modbusSupport.dbd


# Include dbd files from all support applications:
#htscontrol_DBD += xxx.dbd

# Add all the support libraries needed by this IOC
#htscontrol_LIBS += xxx

# htscontrol_registerRecordDeviceDriver.cpp derives from htscontrol.dbd
htscontrol_SRCS += htscontrol_registerRecordDeviceDriver.cpp

# Build the main IOC entry point on workstation OSs.
htscontrol_SRCS_DEFAULT += htscontrolMain.cpp
htscontrol_SRCS_vxWorks += -nil-

# Add support from base/src/vxWorks if needed
#htscontrol_OBJS_vxWorks += $(EPICS_BASE_BIN)/vxComLibrary

ifneq ($(SNCSEQ),)
    # This builds sncExample as a component of tr1
    htscontrol_SNCFLAGS += +r
    htscontrol_DBD  += 
    htscontrol_SRCS += 
    htscontrol_LIBS += seq pv
endif


# Finally link to the EPICS Base libraries
htscontrol_LIBS += asyn stream modbus calc
htscontrol_LIBS += $(EPICS_BASE_IOC_LIBS)

#===========================

include $(TOP)/configure/RULES
#----------------------------------------
#  ADD RULES AFTER THIS LINE

