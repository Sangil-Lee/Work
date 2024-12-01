TOP=../..

include $(TOP)/configure/CONFIG
#----------------------------------------
#  ADD MACRO DEFINITIONS AFTER THIS LINE
#=============================
LIBRARY_IOC += rtpDev

DBD         += rtpDev.dbd
USR_INCLUDES  += -I$(RAON_SITELIBS)/include
CXXFLAGS += -Wno-unused-variable -Wno-deprecated-declaration -Wno-unused-but-set-variable -Wno-sequence-poin -Wno-deprecated-declarations -Wno-sequence-point -Wno-unused-function
rtpDev_LIBS += asyn

rtpDev_SRCS += rtpDevManager.cpp rtpSyncManager.cpp DasSocketObject.cpp

#=============================
# Build the IOC application

PROD_IOC = rtp
# rtp.dbd will be created and installed
DBD += rtp.dbd

# rtp.dbd will be made up from these files:
rtp_DBD += base.dbd
rtp_DBD += rtpDev.dbd

# Include dbd files from all support applications:
#rtp_DBD += xxx.dbd

# Add all the support libraries needed by this IOC
#rtp_LIBS += xxx
rtp_LIBS += rtpDev

# rtp_registerRecordDeviceDriver.cpp derives from rtp.dbd
rtp_SRCS += rtp_registerRecordDeviceDriver.cpp

# Build the main IOC entry point on workstation OSs.
rtp_SRCS_DEFAULT += rtpMain.cpp
rtp_SRCS_vxWorks += -nil-

ifneq ($(SNCSEQ),)
    rtp_SNCFLAGS += +r
    rtp_DBD  += sncWaveToScalar.dbd
    rtp_DBD  += sncWaveToScalar2.dbd
    rtp_DBD  += sncWaveToScalar3.dbd
    rtp_DBD  += sncWaveToScalar4.dbd

    rtp_SRCS += sncWaveToScalar.stt
    rtp_SRCS += sncWaveToScalar2.stt
    rtp_SRCS += sncWaveToScalar3.stt
    rtp_SRCS += sncWaveToScalar4.stt
    rtp_LIBS += seq pv
endif

# Add support from base/src/vxWorks if needed
#rtp_OBJS_vxWorks += $(EPICS_BASE_BIN)/vxComLibrary

# Finally link to the EPICS Base libraries
rtp_LIBS += $(EPICS_BASE_IOC_LIBS)

#===========================

include $(TOP)/configure/RULES
#----------------------------------------
#  ADD RULES AFTER THIS LINE

