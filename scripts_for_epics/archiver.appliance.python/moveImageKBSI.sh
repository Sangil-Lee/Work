#!/bin/bash
# Shell  : moveImage.sh
# Author : Jeong Han Lee
# email  : jhlee@ibs.re.kr
# Date   : Thursday, January  8 10:53:10 KST 2015
# version : 0.1

# bash -c '/home/ctrluser/scripts_for_epics/archiver.appliance.python/moveImage.sh' > /dev/null 2>&1

# cq   : quiet 
# c    : verbose
wget_options="/usr/bin/wget -cq"
target_dir="/var/www/images/"

convert_cmd="/usr/bin/convert"
convert_opt="-gravity South -font courier -fill yellow -pointsize 18 -annotate +0+5"
convert_tgt=${target_dir}"/pngs"
ln_cmd="/bin/ln -sf"


link_images()
{  
    afile=$1
    bfile=$2

    src_filename=${convert_tgt}/${afile}
    tgt_filename=${target_dir}/${bfile}
    
    ${ln_cmd} ${src_filename} ${tgt_filename}
}




overlay_images()
{
    filename=$1
    src_filename="/tmp/${filename}"
    tgt_filename=${convert_tgt}/${filename}

    ${convert_cmd} ${src_filename} ${convert_opt} '%f' ${tgt_filename}

}
 

get_images()
{
    filename=$1
    ipadd=$2

    temp_dir=/tmp/

    $wget_options http://${ipadd}:8080/?action=snapshot -O ${temp_dir}${filename}
    
    overlay_images ${filename}
    
}

LOGDATE=`date +%F-%H:%M:%S.%N_`
# 2015-01-08-11:36:46.132114843_

hostname=${HOSTNAME}

filenum=1

a_filenames=( "kbsi-pi0.png" )
b_filenames=()   
ip_adds=( "164.125.122.23" )

# Rename the saved file with time stamp, when it is retreival
#
for (( i = 0 ; i < ${#a_filenames[@]} ; i++ )) 
do
    b_filenames[$i]=${LOGDATE}${a_filenames[$i]}
done

#
# Get files from camera site, save it into /tmp/
# 

for (( i = 0 ; i < ${filenum} ; i++ )) 
do
    get_images ${b_filenames[$i]} ${ip_adds[$i]}
    link_images ${b_filenames[$i]} ${a_filenames[$i]}
#    echo ""
done

