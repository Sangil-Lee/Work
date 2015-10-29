#!/bin/bash
# Shell   : epics_motors.sh
# Author  : Jeong Han Lee
# email   : jhlee@ibs.re.kr
# Date    : 
# version : 0.0.1
#
#           0.0.1 created  Friday, Jan 16 1:03:20 KST 2015
#


## This tells bash that it should exit the script if any statement returns a non-true return value. 
set -e
# This will exit your script if you try to use an uninitialised variable
set -u


# cq   : quiet 
# c    : continue getting a partially-downloaded file. So it allows us not to re-download an existent file
# 
wget_options="wget -c"
# xzf  : quiet
# xzfv : verbose
# xzfk : don't replace existing files when extracting
tar_command="tar xzf "
make_command_base=""

#
# EPICS/epicsLibs
#

# Define the epics path
epics=${HOME}/epics
# Define the epics download dir
epics_downloads=${epics}/downloads
# Where we are
current_epics_path=${EPICS_PATH}
# Define the epicsLibs
current_epicsLibs_path=${current_epics_path}/epicsLibs
# Make epicsLibs
motorApps_path=${current_epicsLibs_path}/motorApps

ipac_version="2.13"
seq_version="2.1.16"
asyn_version="4-24"
busy_version="1-6-1"
motor_version="6-9"

ipac_name="ipac-${ipac_version}"
seq_name="seq-${seq_version}"
asyn_name="asyn${asyn_version}"
busy_name="busy_R${busy_version}"
motor_name="motorR${motor_version}"


ipac_filename="${ipac_name}.tar.gz"
seq_filename="${seq_name}.tar.gz"
asyn_filename="${asyn_name}.tar.gz"
busy_filename="${busy_name}.tar.gz"
motor_filename="${motor_name}.tar.gz"

ipac_site="http://www.aps.anl.gov/epics/download/modules/"
seq_site="http://www-csr.bessy.de/control/SoftDist/sequencer/releases/"
asyn_site="http://www.aps.anl.gov/epics/download/modules/"
busy_site="http://www.aps.anl.gov/bcda/synApps/tar/"
motor_site="http://www.aps.anl.gov/bcda/synApps/motor/tar/"


make_release()
{
    local name=$1

    # case ${name} in
    # 	ipac* | seq* | asyn* | motor* )
    # 	    name=${name}
    # 	    ;;
    # 	busy* )
    # 	    # if the package is the busy, change its name from busy_R1-6-1 to busy-1-6-1
    # 	    # I don't know why the busy create the different directory name
    # 	    # 2015.01.15, jhlee

    # 	    name=${name//_R/-}
    # 	    #sed -i~  "s|^EPICS_BASE=.*|EPICS_BASE=${EPICS_PATH}/base|g" "${rfile}"
    # 	    ;;
    #    	*)
    #         echo "This script  doesn't support the package : ${name}"
    #         exit 1
    #         ;;
    # esac

    # I think, the following replacement doesn't change anything for ipac, seq, asyn, motor
    # so, in order to speed up, simply use the following command. 
    # if we add more packages in this script or extend it for other purpose, 
    # please consider to change the following line
    # 2014.1.15 jhlee

    
    local rfile="${motorApps_path}/${name//_R/-}/configure/RELEASE"
 

    # ipac  : EPICS_BASE
    # seq   : EPICS_BASE
    # asyn  : EPICS_BASE, IPAC, SNCSEQ, SUPPORT
    # busy  : EPICS_BASE, ASYN, SUPPORT
    # motor : EPICS_BASE, ASYN, SNCSEQ, IPAC, BUSY, SUPPORT
 
    # 1. All release file has the EPICS_BASE, so
    # backup, and change EPICS_BASE with the proper one
 
    sed -i~  "s|^EPICS_BASE=.*|EPICS_BASE=${EPICS_PATH}/base|g" "${rfile}"

    case ${name} in
	asyn* )
	    sed -i  "s|^SUPPORT=.*|SUPPORT=${motorApps_path}|g" "${rfile}"
	    sed -i  "s|^IPAC=.*|IPAC=${motorApps_path}/${ipac_name}|g" "${rfile}"
	    sed -i  "s|^SNCSEQ=.*|SNCSEQ=${motorApps_path}/${seq_name}|g" "${rfile}"
	   ;;
	busy*)
	    sed -i  "s|^SUPPORT=.*|SUPPORT=${motorApps_path}|g" "${rfile}"
	    sed -i  "s|^ASYN=.*|ASYN=${motorApps_path}/${asyn_name}|g" "${rfile}"
	    ;;
	motor*)
	    sed -i  "s|^SUPPORT=.*|SUPPORT=${motorApps_path}|g" "${rfile}"
	    sed -i  "s|^ASYN=.*|ASYN=${motorApps_path}/${asyn_name}|g" "${rfile}"
	    sed -i  "s|^SNCSEQ=.*|SNCSEQ=${motorApps_path}/${seq_name}|g" "${rfile}"
	    sed -i  "s|^IPAC=.*|IPAC=${motorApps_path}/${ipac_name}|g" "${rfile}"
	    sed -i  "s|^BUSY=.*|BUSY=${motorApps_path}/${busy_name//_R/-}|g" "${rfile}"
	    ;;
	*)
	    ;;
    esac

    # # # make release
    # # # make clean uninstall
    # # # make -j
}

downloads()
{
    local site=$1
    local file=$2
    local link=${site}${file}
    $wget_options ${link} -P ${epics_downloads}
    $tar_command ${epics_downloads}/${file}  -C  ${motorApps_path}
    
}




mkdir -p ${motorApps_path}


filenum=5
names=( "${ipac_name}" "${seq_name}" "${asyn_name}" "${busy_name}" "${motor_name}" )
files=( "${ipac_filename}" "${seq_filename}" "${asyn_filename}" "${busy_filename}" "${motor_filename}" )
sites=( "${ipac_site}" "${seq_site}" "${asyn_site}" "${busy_site}" "${motor_site}" )


for (( i = 0 ; i < ${filenum} ; i++ )) 
do
    downloads ${sites[$i]} ${files[$i]} 
done


for (( i = 0 ; i < ${filenum} ; i++ )) 
do
    make_release ${names[$i]} 
done


for (( i = 0 ; i < ${filenum} ; i++ )) 
do
    cd  "${motorApps_path}/${names[$i]//_R/-}"
    make clean uninstall
    make
done



exit
