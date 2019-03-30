#!/bin/sh

#case "$#" in
#2)  P="$1" ; A="$2" ;;
#*) echo "Usage: $0 <P prefix> <Address>" >&2 ; exit 1 ;;
#esac
#SVIFS="$IFS"
#IFS=" ="
#set `grep '^ *ASYN *=' ../configure/RELEASE`
#IFS="$SVIFS"
#ASYN="$2"
#ADL="$ASYN/medm/asynRecord.adl"


#medm -x -macro "P=$P,R=asyn,PORT=L0,ADDR=$A" "$ADL"
medm -x -macro "P=e364xachk:,R=asyn,PORT=L0,ADDR=-1" asynRecord.adl 
