TOP=../..
include $(TOP)/configure/CONFIG
#----------------------------------------
#  ADD MACRO DEFINITIONS AFTER THIS LINE

#----------------------------------------------------
# Create and install (or just install) into <top>/db
# databases, templates, substitutions like this
#DB += xxx.db
DB += aiFloat64.template
DB += aoFloat64.template
DB += bi_word.template
DB += bo_word.template
DB += ndpsdiag001.sub
DB += ndpsdiag002.sub
DB += ndpsdiag003.sub
DB += ndpsCTarget.sub

#----------------------------------------------------
# If <anyname>.db template is not named <anyname>*.template add
# <anyname>_template = <templatename>

include $(TOP)/configure/RULES
#----------------------------------------
#  ADD RULES AFTER THIS LINE

