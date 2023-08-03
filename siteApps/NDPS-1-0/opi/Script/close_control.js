importPackage(Packages.org.csstudio.opibuilder.scriptUtil);
var linkContainer = display.getWidget("CTRL_CONT");
var w = display.getWidget("Link").getPropertyValue("width");

if(w == 1720){
	display.getWidget("Link").setPropertyValue("width", 1920);
}
linkContainer.setPropertyValue("visible", false);
linkContainer.setPropertyValue("macros", null);
linkContainer.setPropertyValue("opi_file", "");