importPackage(Packages.org.csstudio.opibuilder.scriptUtil);

var ip1  = display.getWidget("ip1");
var ip2  = display.getWidget("ip2");
var ip3  = display.getWidget("ip3");
var ip4  = display.getWidget("ip4");

var sub1 = display.getWidget("sub1");
var sub2 = display.getWidget("sub2");
var sub3 = display.getWidget("sub3");
var sub4 = display.getWidget("sub4");

var gate1 = display.getWidget("gate1");
var gate2 = display.getWidget("gate2");
var gate3 = display.getWidget("gate3");
var gate4 = display.getWidget("gate4");

var ioc1 = display.getWidget("ioc1");
var ioc2 = display.getWidget("ioc2");
var ioc3 = display.getWidget("ioc3");
var ioc4 = display.getWidget("ioc4");

var ipArray  = new Array();
var subArray = new Array();
var gateArray = new Array();
var iocArray = new Array();

var ipdata = PVUtil.getString(pvArray[0]);
var subdata = PVUtil.getString(pvArray[1]);
var gatedata = PVUtil.getString(pvArray[2]);
var iocdata = PVUtil.getString(pvArray[3]);

ipArray = ipdata.split('\\.');
subArray = subdata.split('\\.');
gateArray = gatedata.split('\\.');
iocArray = iocdata.split('\\.');

ip1.setPropertyValue("text",ipArray[0]);
ip2.setPropertyValue("text",ipArray[1]);
ip3.setPropertyValue("text",ipArray[2]);
ip4.setPropertyValue("text",ipArray[3]);

sub1.setPropertyValue("text",subArray[0]);
sub2.setPropertyValue("text",subArray[1]);
sub3.setPropertyValue("text",subArray[2]);
sub4.setPropertyValue("text",subArray[3]);

gate1.setPropertyValue("text",gateArray[0]);
gate2.setPropertyValue("text",gateArray[1]);
gate3.setPropertyValue("text",gateArray[2]);
gate4.setPropertyValue("text",gateArray[3]);

ioc1.setPropertyValue("text",iocArray[0]);
ioc2.setPropertyValue("text",iocArray[1]);
ioc3.setPropertyValue("text",iocArray[2]);
ioc4.setPropertyValue("text",iocArray[3]);