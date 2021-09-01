importPackage(Packages.org.csstudio.opibuilder.scriptUtil);

var akp = display.getWidget("pkp");
var aki = display.getWidget("pki");
var akd = display.getWidget("pkd");
var selector;

selector = PVUtil.getString(pvs[0]);

if (selector == "PKP") {
	akp.setPropertyValue("visible", true);
	aki.setPropertyValue("visible", false);
	akd.setPropertyValue("visible", false);
} else if (selector == "PKI") {
	akp.setPropertyValue("visible", false);
	aki.setPropertyValue("visible", true);
	akd.setPropertyValue("visible", false);
} else {
	akp.setPropertyValue("visible", false);
	aki.setPropertyValue("visible", false);
	akd.setPropertyValue("visible", true);
}
