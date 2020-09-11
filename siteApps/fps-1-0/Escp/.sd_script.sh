#!/bin/sh
#upload Date : 2020-06-03
ssh_cmd="ssh"
scp_cmd="scp"
# ssh_cmd="ssh -o StrictHostKeyChecking=no -o UserKnownHostsFile=/dev/null"
# scp_cmd="scp -o StrictHostKeyChecking=no -o UserKnownHostsFile=/dev/null"
if [ ${BoardType} = "AN" ];then
	iocBoot_dir=iocBoot/iocacq
	log_file=acq_log
else
	iocBoot_dir=iocBoot/iocmiti
	log_file=mit_log
fi

func_mount_NFS()
{
	echo "----------------------------------------------------------------"
	echo "			mount NFS(Server)"

	## ADD TO  '.ssh/known_hosts' FILE  [SDCARD -> RAMDISK]
	if [ -d "/home/root/.ssh" ];then
		echo "/home/root/.ssh directory exist"
	else
		echo "/home/root/.ssh directory not exist"
		mkdir /home/root/.ssh
	fi

	if [ -f "/mnt/sdcard/setup/known_hosts" ];then
		echo "/mnt/sdcard/setup/known_hosts file exist"
		cp -rf /mnt/sdcard/setup/known_hosts /home/root/.ssh
	else
		echo "/mnt/sdcard/setup/known_hosts file not exist"
	fi

	if [ ${mode} = "nfs" ];then
		if mountpoint -q "${EPICSROOT}"; then
			echo "Server(NFS) is mounted"
		else
			echo "Server(NFS) is not mounted"
			## NFS CONNECTION EPICS SERVER
			mkdir -p ${EPICSROOT}
			#chmod -R 777 ${EPICSROOT}
			mount -o nolock ${SERVER_IP}:${SERVER_EPICSROOT} ${EPICSROOT}
			echo "Server(NFS) is mounted"
		fi
	fi
	echo "----------------------------------------------------------------"
}

start ()
{
	func_mount_NFS

	echo "----------------------------------------------------------------"
	echo "			start_function"

	## SET BOARD ID
	echo "Set Board ID              ---> ${TR_BID_value}"
	sed -i 's/'${BoardType}'[0-9][0-9]/'${TR_BID_value}'/g' ${EPICSAPPS}/${iocBoot_dir}/acqDB.sub
	sed -i 's/'${BoardType}'[0-9][0-9]/'${TR_BID_value}'/g' ${EPICSAPPS}/${iocBoot_dir}/st.cmd
	if [ ${BoardType} = "AN" ];then
		sed -i 's/LINK=[0-9]/LINK='${LINK}'/g' ${EPICSAPPS}/${iocBoot_dir}/st.cmd
		sed -i 's/NODE=[0-9]/NODE='${NODE}'/g' ${EPICSAPPS}/${iocBoot_dir}/st.cmd
	fi

	echo "----------------------------------------------------------------"
	
	## START APPLICATION 
	cd ${EPICSAPPS}/${iocBoot_dir}
	upLog "EPICS Start"
	./st.cmd
	cd -

	# ${EPICSAPPS}/bin/linux-arm/acq ${EPICSAPPS}/${iocBoot_dir}/st.cmd
}

upCert ()
{
	## remove old certification
	rm -rf /mnt/sdcard/setup/${BoardType}[0-9][0-9]*
	rm -rf /home/root/.ssh/${BoardType}[0-9][0-9]*

	##make certification
	# cd /mnt/sdcard/setup    #excuting the command from this location will result in an error.
	if [ -d "/home/root/.ssh" ];then
		echo "/home/root/.ssh directory exist"
	else
		echo "/home/root/.ssh directory not exist"
		mkdir /home/root/.ssh
	fi
	cd /home/root/.ssh

	upLog "Generate Certification Key"
	echo "Certification Key Name : ${TR_BID_value}"
	ssh-keygen -t rsa -f ${TR_BID_value} -P ""	# make .rsa  & .pub file
	# ssh-keygen -t rsa -f ${TR_BID_value}	# make .rsa  & .pub file
	# ssh-keygen -y -f ${TR_BID_value} | grep ^ssh-rsa > ${TR_BID_value}.pub
	# dropbearkey -t rsa -s 4096 -f ${TR_BID_value}
	# dropbearkey -y -f ${TR_BID_value} | grep ^ssh-rsa > ${TR_BID_value}.pub

	echo "copy certification ramdisk -> sdcard"
	cp -rf /home/root/.ssh/${BoardType}[0-9][0-9]* /mnt/sdcard/setup
	cp -rf /home/root/.ssh/known_hosts /mnt/sdcard/setup

	upLog "Send Certification Key"
	echo "		send certification to server"
	##expect shell
	rm -rf /home/root/.ssh/known_hosts
	expect <<EOFCERT
	spawn ${scp_cmd} ${TR_BID_value}.pub ${SERVER_USER}@${SERVER_IP}:${SERVER_PATH}/.ssh/PK
	expect {
		-re "No route" 	{ exit 1 }
		-re "try again" { exit 1 }
		-re "(y/n)" 	{ send "y\r"; exp_continue; }
		-re "y/n" 		{ send "y\r"; exp_continue; }
		-re "yes/no" 	{ send "yes\r"; exp_continue; }
		-re "password:" { send "${PASSWORD}\r"; exp_continue; }
		-re "100%" 		{interact;}
	}
	expect eof
EOFCERT
}

upIOC ()
{
	if [ ${mode} = "nfs" ];then
		func_mount_NFS

		rm -rf /mnt/sdcard/epics/R3.14.12.5/siteApps/Ctrl_IOC/fps-1-0/*

		echo "......Copy [siteApps/Ctrl_IOC/fps-1-0]"
		cp -rf /mnt/nfs/epics/R3.14.12.5/siteApps/Ctrl_IOC/fps-1-0 /mnt/sdcard/epics/R3.14.12.5/siteApps/Ctrl_IOC

		cd /mnt/sdcard/epics/R3.14.12.5/siteApps/Ctrl_IOC/fps-1-0/${iocBoot_dir}/
		sed -i 's/\/mnt\/nfs/\/mnt\/sdcard/g' envPaths
		cd -
	else
		echo "current mode [${mode}]... need to change [sd] mode into '/mnt/sdcard/script/InfoDev.sh'"
	fi
}

upEpics ()
{
	if [ ${mode} = "nfs" ];then
		func_mount_NFS
		
		# echo "Delete ... [/mnt/sdcard/epics]"
		# rm -rf /mnt/sdcard/epics

		# echo "Make Directory"
		# mkdir -p /mnt/sdcard/epics/R3.14.12.5/siteApps/Ctrl_IOC/

		# cd /mnt/sdcard/epics/R3.14.12.5

		# echo "......Copy [base]"
		# cp -rf /mnt/nfs/epics/R3.14.12.5/base /mnt/sdcard/epics/R3.14.12.5
		# echo "......Copy [epicsLibs]"
		# cp -rf /mnt/nfs/epics/R3.14.12.5/epicsLibs /mnt/sdcard/epics/R3.14.12.5
		# echo "......Copy [extensions]"
		# cp -rf /mnt/nfs/epics/R3.14.12.5/extensions /mnt/sdcard/epics/R3.14.12.5
		# echo "......Copy [siteApps/Ctrl_IOC/fps-1-0]"
		# cp -rf /mnt/nfs/epics/R3.14.12.5/siteApps/Ctrl_IOC/fps-1-0 /mnt/sdcard/epics/R3.14.12.5/siteApps/Ctrl_IOC
		# echo "......Copy [siteLibs]"
		# cp -rf /mnt/nfs/epics/R3.14.12.5/siteLibs /mnt/sdcard/epics/R3.14.12.5
		# echo "Success Update epics project"
		
		# FAT32 unfortunately does not support owner, permissions or symlinks.
		cd /mnt/sdcard/epics/R3.14.12.5/base/lib/linux-arm
		if [ -f "./*.so.3.14" ]; then
			mv libCom.so.3.14 libCom.so
			mv libasIoc.so.3.14 libasIoc.so
			mv libca.so.3.14 libca.so
			mv libcas.so.3.14 libcas.so
			mv libdbIoc.so.3.14 libdbIoc.so
			mv libdbStaticIoc.so.3.14 libdbStaticIoc.so
			mv libdbtoolsIoc.so.3.14 libdbtoolsIoc.so
			mv libgdd.so.3.14 libgdd.so
			mv libmiscIoc.so.3.14 libmiscIoc.so
			mv librecIoc.so.3.14 librecIoc.so
			mv libregistryIoc.so.3.14 libregistryIoc.so
			mv librsrvIoc.so.3.14 librsrvIoc.so
			mv libsoftDevIoc.so.3.14 libsoftDevIoc.so
			mv libtestDevIoc.so.3.14 libtestDevIoc.so
		else
			echo " " >> /dev/null
		fi
		cd -
		
		cd /mnt/sdcard/epics/R3.14.12.5/siteApps/Ctrl_IOC/fps-1-0/${iocBoot_dir}/
		sed -i 's/\/mnt\/nfs/\/mnt\/sdcard/g' envPaths
		cd -
	else
		echo "current mode [${mode}]... need to change [sd] mode into '/mnt/sdcard/script/InfoDev.sh'"
	fi
}

upLog()
{
	echo "----------------------------------------------------------------"
	echo "		Send Log : $1"
	Log_TIME=$(date '+%m-%d %T')
	rm -rf /home/root/.ssh/known_hosts
	expect <<EOFLOG
	# set timeout 2;
	spawn ${ssh_cmd} ${SERVER_USER}@${SERVER_IP} "/bin/./echo ${TR_BID_value} : ${Log_TIME} $1 >>/home/${SERVER_USER}/log/${log_file}"
	##################
	expect {
		-re "No route" 	{ exit 1 }
		-re "try again" { exit 1 }
		-re "(y/n)" 	{ send "y\r"; exp_continue; }
		-re "y/n" 		{ send "y\r"; exp_continue; }
		-re "yes/no" 	{ send "yes\r"; exp_continue; }
		-re "password:" { send "${PASSWORD}\r"; interact;}
		timeout 		{ exit 1}
	}
	expect eof
EOFLOG
	echo "----------------------------------------------------------------"
}

case "$1" in
	init_start)
		if [ -f "/mnt/sdcard/setup/${BID_value}.pub" ];then
			echo ".pub file exist"
			cp -rf  /mnt/sdcard/setup/${BID_value}.pub /home/root/.ssh/
		else
			echo ".pub file not exist"
			upCert;
		fi
		start;
		;;
	start)
		start;
		;;
	upCert)
		echo "################################################################"
		echo "			upCert Start..." 
		upCert; 
		echo "			upCert Completed"
		echo "################################################################"
		;;
	upIOC)
		echo "################################################################"
		echo "			upIOC Start..." 
		upLog "upIOC Start"
		upIOC; 
		upLog "upIOC Completed"
		echo "			upIOC Completed"
		echo "################################################################"
		;;
	upEpics)
		echo "################################################################"
		echo "			upEpics Start..." 
		upLog "upEpics Start"
		upEpics;		
		upLog "upEpics Completed"
		echo "			upEpics Completed"
		echo "################################################################"
		;;
	upAll)
		echo "################################################################"
		echo "			upAll Start..." 
		mode="nfs"

		# upLog "upEpics Start"
		# upEpics;
		# upLog "upEpics Completed"

		upLog "upIOC Start"
		upIOC; 
		upLog "upIOC Completed"

		upLog "mode change from nfs to sd..."
		sed -i 's/export mode="nfs"/export mode="sd"/g' /home/root/sh/InfoDev.sh
		upLog "reboot"
		reboot
		echo "			upAll Completed"
		echo "################################################################"
		;;
	upLog)
		echo "################################################################"
		echo "			upLog Test Start..." 
		upLog function_test; 
		echo "			upLog Test Completed"
		echo "################################################################"
		;;
	*)
		echo "================================================================"
		echo "Usage: $0 { start | upEpics | upCert | upIOC | upLog }"
		exit 1
esac
