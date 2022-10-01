TOP=../..
include $(TOP)/configure/CONFIG
#----------------------------------------
#  ADD MACRO DEFINITIONS AFTER THIS LINE

#----------------------------------------------------
# Create and install (or just install) into <top>/db
# databases, templates, substitutions like this
#DB += xxx.db
#### PXI
DB += pxi_ao.template
DB += pxi_calcout.template
DB += pxi_ai.template
DB += pxi_ao.sub
#DB += pxi_long.db
#DB += pxi_longin_bitfanout.vdb
#DB += pxi_longout_bitfanout.vdb
DB += pxi_mbbi_bitfanout.vdb
DB += pxi_mbbo_bitfanout.vdb
DB += pxi_add.vdb

#### RTP
DB += rtp_mbbi_bitfanout.vdb
DB += rtp_mbbo_bitfanout.vdb
DB += rtp_bi_bit.template
DB += rtp_bo_bit.template
DB += rtp_aiFloat64.template
DB += biWord.template
DB += boWord.template
DB += rtp_modbus.sub

#----------------------------------------------------
# If <anyname>.db template is not named <anyname>*.template add
# <anyname>_template = <templatename>

include $(TOP)/configure/RULES
#----------------------------------------
#  ADD RULES AFTER THIS LINE

