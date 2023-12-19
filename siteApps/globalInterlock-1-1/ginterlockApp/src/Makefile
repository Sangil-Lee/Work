TOP=../..

include $(TOP)/configure/CONFIG
#----------------------------------------
#  ADD MACRO DEFINITIONS AFTER THIS LINE
#=============================

#=============================
# Build the IOC application

PROD_IOC = ginterlock
# ginterlock.dbd will be created and installed
DBD += ginterlock.dbd

# ginterlock.dbd will be made up from these files:
ginterlock_DBD += base.dbd
ginterlock_DBD += epidRecord.dbd  scalerRecord.dbd 
ginterlock_DBD += aCalcoutRecord.dbd  sCalcoutRecord.dbd  sseqRecord.dbd  swaitRecord.dbd  transformRecord.dbd

# Include dbd files from all support applications:
ginterlock_DBD += $(ASYN)/dbd/drvAsynIPPort.dbd
ginterlock_DBD += $(ASYN)/dbd/asyn.dbd
ginterlock_DBD += $(CALC)/dbd/calc.dbd
ginterlock_DBD += $(CALC)/dbd/calcSupport.dbd

# Include dbd files from all support applications:
#ginterlock_DBD += xxx.dbd

# Add all the support libraries needed by this IOC
#ginterlock_LIBS += xxx

# ginterlock_registerRecordDeviceDriver.cpp derives from ginterlock.dbd
ginterlock_SRCS += ginterlock_registerRecordDeviceDriver.cpp

# Build the main IOC entry point on workstation OSs.
ginterlock_SRCS_DEFAULT += ginterlockMain.cpp
ginterlock_SRCS_vxWorks += -nil-

# Add support from base/src/vxWorks if needed
#ginterlock_OBJS_vxWorks += $(EPICS_BASE_BIN)/vxComLibrary
ifneq ($(SNCSEQ),)
    ginterlock_SNCFLAGS += +r
    #ginterlock_DBD   += sncQWRGInt.dbd
    #ginterlock_SRCS  += sncQWRGInt.stt
    ginterlock_DBD   += sncGLInterlockWF.dbd
    ginterlock_SRCS  += sncGLInterlockWF.stt
    ginterlock_LIBS  += seq pv

    PROD_HOST += sncQWRGInt_PROD
    sncQWRGInt_PROD_SNCFLAGS += +m
    sncQWRGInt_PROD_SRCS += sncQWRGInt_PROD.st
    sncQWRGInt_PROD_LIBS += seq pv
    sncQWRGInt_PROD_LIBS += $(EPICS_BASE_HOST_LIBS)

    PROD_HOST += sncHWRGInt_PROD
    sncHWRGInt_PROD_SNCFLAGS += +m
    sncHWRGInt_PROD_SRCS += sncHWRGInt_PROD.st
    sncHWRGInt_PROD_LIBS += seq pv
    sncHWRGInt_PROD_LIBS += $(EPICS_BASE_HOST_LIBS)

    PROD_HOST += sncSSRGInt_PROD
    sncSSRGInt_PROD_SNCFLAGS += +m
    sncSSRGInt_PROD_SRCS += sncSSRGInt_PROD.st
    sncSSRGInt_PROD_LIBS += seq pv
    sncSSRGInt_PROD_LIBS += $(EPICS_BASE_HOST_LIBS)

    PROD_HOST += sncLTSGInt_PROD
    sncLTSGInt_PROD_SNCFLAGS += +m
    sncLTSGInt_PROD_SRCS += sncLTSGInt_PROD.st
    sncLTSGInt_PROD_LIBS += seq pv
    sncLTSGInt_PROD_LIBS += $(EPICS_BASE_HOST_LIBS)



    #PROD_HOST += sncGLInterlockWF_PROD
    #sncGLInterlockWF_PROD_SNCFLAGS += +m
    #sncGLInterlockWF_PROD_SRCS += sncGLInterlockWF_PROD.st
    #sncGLInterlockWF_PROD_LIBS += seq pv
    #sncGLInterlockWF_PROD_LIBS += $(EPICS_BASE_HOST_LIBS)

endif

# Finally link to the EPICS Base libraries
ginterlock_LIBS += asyn calc std sscan 
ginterlock_LIBS += $(EPICS_BASE_IOC_LIBS)

#===========================

include $(TOP)/configure/RULES
#----------------------------------------
#  ADD RULES AFTER THIS LINE

