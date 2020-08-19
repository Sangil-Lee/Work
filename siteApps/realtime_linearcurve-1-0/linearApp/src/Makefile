TOP=../..

include $(TOP)/configure/CONFIG
#----------------------------------------
#  ADD MACRO DEFINITIONS AFTER THIS LINE
#=============================

#=============================
# Build the IOC application

PROD_IOC = linear
# linear.dbd will be created and installed
DBD += linear.dbd

# linear.dbd will be made up from these files:
linear_DBD += base.dbd
linear_DBD += epidRecord.dbd  scalerRecord.dbd 
linear_DBD += aCalcoutRecord.dbd  sCalcoutRecord.dbd  sseqRecord.dbd  swaitRecord.dbd  transformRecord.dbd


# Include dbd files from all support applications:
#linear_DBD += xxx.dbd
linear_DBD += $(ASYN)/dbd/drvAsynIPPort.dbd
linear_DBD += $(ASYN)/dbd/asyn.dbd

# Add all the support libraries needed by this IOC
#linear_LIBS += xxx

# linear_registerRecordDeviceDriver.cpp derives from linear.dbd
linear_SRCS += linear_registerRecordDeviceDriver.cpp

# Build the main IOC entry point on workstation OSs.
linear_SRCS_DEFAULT += linearMain.cpp
linear_SRCS_vxWorks += -nil-

# Add support from base/src/vxWorks if needed
#linear_OBJS_vxWorks += $(EPICS_BASE_BIN)/vxComLibrary

ifneq ($(SNCSEQ),)
    linear_SNCFLAGS += +r
    #linear_DBD  += 
    #linear_SRCS += 
    linear_LIBS += seq pv
endif
# Finally link to the EPICS Base libraries
linear_LIBS += asyn calc std
linear_LIBS += $(EPICS_BASE_IOC_LIBS)

#===========================

include $(TOP)/configure/RULES
#----------------------------------------
#  ADD RULES AFTER THIS LINE

