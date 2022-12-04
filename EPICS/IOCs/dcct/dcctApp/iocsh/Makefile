TOP = ../..
include $(TOP)/configure/CONFIG

# IOCSH += dcct.iocsh
IOCSH += $(patsubst ../%, %, $(wildcard ../*.iocsh))

include $(TOP)/configure/RULES

