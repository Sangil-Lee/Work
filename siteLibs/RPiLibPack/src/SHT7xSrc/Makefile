TOP = ../../..
include $(TOP)/configure/CONFIG

LIBRARY	+= raspSHT7x
DBDINC	+= shtRecord
DBD	+= shtRecord.dbd devSHT7x.dbd

raspSHT7x_SRCS += RPi_SHT1x.c shtRecord.c devSHT7x.c

include $(TOP)/configure/RULES

