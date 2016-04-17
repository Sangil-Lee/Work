TOP = ../..
include $(TOP)/configure/CONFIG

#LIBRARY_IOC += ifStatMon
LIBRARY		+= ifStatMon
DBD         += ifstatRecord.dbd
DBDINC      += ifstatRecord
DBD         += menuInOut.dbd
DBDINC      += menuInOut
ifStatMon_SRCS += data.c drivers.c devIfstatSoft.c ifstatRecord.c

#ifStatMon_LIBS = netsnmp

DBD			+= ifstatDevSoft.dbd

include $(TOP)/configure/RULES
