TOP=../..
include $(TOP)/configure/CONFIG
#----------------------------------------
#  ADD MACRO DEFINITIONS AFTER THIS LINE

#----------------------------------------------------
# Create and install (or just install) into <top>/db
# databases, templates, substitutions like this
#DB += xxx.db
#DB += GInt_INP1.template
DB += GIntXV.template
DB += GIntCV.template
DB += GInt.sub
DB += globalIntWF.template
DB += globalIntWF.sub

DB += globalInt_PT7503.sub
DB += globalInt_PT7503.template

DB += biSim.template
DB += biSim2.template
DB += biSim.sub
DB += aiSim.template
DB += aiSim.sub

#----------------------------------------------------
# If <anyname>.db template is not named <anyname>*.template add
# <anyname>_template = <templatename>

include $(TOP)/configure/RULES
#----------------------------------------
#  ADD RULES AFTER THIS LINE

