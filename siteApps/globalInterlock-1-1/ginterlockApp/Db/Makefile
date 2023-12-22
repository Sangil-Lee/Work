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

## Common OpOnStat
DB += globalInt_OpOnStatXV7502.template
DB += globalInt_OpOnStatXV7301.template
DB += globalInt_OpOnStat.sub

###QWR
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

DB += globalInt_PT7301PT7307.template
DB += globalInt_PT7301PT7307.sub


###HWR
DB += globalInt_PT7502PT7307_HWR.template
DB += globalInt_PT7201PT7307_HWR.template
DB += globalInt_PT7502PT7201_HWR.sub

DB += globalInt_PT7502CV_HWR.template
DB += globalInt_PT7502CV_HWR_IntStat.template
DB += globalInt_PT7502CV_HWR.sub

DB += globalInt_PT7502_HWR.template
DB += globalInt_PT7502_HWR.sub

DB += globalInt_PT7201CV_HWR.template
DB += globalInt_PT7201CV_HWR.sub

DB += globalInt_PT7201_HWR.template
DB += globalInt_PT7201_HWR.sub

###SSR
DB += globalInt_PT7502PT7381_SSR.template 
DB += globalInt_PT7201PT7381_SSR.template
DB += globalInt_PT7502PT7201_SSR.sub

DB += globalInt_PT7502CV_SSR.template
DB += globalInt_PT7502CV_SSR_IntStat.template
DB += globalInt_PT7502CV_SSR.sub

DB += globalInt_PT7502_SSR.template
DB += globalInt_PT7502_SSR.sub

DB += globalInt_PT7201CV_SSR.template
DB += globalInt_PT7201CV_SSR.sub

DB += globalInt_PT7201_SSR.template
DB += globalInt_PT7201_SSR.sub

###LTS
DB += globalInt_PT7502PT7381_LTS.template 
DB += globalInt_PT7301PT7381_LTS.template
DB += globalInt_PT7502PT7301_LTS.sub

DB += globalInt_PT7502CV_LTS.template
DB += globalInt_PT7502CV_LTS_IntStat.template
DB += globalInt_PT7502CV_LTS.sub

DB += globalInt_PT7502_LTS.template
DB += globalInt_PT7502_LTS.sub

DB += globalInt_PT7301CV_LTS.template
DB += globalInt_PT7301CV_LTS.sub

DB += globalInt_PT7301_LTS.template
DB += globalInt_PT7301_LTS.sub

###EBx
#DB += globalInt_PT7501CV_EBx.template
#DB += globalInt_PT7501CV_EBx_IntStat.template
#DB += globalInt_PT7501CV_EBx.sub
###New EBx
DB += globalInt_EBx.template
DB += globalInt_EBx.sub

###VBx-TBx Vacuum
DB += globalInt_VBxVac.db

###TBx
DB += globalInt_TBx.template
DB += globalInt_CV7504CV7545_TBx.template
DB += globalInt_CV7304CV7344_TBx.template
DB += globalInt_OpOnStatCV7504CV7505_TBx.template
DB += globalInt_OpOnStatCV7304CV7344_TBx.template
DB += globalInt_TBx.sub

###Alias DB
DB += Alias_IntStat.db
DB += Alias_SetInt.db

####for Simulation#########
#DB += biSim.template
#DB += biSim2.template
#DB += biSim.sub
#DB += aiSim.template
#DB += aiSim.sub

#----------------------------------------------------
# If <anyname>.db template is not named <anyname>*.template add
# <anyname>_template = <templatename>

include $(TOP)/configure/RULES
#----------------------------------------
#  ADD RULES AFTER THIS LINE

