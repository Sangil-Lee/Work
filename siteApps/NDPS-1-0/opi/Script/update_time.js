importPackage(Packages.org.csstudio.opibuilder.scriptUtil);

var str = PVUtil.getString(pvs[0]);
//ConsoleUtil.writeInfo(str);
widget.setPropertyValue("text", str.substr(0, str.indexOf('.')));