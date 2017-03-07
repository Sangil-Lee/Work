#*************************************************************************
# Copyright (c) 2002 The University of Chicago, as Operator of Argonne
#     National Laboratory.
# Copyright (c) 2002 The Regents of the University of California, as
#     Operator of Los Alamos National Laboratory.
# EPICS BASE Versions 3.13.7
# and higher are distributed subject to a Software License Agreement found
# in file LICENSE that is included with this distribution. 
#*************************************************************************
TOP = ..
include $(TOP)/configure/CONFIG

GTRSUP = $(TOP)/gtrSup
#USR_CFLAGS += -DDEBUG
#USR_CFLAGS += -DHAS_UNIVERSEDMA

LIBRARY_IOC += gtr
gtr_LIBS += $(EPICS_BASE_IOC_LIBS)

SRC_DIRS += $(GTRSUP)/rec
INC +=
SRCS +=
DBD +=

SRC_DIRS += $(GTRSUP)/gtr
INC += drvGtr.h
SRCS += devGtr.c drvGtr.c
DBD += gtr.dbd

SRC_DIRS += $(GTRSUP)/llrf
VME_ONLY_SRCS += drvllrf.c
DBD += drvllrf.dbd

SRCS_linux-x86_64 += $(VME_ONLY_SRCS)
SRCS += $(SRCS_$(T_A))


include $(TOP)/configure/RULES
