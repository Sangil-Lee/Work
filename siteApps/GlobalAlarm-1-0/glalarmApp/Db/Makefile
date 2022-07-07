TOP=../..
include $(TOP)/configure/CONFIG
#----------------------------------------
#  ADD MACRO DEFINITIONS AFTER THIS LINE

#----------------------------------------------------
# Create and install (or just install) into <top>/db
# databases, templates, substitutions like this
#DB += xxx.db
#DB += globalAlarm.vdb
#DB += globalAlarm_test.vdb
#DB += gl_Interlock_XV7502_test.vdb
#DB += gl_Interlock_XV7301_test.vdb
DB += gl_Interlock_XV7502.vdb
DB += gl_Interlock_XV7301.vdb
DB += bo.template
DB += setmanuopen.sub
DB += bi_manuint.template
DB += xv7502_xv7301_manual_int.sub
DB += gl_Interlock_EBx.template
DB += gl_Interlock_EBx.sub


####Simulation
#DB += bi.template
#DB += bi_flnk.template
#DB += ai.template
#DB += sim_pv.sub
#DB += sim_pv_flnk.sub

#----------------------------------------------------
# If <anyname>.db template is not named <anyname>*.template add
# <anyname>_template = <templatename>

include $(TOP)/configure/RULES
#----------------------------------------
#  ADD RULES AFTER THIS LINE

