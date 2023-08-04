importPackage(Packages.org.csstudio.opibuilder.scriptUtil);
var linkContainer = display.getWidget("Link");
var w = display.getWidget("Link").getPropertyValue("width");

if (w == 1720){
	linkContainer.setPropertyValue("width", 1920);
	display.getWidget("CTRL_CONT").setPropertyValue("visible", false);
	display.getWidget("CTRL_CONT").setPropertyValue("opi_file", "");
}
linkContainer.setPropertyValue("opi_file", "");
linkContainer.setPropertyValue("opi_file", widget.getPropertyValue("name")+".opi");