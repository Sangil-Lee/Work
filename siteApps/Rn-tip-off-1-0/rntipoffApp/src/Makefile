TOP=../..

include $(TOP)/configure/CONFIG
#----------------------------------------
#  ADD MACRO DEFINITIONS AFTER THIS LINE
#=============================

#=============================
# Build the IOC application

PROD_IOC = rntipoff
# rntipoff.dbd will be created and installed
DBD += rntipoff.dbd

USR_CPPFLAGS += -Wno-deprecated-declarations -Wno-maybe-uninitialized

# rntipoff.dbd will be made up from these files:
rntipoff_DBD += base.dbd

# Include dbd files from all support applications:
#rntipoff_DBD += xxx.dbd
rntipoff_DBD += asyn.dbd
rntipoff_DBD += calcSupport.dbd
rntipoff_DBD += drvAsynIPPort.dbd
rntipoff_DBD += stream.dbd
rntipoff_DBD += modbusSupport.dbd

# Add all the support libraries needed by this IOC
#rntipoff_LIBS += xxx

# rntipoff_registerRecordDeviceDriver.cpp derives from rntipoff.dbd
rntipoff_SRCS += rntipoff_registerRecordDeviceDriver.cpp

# Build the main IOC entry point on workstation OSs.
rntipoff_SRCS_DEFAULT += rntipoffMain.cpp
rntipoff_SRCS_vxWorks += -nil-

# Add support from base/src/vxWorks if needed
#rntipoff_OBJS_vxWorks += $(EPICS_BASE_BIN)/vxComLibrary

ifneq ($(SNCSEQ),)
    # This builds sncExample as a component of tr1
    rntipoff_SNCFLAGS += +r
    rntipoff_DBD  += 
    rntipoff_SRCS += 
    rntipoff_LIBS += seq pv
endif

# Finally link to the EPICS Base libraries
rntipoff_LIBS += asyn stream modbus calc
rntipoff_LIBS += $(EPICS_BASE_IOC_LIBS)

#===========================

include $(TOP)/configure/RULES
#----------------------------------------
#  ADD RULES AFTER THIS LINE

