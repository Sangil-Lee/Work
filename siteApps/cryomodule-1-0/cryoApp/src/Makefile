$(shell ./fortrancompile.sh)
TOP=../..

include $(TOP)/configure/CONFIG
#----------------------------------------
#  ADD MACRO DEFINITIONS AFTER THIS LINE
#=============================

#=============================
# Build the IOC application

PROD_IOC = cryo
# cryo.dbd will be created and installed
DBD += cryo.dbd

# cryo.dbd will be made up from these files:
cryo_DBD += base.dbd
cryo_DBD += modbusSupport.dbd
cryo_DBD += FortranCalcLogic.dbd
cryo_DBD += aCalcoutRecord.dbd  sCalcoutRecord.dbd  sseqRecord.dbd  swaitRecord.dbd  transformRecord.dbd

cryo_DBD += $(ASYN)/dbd/drvAsynIPPort.dbd
cryo_DBD += $(ASYN)/dbd/asyn.dbd
cryo_DBD += $(CALC)/dbd/calc.dbd
cryo_DBD += $(CALC)/dbd/calcSupport.dbd
cryo_DBD += $(STREAM)/dbd/stream.dbd

# Include dbd files from all support applications:
#cryo_DBD += xxx.dbd
gfortran_DIR=/usr/lib/gcc/x86_64-linux-gnu/8

# Add all the support libraries needed by this IOC
#cryo_LIBS += xxx

# cryo_registerRecordDeviceDriver.cpp derives from cryo.dbd
cryo_SRCS += cryo_registerRecordDeviceDriver.cpp
cryo_SRCS += Dnst_Enthalpy.f
cryo_SRCS += FortranCalcLogic.cpp

# Build the main IOC entry point on workstation OSs.
cryo_SRCS_DEFAULT += cryoMain.cpp
cryo_SRCS_vxWorks += -nil-

# Add support from base/src/vxWorks if needed
#cryo_OBJS_vxWorks += $(EPICS_BASE_BIN)/vxComLibrary
ifneq ($(SNCSEQ),)
    # This builds sncExample as a component of tr1
    cryo_SNCFLAGS += +r
    cryo_DBD  += seqHeater.dbd
    cryo_DBD  += sncSRFCooldown.dbd
    cryo_DBD  += sncSRFCooldown_MFR.dbd
    cryo_SRCS += sncSRFCooldown.stt
    cryo_SRCS += seqHeater.stt
    cryo_SRCS += sncSRFCooldown_MFR.stt
    cryo_LIBS += seq pv

#    cryo_SNCFLAGS += +r
#    cryo_DBD  += sncModbusTCPtoODA.dbd
#    cryo_SRCS += sncModbusTCPtoODA.stt
#    cryo_LIBS += seq pv
endif

# Finally link to the EPICS Base libraries
cryo_LIBS += asyn stream calc std sscan modbus gfortran
cryo_LIBS += $(EPICS_BASE_IOC_LIBS)

#===========================

include $(TOP)/configure/RULES
#----------------------------------------
#  ADD RULES AFTER THIS LINE

