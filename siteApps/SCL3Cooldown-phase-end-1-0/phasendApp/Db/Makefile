TOP=../..
include $(TOP)/configure/CONFIG
#----------------------------------------
#  ADD MACRO DEFINITIONS AFTER THIS LINE

#----------------------------------------------------
# Create and install (or just install) into <top>/db
# databases, templates, substitutions like this
#DB += xxx.db
DB += aiFloat64_sim.template
DB += phaseEndCond.vdb
DB += aiOutLinkTest.vdb
DB += sncTempWF.vdb
DB += LRsncTempWF.vdb
DB += sncTempEvalWF.vdb
DB += sncPressEvalWF.vdb
DB += PhaseEnd.vdb

DB += sncTempWF.template
DB += sncTempWF.sub

DB += sncTempEvalWF.template
DB += sncTempEvalWF.sub

DB += sncPressEvalWF.template
DB += sncPressEvalWF.sub

DB += TempEval_sim.template
DB += scl3_seq_simulate_temp.sub

DB += PressEval_sim.template
DB += scl3_seq_simulate_press.sub

DB += PressEvalOthers_sim.template
DB += scl3_seq_simulate_press_others.sub

#DB += scl3_seq_simulate_press.sub
#DB += scl3_seq_simulate.sub
#DB += phaseEndCond_TempMon.vdb

#DB += Test.vdb

#----------------------------------------------------
# If <anyname>.db template is not named <anyname>*.template add
# <anyname>_template = <templatename>

include $(TOP)/configure/RULES
#----------------------------------------
#  ADD RULES AFTER THIS LINE

