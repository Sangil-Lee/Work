#!/bin/sh
iocF="iocBoot/ioctiming/"
epicsLocal="/home/ctrluser/epics/R3.14.12.5/siteApps/Ctrl_IOC/timing-1-1/"
epicsServer="/mnt/epics/R3.14.12.5/siteApps/Ctrl_IOC/timing-1-1/"

start ()
{
        echo "======================================="
        echo "$0.$1"

# add your app
        ifconfig eth0 down 
        ifconfig eth0 192.168.0.143 up 

        mkdir /mnt/nfs
        mkdir /mnt/epics
        mkdir /home/ctrluser
        
        mount -o nolock 192.168.0.140:/tftpboot /mnt/nfs
        mount -o nolock 192.168.0.140:/home/ctrluser/epics /mnt/epics

        ln -s /mnt/sdcard/sysManage.sh /home/root/sm
        ln -s /mnt/epics/ /home/ctrluser/epics
#        ln -s /mnt/sdcard/epics/ /home/ctrluser/epics/

        ln -s ${epicsLocal}/${iocF} /home/root/iocTiming
        ln -s ${epicsLocal} /home/root/timing

        insmod /mnt/nfs/raonts2gtp.ko
        insmod /mnt/nfs/ts2ev.ko

        export PS1=\$

        echo "$0.$1 - init done."
        cd /home/root/timing/${iocF}
	./st.cmd
}

stop ()
{
        echo "======================================="
        echo "$0.$1"
        rmmod raonts2gtp
        rmmod ts2ev
        umount /mnt/nfs
        echo "$0.$1 - done."
}

restart()
{
        stop
        start
}

epicsRun ()
{
        echo "======================================="
        echo "$0.$1"

# add your app
        cd /home/root/timing/${iocF}
	./st.cmd
}

upSys()
{
        echo "======================================="
        echo "$0.$1"
        cp -f /mnt/nfs/sysManage.sh /mnt/sdcard/sysManage.sh
        sync
        echo "$0.$1 - done."
}

upFpga()
{
        echo "======================================="
        echo "$0.$1 - BOOT.BIN"
        cp -f /mnt/nfs/BOOT.BIN /mnt/sdcard/BOOT.BIN
        sync
        echo "$0.$1 - done."
}

upEpics()
{
        echo "======================================="
        echo "$0.$1"
        
        source /mnt/sdcard/./infoDev.sh
        echo ${TS2_LOCAL_SYS}, ${TS2_LOCAL_SUB_SYS}, ${TS2_LOCAL_DEV}, ${TS2_LOCAL_DEV_NUM}
        
        cp ${epicsServer}/${iocF}new_timing.sub /mnt/sdcard/new_timing.sub
        sync
        mv /mnt/sdcard/new_timing.sub /mnt/sdcard/timing.sub
        sync
	find /mnt/sdcard -iname 'timing.sub' -exec sed -i 's/NEW_SYS/'${TS2_LOCAL_SYS}'/g' {} \;
	find /mnt/sdcard -iname 'timing.sub' -exec sed -i 's/NEW_SUB_SYS/'${TS2_LOCAL_SUB_SYS}'/g' {} \;
	find /mnt/sdcard -iname 'timing.sub' -exec sed -i 's/NEW_DEV_N/'${TS2_LOCAL_DEV}${TS2_LOCAL_DEV_NUM}'/g' {} \;
        sync

	echo ${TS2_LOCAL_SYS}, ${TS2_LOCAL_SUB_SYS}, ${TS2_LOCAL_DEV}, ${TS2_LOCAL_DEV_NUM}
        cp /mnt/sdcard/new_envDev /mnt/sdcard/envDev
        sync
	find /mnt/sdcard -iname 'envDev' -exec sed -i 's/NEW_SYS/'${TS2_LOCAL_SYS}'/g' {} \;
	find /mnt/sdcard -iname 'envDev' -exec sed -i 's/NEW_SUB_SYS/'${TS2_LOCAL_SUB_SYS}'/g' {} \;
	find /mnt/sdcard -iname 'envDev' -exec sed -i 's/NEW_DEV_NUM/'${TS2_LOCAL_DEV_NUM}'/g' {} \;
	find /mnt/sdcard -iname 'envDev' -exec sed -i 's/NEW_DEV/'${TS2_LOCAL_DEV}'/g' {} \;
        sync
        echo "$0.$1 - done."
}

case "$1" in
        start)
                start; ;;
        stop)
                stop; ;;
        restart)
                restart; ;;
        run)
                epicsRun; ;;
        upSys)
                upSys; ;;
        upFpga)
                upFpga; ;;
        upEpics)
                upEpics; ;;

        *)
                echo "======================================="
                echo "Usage: $0 {start|stop|restart|run|upSys|upFpga|upEpics}"
                exit 1
esac

exit $?
