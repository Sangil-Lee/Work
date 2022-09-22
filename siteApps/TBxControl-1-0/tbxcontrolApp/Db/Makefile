TOP=../..
include $(TOP)/configure/CONFIG 

#---------------------------------------- #  ADD MACRO DEFINITIONS AFTER THIS LINE

#----------------------------------------------------
# Create and install (or just install) into <top>/db
# databases, templates, substitutions like this
#
DB  += aiFloat64.template
DB  += aoFloat64.template
DB  += biWord.template
DB  += boWord.template

## Before 0818
#DB  += TBxcontrol.sub
#DB  += TBxcontrol_2.sub
#DB  += TBxcontrol_test.sub

##0818
DB  += TBxcontrol_3.sub


##0818, CPStat
DB  += GBCPStat_XV_TBx.template
DB  += GBCPStat_CV_TBx.template
DB  += GBCPStat_TPDP_TBx.template
DB  += GBCPStat_TBx.sub

##0818, IntStat
DB  += GBIntStat_0INP_TBx.template
DB  += GBIntStat_1INP_TBx.template
DB  += GBIntStat_2INP_TBx.template
DB  += GBIntStat_TBx.sub



##0818, CURStat
DB  += GBCURStat_OVR_Fail.template
DB  += GBCURStat_Int_Fail_DCPC.template
DB  += GBCURStat_Int_Err.template
DB  += GBCURStat_TBx.sub

##0818, SoftPVs
DB  += SoftPVs_DCPC.template
DB  += SoftPVs_OVRStat.template
DB  += SoftPVs_FailStat.template
DB  += SoftPVs_TBx.sub


##HePAK DB
DB  += Density_HePAK.template
DB  += Enthalpy_HePAK.template
DB  += HeatLoad.template
DB  += HeatLoad_4INP.template
DB  += HeatLoad_5INP.template
DB  += TBxcontrol_hepak.sub


#----------------------------------------------------
# If <anyname>.db template is not named <anyname>*.template add
# <anyname>_template = <templatename>

include $(TOP)/configure/RULES
#----------------------------------------
#  ADD RULES AFTER THIS LINE

