TOP=../

include $(TOP)/configure/CONFIG

ifeq (linux-x86_64, $(findstring linux-x86_64, $(T_A)))
LIB_INSTALLS    += ../libNeptuneAPI.so
LIB_INSTALLS    += ../libNeptuneAPI.so.0.9.0.1

else ifeq (linux-x86, $(findstring linux-x86, $(T_A)))

endif

include $(TOP)/configure/RULES
