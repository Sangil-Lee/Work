#!/bin/bash
# Shell   : epics_synApps.sh
# Author  : Jeong Han Lee
# email   : jhlee@ibs.re.kr
# Date    : Friday, December 12 14:37:20 KST 2014
# version : 0.0.6
#
#           0.0.1 created  Friday, December 12 14:37:20 KST 2014
#
#           0.0.2 Tuesday, January 13 17:16:53 KST 2015
#                 - change the installation directory epicsLibs
#                   instead of modules. epicsLibs will be used
#                   for other EPICS driven libraries (EtherIP,
#                   and so on ....)
#           0.0.3 
#                 - remove AREA_DETECTOR, DXP on linux-arm architecture
# 
#         
#           0.0.4 Monday, March 30 07:32:40 KST 2015, jhlee
#                 - change the default synApps as 5_8
#
#           0.0.5 Saturday, April  4 23:35:13 KST 2015, jhlee
#                 - support EPICS_SYNAPPS for replacing undefined
#                   EPICS_SYNAPPS in setEpicsEnv.sh.
#                 - area detector is troublesome in Jessie, so disable it
#                   and three dependent packages, DXP, ADCORE, and ADBINARIES. 
#        
#           0.0.6 Friday, September 11 17:44:37 KST 2015, jhlee
#                 - add the condition to check whether EPICS_PATH is defined or not,
#                   if there is no EPICS_PATH, exit the script, and tell an user
#                   to do source environment shell.
#
#
#
#           Even if EPICS_SYNAPPS is introduced, still we will have the different version of 
#           each support modules in sub-directories in EPICS_SYNAPPS. I am not sure we are using
#           another links to cover all different versions of them. 
#     
#           Monday, April  6 09:18:06 KST 2015, jhlee


## This tells bash that it should exit the script if any statement returns a non-true return value. 
set -e
# This will exit your script if you try to use an uninitialised variable
#set -u


# cq   : quiet 
# c    : continue getting a partially-downloaded file. So it allows us not to re-download an existent file
# 
wget_options="wget -c"
# xzf  : quiet
# xzfv : verbose
# xzfk : don't replace existing files when extracting
tar_command="tar xzf "
make_command=""

output_filename="setEpicsEnv"
current_epics_path=""
current_synApps_path=""
LOGDATE=`date +%Y.%m.%d.%H:%M`


disable_area_detector()
{
    local release_file=$1
    sed -i~ "s|^AREA_DETECTOR=.*|#AREA_DETECTOR=|g" $release_file
    sed -i  "s|^ADCORE=.*|#ADCORE=|g" $release_file
    sed -i  "s|^ADBINARIES=.*|#ADBINARIES=|g" $release_file
    sed -i  "s|^QUADEM=.*|#QUANEM=|g" $release_file
    sed -i  "s|^DXP=.*|#DXP=.*|g" $release_file

}

check_dist()
{
    
    dist=`lsb_release -c | awk '{print $2}'`
    case "$dist" in
	wheezy)
	#	echo "Wheezy">&2
	;;
	jessie)
	    disable_area_detector $1
	#	echo "Jessie">&2
	;;
	*)
	    echo "Doesn't support $dist"
	    exit 1
	    ;;
    esac
    
	
   
}
#
#  The drop_from_path was copied from thisepics.all script, to set up the ROOT build
#  , located in ${ROOTSYS}/bin/
#  Monday, August 25 20:49:41 KST 2014, jhlee


drop_from_path()
{
   # Assert that we got enough arguments
   if test $# -ne 2 ; then
      echo "drop_from_path: needs 2 arguments"
      return 1
   fi

   p=$1
   drop=$2

   newpath=`echo $p | sed -e "s;:${drop}:;:;g" \
                          -e "s;:${drop};;g"   \
                          -e "s;${drop}:;;g"   \
                          -e "s;${drop};;g"`
}

clear_add_env()
{
    
   # Assert that we got enough arguments
   if test $# -ne 2 ; then
       echo "clear_add_env: needs 2 arguments"
       echo $1
       echo $2
       
       return 1
   fi

    local current_epics_env=$1
    # $current_epics_base, $current_epics_extensions, $current_raon_sitelibs, $current_raon_siteapps, $current_epics_path(?)
    
    local check_epics_env=$2

    # $EPICS_BASE, $EPICS_EXTENSIONS, $RAON_SITELIBS, $RAON_SITEAPPS, $EPICS_PATH (?)

    if [ -n "${check_epics_env}" ] ; then
	old_epics_env=${check_epics_env}
    fi


    if [ -n "${old_epics_env}" ] ; then
	if [ -n "${PATH}" ]; then
	    drop_from_path $PATH ${old_epics_env}/bin/${EPICS_HOST_ARCH}
	    PATH=$newpath
	fi
	if [ -n "${LD_LIBRARY_PATH}" ]; then
	    drop_from_path $LD_LIBRARY_PATH ${old_epics_env}/lib/${EPICS_HOST_ARCH}
	    LD_LIBRARY_PATH=$newpath
	fi
    fi


    if [ -z "${PATH}" ]; then
	PATH=${current_epics_env}/bin/${EPICS_HOST_ARCH}; 
    else
	PATH=${current_epics_env}/bin/${EPICS_HOST_ARCH}:$PATH; 
    fi


    if [ -z "${LD_LIBRARY_PATH}" ]; then
	LD_LIBRARY_PATH=${current_epics_env}/lib/${EPICS_HOST_ARCH}; 
    else
	LD_LIBRARY_PATH=${current_epics_env}/lib/${EPICS_HOST_ARCH}:$LD_LIBRARY_PATH;
    fi
}


make_synApps()
{
    local synApps_dir=$1
    cd ${synApps_dir}
#    echo ${synApps_dir}
    synApps_release="${synApps_dir}/configure/RELEASE"
    #
    # substitution a path with b path in a file by using sed
    #
    # PATH has /, so use | instead of / as a seperator
    #
    # ^SUPPORT : ^ selects only the first character
    # ^SUPPORT=.* : Start with SUPPORT=blabla..... 
    #
    sed -i~ "s|^SUPPORT=.*|SUPPORT=${synApps_dir}|g" "${synApps_release}"
    sed -i  "s|^EPICS_BASE=.*|EPICS_BASE=${EPICS_BASE}|g" "${synApps_release}"

    # Jessie, has the following errors :
    #../napi5.h:6:18: fatal error: hdf5.h: No such file or directory
    #  #include <hdf5.h>
    #                   ^
    #  compilation terminated.
    # Even if libhdf5-dev was installed. So remove it in Jessie
    # Sunday, April  5 00:38:40 KST 2015, jhlee
    #
    #  Even if I change hdf5 location to /usr in configure/CONFIG_SITE.local.linux-x86_64
    # HDF5         = /usr/
    #  I see the following szip error, so realize this package doesn't exist in Debian
    # distribution. So automatically installation procedure for area detector doesn't work
    # 
    # 
    # `/APSshare/linux/x86_64/libSZIP/lib/libsz.a', needed by `libNeXus.so'.  Stop.

    # So, it is not the dist issuse, so comment out them...
    # Monday, April  6 19:35:34 KST 2015, jhlee
    # Check what dist is, if Jessi, disable AREADECTOR and its dependent packages
    # 
    #    check_dist ${synApps_release}

    case `uname -sm` in
	"Linux i386" | "Linux i486" | "Linux i586" | "Linux i686")
            EPICS_HOST_ARCH=linux-x86
	    EXTN_LIB_ARCH=i386-linux-gnu
	    disable_area_detector ${synApps_release}
	    make_command="make"
            ;;
	"Linux x86_64")
            EPICS_HOST_ARCH=linux-x86_64
	    EXTN_LIB_ARCH=x86_64-linux-gnu
	    disable_area_detector ${synApps_release}
	    make_command="make"
            ;;
	"Linux armv6l")
	    EPICS_HOST_ARCH=linux-arm
	    EXTN_LIB_ARCH=arm-linux-gnueabihf
	    disable_area_detector ${synApps_release}
	# 
	# There are missing header files when make -j is used on 
	# Raspberry Pi 
	# Tuesday, August 26 14:44:43 KST 2014, jhlee
	# 
	    make_command="make"
	    ;;
	"Linux armv7l")
	    EPICS_HOST_ARCH=linux-arm
	    EXTN_LIB_ARCH=arm-linux-gnueabihf
	    disable_area_detector ${synApps_release}
	    make_command="make"
	    ;;
	*)
            echo "This script  doesn't support this architecture : `uname -sm`"
            exit 1
            ;;
    esac
    
    make release
    make clean uninstall
    $make_command
}



if [ -z ${EPICS_PATH} ]; then
    echo ""
    echo "No EPICS environment variables are found."
    echo "Please source ~/epics/R3.14.12.5/setEpicsEnv.sh or equivalent one first"
    echo ""
    exit 
fi



epics=${HOME}/epics
epics_downloads=${epics}/downloads

current_epics_path=${EPICS_PATH}
current_modules_path=${current_epics_path}/epicsLibs

mkdir -p ${current_modules_path}


#
#  synApps
#  http://www.aps.anl.gov/bcda/synApps/
#
#
# http://www.aps.anl.gov/bcda/synApps/tar/synApps_5_8.tar.gz


synApps_version="5_8"
synApps_name="synApps_${synApps_version}"
synApps_filename="${synApps_name}.tar.gz"
synApps_download_site="http://www.aps.anl.gov/bcda/synApps/tar/"
synApps_link=${synApps_download_site}${synApps_filename}

$wget_options ${synApps_link} -P ${epics_downloads}
$tar_command ${epics_downloads}/${synApps_filename} -C ${current_modules_path}

# Predefine in tar.gz file
#
current_synApps_path=${current_modules_path}/${synApps_name}/support


echo $current_synApps_path
make_synApps "${current_synApps_path}"

clear_add_env "${current_synApps_path}" "${EPICS_SYNAPPS}"

existent_epicsenv_file="${current_epics_path}/${output_filename}.sh"
sed -i~ "s|^export EPICS_SYNAPPS=.*|export EPICS_SYNAPPS=${current_synApps_path}|g" "${existent_epicsenv_file}"
sed -i  "s|^# EPICS_SYNAPPS is empty|# EPICS_SYNAPPS was set.|g" "${existent_epicsenv_file}"
sed -i  "s|^# If epics_synApps.sh.*|# epics_synApps.sh was executed at ${LOGDATE}|g" "${existent_epicsenv_file}"

exit
