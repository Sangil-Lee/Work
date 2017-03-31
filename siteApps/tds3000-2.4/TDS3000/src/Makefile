TOP=../../

include $(TOP)/configure/CONFIG
#----------------------------------------
#  ADD MACRO DEFINITIONS AFTER THIS LINE
#=============================

#==================================================
# build a support library

LIBRARY_IOC += devTDS3000

# install devXxxSoft.dbd into <top>/dbd
DBD += devTDS3000.dbd
DBD += devTDS5000.dbd

# The following are compiled and added to the Support library
devTDS3000_SRCS += devTDS3000.c
devTDS3000_SRCS += devTDS5000.c
devTDS3000_LIBS += asyn
devTDS3000_LIBS += $(EPICS_BASE_IOC_LIBS)

#===========================

include $(TOP)/configure/RULES
#----------------------------------------
#  ADD RULES AFTER THIS LINE

