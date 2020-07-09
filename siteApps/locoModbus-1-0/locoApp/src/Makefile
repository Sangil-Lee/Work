TOP=../..

include $(TOP)/configure/CONFIG
#----------------------------------------
#  ADD MACRO DEFINITIONS AFTER THIS LINE
#=============================

#=============================
# Build the IOC application

PROD_IOC = loco
# loco.dbd will be created and installed
DBD += loco.dbd
#DBD += $(MODBUS)/dbd/modbusSupport.dbd
#DBD += $(ASYN)/dbd/drvAsynIPPort.dbd
#DBD += $(ASYN)/dbd/asyn.dbd

# loco.dbd will be made up from these files:
loco_DBD += base.dbd
loco_DBD += $(MODBUS)/dbd/modbusSupport.dbd
loco_DBD += $(ASYN)/dbd/drvAsynIPPort.dbd
loco_DBD += $(ASYN)/dbd/asyn.dbd

# Include dbd files from all support applications:
#loco_DBD += xxx.dbd
loco_DBD += subStr2BitValue.dbd

# Add all the support libraries needed by this IOC
#loco_LIBS += xxx
loco_LIBS += asyn
loco_LIBS += modbus

# loco_registerRecordDeviceDriver.cpp derives from loco.dbd
loco_SRCS += loco_registerRecordDeviceDriver.cpp
loco_SRCS += subStr2BitValue.cpp


# Build the main IOC entry point on workstation OSs.
loco_SRCS_DEFAULT += locoMain.cpp
loco_SRCS_vxWorks += -nil-

# Add support from base/src/vxWorks if needed
#loco_OBJS_vxWorks += $(EPICS_BASE_BIN)/vxComLibrary
ifneq ($(SNCSEQ),)
    loco_SNCFLAGS += +r
    loco_DBD  += sncLEDSeq.dbd
    loco_SRCS += sncLEDSeq.stt
    loco_LIBS += seq pv
endif

# Finally link to the EPICS Base libraries
loco_LIBS += $(EPICS_BASE_IOC_LIBS)

#===========================

include $(TOP)/configure/RULES
#----------------------------------------
#  ADD RULES AFTER THIS LINE

