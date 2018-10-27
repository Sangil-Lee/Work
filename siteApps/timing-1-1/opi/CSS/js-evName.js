importPackage(Packages.org.csstudio.opibuilder.scriptUtil);
var pv0 = PVUtil.getDouble(pvs[0]);
if(pv0==1)
	widget.setPropertyValue("text","evG");
else if(pv0==2)
	widget.setPropertyValue("text","evR");
else if(pv0==3)
	widget.setPropertyValue("text","evF");
else if(pv0==4)
	widget.setPropertyValue("text","evS");
else if(pv0==5)
	widget.setPropertyValue("text","evR-FP");
else
	widget.setPropertyValue("text","Error");
