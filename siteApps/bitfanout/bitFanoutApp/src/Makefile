TOP=../..

include $(TOP)/configure/CONFIG
#----------------------------------------
#  ADD MACRO DEFINITIONS AFTER THIS LINE
#=============================
USR_CPPFLAGS += -std=gnu++17

#=============================
# Build the IOC application

PROD_IOC = bitFanout
# bitFanout.dbd will be created and installed
DBD += bitFanout.dbd

# bitFanout.dbd will be made up from these files:
bitFanout_DBD += base.dbd

# Include dbd files from all support applications:
#bitFanout_DBD += xxx.dbd
bitFanout_DBD += subStr2BitValue.dbd

# Add all the support libraries needed by this IOC
#bitFanout_LIBS += xxx

# bitFanout_registerRecordDeviceDriver.cpp derives from bitFanout.dbd
bitFanout_SRCS += bitFanout_registerRecordDeviceDriver.cpp
bitFanout_SRCS += subStr2BitValue.cpp

# Build the main IOC entry point on workstation OSs.
bitFanout_SRCS_DEFAULT += bitFanoutMain.cpp
bitFanout_SRCS_vxWorks += -nil-

# Add support from base/src/vxWorks if needed
#bitFanout_OBJS_vxWorks += $(EPICS_BASE_BIN)/vxComLibrary

# Finally link to the EPICS Base libraries
bitFanout_LIBS += $(EPICS_BASE_IOC_LIBS)

#===========================

include $(TOP)/configure/RULES
#----------------------------------------
#  ADD RULES AFTER THIS LINE

