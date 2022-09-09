$(shell ./fortrancompile.sh)

TOP=../..

include $(TOP)/configure/CONFIG
#----------------------------------------
#  ADD MACRO DEFINITIONS AFTER THIS LINE
#=============================
#FC=gfortran
#FFLAGS=-o3 -Wall -Wextra -std=f2008
#FFLAGS=-o3 -Wall -Wextra
#SRC=Dnst_Viscosity.f
#OBJ=${SRC:.f=.o}

#=============================
# Build the IOC application
PROD_IOC = cooldown
# cooldown.dbd will be created and installed
DBD += cooldown.dbd

INCLUDES += -I/usr/include/eigen3

#gfortran_DIR=/usr/lib/gcc/x86_64-linux-gnu/8
gfortran_DIR=/usr/lib/gcc/x86_64-linux-gnu/10

# cooldown.dbd will be made up from these files:
cooldown_DBD += base.dbd
cooldown_DBD += epidRecord.dbd  scalerRecord.dbd 
cooldown_DBD += aCalcoutRecord.dbd  sCalcoutRecord.dbd  sseqRecord.dbd  swaitRecord.dbd  transformRecord.dbd
cooldown_DBD += HePAKLogic.dbd

# Include dbd files from all support applications:
cooldown_DBD += $(ASYN)/dbd/drvAsynIPPort.dbd
cooldown_DBD += $(ASYN)/dbd/asyn.dbd
cooldown_DBD += $(CALC)/dbd/calc.dbd
cooldown_DBD += $(CALC)/dbd/calcSupport.dbd
cooldown_DBD += $(SITELIBS)/dbd/dblfilter.dbd
cooldown_DBD += asubWFParam.dbd
cooldown_DBD += asubLReGression.dbd

# Add all the support libraries needed by this IOC
#cooldown_LIBS += xxx

# cooldown_registerRecordDeviceDriver.cpp derives from cooldown.dbd
cooldown_SRCS += cooldown_registerRecordDeviceDriver.cpp
cooldown_SRCS += Dnst_Viscosity.f
cooldown_SRCS += HePAKLogic.cpp asubWFParam.cpp
cooldown_SRCS += asubLReGression.cpp

# Build the main IOC entry point on workstation OSs.
cooldown_SRCS_DEFAULT += cooldownMain.cpp
cooldown_SRCS_vxWorks += -nil-

ifneq ($(SNCSEQ),)
    cooldown_SNCFLAGS += +r
    cooldown_DBD  += sncSetpt_listWF.dbd
    cooldown_DBD  += sncCooldown03.dbd
    cooldown_DBD  += sncCooldown04.dbd
    cooldown_DBD  += sncCooldown12.dbd
    cooldown_DBD  += sncCDOM14_15_Dic.dbd
    cooldown_DBD  += sncPT7001WF2Scalar.dbd
    cooldown_DBD  += sncMakeTempWaveform.dbd

    cooldown_SRCS += sncSetpt_listWF.stt
    cooldown_SRCS += sncCooldown03.stt
    cooldown_SRCS += sncCooldown04.stt
    cooldown_SRCS += sncCooldown12.stt
    cooldown_SRCS += sncCDOM14_15_Dic.stt
    cooldown_SRCS += sncPT7001WF2Scalar.stt
    cooldown_SRCS += sncMakeTempWaveform.stt
    cooldown_LIBS += seq pv

    PROD_HOST += sncCooldown03_PROD
    sncCooldown03_PROD_SNCFLAGS += +m
    sncCooldown03_PROD_SRCS += sncCooldown03_PROD.st
    sncCooldown03_PROD_LIBS += seq pv
    sncCooldown03_PROD_LIBS += $(EPICS_BASE_HOST_LIBS)

    PROD_HOST += sncCooldown04_PROD
    sncCooldown04_PROD_SNCFLAGS += +m
    sncCooldown04_PROD_SRCS += sncCooldown04_PROD.st
    sncCooldown04_PROD_LIBS += seq pv
    sncCooldown04_PROD_LIBS += $(EPICS_BASE_HOST_LIBS)

    PROD_HOST += sncCooldown12_PROD
    sncCooldown12_PROD_SNCFLAGS += +m
    sncCooldown12_PROD_SRCS += sncCooldown12_PROD.st
    sncCooldown12_PROD_LIBS += seq pv
    sncCooldown12_PROD_LIBS += $(EPICS_BASE_HOST_LIBS)

    PROD_HOST += sncCDOM14_15_Dic_PROD
    sncCDOM14_15_Dic_PROD_SNCFLAGS += +m
    sncCDOM14_15_Dic_PROD_SRCS += sncCDOM14_15_Dic_PROD.st
    sncCDOM14_15_Dic_PROD_LIBS += seq pv
    sncCDOM14_15_Dic_PROD_LIBS += $(EPICS_BASE_HOST_LIBS)

    PROD_HOST += sncCooldown04_1_PROD
    sncCooldown04_1_PROD_SNCFLAGS += +m
    sncCooldown04_1_PROD_SRCS += sncCooldown04_1_PROD.st
    sncCooldown04_1_PROD_LIBS += seq pv
    sncCooldown04_1_PROD_LIBS += $(EPICS_BASE_HOST_LIBS)

    PROD_HOST += sncCooldown04_2_PROD
    sncCooldown04_2_PROD_SNCFLAGS += +m
    sncCooldown04_2_PROD_SRCS += sncCooldown04_2_PROD.st
    sncCooldown04_2_PROD_LIBS += seq pv
    sncCooldown04_2_PROD_LIBS += $(EPICS_BASE_HOST_LIBS)
endif

# Finally link to the EPICS Base libraries
cooldown_LIBS += asyn calc std sscan iocshExt gfortran
cooldown_LIBS += $(EPICS_BASE_IOC_LIBS)

#===========================

include $(TOP)/configure/RULES
#----------------------------------------
#  ADD RULES AFTER THIS LINE

