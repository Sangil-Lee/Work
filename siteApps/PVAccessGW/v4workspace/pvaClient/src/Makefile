# This is a Makefile fragment, see ../Makefile

TOP = ..
include $(TOP)/configure/CONFIG

LIBRARY += pvaClient

SHRLIB_VERSION=4.1.1

INC += pv/pvaClient.h
INC += pv/pvaClientMultiChannel.h

LIBSRCS += pvaClient.cpp
LIBSRCS += pvaClientPutData.cpp
LIBSRCS += pvaClientGetData.cpp
LIBSRCS += pvaClientMonitorData.cpp
LIBSRCS += pvaClientChannel.cpp
LIBSRCS += pvaClientProcess.cpp
LIBSRCS += pvaClientGet.cpp
LIBSRCS += pvaClientPut.cpp
LIBSRCS += pvaClientMonitor.cpp
LIBSRCS += pvaClientPutGet.cpp
LIBSRCS += pvaClientMultiChannel.cpp
LIBSRCS += pvaClientMultiGetDouble.cpp
LIBSRCS += pvaClientMultiPutDouble.cpp
LIBSRCS += pvaClientMultiMonitorDouble.cpp
LIBSRCS += pvaClientNTMultiPut.cpp
LIBSRCS += pvaClientNTMultiData.cpp
LIBSRCS += pvaClientNTMultiGet.cpp
LIBSRCS += pvaClientNTMultiMonitor.cpp

pvaClient_LIBS += pvAccess pvData nt Com
pvaClient_LIBS +=  $(EPICS_BASE_IOC_LIBS)

include $(TOP)/configure/RULES

