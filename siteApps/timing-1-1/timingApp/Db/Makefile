TOP=../..
include $(TOP)/configure/CONFIG
#----------------------------------------
#  ADD MACRO DEFINITIONS AFTER THIS LINE

#----------------------------------------------------
#  Optimization of db files using dbst (DEFAULT: NO)
#DB_OPT = YES

#----------------------------------------------------
# Create and install (or just install) into <top>/db
# databases, templates, substitutions like this
#DB += xxx.db
#DB += timingAsynDriver.db
DB += bi.template
DB += bo.template
DB += mbbi.template
DB += ai.template
DB += ao.template
DB += stringin.template
DB += stringout.template
DB += longout.template
DB += longin.template
DB += timeStamp.db
DB += fanoutBit.vdb
DB += fanoutShort.vdb

#----------------------------------------------------
# If <anyname>.db template is not named <anyname>*.template add
# <anyname>_template = <templatename>

include $(TOP)/configure/RULES
#----------------------------------------
#  ADD RULES AFTER THIS LINE

