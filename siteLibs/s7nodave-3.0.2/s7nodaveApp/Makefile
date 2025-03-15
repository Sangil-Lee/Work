TOP = ..
include $(TOP)/configure/CONFIG
DIRS := $(DIRS) $(filter-out $(DIRS), $(wildcard *src*))
DIRS := $(DIRS) $(filter-out $(DIRS), $(wildcard *Src*))
DIRS := $(DIRS) $(filter-out $(DIRS), $(wildcard *db*))
DIRS := $(DIRS) $(filter-out $(DIRS), $(wildcard *Db*))
DIRS += snap7
src_DEPEND_DIRS = snap7
ifdef BUILD_TESTS
DIRS += tests
tests_DEPEND_DIRS += src
endif
include $(TOP)/configure/RULES_DIRS
