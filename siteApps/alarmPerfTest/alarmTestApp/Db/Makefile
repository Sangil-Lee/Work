TOP=../..
include $(TOP)/configure/CONFIG
#----------------------------------------
#  ADD MACRO DEFINITIONS AFTER THIS LINE

#----------------------------------------------------
# Create and install (or just install) into <top>/db
# databases, templates, substitutions like this
#DB += xxx.db

DB += iocAdminSoft.db

ifneq ($(ASYN),)
DB_INSTALLS += $(ASYN)/db/asynRecord.db
endif

ifneq ($(devIocStats),)
#DB_INSTALLS += iocAdminSoft.db
#DB_INSTALLS += $(devIocStats)/db/ioc_stats.db
endif

ifneq ($(RECCASTER),)
DB_INSTALLS += $(RECCASTER)/db/reccaster.db
endif

ifneq ($(AUTOSAVE),)
DB_INSTALLS += $(AUTOSAVE)/db/save_restoreStatus.db
endif

DB += $(patsubst ../%, %, $(wildcard ../*.template))
DB += $(patsubst ../%, %, $(wildcard ../*.db))
DB += $(patsubst ../%, %, $(wildcard ../*.vdb))
DB += $(patsubst ../%, %, $(wildcard ../*.substitutions))

##
DB += $(patsubst ../%, %, $(wildcard ../*.proto))
DB += $(patsubst ../%, %, $(wildcard ../*.acf))

REQ += $(patsubst ../%, %, $(wildcard ../*.req))

#----------------------------------------------------
# If <anyname>.db template is not named <anyname>*.template add
# <anyname>_template = <templatename>

include $(TOP)/configure/RULES
#----------------------------------------
#  ADD RULES AFTER THIS LINE

