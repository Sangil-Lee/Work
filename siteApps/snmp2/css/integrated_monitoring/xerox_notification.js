importPackage(Packages.org.csstudio.opibuilder.scriptUtil);

var tonerB = PVUtil.getDouble(pvs[0]);
var tonerY = PVUtil.getDouble(pvs[1]);
var tonerM = PVUtil.getDouble(pvs[2]);
var tonerC = PVUtil.getDouble(pvs[3]);


if(tonerY < 25 ) {
widget.setPropertyValue("text", "Yellow Cartridge needs to be replaced soon.");
} else if(tonerM < 25 ) {
widget.setPropertyValue("text", "Magenta Cartridge needs to be replaced soon.");
} else if(tonerC < 25 ) {
widget.setPropertyValue("text", "Cyan Cartridge needs to be replaced soon.");
} else if(tonerB < 25 ) {
widget.setPropertyValue("text", "Black Cartridge needs to be replaced soon.");
} else {
widget.setPropertyValue("text", " ");
}