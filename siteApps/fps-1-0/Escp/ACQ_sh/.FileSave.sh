#!/bin/sh
#upload Date : 2020-06-03
#SERVER_USER="jong"
#PASSWORD="jong123"
SERVER_USER="jung"
PASSWORD="jung"
SERVER_IP="192.168.3.200"
BROADCAST_IP="192.168.3.255"
SERVER_PATH="/home/${SERVER_USER}"
TARGET_DIR="${SERVER_PATH}/Escp/pms_file"

PUB_FILE=$1
PMS_FILE_NAME=$2
echo "----------------------------------------------------------------"
echo "processing FileSave.sh ..."
echo "PMS File Name : " ${PMS_FILE_NAME}
# echo "Node = " ${PUB_FILE} "  File Name : " ${PMS_FILE_NAME}
##send certification to server
##expect shell
echo "PMS File send to server"
expect <<EOFFILESAVE
spawn scp -i /mnt/sdcard/setup/${PUB_FILE} ${PMS_FILE_NAME} ${SERVER_USER}@${SERVER_IP}:${TARGET_DIR}
##################
expect {
	-re "No route" 	{ exit 1; }
	-re "try again" { exit 1; }
	-re "(y/n)" 	{ send "y\r"; 	exp_continue; }
	-re "y/n" 		{ send "y\r"; 	exp_continue; }
	-re "yes/no" 	{ send "yes\r"; exp_continue; }
	-re "password:" { send "${PASSWORD}\r"; exp_continue; }
	-re "100%" 		{ interact; }
}
expect eof
EOFFILESAVE
# echo "rm -rf" ${PMS_FILE_NAME}
rm -rf ${PMS_FILE_NAME}
echo "Deleted PMS File"
echo "----------------------------------------------------------------"
