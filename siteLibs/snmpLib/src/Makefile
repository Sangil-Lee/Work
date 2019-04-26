TOP = ../..
include $(TOP)/configure/CONFIG

LIBRARY += snmpMon
DBDINC       += snmpRecord snmpstrRecord
DBD          += snmpRecord.dbd snmpstrRecord.dbd
DBD          += snmpDevSoft.dbd
snmpMon_SRCS += snmpDevAsync.c snmpRecord.c snmpstrRecord.c
#snmpMon_SRCS += snmpAsync_v3.c snmpRecord.c

netsnmp_DIR = /usr/lib
# for jessie, need to check for Wheezy
# jhlee
netsnmp_DIR += /usr/lib/x86_64-linux-gnu
snmpMon_LIBS +=netsnmp

# need to check where it is in Wheezy



USR_CFLAGS  += `net-snmp-config --cflags`
USR_LDFLAGS += `net-snmp-config --libs`
PROD_LDLIBS += `net-snmp-config --libs`

include $(TOP)/configure/RULES      
