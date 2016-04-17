#Makefile at top of application tree
TOP = .
include $(TOP)/configure/CONFIG
DIRS += configure
DIRS := $(DIRS) $(filter-out $(DIRS), $(wildcard *Lib))
include $(TOP)/configure/RULES_TOP
