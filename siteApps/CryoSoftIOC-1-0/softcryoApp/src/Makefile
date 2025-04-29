TOP=../..

include $(TOP)/configure/CONFIG
#----------------------------------------
#  ADD MACRO DEFINITIONS AFTER THIS LINE
#=============================

#=============================
# Build the IOC application

PROD_IOC = softcryo
# softcryo.dbd will be created and installed
DBD += softcryo.dbd

# softcryo.dbd will be made up from these files:
softcryo_DBD += base.dbd

# Include dbd files from all support applications:
#softcryo_DBD += xxx.dbd

# Add all the support libraries needed by this IOC
#softcryo_LIBS += xxx

# softcryo_registerRecordDeviceDriver.cpp derives from softcryo.dbd
softcryo_SRCS += softcryo_registerRecordDeviceDriver.cpp

# Build the main IOC entry point on workstation OSs.
softcryo_SRCS_DEFAULT += softcryoMain.cpp
softcryo_SRCS_vxWorks += -nil-

# Add support from base/src/vxWorks if needed
#softcryo_OBJS_vxWorks += $(EPICS_BASE_BIN)/vxComLibrary
ifneq ($(SNCSEQ),)
softcryo_SNCFLAGS += +r
softcryo_DBD  += gen_SeqCMSetCP.dbd gen_SeqVBxSetCP.dbd gen_CV7401_Setpt_SetManuRamp.dbd gen_PT7501_Alarm.dbd gen_XV7301_XV7201_SetManuClose.dbd gen_CV7501_Setpt_SetManuRamp.dbd gen_XV7502_SetManuClose.dbd gen_CV8402_Setpt_SetManuRamp_SetManuSel.dbd  gen_CV7502_Setpt_SetManuRamp.dbd gen_FinalPVs_Setpt_SetManuRamp.dbd
softcryo_SRCS += gen_SeqCMSetCP.stt gen_SeqVBxSetCP.stt gen_CV7501_Setpt_SetManuRamp.stt gen_XV7502_SetManuClose.stt gen_CV8402_Setpt_SetManuRamp_SetManuSel.stt gen_CV7401_Setpt_SetManuRamp.stt gen_PT7501_Alarm.stt gen_XV7301_XV7201_SetManuClose.stt gen_CV7502_Setpt_SetManuRamp.stt gen_FinalPVs_Setpt_SetManuRamp.stt
softcryo_LIBS += seq pv
endif
# Finally link to the EPICS Base libraries
softcryo_LIBS += asyn calc stream
softcryo_LIBS += $(EPICS_BASE_IOC_LIBS)

#===========================

include $(TOP)/configure/RULES
#----------------------------------------
#  ADD RULES AFTER THIS LINE

