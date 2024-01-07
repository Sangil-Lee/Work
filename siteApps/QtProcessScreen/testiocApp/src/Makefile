TOP=../..

include $(TOP)/configure/CONFIG
#----------------------------------------
#  ADD MACRO DEFINITIONS BELOW HERE

# Use typed rset structure (see 3.16.1 release notes)
USR_CPPFLAGS += -DUSE_TYPED_RSET

# xxxRecord.h will be created from xxxRecord.dbd
DBDINC += xxxRecord

# Install xxxSupport.dbd into <top>/dbd
DBD += xxxSupport.dbd

# Build an IOC support library
LIBRARY_IOC += testiocSupport

# Compile and add code to the support library
testiocSupport_SRCS += xxxRecord.c
testiocSupport_SRCS += devXxxSoft.c

# Link locally-provided code into the support library,
# rather than directly into the IOC application, that
# causes problems on Windows DLL builds
testiocSupport_SRCS += dbSubExample.c
testiocSupport_SRCS += devtestiocVersion.c
testiocSupport_SRCS += testiocHello.c
testiocSupport_SRCS += initTrace.c

testiocSupport_LIBS += $(EPICS_BASE_IOC_LIBS)

# Auto-generate a header file containing a version string.
# Version comes from the VCS if available, else date+time.
GENVERSION = testiocVersion.h
# Macro name
GENVERSIONMACRO = testiocVERSION

# Build the IOC application
PROD_IOC = testioc

# testioc.dbd will be created and installed
DBD += testioc.dbd

# testioc.dbd will include these files:
testioc_DBD += base.dbd
testioc_DBD += xxxSupport.dbd
testioc_DBD += dbSubExample.dbd
testioc_DBD += devtestiocVersion.dbd
testioc_DBD += testiocHello.dbd
testioc_DBD += initTrace.dbd
testioc_DBD += aCalcoutRecord.dbd

# Include dbd files from all support applications:
testioc_DBD += $(ASYN)/dbd/drvAsynIPPort.dbd
testioc_DBD += $(ASYN)/dbd/asyn.dbd
testioc_DBD += $(CALC)/dbd/calc.dbd
testioc_DBD += $(CALC)/dbd/calcSupport.dbd

# testioc_registerRecordDeviceDriver.cpp derives from testioc.dbd
testioc_SRCS += testioc_registerRecordDeviceDriver.cpp

# Build the main IOC entry point where needed
testioc_SRCS_DEFAULT += testiocMain.cpp
testioc_SRCS_vxWorks += -nil-

# Link in the code from our support library
testioc_LIBS += testiocSupport

# To build SNL programs, SNCSEQ must be defined
# in the <top>/configure/RELEASE file
ifneq ($(SNCSEQ),)
    # Build sncExample into testiocSupport
    sncExample_SNCFLAGS += +r
    testioc_DBD += sncExample.dbd
    # A .stt sequence program is *not* pre-processed:
    testiocSupport_SRCS += sncExample.stt
    testiocSupport_LIBS += seq pv
    testioc_LIBS += seq pv

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
    testioc_LIBS += qsrv
    testioc_LIBS += $(EPICS_BASE_PVA_CORE_LIBS)
    testioc_DBD += PVAServerRegister.dbd
    testioc_DBD += qsrv.dbd
endif

# Finally link IOC to the EPICS Base libraries
testioc_LIBS += asyn calc std
testioc_LIBS += $(EPICS_BASE_IOC_LIBS)

include $(TOP)/configure/RULES
#----------------------------------------
#  ADD EXTRA GNUMAKE RULES BELOW HERE

# Explicit dependency needed for generated header file
devtestiocVersion$(DEP): $(COMMON_DIR)/$(GENVERSION)
