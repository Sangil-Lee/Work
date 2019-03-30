#!/bin/sh
# Shell  : ioc_launch.sh
# Author : Jeong Han Lee
# email  : jhlee@ibs.re.kr
# Date   : Thursday, June  4 19:50:01 KST 2015
# version : 0.0.1
#
#
# In /etc/rc.local, this script is called via
# 
# sh '/home/pi/epics/R3.14.12.5/siteApps/bin/linux-arm/ioc_launch.sh'
#
# 

ioc_name="epics"

. /home/pi/epics/R3.14.12.5/setEpicsEnv.sh 

## The following conditions is used for only developing stage, so comment out
#
CHECKPLAT=$(eval screen -ls |grep ${ioc_name})
if [ -z "$CHECKPLAT" ] ; then
    /usr/bin/screen -dm -S ${ioc_name} -c ${RAON_SITEAPPS}/bin/ioc_screenrc -t ${ioc_name} ${RAON_SITEAPPS}/raspberry/bin/linux-arm/md5dh14 ${RAON_SITEAPPS}/raspberry/iocBoot/iocRaspberryPi/st.cmd.md5dh14.boot
else
## If everything is fine, we cannot see the exec possibility of this below line command :
    screen -x ${ioc_name}
fi

exit 0
