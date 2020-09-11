#!/bin/sh
#upload Date : 2020-06-03
#export SERVER_USER="jong"
#export PASSWORD="jong123"
export SERVER_USER="jung"
export PASSWORD="jung"
export SERVER_IP="192.168.3.200"
export BROADCAST_IP="192.168.3.255"
export SERVER_PATH="/home/${SERVER_USER}"
export SERVER_EPICSROOT="${SERVER_PATH}/epics"

export ipAddr=192.168.3.101
export macAddr=00:0A:35:00:1E:01
export BID_value=MN01
export LINK=1
export NODE=1
export TARGET=01		# using IOC
export BoardType=MN		# using .sd_script.sh

#Import Standard Library
# cp -rf /mnt/sdcard/lib/* /lib

##Setup Script Mode & Boot Mode
export mode="nfs"	## mode : sd , nfs
export args="$1"	## args : start, upEpics , upCert

if [ ${mode} == nfs ];then
	export EPICSROOT="/mnt/nfs/epics"
else
	export EPICSROOT="/mnt/sdcard/epics"
fi

export PATH=$PATH:/usr/lib:/lib
export PATH=$PATH:/usr/bin:/bin:/usr/sbin:/sbin
export PATH=$PATH:${EPICSROOT}/R3.14.12.5/base/bin/linux-arm:${EPICSROOT}/R3.14.12.5/siteApps/Ctrl_IOC/fps-1-0/bin/linux-arm
export LD_LIBRARY_PATH=$LD_LIBRARY_PATH:${EPICSROOT}/R3.14.12.5/base/lib/linux-arm
export LD_LIBRARY_PATH=$LD_LIBRARY_PATH:${EPICSROOT}/R3.14.12.5/epicsLibs/synApps/support/asyn-4-26/lib/linux-arm
export LD_LIBRARY_PATH=$LD_LIBRARY_PATH:${EPICSROOT}/R3.14.12.5/siteApps/Ctrl_IOC/fps-1-0/lib/linux-arm
export LD_LIBRARY_PATH=$LD_LIBRARY_PATH:${EPICSROOT}/R3.14.12.5/siteLibs/lib/linux-arm
export EPICSAPPS=${EPICSROOT}/R3.14.12.5/siteApps/Ctrl_IOC/fps-1-0

## NETWORK SETTING
echo "----------------------------------------------------------------"
echo "[SETUP INFORMATION]"
ifconfig >> /dev/null
ifconfig eth0 down
ifconfig eth0 hw ether ${macAddr}
ifconfig eth0 broadcast ${BROADCAST_IP}
ifconfig eth0 ${ipAddr} up

echo "Setting IP Address        ---> ${ipAddr}"
echo "Booting Mode              ---> ${mode}"
echo "Server IP                 ---> ${SERVER_IP}"
echo "Server User Name          ---> ${SERVER_USER}"
echo "Server Directory          ---> ${SERVER_PATH}"
echo "Mount EPICS Path          ---> ${EPICSROOT}"
echo "NFS Server EPICS Path     ---> ${SERVER_EPICSROOT}"
echo "Dev Password              ---> #######"
TR_BID_value=`echo ${BID_value} | tr '[a-z]' '[A-Z]'`
echo "Read Board ID             ---> ${TR_BID_value}"

# Renewal Board Time
echo "----------------------------------------------------------------"
echo "			Set Board Time..."
/etc/init.d/ntpd stop #ntpd stop
ntpdate ${SERVER_IP}
/etc/init.d/ntpd start #ntpd start 
date
echo "----------------------------------------------------------------"

#sed -i 's/export mode="nfs"/export mode="sd"/g' /home/root/sh/InfoDev.sh

#sed -i 's/args=upIOC/#args=upIOC/g' /home/root/sh/InfoDev.sh
#args=upIOC

# sed -i 's/args=upAll/#args=upAll/g' /home/root/sh/InfoDev.sh
# args=upAll

if [ x${args} == x ];then
		source /mnt/sdcard/script/./.sd_script.sh init_start
else
		source /mnt/sdcard/script/./.sd_script.sh ${args}
fi
