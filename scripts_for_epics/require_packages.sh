#!/bin/bash
#
#  Shell   : require_packages.sh
#  Author  : Jeong Han Lee
#  email   : jhlee@ibs.re.kr
#  Date    : Tuesday, March 17 09:34:25 KST 2015
#  version : 0.0.4
#
#   - 0.0.1  December 1 00:01 KST 2014, jhlee
#           * created
#
#   - 0.0.2 Tuesday, December  2 16:28:19 KST 2014, jhlee
#           * seperated package lists according to distribution
#
#   - 0.0.3 Monday, March 16 09:13:20 KST 2015, jhlee
#
#           * better handling the installation package name list
#             so, now we can use one aptitude command to install
#             all packages at once (technically, 2 or 3  instead of
#             total package numbers)
#
#           * comment out "set -e" in order to work at least,
#             don't know how to handle "if any statement returns
#             a non-true return value"   
#    - 0.0.4 Tuesday, March 17 09:32:53 KST 2015, jhlee
#     
#           * seperate packages list according to what he/she want
#             to install on their environment.          
#             all, epics, raon, ctrl, dist, and so on...
# 
#    - 0.0.5 Thursday, March 26 10:11:37 KST 2015, jhlee
#           * reduce the argu option to simply the installation procedure
#
#    - 0.0.6 Thursday, April 16 16:49:09 KST 2015, jhlee
#           * move all package related list files to a specific directory,
#             i.e., package_lists
#
#
# Some errors are printing while installing.....
#
# E: Can not write log (Is stdout a terminal?) - tcgetattr (25: Inappropriate ioctl for device)



## This tells bash that it should exit the script if any statement returns a non-true return value. 
#set -e
#

#
# This will exit your script if you try to use an uninitialised variable
#set -u


declare -a packagelist

# 1st arg : the packagelist
# 2nd arg : the logfilename

function aptitude_from_list()
{
    unset packagelist
    local i
    local log=$2
    let i=0
    while IFS= read -r line_data; do
	if [ "$line_data" ]; then
	# Skip command 
	    [[ "$line_data" =~ ^#.*$ ]] && continue
	    packagelist[i]="${line_data}"
	    ((++i))
	fi
    done < $1
    echo ">>>>>>>>"
    echo "aptitude  -q --log-level=info --log-file=${log} --assume-yes install ${packagelist[@]}"
    echo "<<<<<<<<"
    aptitude  -q --log-level=info --log-file=${log} --assume-yes install ${packagelist[@]}
}




# Package List has three categories
# 1. Required Packages for the only EPICS IOC  : EPICS
#    package_list_epics
# 2. Common Packages for a common PC user    : Common
#    package_list_common
# 3. Packages dependent upon the Debian distribution (Wheezy, Jessie, etc) : Dist
#    package_list_${dist}
# 4. Required package for the RAON control : raon


#
# Check the distribution version of Debian
#
dist=`lsb_release -c | awk '{print $2}'`
#echo $version

# add logic to install some packa
case "$dist" in
    wheezy)
#	echo "Wheezy">&2
#	filename_version="package_list_wheezy"
	;;
    jessie)
#	echo "Jessie">&2
#	package_filename="package_list_jessie"
	;;
    *)
	echo >&2
	echo "Doesn't support $dist" >&2
	echo >&2
	exit 0
	;;
esac


#
# find the script directory where require_packages.sh is
#
DIR=$( cd "$( dirname "${BASH_SOURCE[0]}" )" && pwd )


filename_prefix="package_list"

filename_epics=${DIR}/package_lists/${filename_prefix}"_epics"
filename_common=${DIR}/package_lists/${filename_prefix}"_common"
filename_dist=${DIR}/package_lists/${filename_prefix}"_"${dist}
filename_raon=${DIR}/package_lists/${filename_prefix}"_raon"

logfile="/tmp/log_require_packages.log"



# What should we do?
DO="$1"
 
case "$DO" in

    all)
	aptitude update
	aptitude_from_list ${filename_epics}  ${logfile}
	aptitude_from_list ${filename_common} ${logfile}
	aptitude_from_list ${filename_dist}   ${logfile}
	aptitude_from_list ${filename_raon}   ${logfile}
        ;;
    ctrl)
	aptitude update
	aptitude_from_list ${filename_epics}  ${logfile}
	aptitude_from_list ${filename_raon}   ${logfile}
	aptitude_from_list ${filename_dist}   ${logfile}
	;;
    common)
	aptitude update
	aptitude_from_list ${filename_common} ${logfile}
	;;

#    dist)
#	aptitude update
#	aptitude_from_list ${filename_dist}   ${logfile}
#	;;
 #   epics)
#	aptitude update
#	aptitude_from_list ${filename_epics}  ${logfile}
#	;;
#    raon)
#	aptitude update
#	aptitude_from_list ${filename_raon}   ${logfile}
#	;;
    *)
	echo "">&2
        echo "usage: $0 <command>" >&2
        echo >&2
        echo "  commands: explaination" >&2
	echo ""
        echo "          all    : install all packages ">&2
        echo ""
	echo "          ctrl*  : install Control System packages ">&2
        echo ""
        echo "          common : install common Linux packages" >&2
	echo ""
#	echo "          dist   : install distribution dependent packages" >&2
#	echo ""
#	echo "          epics  : install the required packagesfor EPICS" >&2
#	echo ""
#	echo "          raon   : install the RAON specified packages" >&2
#	echo ""
        echo >&2
	exit 0
        ;;
esac

