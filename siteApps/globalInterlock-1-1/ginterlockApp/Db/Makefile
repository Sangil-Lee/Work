TOP=../..
include $(TOP)/configure/CONFIG
#----------------------------------------
#  ADD MACRO DEFINITIONS AFTER THIS LINE

#----------------------------------------------------
# Create and install (or just install) into <top>/db
# databases, templates, substitutions like this
#DB += xxx.db
#DB += GInt_INP1.template
#DB += GIntXV.template
#DB += GIntCV.template
#DB += GInt.sub

DB += globalIntWF.template
DB += globalIntWF.sub

DB += globalInt_PT7503.template
DB += globalInt_PT7503.sub
DB += globalInt_PT7301.template
DB += globalInt_PT7301.sub

DB += globalInt_PT7503PT7307.template
DB += globalInt_PT7503PT7307.sub

DB += globalInt_PT7503CV_QWR.template
DB += globalInt_PT7503CV.template
DB += globalInt_PT7503CV.sub

DB += globalInt_PT7301CV.template
DB += globalInt_PT7301CV.sub


###HWR
DB += globalInt_PT7502_HWR.template
DB += globalInt_PT7201_HWR.template
DB += globalInt_PT7502PT7201_HWR.sub


####for Simulation#########
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

