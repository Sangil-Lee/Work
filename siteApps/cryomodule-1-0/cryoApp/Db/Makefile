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
#DB += ls224.db
#DB += oda.db
#HWRA
#DB += temperature.db
#QWR
DB += temperature_qwr.db
#HWRA
#DB += heater.db
#QWR
DB += heater_qwr.db
#HWRA
DB += tpg256A.db
#QWR
DB += mks937b_qwr.db

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
DB += Helium_HeatLoad.vdb

DB += ai_SRF_Cooldown.template
DB += bi_SRF_Cooldown.template
DB += longin_SRF_Cooldown.template
DB += stringin_SRF_Cooldown.template
DB += calc_SRF_Cooldown.template
DB += Additional.sub
DB += PressR.template
DB += ValveR.template
DB += TempR.template
DB += calcout_ao_OM1.4_step6.template
DB += RefValue.vdb
DB += PhaseEnd.vdb

DB += calc_SRF_Cooldown_MFR.vdb
DB += SRF_HWR_Cooldown_MFR.template
DB += SRF_HWR_Cooldown.sub
DB += SRF_HWR_Cooldown_MFR.sub

#----------------------------------------------------
# If <anyname>.db template is not named <anyname>*.template add
# <anyname>_template = <templatename>

include $(TOP)/configure/RULES
#----------------------------------------
#  ADD RULES AFTER THIS LINE

