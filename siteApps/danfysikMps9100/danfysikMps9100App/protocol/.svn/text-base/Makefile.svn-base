TOP = ../..

include $(TOP)/config/CONFIG_APP

buildInstall:: createLink

include $(TOP)/config/RULES_ARCHS

createLink:
	@if [ ! -d $(TOP)/data ]; then ln -s `pwd` $(TOP)/data; fi
