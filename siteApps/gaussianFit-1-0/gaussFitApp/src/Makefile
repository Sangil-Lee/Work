TOP=../..

include $(TOP)/configure/CONFIG
#----------------------------------------
#  ADD MACRO DEFINITIONS AFTER THIS LINE
#=============================

#=============================
# Build the IOC application

PROD_IOC = gaussFit
# gaussFit.dbd will be created and installed
DBD += gaussFit.dbd

USR_CPPFLAGS += -Wno-deprecated-declarations -Wno-maybe-uninitialized -Wno-unused-variable

gsl_DIR=/usr/lib/x86_64-linux-gnu
gslcblas_DIR=$(gsl_DIR)

# gaussFit.dbd will be made up from these files:
gaussFit_DBD += base.dbd

# Include dbd files from all support applications:
#gaussFit_DBD += xxx.dbd
gaussFit_DBD += asubMakeGaussFit.dbd randomWave.dbd

#gaussFit_DBD += asubWSGaussFit.dbd

# Add all the support libraries needed by this IOC
#gaussFit_LIBS += xxx
gaussFit_LIBS += gauss1d gsl gslcblas

# gaussFit_registerRecordDeviceDriver.cpp derives from gaussFit.dbd
gaussFit_SRCS += gaussFit_registerRecordDeviceDriver.cpp
gaussFit_SRCS += asubMakeGaussFit.cpp randomWave.cpp
#gaussFit_SRCS += asubWSGaussFit.cpp

# Build the main IOC entry point on workstation OSs.
gaussFit_SRCS_DEFAULT += gaussFitMain.cpp
gaussFit_SRCS_vxWorks += -nil-

# Add support from base/src/vxWorks if needed
#gaussFit_OBJS_vxWorks += $(EPICS_BASE_BIN)/vxComLibrary

# Finally link to the EPICS Base libraries
gaussFit_LIBS += $(EPICS_BASE_IOC_LIBS)

#===========================

include $(TOP)/configure/RULES
#----------------------------------------
#  ADD RULES AFTER THIS LINE

