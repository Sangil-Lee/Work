TOP = ../..
include $(TOP)/configure/CONFIG

# IOCSH += alarmTest.iocsh
IOCSH += $(patsubst ../%, %, $(wildcard ../*.iocsh))

include $(TOP)/configure/RULES

