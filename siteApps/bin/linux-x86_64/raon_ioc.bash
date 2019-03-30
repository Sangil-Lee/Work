#!/bin/bash
#
# Shell  : raon_ioc.bash
# Author : Jeong Han Lee
# email  : jhlee@ibs.re.kr
# Date   : Tuesday, March 31 09:45:04 KST 2015
# version : 0.0.1
#
#   * Requirements
#
#     RAON EPICS environmental script must be exectued before this script
#     In current envrionemt, for EPICS 3.14.12.5, execute the following
#
#     $ . ~/epics/R3.14.12.5/setEpicsEnv.sh
#     $ cd ${RAON_SITEAPPS}
#     # bash scripts/raon_ioc.bash "keithley6514" 
#    
#     It can start the IOC within the SCREEN session, 
#     -- if the same ioc screen is running, attach to it
#     -- if no ioc screen is running, create and attach to it
#
#     one can check the screen via 
#     $ screen -ls
#
#
#  - 0.0.1  Tuesday, March 31 09:45:25 KST 2015
#           * created
#

this_script_name=`basename $0`
LOGDATE=`date +%Y.%m.%d.%H:%M`
host_name=${HOSTNAME}

# This tells bash that it should exit the script if any statement returns a non-true return value. 
#set -e
#
# This will exit your script if you try to use an uninitialised variable
#set -u

# default_version="3.14.12.5"

goApps() {

    if [ -z ${RAON_SITEAPPS} ]; then
	echo "No EPICS environment variables are found. Force to set the default one."
	echo "~/epics/R3.14.12.5/setEpicsEnv.sh"
	. ~/epics/R3.14.12.5/setEpicsEnv.sh
    fi

    if [[ $# -eq 0 ]] ; then
	cd ${RAON_SITEAPPS}
    else
	cd ${RAON_SITEAPPS}
	cd $1
    fi
}

goIoc() {
    goApps $1
    cd iocBoot/ioc$1
}

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
	    
	    goIoc ${ioc_name}
	    screen -fn -S ${ioc_name} -c ${RAON_SITEAPPS}/bin/ioc_screenrc -t ${ioc_name} ${RAON_SITEAPPS}/${ioc_name}/bin/${EPICS_HOST_ARCH}/${ioc_name} st.cmd
	    
	else
	    echo "${ioc_name} IOC is running o $HOSTNAME, attaching ...."
	    screen -x $ioc_name
	fi

	;;

esac




exit 0
