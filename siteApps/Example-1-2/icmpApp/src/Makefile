TOP=../..

include $(TOP)/configure/CONFIG
#----------------------------------------
#  ADD MACRO DEFINITIONS AFTER THIS LINE
#=============================
USR_INCLUDES  += -I$(RAON_SITELIBS)/include
USR_DBDFLAGS  += -I${RAON_SITELIBS}/dbd/
#=============================
# Build the IOC application

PROD_IOC = icmp
# icmp.dbd will be created and installed
DBD += icmp.dbd

# icmp.dbd will be made up from these files:
icmp_DBD += base.dbd
icmp_DBD += icmpRecord.dbd
icmp_DBD += icmpDevSoft.dbd
icmp_DBD += debug.dbd

# Include dbd files from all support applications:
#icmp_DBD += xxx.dbd

icmp_DIR += $(RAON_SITELIBS)/lib/$(T_A)
debugPrint_DIR += $(RAON_SITELIBS)/lib/$(T_A)

# Add all the support libraries needed by this IOC
icmp_LIBS += icmp
icmp_LIBS += debugPrint

# icmp_registerRecordDeviceDriver.cpp derives from icmp.dbd
icmp_SRCS += icmp_registerRecordDeviceDriver.cpp

# Build the main IOC entry point on workstation OSs.
icmp_SRCS_DEFAULT += icmpMain.cpp
icmp_SRCS_vxWorks += -nil-

# Add support from base/src/vxWorks if needed
#icmp_OBJS_vxWorks += $(EPICS_BASE_BIN)/vxComLibrary

# Finally link to the EPICS Base libraries
icmp_LIBS += $(EPICS_BASE_IOC_LIBS)

#===========================

include $(TOP)/configure/RULES
#----------------------------------------
#  ADD RULES AFTER THIS LINE

