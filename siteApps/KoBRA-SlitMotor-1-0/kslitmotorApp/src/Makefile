TOP=../..

include $(TOP)/configure/CONFIG
#----------------------------------------
#  ADD MACRO DEFINITIONS AFTER THIS LINE
#=============================

#=============================
# Build the IOC application

PROD_IOC = kslitmotor
# kslitmotor.dbd will be created and installed
DBD += kslitmotor.dbd

# kslitmotor.dbd will be made up from these files:
kslitmotor_DBD += base.dbd
kslitmotor_DBD += asyn.dbd
kslitmotor_DBD += drvAsynIPPort.dbd
kslitmotor_DBD += modbusSupport.dbd
kslitmotor_DBD += motorSupport.dbd
kslitmotor_DBD += phytronSupport.dbd

# Include dbd files from all support applications:
#kslitmotor_DBD += xxx.dbd

# Add all the support libraries needed by this IOC
#kslitmotor_LIBS += xxx

# kslitmotor_registerRecordDeviceDriver.cpp derives from kslitmotor.dbd
kslitmotor_SRCS += kslitmotor_registerRecordDeviceDriver.cpp

# Build the main IOC entry point on workstation OSs.
kslitmotor_SRCS_DEFAULT += kslitmotorMain.cpp
kslitmotor_SRCS_vxWorks += -nil-

# Add support from base/src/vxWorks if needed
#kslitmotor_OBJS_vxWorks += $(EPICS_BASE_BIN)/vxComLibrary
ifneq ($(SNCSEQ),)
    kslitmotor_SNCFLAGS += +r
    #iftarget_DBD  +=
    #iftarget_SRCS +=
    kslitmotor_LIBS += seq pv
endif

# Finally link to the EPICS Base libraries
kslitmotor_LIBS += asyn modbus motor phytronAxisMotor
kslitmotor_LIBS += $(EPICS_BASE_IOC_LIBS)

#===========================

include $(TOP)/configure/RULES
#----------------------------------------
#  ADD RULES AFTER THIS LINE

