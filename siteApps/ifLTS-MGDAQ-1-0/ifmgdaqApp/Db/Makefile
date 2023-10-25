TOP=../..
include $(TOP)/configure/CONFIG
#----------------------------------------
#  ADD MACRO DEFINITIONS AFTER THIS LINE

#----------------------------------------------------
# Create and install (or just install) into <top>/db
# databases, templates, substitutions like this
#DB += xxx.db

###NI-cDAQ PVs
DB += nicDAQ_ai.template  
DB += nicDAQ_waveform.template
DB += nicDAQmx.sub
DB += NIcDAQRegister.reg  

###Make FFTWaveform from cDAQ sampling data
DB += fftWaveform.template
DB += fftWaveform.sub


#----------------------------------------------------
# If <anyname>.db template is not named <anyname>*.template add
# <anyname>_template = <templatename>

include $(TOP)/configure/RULES
#----------------------------------------
#  ADD RULES AFTER THIS LINE

