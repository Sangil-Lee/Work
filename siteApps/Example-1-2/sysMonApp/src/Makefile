TOP=../..

include $(TOP)/configure/CONFIG

USR_INCLUDES  += -I$(RAON_SITELIBS)/include
USR_DBDFLAGS  += -I$(RAON_SITELIBS)/dbd
USR_INCLUDES  += -I$(EPICS_EXTENSIONS)/include

sysMon_DIR    += $(RAON_SITELIBS)/lib/$(T_A)
ifStatMon_DIR += $(RAON_SITELIBS)/lib/$(T_A)
timeStamp_DIR += $(RAON_SITELIBS)/lib/$(T_A)

#----------------------------------------
#  ADD MACRO DEFINITIONS AFTER THIS LINE
#=============================

#=============================
# Build the IOC application

PROD_IOC = sysMon
# sysMon.dbd will be created and installed
DBD += sysMon.dbd

# sysMon.dbd will be made up from these files:
sysMon_DBD += base.dbd
sysMon_DBD += menuInOut.dbd
sysMon_DBD += ifstatRecord.dbd
sysMon_DBD += ifstatDevSoft.dbd
sysMon_DBD += timestampRecord.dbd
sysMon_DBD += timestampDevSoft.dbd

# Include dbd files from all support applications:
#sysMon_DBD += xxx.dbd
sysMon_DBD += sysMon.dbd

# Add all the support libraries needed by this IOC
#sysMon_LIBS += xxx

# sysMon_registerRecordDeviceDriver.cpp derives from exSysMon.dbd
sysMon_SRCS += sysMon_registerRecordDeviceDriver.cpp

# Build the main IOC entry point on workstation OSs.
sysMon_SRCS_DEFAULT += sysMonMain.cpp
sysMon_SRCS_vxWorks += -nil-

# Add support from base/src/vxWorks if needed
#sysMon_OBJS_vxWorks += $(EPICS_BASE_BIN)/vxComLibrary
sysMon_LIBS += sysMon
sysMon_LIBS += ifStatMon
sysMon_LIBS += timeStamp

# Finally link to the EPICS Base libraries
sysMon_LIBS += $(EPICS_BASE_IOC_LIBS)

#===========================

include $(TOP)/configure/RULES
#----------------------------------------
#  ADD RULES AFTER THIS LINE

