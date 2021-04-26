TOP=../..

include $(TOP)/configure/CONFIG
#----------------------------------------
#  ADD MACRO DEFINITIONS BELOW HERE

# Use typed rset structure (see 3.16.1 release notes)
USR_CPPFLAGS += -DUSE_TYPED_RSET

# xxxRecord.h will be created from xxxRecord.dbd
DBDINC += xxxRecord

# Install xxxSupport.dbd into <top>/dbd
#DBD += xxxSupport.dbd

# Build an IOC support library
LIBRARY_IOC += ex7Support

# Compile and add code to the support library
ex7Support_SRCS += xxxRecord.c
ex7Support_SRCS += devXxxSoft.c

# Link locally-provided code into the support library,
# rather than directly into the IOC application, that
# causes problems on Windows DLL builds
ex7Support_SRCS += dbSubExample.c
ex7Support_SRCS += devex7Version.c
ex7Support_SRCS += ex7Hello.c
ex7Support_SRCS += initTrace.c
ex7Support_SRCS += pvaRecord.cpp
ex7Support_SRCS += pvaRecordRegister.cpp
ex7Support_SRCS += powerSupplyRecord.cpp

ex7Support_LIBS += $(EPICS_BASE_IOC_LIBS)

# Auto-generate a header file containing a version string.
# Version comes from the VCS if available, else date+time.
GENVERSION = ex7Version.h
# Macro name
GENVERSIONMACRO = ex7VERSION

# Build the IOC application
PROD_IOC = ex7

# ex7.dbd will be created and installed
DBD += ex7.dbd

# ex7.dbd will include these files:
#ex7_DBD += base.dbd
#ex7_DBD += xxxSupport.dbd
#ex7_DBD += dbSubExample.dbd
#ex7_DBD += devex7Version.dbd
#ex7_DBD += ex7Hello.dbd
#ex7_DBD += initTrace.dbd

# ex7_registerRecordDeviceDriver.cpp derives from ex7.dbd
ex7_SRCS += ex7_registerRecordDeviceDriver.cpp

# Build the main IOC entry point where needed
ex7_SRCS_DEFAULT += ex7Main.cpp
ex7_SRCS_vxWorks += -nil-

# Link in the code from our support library
ex7_LIBS += ex7Support

# To build SNL programs, SNCSEQ must be defined
# in the <top>/configure/RELEASE file
ifneq ($(SNCSEQ),)
    # Build sncExample into ex7Support
    sncExample_SNCFLAGS += +r
    ex7_DBD += sncExample.dbd
    # A .stt sequence program is *not* pre-processed:
    ex7Support_SRCS += sncExample.stt
    ex7Support_LIBS += seq pv
    ex7_LIBS += seq pv

    # Build sncProgram as a standalone program
    PROD_HOST += sncProgram
    sncProgram_SNCFLAGS += +m
    # A .st sequence program *is* pre-processed:
    sncProgram_SRCS += sncProgram.st
    sncProgram_LIBS += seq pv
    sncProgram_LIBS += $(EPICS_BASE_HOST_LIBS)
endif

# Link QSRV (pvAccess Server) if available
#ifdef EPICS_QSRV_MAJOR_VERSION
    #ex7_LIBS += qsrv
    #ex7_LIBS += $(EPICS_BASE_PVA_CORE_LIBS)
    #ex7_DBD += PVAServerRegister.dbd
    #ex7_DBD += qsrv.dbd
#endif

# Finally link IOC to the EPICS Base libraries
ex7_LIBS += qsrv pvaClient pvDatabase pvAccessIOC pvAccess pvAccessCA nt pvData
ex7_LIBS += $(EPICS_BASE_PVA_CORE_LIBS)
ex7_LIBS += $(EPICS_BASE_IOC_LIBS)

include $(TOP)/configure/RULES
#----------------------------------------
#  ADD EXTRA GNUMAKE RULES BELOW HERE

# Explicit dependency needed for generated header file
devex7Version$(DEP): $(COMMON_DIR)/$(GENVERSION)
