# This is an example V3 IOC that includes the pvaSrv pvAccess server
# To add this functionality:
# 1) link against the pvData, pvAccess and pvIOC libraries
# 2) include v3Channel.dbd
# 3) call startV3Channel in st.cmd

TOP=../..

include $(TOP)/configure/CONFIG

DBD += simpleDbPv.dbd
PROD_IOC += simpleDbPv

simpleDbPv_SRCS += simpleDbPv_registerRecordDeviceDriver.cpp
simpleDbPv_SRCS_DEFAULT += simpleDbPvMain.cpp

simpleDbPv_LIBS += pvaSrv pvAccess pvData $(MBLIB)
simpleDbPv_LIBS += $(EPICS_BASE_IOC_LIBS)

simpleDbPv_SRCS_vxWorks += -nil-
simpleDbPv_OBJS_vxWorks += $(EPICS_BASE_BIN)/vxComLibrary

include $(TOP)/configure/RULES
