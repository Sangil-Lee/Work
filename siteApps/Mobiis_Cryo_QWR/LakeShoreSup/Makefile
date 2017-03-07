TOP=..
include $(TOP)/configure/CONFIG
#=======================================

# Build devLakeShore as a library for an IOC:
LIBRARY_IOC += devLakeShore

# Library Source files
devLakeShore_SRCS += devLakeShore.c

# Link with the asyn and base libraries
devLakeShore_LIBS += asyn
devLakeShore_LIBS += $(EPICS_BASE_IOC_LIBS)

# Install .dbd and .db files
DBD += devLakeShore.dbd
DB_INSTALLS += devLakeShore.db

#=======================================
include $(TOP)/configure/RULES
