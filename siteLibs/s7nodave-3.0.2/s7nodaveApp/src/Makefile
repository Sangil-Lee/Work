TOP=../..

include $(TOP)/configure/CONFIG
#----------------------------------------
#  ADD MACRO DEFINITIONS AFTER THIS LINE
#=============================

#==================================================
# build a support library

LIBRARY_IOC += s7nodave

# install s7nodave.dbd into <top>/dbd
DBD += s7nodave.dbd

#s7nodave_DBD = asyn.dbd

# specify all source files to be compiled and added to the library
s7nodave_SRCS += AnalogSupport.cc
s7nodave_SRCS += ArraySupport.cc
s7nodave_SRCS += BinarySupport.cc
s7nodave_SRCS += LongSupport.cc
s7nodave_SRCS += MultiBinarySupport.cc
s7nodave_SRCS += PlcAddress.cc
s7nodave_SRCS += PollGroup.cc
s7nodave_SRCS += PortDriver.cc
s7nodave_SRCS += PortDriverReadItem.cc
s7nodave_SRCS += PortDriverReadOptimizer.cc
s7nodave_SRCS += S7nodaveInputRecord.cc
s7nodave_SRCS += S7nodaveOutputRecord.cc
s7nodave_SRCS += S7nodaveRecord.cc
s7nodave_SRCS += S7nodaveRecordAddress.cc
s7nodave_SRCS += s7nodaveAsyn.cc
s7nodave_SRCS += s7nodaveRecordDefinitions.c
s7nodave_SRCS += s7nodaveRegistrar.c
s7nodave_SRCS += StringSupport.cc
s7nodave_SRCS += utilities.cc

s7nodave_LIBS += asyn
s7nodave_LIBS += snap7
s7nodave_LIBS += $(EPICS_BASE_IOC_LIBS)

#===========================

include $(TOP)/configure/RULES
#----------------------------------------
#  ADD RULES AFTER THIS LINE

