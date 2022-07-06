TOP=../..
include $(TOP)/configure/CONFIG
#----------------------------------------
#  ADD MACRO DEFINITIONS AFTER THIS LINE

#----------------------------------------------------
# Create and install (or just install) into <top>/db
# databases, templates, substitutions like this
#DB += xxx.db
##Example
#DB += GBIntStat.template
#DB += GBIntStat.sub

##Real
DB += GBIntStat_3INP_CMs.template
DB += GBIntStat_2INP_CMs.template
DB += GBIntStat_1INP_CMs.template
DB += GBIntStat_CMs.sub
DB += GBIntStat_5INP_VBx.template
DB += GBIntStat_3INP_VBx.template
DB += GBIntStat_1INP_VBx.template
DB += GBIntStat_VBx.sub
DB += GBIntStat_2INP_EBx.template
DB += GBIntStat_1INP_EBx.template
DB += GBIntStat_EBx.sub
DB += GBCURStat.template
DB += GBCURStat.sub
DB += GBCPStat_XV.template
DB += GBCPStat_CV.template
DB += GBCPStat.sub

#DB += BitWiseTest.vdb
#DB += GBCURStat.vdb

#----------------------------------------------------
# If <anyname>.db template is not named <anyname>*.template add
# <anyname>_template = <templatename>

include $(TOP)/configure/RULES
#----------------------------------------
#  ADD RULES AFTER THIS LINE

