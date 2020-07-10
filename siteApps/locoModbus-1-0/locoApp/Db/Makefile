TOP=../..
include $(TOP)/configure/CONFIG
#----------------------------------------
#  ADD MACRO DEFINITIONS AFTER THIS LINE

#----------------------------------------------------
# Create and install (or just install) into <top>/db
# databases, templates, substitutions like this
#DB += xxx.db
DB += aiFloat64.template  aoFloat64.template  
DB += ai.template ao.template bi_bit.template bo_bit.template longin.template
DB += bi_word.template  bo_word.template longout.template
DB += asynRecord.template
DB += StrBitOut.vdb


#----------------------------------------------------
# If <anyname>.db template is not named <anyname>*.template add
# <anyname>_template = <templatename>

include $(TOP)/configure/RULES
#----------------------------------------
#  ADD RULES AFTER THIS LINE

