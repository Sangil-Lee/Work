TOP=../..

include $(TOP)/configure/CONFIG
#----------------------------------------
#  ADD MACRO DEFINITIONS AFTER THIS LINE
#=============================

#=============================
# Build the IOC application

PROD_IOC = breakTable
# breakTable.dbd will be created and installed
DBD += breakTable.dbd
DBD += bptHWRIH.dbd
DBD += bptQWRNEWIH.dbd
DBD += bptQWROLDIH.dbd
DBD += menuConvert.dbd


# breakTable.dbd will be made up from these files:
breakTable_DBD += base.dbd
breakTable_DBD += bptHWRIH.dbd
breakTable_DBD += bptQWRNEWIH.dbd
breakTable_DBD += bptQWROLDIH.dbd

# Include dbd files from all support applications:
#breakTable_DBD += xxx.dbd

# Add all the support libraries needed by this IOC
#breakTable_LIBS += xxx

# breakTable_registerRecordDeviceDriver.cpp derives from breakTable.dbd
breakTable_SRCS += breakTable_registerRecordDeviceDriver.cpp

# Build the main IOC entry point on workstation OSs.
breakTable_SRCS_DEFAULT += breakTableMain.cpp
breakTable_SRCS_vxWorks += -nil-

# Add support from base/src/vxWorks if needed
#breakTable_OBJS_vxWorks += $(EPICS_BASE_BIN)/vxComLibrary

# Finally link to the EPICS Base libraries
breakTable_LIBS += $(EPICS_BASE_IOC_LIBS)

#===========================

include $(TOP)/configure/RULES
#----------------------------------------
#  ADD RULES AFTER THIS LINE

