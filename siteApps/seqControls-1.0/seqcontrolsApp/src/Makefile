TOP=../..
include $(TOP)/configure/CONFIG
#----------------------------------------
#  ADD MACRO DEFINITIONS AFTER THIS LINE
#=============================
#PYTHON = python
#SCRIPT = genSeqControls.py
#python:
#      $(PYTHON) $(SCRIPT)
#=============================
# Build the IOC application
PROD_IOC = seqcontrols
# seqcontrols.dbd will be created and installed
DBD += seqcontrols.dbd
# seqcontrols.dbd will be made up from these files:
seqcontrols_DBD += base.dbd
# Include dbd files from all support applications:
#seqcontrols_DBD += xxx.dbd
seqcontrols_DBD += asyn.dbd
seqcontrols_DBD += drvAsynIPPort.dbd
seqcontrols_DBD += calc.dbd sCalcoutRecord.dbd
seqcontrols_DBD += stream.dbd
# Add all the support libraries needed by this IOC
#seqcontrols_LIBS += xxx
# seqcontrols_registerRecordDeviceDriver.cpp derives from seqcontrols.dbd
seqcontrols_SRCS += seqcontrols_registerRecordDeviceDriver.cpp
# Build the main IOC entry point on workstation OSs.
seqcontrols_SRCS_DEFAULT += seqcontrolsMain.cpp
seqcontrols_SRCS_vxWorks += -nil-
# Add support from base/src/vxWorks if needed
#seqcontrols_OBJS_vxWorks += $(EPICS_BASE_BIN)/vxComLibrary
ifneq ($(SNCSEQ),)
seqcontrols_SNCFLAGS += +r
seqcontrols_DBD  += gen_SeqVBxSetCP.dbd gen_QWR_alarmlist.dbd gen_HWR_alarmlist.dbd
seqcontrols_SRCS += gen_SeqVBxSetCP.stt gen_QWR_alarmlist.stt gen_HWR_alarmlist.stt
seqcontrols_LIBS += seq pv
endif
# Finally link to the EPICS Base libraries
seqcontrols_LIBS += asyn calc stream
seqcontrols_LIBS += $(EPICS_BASE_IOC_LIBS)
#===========================
include $(TOP)/configure/RULES
#----------------------------------------
#  ADD RULES AFTER THIS LINE
