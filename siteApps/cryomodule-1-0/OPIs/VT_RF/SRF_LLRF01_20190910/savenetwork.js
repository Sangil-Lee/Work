importPackage(Packages.org.csstudio.opibuilder.scriptUtil);
importPackage(Packages.org.eclipse.jface.dialogs);

var ip1  = display.getWidget("ip1");
var ip2  = display.getWidget("ip2");
var ip3  = display.getWidget("ip3");
var ip4  = display.getWidget("ip4");

var sub1  = display.getWidget("sub1");
var sub2  = display.getWidget("sub2");
var sub3  = display.getWidget("sub3");
var sub4  = display.getWidget("sub4");

var gate1  = display.getWidget("gate1");
var gate2  = display.getWidget("gate2");
var gate3  = display.getWidget("gate3");
var gate4  = display.getWidget("gate4");

var ioc1 =  display.getWidget("ioc1");
var ioc2 =  display.getWidget("ioc2");
var ioc3 =  display.getWidget("ioc3");
var ioc4 =  display.getWidget("ioc4");

var saveip = display.getWidget("saveip");
var savesub = display.getWidget("savesub");
var savegate = display.getWidget("savegate");
var saveioc = display.getWidget("saveioc");

var ipstr = "";
var substr = "";
var gatestr = "";
var iocstr = "";
 
ipstr = ipstr.concat(ip1.getPropertyValue("text") + "." + ip2.getPropertyValue("text") + "." + ip3.getPropertyValue("text") + "." + ip4.getPropertyValue("text"));
substr = substr.concat(sub1.getPropertyValue("text") + "." + sub2.getPropertyValue("text") + "." + sub3.getPropertyValue("text") + "." + sub4.getPropertyValue("text"));
gatestr = gatestr.concat(gate1.getPropertyValue("text") + "." + gate2.getPropertyValue("text") + "." + gate3.getPropertyValue("text") + "." + gate4.getPropertyValue("text"));
iocstr = iocstr.concat(ioc1.getPropertyValue("text") + "." + ioc2.getPropertyValue("text") + "." + ioc3.getPropertyValue("text") + "." + ioc4.getPropertyValue("text"));

saveip.setPropertyValue("text",ipstr);
savesub.setPropertyValue("text",substr);
savegate.setPropertyValue("text",gatestr);
saveioc.setPropertyValue("text",iocstr);

var req = MessageDialog.openQuestion(null, "Dialog from JavaScript", "System restart??");
if(req == true)
{
	
	saveip.executeAction(0);
	savesub.executeAction(0);
	savegate.executeAction(0);
	saveioc.executeAction(0);
}