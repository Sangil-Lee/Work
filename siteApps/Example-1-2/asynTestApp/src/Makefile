TOP=../..

include $(TOP)/configure/CONFIG
#----------------------------------------
#  ADD MACRO DEFINITIONS AFTER THIS LINE
#=============================

#ASYN += ${EPICS_PATH}/modules/synApps_5_7/support/asyn-4-21

#USR_INCLUDES  += -I$(RAON_SITELIBS)/include
#USR_DBDFLAGS  += -I$(RAON_SITELIBS)/dbd
#USR_INCLUDES  += -I$(EPICS_EXTENSIONS)/include

#asyn_DIR += $(ASYN)/lib/$(T_A)


#=============================
# Build the IOC application

PROD_IOC = asynTest
# asynTest.dbd will be created and installed
DBD += asynTest.dbd

# asynTest.dbd will be made up from these files:
asynTest_DBD += asynTest.dbd

# Include dbd files from all support applications:
#asynTest_DBD += xxx.dbd
#asynTest_DBD += asyn.dbd

# Add all the support libraries needed by this IOC
asynTest_LIBS += asyn
asynTest_LIBS += stream

# asynTest_registerRecordDeviceDriver.cpp derives from exAsyn.dbd
asynTest_SRCS += asynTest_registerRecordDeviceDriver.cpp

# Build the main IOC entry point on workstation OSs.
asynTest_SRCS_DEFAULT += asynTestMain.cpp
asynTest_SRCS_vxWorks += -nil-

# Add support from base/src/vxWorks if needed
#asynTest_OBJS_vxWorks += $(EPICS_BASE_BIN)/vxComLibrary

# Finally link to the EPICS Base libraries
asynTest_LIBS += $(EPICS_BASE_IOC_LIBS)

#===========================

include $(TOP)/configure/RULES
#----------------------------------------
#  ADD RULES AFTER THIS LINE

