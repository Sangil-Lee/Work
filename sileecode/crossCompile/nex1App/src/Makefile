TOP=../..

include $(TOP)/configure/CONFIG
#----------------------------------------
#  ADD MACRO DEFINITIONS AFTER THIS LINE
#=============================

#=============================
# Build the Library for 
LIBRARY += nex1Lib

SRC = $(TOP)/nex1App/src

SRC_DIRS += $(SRC)/Hello
nex1Lib_SRCS += hello.cpp
INC += hello.h

##SRC_DIRS += $(SRC)/Other_DIR
##nex1Lib_SRCS += other.cpp
##INC += other.h

#INC += xrfdc.h
#nex1Lib_LIBS += Xilinx Library.

#=============================
# Build the application

#PROD_IOC = nex1
PROD = nex1

# Add all the support libraries needed by this IOC
#nex1_LIBS += xxx

nex1_SRCS += 

# Build the main IOC entry point on workstation OSs.
nex1_SRCS_DEFAULT += main.cc

# Finally link to the EPICS Base libraries
nex1_LIBS += nex1Lib

#===========================

include $(TOP)/configure/RULES
#----------------------------------------
#  ADD RULES AFTER THIS LINE

