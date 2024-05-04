# Embedded python script
from org.csstudio.display.builder.runtime.script import PVUtil, ScriptUtil

#print PVUtil.getString(pvs[0]), NDPS-VAC:GV007:Enable
widget.setPropertyValue(str('Enabled').lower(), PVUtil.getString(pvs[0]))
