TOP=../..

include $(TOP)/configure/CONFIG
#----------------------------------------
#  ADD MACRO DEFINITIONS AFTER THIS LINE
#=============================

USR_CXXFLAGS += -Wno-unused-variable -Wno-deprecated-declarations

#=============================
# Build the IOC application

PROD_IOC = glalarm
# glalarm.dbd will be created and installed
DBD += glalarm.dbd

# glalarm.dbd will be made up from these files:
glalarm_DBD += base.dbd
glalarm_DBD += aCalcoutRecord.dbd  sCalcoutRecord.dbd  sseqRecord.dbd  swaitRecord.dbd  transformRecord.dbd

# Include dbd files from all support applications:
#glalarm_DBD += xxx.dbd
glalarm_DBD += $(CALC)/dbd/calc.dbd
glalarm_DBD += $(CALC)/dbd/calcSupport.dbd
glalarm_DBD += asubGlobalInterlock.dbd
glalarm_DBD += asubGlobalInterlock_XV7301.dbd

# Add all the support libraries needed by this IOC
#glalarm_LIBS += xxx

# glalarm_registerRecordDeviceDriver.cpp derives from glalarm.dbd
glalarm_SRCS += glalarm_registerRecordDeviceDriver.cpp
glalarm_SRCS += asubGlobalInterlock.cpp
glalarm_SRCS += asubGlobalInterlock_XV7301.cpp

# Build the main IOC entry point on workstation OSs.
glalarm_SRCS_DEFAULT += glalarmMain.cpp
glalarm_SRCS_vxWorks += -nil-

# Add support from base/src/vxWorks if needed
#glalarm_OBJS_vxWorks += $(EPICS_BASE_BIN)/vxComLibrary

ifneq ($(SNCSEQ),)
    glalarm_SNCFLAGS += +r
    glalarm_DBD  += sncGLInterlockWF.dbd
    glalarm_DBD  += sncGLInterlock_EBx.dbd
    glalarm_SRCS += sncGLInterlockWF.stt
    glalarm_SRCS += sncGLInterlock_EBx.stt
    glalarm_LIBS += seq pv
endif

# Finally link to the EPICS Base libraries
glalarm_LIBS += calc std sscan
glalarm_LIBS += $(EPICS_BASE_IOC_LIBS)

#===========================

include $(TOP)/configure/RULES
#----------------------------------------
#  ADD RULES AFTER THIS LINE

