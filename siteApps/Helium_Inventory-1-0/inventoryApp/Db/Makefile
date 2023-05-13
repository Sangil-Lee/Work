TOP=../..
include $(TOP)/configure/CONFIG
#----------------------------------------
#  ADD MACRO DEFINITIONS AFTER THIS LINE

#----------------------------------------------------
# Create and install (or just install) into <top>/db
# databases, templates, substitutions like this
#DB += xxx.db
DB += Volume_QWR_4K.template
DB += Volume_HWRA_4K.template
DB += Volume_HWRB_4K.template
DB += Volume_HWRA_2K.template
DB += Volume_HWRB_2K.template
DB += Volume_TBx_Line.template
DB += Reservoir.sub
DB += TBx_Line.sub

#----------------------------------------------------
# If <anyname>.db template is not named <anyname>*.template add
# <anyname>_template = <templatename>

include $(TOP)/configure/RULES
#----------------------------------------
#  ADD RULES AFTER THIS LINE

