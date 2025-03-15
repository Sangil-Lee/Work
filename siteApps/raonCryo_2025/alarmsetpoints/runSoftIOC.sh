#!/usr/bin/env bash
#
#  author  : Jeong Han Lee
#  email   : jeonghan.lee@gmail.com
#  version : 0.0.1


declare -g SC_RPATH;
#declare -g SC_NAME;
declare -g SC_TOP;
declare -g SC_TIME;

SC_RPATH="$(realpath "$0")";
#SC_NAME=${0##*/};
SC_TOP="${SC_RPATH%/*}"
SC_TIME="$(date +%y%m%d%H%M)"

SOFTIOC_CMD="softIocPVX"

echo "EPICS_BASE = $EPICS_BASE"

if hash "${SOFTIOC_CMD}" 2>/dev/null ; then
    echo "We found ${SOFTIOC_CMD}"
    ${SOFTIOC_CMD} -s -m P=ctrlslab: -d ${SC_TOP}/alarmDemo.db
elif hash softIoc 2>/dev/null; then
    echo "We found softIoc"
    softIoc -s -m P=ctrlslab: -d ${SC_TOP}/alarmDemo.db
elif [ -f ~/epics/1.1.0/*/7.0.7/setEpicsEnv.bash ]; then
    echo "We found the ALS-U EPICS environment."
    . ~/epics/1.1.0/*/7.0.7/setEpicsEnv.bash disable
    ${SOFTIOC_CMD} -s -m P=ctrlslab: -d ${SC_TOP}/alarmDemo.db
elif [ -f  ~/alsu-epics-environment/1.1.1-rc1/debian-12/7.0.7/setEpicsEnv.bash ]; then
    echo "We found the ALS-U EPICS environment - Distribution binary."
    . ~/alsu-epics-environment/1.1.1-rc1/debian-12/7.0.7/setEpicsEnv.bash disable
    ${SOFTIOC_CMD} -s -m P=ctrlslab: -d ${SC_TOP}/alarmDemo.db
else 
    echo "Cannot find the epics environment, please add it to your path"
fi
