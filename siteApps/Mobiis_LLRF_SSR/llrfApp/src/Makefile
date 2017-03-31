TOP=../..

include $(TOP)/configure/CONFIG
#----------------------------------------
#  ADD MACRO DEFINITIONS AFTER THIS LINE

#=============================
# Build the IOC support library

#LIBRARY_IOC += llrfSupport

llrfSupport_LIBS += $(EPICS_BASE_IOC_LIBS)

#=============================
# Build the IOC application

PROD_IOC = llrf
# llrf.dbd will be created and installed
DBD += llrf.dbd

# llrf.dbd will be made up from these files:
llrf_DBD += base.dbd
llrf_DBD += asSupport.dbd
#llrf_DBD += axisStatusRecord.dbd axisParamSetRecord.dbd nctoplcRecord.dbd plctoncRecord.dbd
llrf_DBD += drvllrf.dbd

# llrf_registerRecordDeviceDriver.cpp derives from llrf.dbd
llrf_SRCS += llrf_registerRecordDeviceDriver.cpp

# Build the main IOC entry point on workstation OSs.
llrf_SRCS_DEFAULT += llrfMain.cpp
llrf_SRCS_vxWorks += -nil-

# Add support from base/src/vxWorks if needed
#llrf_OBJS_vxWorks += $(EPICS_BASE_BIN)/vxComLibrary

# Link in the code from the support library
llrf_LIBS += autosave 
llrf_LIBS += gtr
#llrf_LIBS += llrfSupport

# NOTE: To build SNL programs, SNCSEQ must be defined
# in the <top>/configure/RELEASE file

# Finally link to the EPICS Base libraries
llrf_LIBS += $(EPICS_BASE_IOC_LIBS)

#=============================

include $(TOP)/configure/RULES
#----------------------------------------
#  ADD RULES AFTER THIS LINE

