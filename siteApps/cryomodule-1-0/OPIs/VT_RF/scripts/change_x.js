importPackage(Packages.org.csstudio.opibuilder.scriptUtil); 
importPackage(Packages.org.csstudio.opibuilder.scriptUtil); 
importPackage(Packages.org.eclipse.jface.dialogs);
importPackage(Packages.org.csstudio.platform.data);
importPackage(Packages.java.lang);

var pvStr0 = PVUtil.getString(pvs[0]);

importPackage(Packages.org.csstudio.opibuilder.scriptUtil); 
if(pvStr0=="Pit1") {
	display.getWidget("XY").setPropertyValue("trace_0_x_pv","SRF01-VT:Cavity-Pres:2KLowValPit1");
	display.getWidget("XY").setPropertyValue("title","Pit1");
}
else if(pvStr0=="Pit2") {
	display.getWidget("XY").setPropertyValue("trace_0_x_pv","SRF01-VT:Cavity-Pres:2KLowValPit2");
	display.getWidget("XY").setPropertyValue("title","Pit2");
}
else if(pvStr0=="Pit3") {
	display.getWidget("XY").setPropertyValue("trace_0_x_pv","SRF01-VT:Cavity-Pres:2KLowValPit3");
	display.getWidget("XY").setPropertyValue("title","Pit3");
}
else {
	display.getWidget("XY").setPropertyValue("trace_0_x_pv","SRF01-VT:Cavity-Pres:2KLowValPit2");
	display.getWidget("XY").setPropertyValue("title","Pit2");
}
	
