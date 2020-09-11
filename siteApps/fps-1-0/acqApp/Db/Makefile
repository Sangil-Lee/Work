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
DB += timeStamp.db

#----------------------------------------------------
# If <anyname>.db template is not named <anyname>*.template add
# <anyname>_template = <templatename>
DB += stringin.template
DB += longin.template
DB += longout.template
DB += acq_AllFileSave_Calc.template
DB += acq_PMS_Enable.template

DB += acq_PMSEN_calc.db

# DB += int32.template
# DB += uint32.db
# DB += mbbiDirectTest.vdb
# DB += int32array.template
# DB += bi.template
# DB += bo.template
# DB += calc.template

include $(TOP)/configure/RULES
#----------------------------------------
#  ADD RULES AFTER THIS LINE

