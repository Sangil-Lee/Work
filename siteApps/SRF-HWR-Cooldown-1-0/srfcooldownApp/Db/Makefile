TOP=../..
include $(TOP)/configure/CONFIG
#----------------------------------------
#  ADD MACRO DEFINITIONS AFTER THIS LINE

#----------------------------------------------------
# Create and install (or just install) into <top>/db
# databases, templates, substitutions like this
#DB += xxx.db
DB += ai.template
DB += ao.template
DB += bi.template
DB += longin.template
DB += stringin.template
DB += calc.template
DB += Additional.sub
DB += PressR.template
DB += TempR.template
DB += calcout_ao_OM1.4_step6.template
DB += cooldownlogic_eval_OM1.4_step6_SRF.sub
DB += cooldownlogic_eval_OM1.4_step7_SRF.sub
DB += RefValue.vdb
DB += SRF_HWR_Cooldown.sub
DB += PhaseEnd.vdb
DB += IntLockFanout.vdb

#----------------------------------------------------
# If <anyname>.db template is not named <anyname>*.template add
# <anyname>_template = <templatename>

include $(TOP)/configure/RULES
#----------------------------------------
#  ADD RULES AFTER THIS LINE

