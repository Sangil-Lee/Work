TOP=../..

include $(TOP)/configure/CONFIG
#----------------------------------------
#  ADD MACRO DEFINITIONS AFTER THIS LINE
#=============================

#=============================
# Build the IOC application

PROD_IOC = modbusDesign
# modbusDesign.dbd will be created and installed
DBD += modbusDesign.dbd

# modbusDesign.dbd will be made up from these files:
modbusDesign_DBD += base.dbd

modbusDesign_DBD += asyn.dbd
modbusDesign_DBD += drvAsynIPPort.dbd
modbusDesign_DBD += modbusSupport.dbd

# Include dbd files from all support applications:
#modbusDesign_DBD += xxx.dbd

# Add all the support libraries needed by this IOC
#modbusDesign_LIBS += xxx

# modbusDesign_registerRecordDeviceDriver.cpp derives from modbusDesign.dbd
modbusDesign_SRCS += modbusDesign_registerRecordDeviceDriver.cpp

# Build the main IOC entry point on workstation OSs.
modbusDesign_SRCS_DEFAULT += modbusDesignMain.cpp
modbusDesign_SRCS_vxWorks += -nil-

# Add support from base/src/vxWorks if needed
#modbusDesign_OBJS_vxWorks += $(EPICS_BASE_BIN)/vxComLibrary
ifneq ($(SNCSEQ),)
    modbusDesign_SNCFLAGS += +r
    #modbusDesign_DBD  +=
    #modbusDesign_SRCS +=
    modbusDesign_LIBS += seq pv
endif


# Finally link to the EPICS Base libraries
modbusDesign_LIBS += asyn modbus
modbusDesign_LIBS += $(EPICS_BASE_IOC_LIBS)

#===========================

include $(TOP)/configure/RULES
#----------------------------------------
#  ADD RULES AFTER THIS LINE

