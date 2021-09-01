importPackage(Packages.org.csstudio.opibuilder.scriptUtil);

var akp = display.getWidget("akp");
var aki = display.getWidget("aki");
var akd = display.getWidget("akd");
var selector;

selector = PVUtil.getString(pvs[0]);

if (selector == "AKP") {
	akp.setPropertyValue("visible", true);
	aki.setPropertyValue("visible", false);
	akd.setPropertyValue("visible", false);
} else if (selector == "AKI") {
	akp.setPropertyValue("visible", false);
	aki.setPropertyValue("visible", true);
	akd.setPropertyValue("visible", false);
} else {
	akp.setPropertyValue("visible", false);
	aki.setPropertyValue("visible", false);
	akd.setPropertyValue("visible", true);
}
