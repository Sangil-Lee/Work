#!/bin/bash
LINK_LIST="1 "
# LINK_LIST="1 2"
USER="root"
PASSWD="root"
IPADDR="192.168.3.10"
TARGET="1"
ssh_cmd="ssh -o StrictHostKeyChecking=no -o UserKnownHostsFile=/dev/null"
scp_cmd="scp -o StrictHostKeyChecking=no -o UserKnownHostsFile=/dev/null"
reboot_cmd="/sbin/./reboot"
Certkey_rm_cmd="/bin/./rm -rf /mnt/sdcard/setup/MN*"
old_script_rm_cmd="/bin/./rm -rf /mnt/sdcard/script/sd_script.sh"

upScript()
{
	SCP_FILE1="InfoDev.sh"
	SCP_FILE2="../.sd_script.sh"
	SCP_DIR="/mnt/sdcard/script"

	UPLOAD_DAY=$(date '+%m-%d')
	echo "Script File Log update"
	sed -i 's/2020-[0-9][0-9]-[0-9][0-9]/2020-'${UPLOAD_DAY}'/g' $SCP_FILE1 $SCP_FILE2

	echo "Password  : " $PASSWD
	echo "File Nmae : " $SCP_FILE
	echo "Dest Dir  : " $SCP_DIR
	rm -rf ~/.ssh/known_hosts

	for i in ${LINK_LIST};
	do
	echo "##############################################################"
	let TARGET=$i

	#InfoDev.sh 파일의 Link&Node 값 변경
	sed -i 's/ipAddr=192.168.3.[0-9][0-9][0-9]/ipAddr=192.168.3.10'${TARGET}'/g' $SCP_FILE1
	sed -i 's/macAddr=00:0A:35:00:1E:[0-9][0-9]/macAddr=00:0A:35:00:1E:0'${TARGET}'/g' $SCP_FILE1
	sed -i 's/BID_value=MN[0-9][0-9]/BID_value=MN0'${TARGET}'/g' $SCP_FILE1
	sed -i 's/TARGET=0[0-9]/TARGET=0'${TARGET}'/g' $SCP_FILE1

	echo "Target IP : " $IPADDR$TARGET
	expect <<EOFSCRIPT

	spawn ${scp_cmd} ${SCP_FILE1} ${SCP_FILE2} ${USER}@${IPADDR}${TARGET}:${SCP_DIR}

	##################
	expect {
		-re "No route" 	{ exit 1; }
		-re "try again" { exit 1; }
		-re "(y/n)" 	{ send "y\r"; 	exp_continue; }
		-re "yes/no" 	{ send "yes\r"; exp_continue; }
		-re "password:" { send "${PASSWD}\r"; exp_continue; }
		-re "100%" 		{sleep 0.5; interact;}
	}
	expect eof
EOFSCRIPT
	done
}

runReboot()
{
	echo " Board Reboot Processing ... "

	for i in ${LINK_LIST};
	do
	echo "##############################################################"
	let TARGET=$i
	# BID_value="AN"$TARGET

	expect <<EOFREBOOT

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
}

rmCert()
{
	echo " remove Cert Key Processing ... "

	for i in ${LINK_LIST};
	do
	echo "##############################################################"
	let TARGET=$i
	# BID_value="AN"$TARGET

	expect <<EOFCERT
	set timeout 5;
	spawn ${ssh_cmd} ${USER}@${IPADDR}${TARGET} ${Certkey_rm_cmd}	
	##################
	expect {
		-re "No route" 	{ exit 1; }
		-re "try again" { exit 1; }
		-re "(y/n)" 	{ send "y\r"; 	exp_continue; }
		-re "yes/no" 	{ send "yes\r"; exp_continue; }
		-re "password:" { send "${PASSWD}\r"; sleep 0.5; interact; }
		# -re ":~# " 	{ send "/bin/./rm -rf /mnt/sdcard/setup/MN*\r"; sleep 0.5; send "exit\r"}
		timeout 		{ exit 1; }
	}
	expect eof
EOFCERT
	done
}

rmOldScript()
{
	echo " remove Old Script Processing ... "
	for i in ${LINK_LIST};
	do
	echo "##############################################################"
	let TARGET=$i
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
}


upBootImage()
{
	echo "upload boot image $PWD"
	for i in ${LINK_LIST};
	do
	echo "##############################################################"
	let TARGET=$i

	echo "Target IP : " $IPADDR$TARGET
	expect <<EOFBOOT

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
}

##########################################################################
echo "#############Mitigation Script#############"
echo "Enter the number of you want to process"
echo "	1 : upload Script File"
echo "	2 : Reboot Borad"
echo "	3 : Remove Certification Key of Board"
echo "	4 : Change BOOT.bin & image.ub file"
# echo "	5 : Restart Epics"
echo ""
echo -n "	Enter the number : "
read selected_number
rm -rf ~/.ssh/known_hosts
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
	upBootImage
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