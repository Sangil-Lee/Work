TOP=../..
include $(TOP)/configure/CONFIG
#----------------------------------------
#  ADD MACRO DEFINITIONS AFTER THIS LINE

#----------------------------------------------------
# Create and install (or just install) into <top>/db
# databases, templates, substitutions like this

###IF LTS Magnet DAQ PVs
DB += ifLTSMGDAQ_ai.template 
DB += ifLTSMGDAQ_waveform.template
DB += ifLTSMGDAQ_Register.reg 
DB += ifLTSMG_DAQmx.sub  

###Make FFTWaveform from cDAQ sampling data
#DB += fftWaveform.template
#DB += fftWaveform.sub



#----------------------------------------------------
# If <anyname>.db template is not named <anyname>*.template add
# <anyname>_template = <templatename>

include $(TOP)/configure/RULES
#----------------------------------------
#  ADD RULES AFTER THIS LINE

