#!/bin/sh

OLD_PWD=$(pwd)
echo $OLD_PWD >> /dev/null

cd ${EPICS_PATH}/siteApps/Ctrl_IOC/fps-1-0/
echo $PWD
sed -i 's/\/home\/jung/\/mnt\/nfs/g' ./iocBoot/iocacq/envPaths
sed -i 's/\/home\/jung/\/mnt\/nfs/g' ./iocBoot/iocmiti/envPaths

cd $OLD_PWD
