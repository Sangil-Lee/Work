TOP=../..
include $(TOP)/configure/CONFIG
#----------------------------------------
#  ADD MACRO DEFINITIONS AFTER THIS LINE

#----------------------------------------------------
# Create and install (or just install) into <top>/db
# databases, templates, substitutions like this
#DB += xxx.db
DB += bi_macro.template
DB += bi_word.template
DB += bo_word.template
DB += bi_bit.template
DB += bo_bit.template
DB += longin.template
DB += longout.template
DB += longinInt32.template
DB += longoutInt32.template
DB += mbbiDirect.template
DB += mbboDirect.template
DB += ai.template
DB += ai_average.template
DB += ao.template
DB += aiFloat64.template
DB += aoFloat64.template

#----------------------------------------------------
# If <anyname>.db template is not named <anyname>*.template add
# <anyname>_template = <templatename>

include $(TOP)/configure/RULES
#----------------------------------------
#  ADD RULES AFTER THIS LINE

