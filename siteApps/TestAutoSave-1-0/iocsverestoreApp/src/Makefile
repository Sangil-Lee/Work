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
LIBRARY_IOC += iocsverestoreSupport

# Compile and add code to the support library
iocsverestoreSupport_SRCS += xxxRecord.c
iocsverestoreSupport_SRCS += devXxxSoft.c

# Link locally-provided code into the support library,
# rather than directly into the IOC application, that
# causes problems on Windows DLL builds
iocsverestoreSupport_SRCS += dbSubExample.c
iocsverestoreSupport_SRCS += deviocsverestoreVersion.c
iocsverestoreSupport_SRCS += iocsverestoreHello.c
iocsverestoreSupport_SRCS += initTrace.c

iocsverestoreSupport_LIBS += $(EPICS_BASE_IOC_LIBS)

# Auto-generate a header file containing a version string.
# Version comes from the VCS if available, else date+time.
GENVERSION = iocsverestoreVersion.h
# Macro name
GENVERSIONMACRO = iocsverestoreVERSION

# Build the IOC application
PROD_IOC = iocsverestore

# iocsverestore.dbd will be created and installed
DBD += iocsverestore.dbd

# iocsverestore.dbd will include these files:
iocsverestore_DBD += base.dbd
iocsverestore_DBD += xxxSupport.dbd
iocsverestore_DBD += dbSubExample.dbd
iocsverestore_DBD += deviocsverestoreVersion.dbd
iocsverestore_DBD += iocsverestoreHello.dbd
iocsverestore_DBD += initTrace.dbd
iocsverestore_DBD += asSupport.dbd

# iocsverestore_registerRecordDeviceDriver.cpp derives from iocsverestore.dbd
iocsverestore_SRCS += iocsverestore_registerRecordDeviceDriver.cpp

# Build the main IOC entry point where needed
iocsverestore_SRCS_DEFAULT += iocsverestoreMain.cpp
iocsverestore_SRCS_vxWorks += -nil-

# Link in the code from our support library
iocsverestore_LIBS += iocsverestoreSupport

# To build SNL programs, SNCSEQ must be defined
# in the <top>/configure/RELEASE file
ifneq ($(SNCSEQ),)
    # Build sncExample into iocsverestoreSupport
    sncExample_SNCFLAGS += +r
    iocsverestore_DBD += sncExample.dbd
    # A .stt sequence program is *not* pre-processed:
    iocsverestoreSupport_SRCS += sncExample.stt
    iocsverestoreSupport_LIBS += seq pv
    iocsverestore_LIBS += seq pv

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
    iocsverestore_LIBS += qsrv
    iocsverestore_LIBS += $(EPICS_BASE_PVA_CORE_LIBS)
    iocsverestore_DBD += PVAServerRegister.dbd
    iocsverestore_DBD += qsrv.dbd
endif

# Finally link IOC to the EPICS Base libraries
iocsverestore_LIBS += autosave
iocsverestore_LIBS += $(EPICS_BASE_IOC_LIBS)

include $(TOP)/configure/RULES
#----------------------------------------
#  ADD EXTRA GNUMAKE RULES BELOW HERE

# Explicit dependency needed for generated header file
deviocsverestoreVersion$(DEP): $(COMMON_DIR)/$(GENVERSION)
