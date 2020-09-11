#!/bin/bash

##########################################################################
loop=1
while [ loop=1 ]
do
	echo "#################################################"
	echo "Enter the number of you want to process"
	echo "	1 : Start Acquisition Install Script"
	echo "	2 : Start Mitigation  Install Script"
	echo "	3 : Build FPS-IOC ( make )"
	echo "	4 : Build FPS-IOC ( make clean & make )"
	echo "	5 : Remove Certification Key"
	echo "	6 : Merge Certification Key"
	echo "	0 : Exit Script"
	echo -n "	Enter the number : "
	read selected_number
	echo "#################################################"
	case "${selected_number}" in
		1)
			echo "selected case : Start Acquisition Install Script"
			cd /home/jung/Escp/ACQ_sh/
			./install_script_Acquisition.sh
			;;
		2)
			echo "selected case : Start Mitigation Install Script"
			cd /home/jung/Escp/MIT_sh/
			./install_script_Mitigation.sh
			;;
		3)
			echo "selected case : Build FPS-IOC ( make )"
			cd ~/epics/R3.14.12.5/siteApps/Ctrl_IOC/fps-1-0
			make
			sed -i 's/\/home\/jung/\/mnt\/nfs/g' ./iocBoot/iocacq/envPaths
			sed -i 's/\/home\/jung/\/mnt\/nfs/g' ./iocBoot/iocmiti/envPaths
			;;
		4)
			echo "selected case : Build FPS-IOC (  make clean & make )"
			cd ~/epics/R3.14.12.5/siteApps/Ctrl_IOC/fps-1-0
			make clean
			make
			sed -i 's/\/home\/jung/\/mnt\/nfs/g' ./iocBoot/iocacq/envPaths
			sed -i 's/\/home\/jung/\/mnt\/nfs/g' ./iocBoot/iocmiti/envPaths
			;;
		5)
			echo "selected case : Remove the Certification Key"
			cd ~/.ssh
			rm -rfv known_hosts authorized_keys
			rm -rfv PK/*
			;;
		6)
			echo "selected case : Merge the Certification Key"
			cd ~/.ssh
			cat PK/*.pub >> authorized_keys
			cat id_rsa.pub >> authorized_keys	# 서버 키 
			;;
		0)
			echo "selected case : exit script"
			loop=0
			exit 1
			;;
		*)
			echo ""
			echo "You have entered ${selected_number}"
			echo "Please select the number in the list..."
			# echo "Exit the Script"
			# echo ""
			# exit 1
			;;
	esac
done
exit 0
