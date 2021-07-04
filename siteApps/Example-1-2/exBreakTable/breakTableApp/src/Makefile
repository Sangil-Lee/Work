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
DBD += menuConvert.dbd
DBD += bptHWRHI.dbd
DBD += bptQWRNEWHI.dbd
DBD += bptQWROLDHI.dbd

# breakTable.dbd will be made up from these files:
breakTable_DBD += base.dbd

# Include dbd files from all support applications:
#breakTable_DBD += xxx.dbd
breakTable_DBD += bptHWRHI.dbd
breakTable_DBD += bptQWRNEWHI.dbd
breakTable_DBD += bptQWROLDHI.dbd
breakTable_DBD += subFieldMapCurrent.dbd

# Add all the support libraries needed by this IOC
#breakTable_LIBS += xxx

# breakTable_registerRecordDeviceDriver.cpp derives from breakTable.dbd
breakTable_SRCS += breakTable_registerRecordDeviceDriver.cpp

breakTable_SRCS += subFieldMapCurrent.cpp

# Build the main IOC entry point on workstation OSs.
breakTable_SRCS_DEFAULT += breakTableMain.cpp
breakTable_SRCS_vxWorks += -nil-

# Add support from base/src/vxWorks if needed
#breakTable_OBJS_vxWorks += $(EPICS_BASE_BIN)/vxComLibrary

ifneq ($(SNCSEQ),)
    # This builds sncExample as a component of tr1
    breakTable_SNCFLAGS += +r
    breakTable_DBD  += sncFieldMap.dbd
    breakTable_SRCS += sncFieldMap.stt
    breakTable_LIBS += seq pv
endif

# Finally link to the EPICS Base libraries
breakTable_LIBS += $(EPICS_BASE_IOC_LIBS)

#===========================

include $(TOP)/configure/RULES
#----------------------------------------
#  ADD RULES AFTER THIS LINE

