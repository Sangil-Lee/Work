#!/bin/bash

echo $1

if [ "$#" -ne 1 ] ; then
	echo "$0 logfilename"
	exit 3
fi

source ${HOME}/epics/debian-11/7.0.7/setEpicsEnv.bash

#### ocLogServer in a host 
export EPICS_IOC_LOG_FILE_NAME=$1

echo "Started iocLogServer ==> "
iocLogServer


#############[iocClient]#############
## st.cmd
#epicsEnvSet("EPICS_IOC_LOG_INET","192.168.75.70")
#setIocLogDisable(0 or 1), 0=enable, 1=disable
#iocLogInit()
#####################
## iocLogServer message test
##7.0.7>iocLogShow
##log client: connected to log server at '192.168.75.70:7004', //Default EPICS_IOC_LOG_PORT(7004)
##7.0.7>errlog logtest
