TOP=..
include $(TOP)/configure/CONFIG
#=======================================

# Build deve364xa as a library for an IOC:
LIBRARY_IOC += deve364xa

# Library Source files
deve364xa_SRCS += deve364xa.c

# Link with the asyn and base libraries
deve364xa_LIBS += asyn
deve364xa_LIBS += $(EPICS_BASE_IOC_LIBS)

# Install .dbd and .db files
DBD += deve364xa.dbd
DB_INSTALLS += deve364xa.db

#=======================================
include $(TOP)/configure/RULES
