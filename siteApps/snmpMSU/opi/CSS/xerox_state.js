importPackage(Packages.org.csstudio.opibuilder.scriptUtil);
importPackage(Packages.org.apache.commons.lang.StringUtils);

var pv1 = PVUtil.getString(pvs[0]);
var pv2 = PVUtil.getString(pvs[1]);

var print = pv1.trim();
var state = pv2.trim();

if (print == "onRequest(2)") {
widget.setPropertyValue("on_label","Check printer");
widget.setPropertyValue("on_color","Minor");
widget.setPropertyValue("font",ColorFontUtil.getFont("Cantarell",28,1));

} else if (print == "unavailable(3)") {
widget.setPropertyValue("on_label","Printer unavailable");
widget.setPropertyValue("font",ColorFontUtil.getFont("Cantarell",20,1));
widget.setPropertyValue("on_color","Minor");

} else if (print == "initializing(6)") {
widget.setPropertyValue("on_label","Printer initializing");
widget.setPropertyValue("font",ColorFontUtil.getFont("Cantarell",28,1));
widget.setPropertyValue("on_color","Invalid");

} else if (print == "terminating(7)") {
widget.setPropertyValue("on_label","Printer terminating");
widget.setPropertyValue("font",ColorFontUtil.getFont("Cantarell",20,1));
widget.setPropertyValue("on_color","Invalid");

} else if(print == "ready(1)") {
widget.setPropertyValue("on_label","Ready");
widget.setPropertyValue("on_color",ColorFontUtil.getColorFromRGB(0,255,0));

} else if (state == "Copying...") {
widget.setPropertyValue("on_label","Copying...");
widget.setPropertyValue("font",ColorFontUtil.getFont("Cantarell",28,1));
widget.setPropertyValue("on_color",ColorFontUtil.getColorFromRGB(30,144,255));

} else if (state == "Copy-User Action") {
widget.setPropertyValue("on_label","Copying...");
widget.setPropertyValue("font",ColorFontUtil.getFont("Cantarell",28,1));
widget.setPropertyValue("on_color",ColorFontUtil.getColorFromRGB(30,144,255));

} else if (print == "busy(5)") {
widget.setPropertyValue("on_label","Printing...");
widget.setPropertyValue("on_color",ColorFontUtil.getColorFromRGB(30,144,255));

} else if (state == "Scan-User Action") {
widget.setPropertyValue("on_label","Scanning...");
widget.setPropertyValue("on_color",ColorFontUtil.getColorFromRGB(30,144,255));

} else {
widget.setPropertyValue("on_label","Check printer");
widget.setPropertyValue("font",ColorFontUtil.getFont("Cantarell",28,1));
widget.setPropertyValue("on_color",ColorFontUtil.getColorFromRGB(255,0,0));
}