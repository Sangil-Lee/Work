$(shell ./fortrancompile.sh)
TOP=../..

include $(TOP)/configure/CONFIG
#----------------------------------------
#  ADD MACRO DEFINITIONS AFTER THIS LINE
#=============================

#=============================
# Build the IOC application

PROD_IOC = inventory
# inventory.dbd will be created and installed
DBD += inventory.dbd

# inventory.dbd will be made up from these files:
inventory_DBD += base.dbd

# Include dbd files from all support applications:
#inventory_DBD += xxx.dbd
inventory_DBD += FortranCalcLogic.dbd
inventory_DBD += asyn.dbd
inventory_DBD += stdSupport.dbd calcSupport.dbd
inventory_DBD += aSubNoise.dbd

# Add all the support libraries needed by this IOC
#inventory_LIBS += xxx

# inventory_registerRecordDeviceDriver.cpp derives from inventory.dbd
inventory_SRCS += inventory_registerRecordDeviceDriver.cpp
inventory_SRCS += Dnst_Enthalpy.f
inventory_SRCS += FortranCalcLogic.cpp
inventory_SRCS += aSubNoise.cpp

# Build the main IOC entry point on workstation OSs.
inventory_SRCS_DEFAULT += inventoryMain.cpp
inventory_SRCS_vxWorks += -nil-

ifneq ($(SNCSEQ),)
  inventory_SNCFLAGS += +r
  inventory_DBD += sncMakeAIDynamic.dbd
  inventory_DBD += sncMakeTotalInventory.dbd
  inventory_SRCS += sncMakeAIDynamic.stt
  inventory_SRCS += sncMakeTotalInventory.stt
  inventory_LIBS += seq pv
endif

# Add support from base/src/vxWorks if needed
#inventory_OBJS_vxWorks += $(EPICS_BASE_BIN)/vxComLibrary

# Finally link to the EPICS Base libraries
inventory_LIBS += asyn calc std
inventory_LIBS += $(EPICS_BASE_IOC_LIBS)

#===========================

include $(TOP)/configure/RULES
#----------------------------------------
#  ADD RULES AFTER THIS LINE

