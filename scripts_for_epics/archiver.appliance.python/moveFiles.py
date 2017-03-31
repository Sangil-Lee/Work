#!/usr/bin/python
# coding=utf-8 
#
# Shell   : moveFiles.py
# Author  : Jeong Han Lee
# email   : jhlee@ibs.re.kr
# Date    : 
# Version : 0.1.0
#
#     crontab -e, add the following line
#
#*/5 *  * * * export DISPLAY=:0.0 && /usr/bin/python /home/ctrluser/scripts_for_epics/archiver.appliance.python/moveFiles.py >/dev/null 2>&1

import os
import sys
import shutil
# import numpy as np
#from chaco.shell import *
import socket
from datetime import timedelta, datetime, date
#import time


# def overlayTextCmdn(filename):
        
#     temp_filename = ""
#     dest_filename = ""
#     time_filename = ""
   
#     _now = datetime.now()
#     _now_iso_string  = _now.isoformat()

#     temp_filename = "/tmp/" + filename
#     time_filename = _now_iso_string + "_" + filename
# convert test.png -gravity South -font courier -fill yellow -pointsize 18 -annotate +0+5 "what is test" movingtest/test_new.png

    
#     convert_cmd  = "/usr/bin/convert "
#     convert_opts = " -gravity South -font monospace -fill yellow -pointsize 18 -annotate +0+5 "
#     command      = convert_cmd + filename + convert_opts +
#     os.system(command + '' + )
    
#     convert 2015-01-06T17_37_06.894613_munji-pi0.png -gravity South -font monospace -fill yellow -pointsize 18 -annotate +0+5 '%f'



#         temp_filename = "/tmp/" + afile
#         time_filename = _now_iso_string + "_" + afile
  
#         dest_filename = pngs_directory + time_fi _now_iso_string + "_" + afile
# #        shutil.copy (temp_filename, dest_directory)
# #        print temp_filename, dest_filename

#         shutil.copy2(temp_filename, dest_filename)

# #  ln -sf /var/www/images/pngs/2015-01-06T17:40:34.803806_munji-pi0.png /var/www/images/current.png

#         os.system('ln -sf ' + dest_filename + ' ' + dest_directory + afile)

        
    
#     return url + "/retrieval/data/getData.raw"

    

def main():


#    print "Total length" , len(matchingPVs)

    
    hostname = socket.gethostname() 
    hostip   = socket.gethostbyname(hostname)
    
    capture_filename1 = "munji-pi0.png"
    capture_filename2 = "munji-pi1.png"

    dest_directory = "/var/www/images/"
    pngs_directory = dest_directory + "pngs"
    
    fileList =[]

    fileList.append(capture_filename1)
    fileList.append(capture_filename2)

    # temp_filename = ""
    # dest_filename = ""
    # time_filename = ""
   
    
#    wget_command  = ""
#    print fileList

    wget_pi1 = "/usr/bin/wget -O /tmp/" + capture_filename1 + " http://192.168.1.100:8080/?action=snapshot"
    wget_pi2 = "/usr/bin/wget -O /tmp/" + capture_filename2 + " http://192.168.1.101:8080/?action=snapshot"

    
#    print wget_pi1
    os.system(wget_pi1)
    os.system(wget_pi2)

    convert_cmd  = "/usr/bin/convert "
    convert_opts = " -gravity South -font monospace -fill yellow -pointsize 18 -annotate +0+5 "

    for afile in fileList:
        
        temp_filename = "/tmp/" + afile
        time_filename = _now_iso_string + "_" + afile
  
        dest_filename = pngs_directory + time_fi _now_iso_string + "_" + afile
#        shutil.copy (temp_filename, dest_directory)
#        print temp_filename, dest_filename

        shutil.copy2(temp_filename, dest_filename)

#  ln -sf /var/www/images/pngs/2015-01-06T17:40:34.803806_munji-pi0.png /var/www/images/current.png

        os.system('ln -sf ' + dest_filename + ' ' + dest_directory + afile)
#        time.sleep(.200)
        # plot(secs, vals, "r-")
        # xscale('time')
        # show()



    sys.exit()

if __name__ == '__main__': main()

