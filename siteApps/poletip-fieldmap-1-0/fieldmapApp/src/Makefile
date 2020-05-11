TOP=../..

include $(TOP)/configure/CONFIG
#----------------------------------------
#  ADD MACRO DEFINITIONS AFTER THIS LINE
#=============================

#=============================
# Build the IOC application

PROD_IOC = fieldmap
# fieldmap.dbd will be created and installed
DBD += fieldmap.dbd
DBD += subFieldMapCurrent.dbd

# fieldmap.dbd will be made up from these files:
fieldmap_DBD += base.dbd
fieldmap_DBD += subFieldMapCurrent.dbd

# Include dbd files from all support applications:
#fieldmap_DBD += xxx.dbd

# Add all the support libraries needed by this IOC
#fieldmap_LIBS += xxx

# fieldmap_registerRecordDeviceDriver.cpp derives from fieldmap.dbd
fieldmap_SRCS += fieldmap_registerRecordDeviceDriver.cpp
fieldmap_SRCS += subFieldMapCurrent.cpp

# Build the main IOC entry point on workstation OSs.
fieldmap_SRCS_DEFAULT += fieldmapMain.cpp
fieldmap_SRCS_vxWorks += -nil-

# Add support from base/src/vxWorks if needed
#fieldmap_OBJS_vxWorks += $(EPICS_BASE_BIN)/vxComLibrary
ifneq ($(SNCSEQ),)
    # This builds sncExample as a component of tr1
    fieldmap_SNCFLAGS += +r
    fieldmap_DBD  += sncFieldMap.dbd
    fieldmap_SRCS += sncFieldMap.stt
    fieldmap_LIBS += seq pv
endif

# Finally link to the EPICS Base libraries
fieldmap_LIBS += $(EPICS_BASE_IOC_LIBS)

#===========================

include $(TOP)/configure/RULES
#----------------------------------------
#  ADD RULES AFTER THIS LINE

