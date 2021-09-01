importPackage(Packages.org.csstudio.opibuilder.scriptUtil);

var offsetSwitch = display.getWidget("offsetSwitch");
var refOffset = display.getWidget("REF_OFFSET");
var ch1Offset = display.getWidget("CH1_OFFSET");
var ch2Offset = display.getWidget("CH2_OFFSET");
var ch3Offset = display.getWidget("CH3_OFFSET");
var pacOffset = display.getWidget("PAC_OFFSET");
var refOffsetEnable = refOffset.getPropertyValue("enabled");
var backgroundEnable = ColorFontUtil.getColorFromRGB(230,230,230);
var backgroundDisable = ColorFontUtil.getColorFromRGB(210,210,210);
var r;

if (refOffsetEnable == true) {
	refOffset.setPropertyValue("enabled", false);
	ch1Offset.setPropertyValue("enabled", false);
	ch2Offset.setPropertyValue("enabled", false);
	ch3Offset.setPropertyValue("enabled", false);
	pacOffset.setPropertyValue("enabled", false);
	
	refOffset.setPropertyValue("background_color", backgroundDisable);
	ch1Offset.setPropertyValue("background_color", backgroundDisable);
	ch2Offset.setPropertyValue("background_color", backgroundDisable);
	ch3Offset.setPropertyValue("background_color", backgroundDisable);
	pacOffset.setPropertyValue("background_color", backgroundDisable);
} else {
	r  = GUIUtil.openPasswordDialog("Please input password ", "1234");
	if (r == 1) {
		refOffset.setPropertyValue("enabled", true);
		ch1Offset.setPropertyValue("enabled", true);
		ch2Offset.setPropertyValue("enabled", true);
		ch3Offset.setPropertyValue("enabled", true);
		pacOffset.setPropertyValue("enabled", true);		
		
		refOffset.setPropertyValue("background_color", backgroundEnable);
		ch1Offset.setPropertyValue("background_color", backgroundEnable);
		ch2Offset.setPropertyValue("background_color", backgroundEnable);
		ch3Offset.setPropertyValue("background_color", backgroundEnable);
		pacOffset.setPropertyValue("background_color", backgroundEnable);
	}
}
