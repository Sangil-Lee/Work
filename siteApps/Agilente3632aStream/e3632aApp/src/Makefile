TOP=../..

include $(TOP)/configure/CONFIG
#----------------------------------------
#  ADD MACRO DEFINITIONS AFTER THIS LINE
#=============================
PCRE_LIB = /usr/lib/x86_64-linux-gnu
PCRE_INC = /usr/include
USR_INCLUDES  += -I$(RAON_SITELIBS)/include
USR_INCLUDES  += -I$(PCRE_INC)


#=============================
# Build the IOC application

PROD_IOC = e3632a
# e3632a.dbd will be created and installed
DBD += e3632a.dbd

e3632a_DIR += $(PCRE_LIB)
pcre_DIR += $(PCRE_LIB)
# e3632a.dbd will be made up from these files:
e3632a_DBD += base.dbd

# Include dbd files from all support applications:
#e3632a_DBD += xxx.dbd

e3632a_DBD += stream.dbd
e3632a_DBD += drvAsynIPPort.dbd


# Add all the support libraries needed by this IOC
#e3632a_LIBS += xxx
e3632a_LIBS += asyn
e3632a_LIBS += stream
e3632a_LIBS += pcre 


# e3632a_registerRecordDeviceDriver.cpp derives from e3632a.dbd
e3632a_SRCS += e3632a_registerRecordDeviceDriver.cpp

# Build the main IOC entry point on workstation OSs.
e3632a_SRCS_DEFAULT += e3632aMain.cpp
e3632a_SRCS_vxWorks += -nil-

# Add support from base/src/vxWorks if needed
#e3632a_OBJS_vxWorks += $(EPICS_BASE_BIN)/vxComLibrary

# Finally link to the EPICS Base libraries
e3632a_LIBS += $(EPICS_BASE_IOC_LIBS)

#===========================

include $(TOP)/configure/RULES
#----------------------------------------
#  ADD RULES AFTER THIS LINE

