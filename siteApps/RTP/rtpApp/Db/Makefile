TOP=../..
include $(TOP)/configure/CONFIG
#----------------------------------------
#  ADD MACRO DEFINITIONS AFTER THIS LINE

#----------------------------------------------------
# Create and install (or just install) into <top>/db
# databases, templates, substitutions like this
# DB += xxx.db
DB += RTPTest.vdb
DB += RTPFloat.vdb
DB += RTPMulti.vdb
DB += RTPWaveform.db
DB += RTPAI.template
DB += RTPLI.template
DB += RTPSignal.sub
DB += RTPSignal2.sub
DB += RTPWrite.db
DB += PythonDB.db


#----------------------------------------------------
# If <anyname>.db template is not named <anyname>*.template add
# <anyname>_template = <templatename>

include $(TOP)/configure/RULES
#----------------------------------------
#  ADD RULES AFTER THIS LINE

