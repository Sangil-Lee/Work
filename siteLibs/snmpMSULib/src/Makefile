TOP = ../..
include $(TOP)/configure/CONFIG

#LIBRARY_IOC += devSnmpMon
LIBRARY      += devSnmp
DBD          += snmpMSURecord.dbd
DBDINC       += snmpMSURecord
DBD          += menuCalc.dbd
DBDINC       += menuCalc
devSnmp_SRCS += snmpRegister.cpp snmpSessShow.c devSnmpSoft.cpp snmpMSURecord.c
#devSnmp_SRCS += snmpRegister.cpp snmpSessShow.c devSnmpSoft2.cpp snmpRecord.c

USR_CFLAGS  += `net-snmp-config --cflags`
USR_LDFLAGS += `net-snmp-config --libs`
PROD_LDLIBS += `net-snmp-config --libs`

USR_CFLAGS   += $(shell $(PERL) ../getNetSNMPversion.pl)
USR_CPPFLAGS += $(shell $(PERL) ../getNetSNMPversion.pl)


#ifStatMon_LIBS = netsnmp

DBD          += snmpMSUDevSoft.dbd


include $(TOP)/configure/RULES                                  
