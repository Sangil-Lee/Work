TOP=../..
include $(TOP)/configure/CONFIG
#----------------------------------------
#  ADD MACRO DEFINITIONS AFTER THIS LINE

#----------------------------------------------------
# Create and install (or just install) into <top>/db
# databases, templates, substitutions like this
#DB += xxx.db

## RTP Protocol Test
#DB += RTPTest.vdb
#DB += RTPOneTest.vdb

## RTP Protocol DB
DB += RTP_AI.template
DB += cdmsTester_RTP.sub

## RTP ModbusTCP DB
#DB += aiFloat64.template biWord.template
#DB += aoFloat64.template boWord.template
#DB += cdmsTester_Modbus.sub

## NI-PXI DB

## Soft PVs
DB += CDMSTester.db
#----------------------------------------------------
# If <anyname>.db template is not named <anyname>*.template add
# <anyname>_template = <templatename>

include $(TOP)/configure/RULES
#----------------------------------------
#  ADD RULES AFTER THIS LINE

