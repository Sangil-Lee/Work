TOP = ../..
include $(TOP)/configure/CONFIG

LIBRARY += timeStamp

USR_INCLUDES  += -I$(RAON_SITELIBS)/include

DBD += timestampDevSoft.dbd
DBDINC += timestampRecord


timeStamp_SRCS += timestampRecord.c
#timeStamp_SRCS += timestampRecord.cpp
timeStamp_SRCS += devTimestampSoft.c

timeStamp_LIBS += $(EPICS_BASE_IOC_LIBS)

include $(TOP)/configure/RULES
