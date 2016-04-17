TOP=../..

# R3.14
include $(TOP)/configure/CONFIG

# Allow for debugging
HOST_OPT = NO

# Want local functions non-static? Define DEBUG 
#CFLAGS += -DDEBUG

# library
LIBRARY  += s7plc
s7plc_SRCS += drvS7plc.c devS7plc.c
LIB_LIBS += $(EPICS_BASE_IOC_LIBS)

DBD += s7plcBase.dbd
DBD += s7plcCalcout.dbd
DBD += s7plcReg.dbd

# stand alone application program
PROD_DEFAULT = s7plcApp
PROD_vxWorks = -nil-
s7plcApp_SRCS += s7plcApp_registerRecordDeviceDriver.cpp
s7plcApp_SRCS += appMain.cc
s7plcApp_DBD += base.dbd
s7plcApp_DBD += s7plcBase.dbd
s7plcApp_DBD += s7plcCalcout.dbd
s7plcApp_DBD += s7plcReg.dbd
s7plcApp_LIBS += s7plc
s7plcApp_LIBS += $(EPICS_BASE_IOC_LIBS)

include $(TOP)/configure/RULES
