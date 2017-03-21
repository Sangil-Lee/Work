TOP=../..

include $(TOP)/configure/CONFIG
#----------------------------------------
#  ADD MACRO DEFINITIONS BELOW HERE

# xxxRecord.h will be created from xxxRecord.dbd
#DBDINC += xxxRecord
DBDINC      += icmpRecord

# Install xxxSupport.dbd into <top>/dbd
#DBD += xxxSupport.dbd
DBD         += icmpRecord.dbd
DBD         += icmpDevSoft.dbd

# Build an IOC support library
LIBRARY_IOC += icmpOping

USR_CXXFLAGS += -Wno-unused-function -fpermissive -Wno-unused-variable

OPINGLIB = /usr/lib/x86_64-linux-gnu
OPINGINC = /usr/include

USR_INCLUDES  += -I$(RAON_SITELIBS)/include
USR_INCLUDES  += -I$(OPINGINC)

# Compile and add the code to the support library
icmpOping_SRCS += icmpRecord.c

# Link locally-provided code into the support library,
# rather than directly into the IOC application.
# This is required for Windows DLL builds.
icmpOping_SRCS += icmpOPingDev.cpp

oping_DIR += $(OPINGLIB)

icmpOping_LIBS += oping
icmpOping_LIBS += $(EPICS_BASE_IOC_LIBS)

# Build the IOC application
PROD_IOC = icmp

# icmp.dbd will be created and installed
DBD += icmp.dbd

# icmp.dbd will include these files:
icmp_DBD += base.dbd
icmp_DBD += icmpRecord.dbd
icmp_DBD += icmpDevSoft.dbd

# icmp_registerRecordDeviceDriver.cpp derives from icmp.dbd
icmp_SRCS += icmp_registerRecordDeviceDriver.cpp

# Build the main IOC entry point where needed
icmp_SRCS_DEFAULT += icmpMain.cpp
icmp_SRCS_vxWorks += -nil-

# Add support from base/src/vxWorks if needed
#icmp_OBJS_vxWorks += $(EPICS_BASE_BIN)/vxComLibrary

# Link in the code from our support library
icmp_LIBS += icmpOping

# Finally link IOC to the EPICS Base libraries
icmp_LIBS += $(EPICS_BASE_IOC_LIBS)

include $(TOP)/configure/RULES
#----------------------------------------
#  ADD EXTRA GNUMAKE RULES BELOW HERE

