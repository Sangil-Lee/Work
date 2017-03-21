TOP=..
include $(TOP)/configure/CONFIG
#=======================================

# Build devSorensen as a library for an IOC:
LIBRARY_IOC += devSorensen

# Library Source files
devSorensen_SRCS += devSorensen.c

# Link with the asyn and base libraries
devSorensen_LIBS += asyn
devSorensen_LIBS += $(EPICS_BASE_IOC_LIBS)

# Install .dbd and .db files
DBD += devSorensen.dbd
DB_INSTALLS += devSorensen.db

#=======================================
include $(TOP)/configure/RULES
