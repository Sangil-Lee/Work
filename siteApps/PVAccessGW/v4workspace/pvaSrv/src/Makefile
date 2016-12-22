# Makefile for the pvaSrv library

TOP = ..
include $(TOP)/configure/CONFIG

LIBRARY_IOC = pvaSrv

SHRLIB_VERSION = 0.11.0

PVASRV_SRC = $(TOP)/src

include $(PVASRV_SRC)/dbPv/Makefile
#include $(PVASRV_SRC)/dbGroup/Makefile

pvaSrv_LIBS += pvAccess pvData
pvaSrv_LIBS += $(EPICS_BASE_IOC_LIBS)

FINAL_LOCATION ?= $(shell $(PERL) $(TOOLS)/fullPathName.pl $(INSTALL_LOCATION))

USR_CPPFLAGS += -DFINAL_LOCATION="\"$(FINAL_LOCATION)\""

PROD_IOC += softIocPVA

softIocPVA_SRCS += softMain.cpp
softIocPVA_SRCS += softIocPVA_registerRecordDeviceDriver.cpp

softIocPVA_LIBS += pvaSrv pvAccess pvData $(MBLIB)
softIocPVA_LIBS += $(EPICS_BASE_IOC_LIBS)

DBD += softIocPVA.dbd

softIocPVA_DBD += softIoc.dbd
softIocPVA_DBD += dbPv.dbd
softIocPVA_DBD += PVAServerRegister.dbd
include $(TOP)/configure/RULES
