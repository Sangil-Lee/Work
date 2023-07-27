TOP=../..
include $(TOP)/configure/CONFIG
#----------------------------------------
#  ADD MACRO DEFINITIONS AFTER THIS LINE

#----------------------------------------------------
# Create and install (or just install) into <top>/db
# databases, templates, substitutions like this
DB += aiFloat64.template  
DB += aoFloat64.template  
DB += biWord.template  
DB += boWord.template
DB += IFTarget.sub

## Flowmeter data processing
DB += longin.template
DB += flowMeter.sub
DB += DataFanout.vdb

DB += Phytron_I1AM01.db
DB += Phytron_MCM01.db
DB += Phytron_motor.db
DB += phytronMotor.sub

#----------------------------------------------------
# If <anyname>.db template is not named <anyname>*.template add
# <anyname>_template = <templatename>

include $(TOP)/configure/RULES
#----------------------------------------
#  ADD RULES AFTER THIS LINE

