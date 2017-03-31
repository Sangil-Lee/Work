importPackage(Packages.org.csstudio.opibuilder.scriptUtil);


var tr1 = PVUtil.getDouble(pvs[0]);
var tr2 = PVUtil.getDouble(pvs[1]);
var tr3 = PVUtil.getDouble(pvs[2]);
var tr4 = PVUtil.getDouble(pvs[3]);
var tr5 = PVUtil.getDouble(pvs[4]);


if(tr1 < 25 ) {
widget.setPropertyValue("text", "Tray1 needs to be filled.");
} else if(tr2 < 25 ) {
widget.setPropertyValue("text", "Tray2 needs to be filled.");
} else if(tr3 < 25 ) {
widget.setPropertyValue("text", "Tray3 needs to be filled.");
} else if(tr4 < 25 ) {
widget.setPropertyValue("text", "Tray4 needs to be filled.");
} else if(tr5 < 25 ) {
widget.setPropertyValue("text", "Tray5 needs to be filled.");
} else {
widget.setPropertyValue("text", " ");
}