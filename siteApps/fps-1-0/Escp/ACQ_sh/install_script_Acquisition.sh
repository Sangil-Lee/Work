#!/bin/bash
# args="$1"   ## args : start, upEpics , upCert
# LINK_LIST="1 2 3 4 5 6 7 8"
# NODE_LIST="3"
LINK_LIST="6"
NODE_LIST="1 2 3 4 5 6 7"
#NODE_LIST="1 "
USER="root"
PASSWD="root"
IPADDR="192.168.3."
TARGET="11"
BID_value=AN11
ssh_cmd="ssh -o StrictHostKeyChecking=no -o UserKnownHostsFile=/dev/null"
scp_cmd="scp -o StrictHostKeyChecking=no -o UserKnownHostsFile=/dev/null"
reboot_cmd="/sbin/./reboot"
epics_exit="killall st.cmd"
epics_run="/etc/init.d/init-acq"
Certkey_rm_cmd="/bin/./rm -rf /mnt/sdcard/setup/AN*"
old_script_rm_cmd="/bin/./rm -rf /mnt/sdcard/script/sd_script.sh /mnt/sdcard/script/FileSave.sh "

upScript()
{
	SCP_FILE1="InfoDev.sh"
	SCP_FILE2="../.sd_script.sh"
	SCP_FILE3=".FileSave.sh"
	SCP_DIR="/mnt/sdcard/script"

	UPLOAD_DAY=$(date '+%m-%d')
	echo "Script File Log update"
	sed -i 's/2020-[0-9][0-9]-[0-9][0-9]/2020-'${UPLOAD_DAY}'/g' $SCP_FILE1 $SCP_FILE2 $SCP_FILE3

	echo "Password  : " $PASSWD
	echo "File Nmae : " $SCP_FILE1
	echo "Dest Dir  : " $SCP_DIR
	rm -rf ~/.ssh/known_hosts

	for i in ${LINK_LIST};
	do
		echo "##############################################################"
	for j in ${NODE_LIST};
	do
	let TARGET=$i*10+$j
	BID_value="AN"$TARGET

	#InfoDev.sh 파일의 Link&Node 값 변경
	sed -i 's/ipAddr=192.168.3.[0-9][0-9]/ipAddr=192.168.3.'${TARGET}'/g' $SCP_FILE1
	sed -i 's/macAddr=00:0A:35:00:1E:[0-9][0-9]/macAddr=00:0A:35:00:1E:'${TARGET}'/g' $SCP_FILE1
	sed -i 's/BID_value=AN[0-9][0-9]/BID_value='${BID_value}'/g' $SCP_FILE1
	sed -i 's/TARGET=[0-9][0-9]/TARGET='${TARGET}'/g' $SCP_FILE1
	sed -i 's/LINK=[0-9]/LINK='${i}'/g' $SCP_FILE1
	sed -i 's/NODE=[0-9]/NODE='${j}'/g' $SCP_FILE1

	echo "Target IP : " $IPADDR$TARGET
	expect <<EOFSCRIPT

	# 1. param check... param count
	# 2. for loop spawn scp command
	# 3. transmit test of big size file
	##### command ####

	# set timeout 5;

	spawn ${scp_cmd} ${SCP_FILE1} ${SCP_FILE2} ${SCP_FILE3} ${USER}@${IPADDR}${TARGET}:${SCP_DIR}
	
	##################
	expect {
		-re "No route" 	{ exit 1; }
		-re "try again" { exit 1; }
		-re "(y/n)" 	{ send "y\r"; 	exp_continue; }
		-re "yes/no" 	{ send "yes\r"; exp_continue; }
		-re "password" 	{ send "${PASSWD}\r"; exp_continue; }
		-re "100%" 		{ sleep 0.5; interact; }
	}
	expect eof
EOFSCRIPT
	done
	done
}

runReboot()
{
	echo " Board Reboot Processing ... "

	for i in ${LINK_LIST};
	do
		echo "##############################################################"
	for j in ${NODE_LIST};
	do
	let TARGET=$i*10+$j
	# BID_value="AN"$TARGET

	expect <<EOFREBOOT

	# 1. param check... param count
	# 2. for loop spawn scp command
	# 3. transmit test of big size file
	##### command ####

	# set timeout 5;
	spawn ${ssh_cmd} ${USER}@${IPADDR}${TARGET} ${reboot_cmd}
	
	##################
	expect {
		-re "No route" 	{ exit 1; }
		-re "try again" { exit 1; }
		-re "(y/n)" 	{ send "y\r"; 	exp_continue; }
		-re "yes/no" 	{ send "yes\r"; exp_continue; }
		-re "password:" { send "${PASSWD}\r"; sleep 0.5; interact; }
	}
	expect eof
EOFREBOOT
	done
	done
}


rmCert()
{
	echo " remove Cert Key Processing ... "

	for i in ${LINK_LIST};
	do
		echo "##############################################################"
	for j in ${NODE_LIST};
	do
	let TARGET=$i*10+$j
	# BID_value="AN"$TARGET

	expect <<EOFCERT
	set timeout 5;
	# spawn ssh ${USER}@${IPADDR}${TARGET} 'rm -rf /mnt/sdcard/setup/AN61'
	# spawn ${ssh_cmd} ${USER}@${IPADDR}${TARGET}
	spawn ${ssh_cmd} ${USER}@${IPADDR}${TARGET} ${Certkey_rm_cmd}
	
	##################
	expect {
		-re "No route"	{ exit 1; }
		-re "try again" { exit 1; }
		-re "logout"	{ exit 1; }
		-re "(y/n)" 	{ send "y\r"; 	exp_continue; }
		-re "yes/no" 	{ send "yes\r"; exp_continue; }
		-re "password:" { send "${PASSWD}\r"; sleep 0.5; interact; }
		# -re "root@jong:~# " { send "/bin/./rm -rf /mnt/sdcard/setup/AN*\r"; sleep 0.5; send "exit\r"}
		timeout 		{ exit 1; }
	}
	expect eof
EOFCERT
	done
	done
}

rmOldScript()
{
	echo " remove Old Script Processing ... "

	for i in ${LINK_LIST};
	do
		echo "##############################################################"
	for j in ${NODE_LIST};
	do
	let TARGET=$i*10+$j
	# BID_value="AN"$TARGET

	expect <<EOFRMSCRIPT
	set timeout 5;
	spawn ${ssh_cmd} ${USER}@${IPADDR}${TARGET} ${old_script_rm_cmd}
	
	##################
	expect {
		-re "No route"	{ exit 1; }
		-re "try again" { exit 1; }
		-re "logout"	{ exit 1; }
		-re "(y/n)" 	{ send "y\r"; 	exp_continue; }
		-re "yes/no" 	{ send "yes\r"; exp_continue; }
		-re "password:" { send "${PASSWD}\r"; sleep 0.5; interact; }
		timeout 		{ exit 1; }
	}
	expect eof
EOFRMSCRIPT
	done
	done
}

upBootImage()
{
	echo "upload boot image $PWD"
	for i in ${LINK_LIST};
	do
		echo "##############################################################"
	for j in ${NODE_LIST};
	do
	let TARGET=$i*10+$j
	BID_value="AN"$TARGET

	echo "Target IP : " $IPADDR$TARGET
	expect <<EOFBOOT

	# 1. param check... param count
	# 2. for loop spawn scp command
	# 3. transmit test of big size file
	##### command ####
	# set timeout 5;
	spawn ${scp_cmd} BOOT.BIN ${USER}@${IPADDR}${TARGET}:/mnt/sdcard
	
	##################
	expect {
		-re "No route" 	{ exit 1; }
		-re "try again" { exit 1; }
		-re "(y/n)" 	{ send "y\r"; 	exp_continue; }
		-re "yes/no" 	{ send "yes\r"; exp_continue; }
		-re "password:" { send "${PASSWD}\r"; exp_continue; }
		-re "100%" 		{ interact; }
	}
	expect eof
EOFBOOT
	expect <<EOFIMAGE

	# 1. param check... param count
	# 2. for loop spawn scp command
	# 3. transmit test of big size file
	##### command ####
	# set timeout 5;
	spawn ${scp_cmd} image.ub ${USER}@${IPADDR}${TARGET}:/mnt/sdcard
	
	##################
	expect {
		-re "No route" 	{ exit 1; }
		-re "try again" { exit 1; }
		-re "(y/n)" 	{ send "y\r"; 	exp_continue; }
		-re "yes/no" 	{ send "yes\r"; exp_continue; }
		-re "password:" { send "${PASSWD}\r"; exp_continue; }
		-re "100%" 		{ interact; }
	}
	expect eof
EOFIMAGE
	done
	done
}

restart_Epics()
{
	echo " Restart Processing ... "

	for i in ${LINK_LIST};
	do
		echo "##############################################################"
	for j in ${NODE_LIST};
	do
	let TARGET=$i*10+$j
	# BID_value="AN"$TARGET

	expect <<EOFEXIT

	spawn ${ssh_cmd} ${USER}@${IPADDR}${TARGET} ${epics_exit}
	
	##################
	expect {
		-re "No route" 	{ exit 1; }
		-re "try again" { exit 1; }
		-re "(y/n)" 	{ send "y\r"; 	exp_continue; }
		-re "yes/no" 	{ send "yes\r"; exp_continue; }
		-re "password:" { send "${PASSWD}\r"; sleep 0.5; interact; }
	}
	expect eof
EOFEXIT
	echo "EPICS EXIT"
	expect <<EOFRUN

	spawn ${ssh_cmd} ${USER}@${IPADDR}${TARGET} ${epics_run}
	
	##################
	expect {
		-re "No route" 	{ exit 1; }
		-re "try again" { exit 1; }
		-re "(y/n)" 	{ send "y\r"; 	exp_continue; }
		-re "yes/no" 	{ send "yes\r"; exp_continue; }
		-re "password:" { send "${PASSWD}\r"; sleep 0.5; interact; }
	}
	expect eof
EOFRUN

	done
	done
}

##########################################################################
echo "#############Acqusition Script#############"
echo "Enter the number of you want to process"
echo "	1 : upload Script File"
echo "	2 : Reboot Borad"
echo "	3 : Remove Certification Key of Board"
echo "	4 : Change BOOT.bin & image.ub file"
#echo "	5 : Restart Epics"
echo ""
echo -n "	Enter the number : "
read selected_number
case "${selected_number}" in
1)
	echo "selected case : upload Script File"
	upScript
	;;
2)
	echo "selected case : Reboot Borad"
	runReboot
	;;
3)
	echo "selected case : Remove Certification Key of Board"
	rmCert
	;;
4)
	echo "selected case : Change BOOT.bin & image.ub file "
	if [ -f "./BOOT.BIN" ];then
		if [ -f "image.ub" ];then
			upBootImage
		else
			echo "./image.ub file not exist"
		fi
	else
		echo "./BOOT.BIN file not exist"
	fi
	;;
5)
	echo "selected case : Remove old script "
	rmOldScript
	;;
*)
	echo ""
	echo "You have entered ${selected_number}"
	echo "Please select the number in the list..."
	echo "Exit the Script"
	echo ""
	exit 1
	;;
esac
exit 0

# if [ x${args} == x ];then
# 	upScript
# else
# 	case "$1" in
# 	rmcert)
# 		echo rmCert; ;;
# 	reboot)
# 		echo runReboot; ;;
# 	upScript)
# 		echo upScript; ;;
# 	esac
# fi


# if [ ${args} == sd ];then
# 	upScript
# 	runReboot
# else
# 	upScript
# 	runReboot
# fi
