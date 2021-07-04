TOP=../..

include $(TOP)/configure/CONFIG


USR_INCLUDES  += -I$(RAON_SITELIBS)/include
USR_DBDFLAGS  += -I$(RAON_SITELIBS)/dbd
USR_INCLUDES  += -I$(EPICS_EXTENSIONS)/include

ifStatMon_DIR += $(RAON_SITELIBS)/lib/$(T_A)
timeStamp_DIR += $(RAON_SITELIBS)/lib/$(T_A)


#----------------------------------------
#  ADD MACRO DEFINITIONS AFTER THIS LINE
#=============================

#=============================
# Build the IOC application

PROD_IOC = timeStamp
# timeStamp.dbd will be created and installed
DBD += timeStamp.dbd

# timeStamp.dbd will be made up from these files:
timeStamp_DBD += base.dbd
timeStamp_DBD += menuInOut.dbd
timeStamp_DBD += ifstatRecord.dbd
timeStamp_DBD += timestampRecord.dbd

# Include dbd files from all support applications:
#timeStamp_DBD += xxx.dbd
timeStamp_DBD += ifstatDevSoft.dbd
timeStamp_DBD += timestampDevSoft.dbd

# Add all the support libraries needed by this IOC
#timeStamp_LIBS += xxx
timeStamp_LIBS += ifStatMon timeStamp

# timeStamp_registerRecordDeviceDriver.cpp derives from tr6.dbd
timeStamp_SRCS += timeStamp_registerRecordDeviceDriver.cpp

# Build the main IOC entry point on workstation OSs.
timeStamp_SRCS_DEFAULT += timeStampMain.cpp
timeStamp_SRCS_vxWorks += -nil-

# Add support from base/src/vxWorks if needed
#timeStamp_OBJS_vxWorks += $(EPICS_BASE_BIN)/vxComLibrary

# Finally link to the EPICS Base libraries
timeStamp_LIBS += $(EPICS_BASE_IOC_LIBS)

#===========================

include $(TOP)/configure/RULES
#----------------------------------------
#  ADD RULES AFTER THIS LINE

