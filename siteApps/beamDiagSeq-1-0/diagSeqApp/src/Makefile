TOP=../..

include $(TOP)/configure/CONFIG
#----------------------------------------
#  ADD MACRO DEFINITIONS AFTER THIS LINE
#=============================

#=============================
# Build the IOC application

PROD_IOC = diagSeq
# diagSeq.dbd will be created and installed
DBD += diagSeq.dbd

# diagSeq.dbd will be made up from these files:
diagSeq_DBD += base.dbd

# Include dbd files from all support applications:
#diagSeq_DBD += xxx.dbd

# Add all the support libraries needed by this IOC
#diagSeq_LIBS += xxx

# diagSeq_registerRecordDeviceDriver.cpp derives from diagSeq.dbd
diagSeq_SRCS += diagSeq_registerRecordDeviceDriver.cpp

# Build the main IOC entry point on workstation OSs.
diagSeq_SRCS_DEFAULT += diagSeqMain.cpp
diagSeq_SRCS_vxWorks += -nil-

# Add support from base/src/vxWorks if needed
#diagSeq_OBJS_vxWorks += $(EPICS_BASE_BIN)/vxComLibrary
ifneq ($(SNCSEQ),)
    diagSeq_SNCFLAGS += +r
    diagSeq_DBD  += sncBeamDiagSeq.dbd
    diagSeq_SRCS += sncBeamDiagSeq.stt
    diagSeq_DBD  += sncBeamMotorSeq.dbd
    diagSeq_SRCS += sncBeamMotorSeq.stt
    diagSeq_LIBS += seq pv
endif

# Finally link to the EPICS Base libraries
diagSeq_LIBS += $(EPICS_BASE_IOC_LIBS)

#===========================

include $(TOP)/configure/RULES
#----------------------------------------
#  ADD RULES AFTER THIS LINE

