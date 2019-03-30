TOP=../..

include $(TOP)/configure/CONFIG
#----------------------------------------
#  ADD MACRO DEFINITIONS AFTER THIS LINE
#=============================

#==================================================
# build a support library

LIBRARY_IOC += icmp

# icmpRecord.h will be created from icmpRecord.dbd
DBDINC += icmpRecord

# install icmp.dbd into <top>/dbd
DBD += icmp.dbd
DBD += icmpRecord.dbd
DBD += icmpDevSoft.dbd

USR_CXXFLAGS += -Wno-unused-function -fpermissive -Wno-unused-variable -Wno-deprecated

INC += icmpOPingDev.h oping.h

USR_INCLUDES  += -I$(RAON_SITELIBS)/include
USR_DBDFLAGS  += -I${RAON_SITELIBS}/dbd/


# specify all source files to be compiled and added to the library
#icmp_SRCS += xxx
icmp_SRCS += icmpRecord.c liboping.c
icmp_SRCS += icmpOPingDev.cpp

debugPrint_DIR += $(RAON_SITELIBS)/lib/$(T_A)

icmp_DBD += debug.dbd
icmp_LIBS += debugPrint
icmp_LIBS += $(EPICS_BASE_IOC_LIBS)

icmp_LIBS += $(EPICS_BASE_IOC_LIBS)

#===========================

include $(TOP)/configure/RULES
#----------------------------------------
#  ADD RULES AFTER THIS LINE

