TOP=../..

include $(TOP)/configure/CONFIG
#----------------------------------------
#  ADD MACRO DEFINITIONS AFTER THIS LINE
#=============================

#=============================
# Build the IOC application

PROD_IOC = scl3linac
# scl3linac.dbd will be created and installed
DBD += scl3linac.dbd

# scl3linac.dbd will be made up from these files:
scl3linac_DBD += base.dbd

# Include dbd files from all support applications:
#scl3linac_DBD += xxx.dbd
scl3linac_DBD += asyn.dbd
scl3linac_DBD += calcSupport.dbd
scl3linac_DBD += drvAsynIPPort.dbd
scl3linac_DBD += stream.dbd
scl3linac_DBD += modbusSupport.dbd

# Add all the support libraries needed by this IOC
#scl3linac_LIBS += xxx

# scl3linac_registerRecordDeviceDriver.cpp derives from scl3linac.dbd
scl3linac_SRCS += scl3linac_registerRecordDeviceDriver.cpp

# Build the main IOC entry point on workstation OSs.
scl3linac_SRCS_DEFAULT += scl3linacMain.cpp
scl3linac_SRCS_vxWorks += -nil-

# Add support from base/src/vxWorks if needed
#scl3linac_OBJS_vxWorks += $(EPICS_BASE_BIN)/vxComLibrary

ifneq ($(SNCSEQ),)
    # This builds sncExample as a component of tr1
    scl3linac_SNCFLAGS += +r
    #scl3linac_DBD  += seqHeater.dbd
    #scl3linac_SRCS += seqHeater.stt
    scl3linac_LIBS += seq pv
endif

# Finally link to the EPICS Base libraries
scl3linac_LIBS += asyn stream modbus calc
scl3linac_LIBS += $(EPICS_BASE_IOC_LIBS)

#===========================

include $(TOP)/configure/RULES
#----------------------------------------
#  ADD RULES AFTER THIS LINE

