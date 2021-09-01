importPackage(Packages.org.csstudio.opibuilder.scriptUtil);

var rawGraph = display.getWidget("XY Graph_raw");
var ampGraph = display.getWidget("XY Graph_amplitude");
var phaseGraph = display.getWidget("XY Graph_phase");
var ipv;
var pvName;
var graphType;

graphType = PVUtil.getString(pvs[0]);

if (graphType == "RAW") {
	rawGraph.setPropertyValue("visible", true);
	ampGraph.setPropertyValue("visible", false);
	phaseGraph.setPropertyValue("visible", false);
} else if (graphType == "AMP") {
	rawGraph.setPropertyValue("visible", false);
	ampGraph.setPropertyValue("visible", true);
	phaseGraph.setPropertyValue("visible", false);
} else {
	rawGraph.setPropertyValue("visible", false);
	ampGraph.setPropertyValue("visible", false);
	phaseGraph.setPropertyValue("visible", true);
}
