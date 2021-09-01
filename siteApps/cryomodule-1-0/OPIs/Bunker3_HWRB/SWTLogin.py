from org.eclipse.jface.dialogs import MessageDialog
from org.eclipse.jface.window import Window
from SWTLoginDialog import *    
        
    
dialog = LoginDialog(Display.getCurrent().getActiveShell())
a = dialog.open() 
if a == Window.OK:
    info = dialog.getLoginInfo()
    if info[0] == "cm" and info[1]=="cm":
        display.getWidget("VC").setPropertyValue("enabled",True)
        display.getWidget("FCD").setPropertyValue("enabled",True)
        display.getWidget("HC").setPropertyValue("enabled",True)
        display.getWidget("HTC").setPropertyValue("enabled",True)
        display.getWidget("SVP").setPropertyValue("enabled",True)
             

    else:
        MessageDialog.openError(None, "Error", "The user name or password you input is wrong!")
