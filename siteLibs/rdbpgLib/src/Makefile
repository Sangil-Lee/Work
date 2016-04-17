TOP = ../..
include $(TOP)/configure/CONFIG

#LIBRARY_IOC += ifStatMon
LIBRARY		+= rdbPostgreSQL


DBD         += rdbpostgreSQLRecord.dbd
DBDINC      += rdbpostgreSQLRecord

#DBD         += menuInOut.dbd
#DBDINC      += menuInOut
ifStatMon_SRCS += devRdbpostgreSQL.c rdbpostgreSQLRecord.c

DBD			+= rdbpostgreSQLDevSoft.dbd
include $(TOP)/configure/RULES
