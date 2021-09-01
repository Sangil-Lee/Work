importPackage(Packages.org.csstudio.opibuilder.scriptUtil);


var linkGraph = display.getWidget("LinkValue");
var macroInput = DataUtil.createMacrosInput(true);

//Put a macro in the new Macro Input
macroInput.put("ch", PVUtil.getString(pvArray[0]));
linkGraph.setPropertyValue("macros", macroInput);
linkGraph.setPropertyValue("opi_file", linkGraph.getPropertyValue("opi_file"), true);
