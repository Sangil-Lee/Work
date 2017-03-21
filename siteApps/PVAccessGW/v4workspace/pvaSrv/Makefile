# Makefile for the EPICS V4 pvaSrv module

TOP = .
include $(TOP)/configure/CONFIG

DIRS := configure

DIRS += src
src_DEPEND_DIRS = configure

EMBEDDED_TOPS := $(wildcard *Top)

DIRS += $(EMBEDDED_TOPS)

define dir_DEP
  $(1)_DEPEND_DIRS = src
endef

$(foreach dir, $(EMBEDDED_TOPS), $(eval $(call dir_DEP,$(dir))))

include $(TOP)/configure/RULES_TOP
