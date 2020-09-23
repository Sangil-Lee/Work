#!/bin/bash


HOSTLIST=("IOC1" "IOC2" "IOC3" "IOC4" "IOC5" "IOC6" "IOC7" "IOC8" "IOC9" "IOC10")
IPLIST=("192.168.68.101" "192.168.68.102" "192.168.68.103" "192.168.68.105" "192.168.68.106" "192.168.68.107" "192.168.68.108" "192.168.68.109" "192.168.68.110" "192.168.68.111")
ACCOUNT="ctrluser"
SSHPASSWD="sshpass -pqwer1234"
SSHCMD="ssh -oStrictHostKeyChecking=no -o UserKnownHostsFile=/dev/null"
SCPCMD="scp -oStrictHostKeyChecking=no -o UserKnownHostsFile=/dev/null -o ConnectTimeout=3"
#COPYLIST=("testfile.txt" "bootimage")
COPYLIST="testfile.txt"
COPYDIR="./data/"
REMOTEDIR="~/data/"

CMDLIST=("sshpass" "ssh" "scp" "tmux" "screen")

#for host in ${HOSTLIST[@]};do
#	echo "${ACCOUNT}@${host}"
#done
#
#for ipaddr in ${IPLIST[@]};do
#	echo "${ACCOUNT}@${ipaddr}"
#done

RemoteHostCopyAll()
{
	for host in ${HOSTLIST[@]};do
		echo "${SSHPASSWD} ${SCPCMD} ${COPYDIR}${COPYLIST} ${ACCOUNT}@${host}:${REMOTEDIR}"
		#${SSHPASSWD} ${SCPCMD} ${COPYDIR}${COPYLIST} ${ACCOUNT}@${host}:${REMOTEDIR}
	done

}

RemoteIPCopyAll()
{
	for ipaddr in ${IPLIST[@]};do
		echo "${SSHPASSWD} ${SCPCMD} ${COPYDIR}${COPYLIST} ${ACCOUNT}@${ipaddr}:${REMOTEDIR}"
		${SSHPASSWD} ${SCPCMD} ${COPYDIR}${COPYLIST} ${ACCOUNT}@${ipaddr}:${REMOTEDIR}
	done

}

echo ""
echo "Remote Commander for RAON Control system"
echo "Enter the number"
echo "1 : Remote Host Copy All"
echo "2 : Remote IP Copy All"
echo "0 : Exit script"
echo ""
echo -n "Enter the number : "
read answer
case "${answer}" in
        1)
                echo "Copy All ... "
				RemoteHostCopyAll
                ;;

        2)
                echo "Copy All ... "
				RemoteIPCopyAll
                ;;
		0)
		echo "Exit the script"
		exit 1
		;;
        *)
        echo ""
		echo "You have entered ${answer}"
		echo "Please select the number in the list..."
		echo "Exit the script"
		echo ""
                exit 1
                ;;
esac
exit 0



##### Without ssh-keygen, Remote Copy and Remote Command #########################
##### With sshpass
##(base) ctrluser@sidesk:~$ sshpass -pqwer1234 scp -oStrictHostKeyChecking=no hello.txt ctrluser@10.1.181.125:~/CopyDir
##
##### Command
##sshpass -pqwer1234 ssh -oStrictHostKeyChecking=no -o UserKnownHostsFile=/dev/null ctrluser@10.1.181.125 "ls -al"
##
##### Remote Command using screen without ssh-keygen
##$>sshpass -pqwer1234 ssh -oStrictHostKeyChecking=no -o UserKnownHostsFile=/dev/null ctrluser@192.168.68.102 "cd /home/ctrluser/epics/7.0.3/siteApps/Ctrl_IOC/Training-1-0/iocBoot/ioctr1;screen -S IOC -d -m ./st.cmd"
##
##### IOC kill with screen
##$>sshpass -pqwer1234 ssh -oStrictHostKeyChecking=no -o UserKnownHostsFile=/dev/null ctrluser@192.168.68.102 "screen -S IOC -X quit"
##
#### Screen Scroll mode: Ctrl-a ESC, then PageUP PageDown key
#### Screen Scroll mode-Off: ESC
###
##### Remote Command using tmux without ssh-keygen, then dbl>dblist on epics prompt
##$>sshpass -pqwer1234 ssh -oStrictHostKeyChecking=no -o UserKnownHostsFile=/dev/null ctrluser@192.168.68.102 "cd /home/ctrluser/epics/7.0.3/siteApps/Ctrl_IOC/Training-1-0/iocBoot/ioctr1;tmux new-session -d -s IOC ./st.cmd;tmux send-keys 'dbl>dblist' C-m;tmux detach -s IOC"
##
##### Remote Command on EPICS Shell Prompt with tmux, IOC(tmux session):0(pane number), this is good method for remote manager shell script.
##$>sshpass -pqwer1234 ssh -oStrictHostKeyChecking=no -o UserKnownHostsFile=/dev/null ctrluser@192.168.68.102 "tmux send-keys -t IOC:0 'dbl>dblist7' C-m;"
##
##### Ex). Magnet P.S, IOC Fieldmap file reloading...
##$>sshpass -pqwer1234 ssh -oStrictHostKeyChecking=no -o UserKnownHostsFile=/dev/null ctrluser@192.168.68.102 "cd /home/ctrluser/epics/7.0.3/siteApps/Ctrl_IOC/poletip-fieldmap-1-0/iocBoot/iocfieldmap;tmux new-session -d -s IOC ./st.cmd;tmux send-keys 'dbl>dblist' C-m;tmux detach -s IOC"
##$>sshpass -pqwer1234 ssh -oStrictHostKeyChecking=no -o UserKnownHostsFile=/dev/null ctrluser@192.168.68.102 "tmux send-keys -t IOC:0  C-m 'reLoadMapFile ./BI_Map,40000,200,0' C-m;" #reLoadMapFile("Mapfilepath",mapsize,maxcurrent,load), load(0): Print new splined fieldmap list
##$>sshpass -pqwer1234 ssh -oStrictHostKeyChecking=no -o UserKnownHostsFile=/dev/null ctrluser@192.168.68.102 "tmux send-keys -t IOC:0  C-m 'reLoadMapFile ./BI_Map,40000,200,1' C-m;" #reLoadMapFile("Mapfilepath",mapsize,maxcurrent,load), load(1): Applying new splined fieldmap list
##$>sshpass -pqwer1234 ssh -oStrictHostKeyChecking=no -o UserKnownHostsFile=/dev/null ctrluser@192.168.68.102 "tmux send-keys -t IOC:0 C-m 'reLoadMapFile ./BI_Map,100,200,0' C-m;"
##
##### IOC kill with tmux
##$>sshpass -pqwer1234 ssh -oStrictHostKeyChecking=no -o UserKnownHostsFile=/dev/null ctrluser@192.168.68.102 "tmux kill-session -t IOC"
##
#### Tmux Attach: $>tmux attach -t IOC(session name)
#### Tmux Detach: (Ctrl+b)d
#### Tmux Session Kill: $>tmux kill-session -t IOC(session name)
#### Tmux Scroll mode: (Ctrl+b)[, Then PageUp/PageDown
#### Tmux Mouse Scroll mode:  ~/.tmux.conf -> set -g set -g terminal-overrides 'xfce4-terminal:smcup@:rmcup@' , then scroll mode: (Ctrl+b)[, then mouse scrolling.
##
##
##
##### With expect
###
##(base) ctrluser@sidesk:~$ expect << EOF
##spawn scp -oStrictHostKeyChecking=no -o UserKnownHostsFile=/dev/null hello.txt ctrluser@10.1.181.125:~/CopyDir
##expect "password:"
##send "qwer1234\r"
##expect eof
##EOF
##
##(base) ctrluser@sidesk:~$ expect << EOF
##spawn ssh -oStrictHostKeyChecking=no -o UserKnownHostsFile=/dev/null hello.txt ctrluser@10.1.181.125 "ls -al"
##expect "password:"
##send "qwer1234\r"
##expect eof
##EOF
##
##
##
##
