TOP=../..
include $(TOP)/configure/CONFIG

#EPICS_DB_INCLUDE_PATH += $(RAON_SITELIBS)/db
#----------------------------------------
#  ADD MACRO DEFINITIONS AFTER THIS LINE

#----------------------------------------------------
#  Optimization of db files using dbst (DEFAULT: NO)
#DB_OPT = YES

#----------------------------------------------------
# Create and install (or just install) into <top>/db
# databases, templates, substitutions like this
#DB += xxx.db
DB += ${RAON_SITELIBS}/db/dbSysMon.db
DB += ${RAON_SITELIBS}/db/dbIfstat.vdb
DB += ${RAON_SITELIBS}/db/dbSysTimeStamp.vdb

#----------------------------------------------------
# If <anyname>.db template is not named <anyname>*.template add
# <anyname>_template = <templatename>

include $(TOP)/configure/RULES
#----------------------------------------
#  ADD RULES AFTER THIS LINE

