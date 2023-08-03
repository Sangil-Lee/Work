importPackage(Packages.org.csstudio.opibuilder.scriptUtil);
var linkContainer = display.getWidget("CTRL_CONT");
var macroInput = DataUtil.createMacrosInput(true);
var w = display.getWidget("Link").getPropertyValue("width");
var link_macros = widget.getPropertyValue("macros").toString();
var macroValue = widget.getMacroValue("OPI"); 
var macros = link_macros.substr(17, link_macros.length()-18);
var input_macros = "Parent Macros, "+macros;
var macroValue = widget.getMacroValue("OPI"); 
macroInput = macroInput.recoverFromString(input_macros);

if(linkContainer.getPropertyValue("visible") == true)
{
	linkContainer.setPropertyValue("macros", null);
	linkContainer.setPropertyValue("opi_file", "");
}

//macroInput.put("SUBDEV", widget.getPropertyValue("name"));
if(w == 1920){
	display.getWidget("Link").setPropertyValue("width",1720);
}
linkContainer.setPropertyValue("visible", true);
linkContainer.setPropertyValue("macros", macroInput);
linkContainer.setPropertyValue("opi_file", macroValue+".opi");
