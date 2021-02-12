TOP=../..

include $(TOP)/configure/CONFIG
#----------------------------------------
#  ADD MACRO DEFINITIONS AFTER THIS LINE
#=============================

#==================================================
# build a support library

LIBRARY_IOC += iocshExt

# xxxRecord.h will be created from xxxRecord.dbd
#DBDINC += xxxRecord
# install debug.dbd into <top>/dbd
DBD += dblfilter.dbd

# specify all source files to be compiled and added to the library
#debug_SRCS += xxx

SRCS += dblfilter.cpp


#===========================

include $(TOP)/configure/RULES
#----------------------------------------
#  ADD RULES AFTER THIS LINE

