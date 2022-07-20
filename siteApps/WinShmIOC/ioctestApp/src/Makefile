TOP=../..

include $(TOP)/configure/CONFIG
#----------------------------------------
#  ADD MACRO DEFINITIONS BELOW HERE

# Use typed rset structure (see 3.16.1 release notes)
USR_CPPFLAGS += -DUSE_TYPED_RSET
USR_INCLUDES += -IC:/boost_1_79_0
# xxxRecord.h will be created from xxxRecord.dbd
DBDINC += xxxRecord

# Install xxxSupport.dbd into <top>/dbd
DBD += xxxSupport.dbd

# Build an IOC support library
LIBRARY_IOC += ioctestSupport

# Compile and add code to the support library
ioctestSupport_SRCS += xxxRecord.c
ioctestSupport_SRCS += devXxxSoft.c

# Link locally-provided code into the support library,
# rather than directly into the IOC application, that
# causes problems on Windows DLL builds
ioctestSupport_SRCS += dbSubExample.cpp
ioctestSupport_SRCS += devioctestVersion.c
ioctestSupport_SRCS += ioctestHello.c
ioctestSupport_SRCS += initTrace.c

ioctestSupport_LIBS += $(EPICS_BASE_IOC_LIBS)

# Auto-generate a header file containing a version string.
# Version comes from the VCS if available, else date+time.
GENVERSION = ioctestVersion.h
# Macro name
GENVERSIONMACRO = ioctestVERSION

# Build the IOC application
PROD_IOC = ioctest

# ioctest.dbd will be created and installed
DBD += ioctest.dbd

# ioctest.dbd will include these files:
ioctest_DBD += base.dbd
ioctest_DBD += xxxSupport.dbd
ioctest_DBD += dbSubExample.dbd
ioctest_DBD += devioctestVersion.dbd
ioctest_DBD += ioctestHello.dbd
ioctest_DBD += initTrace.dbd

# ioctest_registerRecordDeviceDriver.cpp derives from ioctest.dbd
ioctest_SRCS += ioctest_registerRecordDeviceDriver.cpp

# Build the main IOC entry point where needed
ioctest_SRCS_DEFAULT += ioctestMain.cpp
ioctest_SRCS_vxWorks += -nil-

# Link in the code from our support library
ioctest_LIBS += ioctestSupport

# To build SNL programs, SNCSEQ must be defined
# in the <top>/configure/RELEASE file
ifneq ($(SNCSEQ),)
    # Build sncExample into ioctestSupport
    sncExample_SNCFLAGS += +r
    ioctest_DBD += sncExample.dbd
    # A .stt sequence program is *not* pre-processed:
    ioctestSupport_SRCS += sncExample.stt
    ioctestSupport_LIBS += seq pv
    ioctest_LIBS += seq pv

    # Build sncProgram as a standalone program
    PROD_HOST += sncProgram
    sncProgram_SNCFLAGS += +m
    # A .st sequence program *is* pre-processed:
    sncProgram_SRCS += sncProgram.st
    sncProgram_LIBS += seq pv
    sncProgram_LIBS += $(EPICS_BASE_HOST_LIBS)
endif

# Link QSRV (pvAccess Server) if available
ifdef EPICS_QSRV_MAJOR_VERSION
    ioctest_LIBS += qsrv
    ioctest_LIBS += $(EPICS_BASE_PVA_CORE_LIBS)
    ioctest_DBD += PVAServerRegister.dbd
    ioctest_DBD += qsrv.dbd
endif

# Finally link IOC to the EPICS Base libraries
ioctest_LIBS += $(EPICS_BASE_IOC_LIBS)

include $(TOP)/configure/RULES
#----------------------------------------
#  ADD EXTRA GNUMAKE RULES BELOW HERE

# Explicit dependency needed for generated header file
devioctestVersion$(DEP): $(COMMON_DIR)/$(GENVERSION)
