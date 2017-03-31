#!/bin/sh
# Shell  : an+ioc_launch.sh
# Author : Jeong Han Lee
# email  : jhlee@ibs.re.kr
# Date   : Tuesday, July 14 12:49:22 KST 2015
# version : 0.0.1
#
#
# In /etc/rc.local, this script is called via
# 
# sh '/home/rfquser/epics/R3.14.12.5/siteApps/bin/linux-x86_64/an_ioc_launch.sh' 'xgs600'
#
# 

#
# 1. check "user", and if the following is incorrect,
#    replace it with the correct one : "whoami" will return
#    the correct user name
# 2. who does execute this script, if this script is executed by
#    root, whoami doesn't return what we expect. 
#
#

#USER_NAME=$(eval whoami)
#. /home/${USER_NAME}/epics/R3.14.12.5/setEpicsEnv.sh 
. /home/rfquser/epics/R3.14.12.5/setEpicsEnv.sh 

ioc_name="$1"


case "$ioc_name" in
    "")
	echo "">&2
        echo "usage: $0 <IOC name>" >&2
        echo >&2
        echo "  IOC name : example " >&2
	echo ""
        echo "       $0 keithley6514 ">&2
        echo "       $0 snmp2 ">&2
        echo ""
        echo >&2
	exit 0
        ;;
    *)

## The following conditions is used for only developing stage, so comment out
#
	CHECKPLAT=$(eval screen -ls |grep ${ioc_name})

	if [ -z "$CHECKPLAT" ] ; then
	    case `uname -sm` in
		"Linux i386" | "Linux i486" | "Linux i586" | "Linux i686")
		    EPICS_HOST_ARCH=linux-x86
		    ;;
		"Linux x86_64")
		    EPICS_HOST_ARCH=linux-x86_64
		    ;;
		"Linux armv6l")
		    EPICS_HOST_ARCH=linux-arm
		    ;;
		"Linux armv7l")
		    EPICS_HOST_ARCH=linux-arm
		    ;;
		*)
		    echo "This script  doesn't support this architecture : `uname -sm`"
		    exit 1
		    ;;
	    esac
	    

	    /usr/bin/screen -dm -S ${ioc_name} -c ${RAON_SITEAPPS}/bin/ioc_screenrc -t ${ioc_name} ${RAON_SITEAPPS}/${ioc_name}/bin/${EPICS_HOST_ARCH}/${ioc_name} ${RAON_SITEAPPS}/${ioc_name}/iocBoot/ioc${ioc_name}/st.cmd	    
	else
## If everything is fine, we cannot see the exec possibility of this below line command :
	    screen -x ${ioc_name}
	fi
	
	;;
esac

exit 0
