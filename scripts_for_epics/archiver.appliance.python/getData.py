#!/usr/bin/python
# coding=utf-8 
#
# Shell   : getData.py
# Author  : Jeong Han Lee
# email   : jhlee@ibs.re.kr
# Date    : Monday, March 30 09:36:19 KST 2015
# Version : 0.2.0
#
#   * I intend to develop this script in order to extact or get data from
#     Archiver Appliance Service, which is running on an Ethernet accessiable
#     server. This script creates a file per a PV. In one example,
#     
#    # Filename    : /tmp/pi2_dht11_tem.txt
#    # PV name     : PI2:DHT11:TEM
#    # From        : 2014-12-15T19:15:02.200201
#    # To          : 2014-12-22T19:15:02.200201
#    # queryString : ?pv=mean_300(PI2:DHT11:TEM)&from=2014-12-15T19%3A15%3A02.200201%2B09%3A00&to=2014-12-22T19%3A15%3A02.200201%2B09%3A00
#    # hostname    : kaffee
#    # host IP     : 10.1.4.24
#    # 
#    # time, val, nanos, status, severity
#    2014-12-19T11:07:30, 22.4814814815, 0, 0, 0 
#    2014-12-19T11:12:30, 22.45, 0, 0, 0 
#    2014-12-19T11:17:30, 21.6279069767, 0, 0, 0 
#    2014-12-19T11:22:30, 21.4333333333, 0, 0, 0 
#    2014-12-19T11:27:30, 21.4936708861, 0, 0, 0 
#    2014-12-19T11:32:30, 21.4791666667, 0, 0, 0 
#
#     By default, time is the mean over 5 mins. 
#    
#  * The created files are located in /tmp/ and copy them in to /var/www/data directory.   
#    In there, another scripts read these files, and generate a *static* web site by using
#    SIMILE Timeplot. See http://www.simile-widgets.org/timeplot/
#
#
#  - 0.0.0  Friday, December 19 10:18:02 KST 2014
#           Created.
#  - 0.1.1  Monday, December 22 19:19:27 KST 2014
#           Real Working Script...
#
#  - 0.2.0  Monday, March 30 09:36:52 KST 2015, jhlee
#           - Export the selected PV lists based on the input PV list (as input file)
#           - Clean up some lines in code, such as the argument default values, unused variables, 

#
#
#
#
#
#     crontab -e, add the following line
#
#* *  * * * export DISPLAY=:0.0 && /usr/bin/python /where/the/script/is/getData.py >/dev/null 2>&1
# */5 *  * * * export DISPLAY=:0.0 && /usr/bin/python /home/jhlee/programming/scripts/python/archiver.appliance/getData.py -i 10.1.4.173 -d 7 >/dev/null 2>&1
#*/5 *  * * * export DISPLAY=:0.0 && /usr/bin/python /home/sjkim/scripts_for_epics/archiver.appliance.python/getData.py -i 164.125.122.212 -d 7  >/dev/null 2>&1
import os
import sys
import argparse 
import socket
import shutil
# import numpy as np
#from chaco.shell import *

import urllib
import urllib2
import json
from datetime import timedelta, datetime, date


# epoch_secs : 
# This is the Java epoch seconds of the EPICS record processing timestamp. The times are in UTC; so any conversion to local time needs to happen at the client. 

def convertDate(epoch_secs):
    _date = datetime.fromtimestamp(epoch_secs)
    return _date.isoformat()


def setMGMTurl(url):
    return url + "/mgmt/bpl/"


def print_data_info(element):
    #  {u'nanos': 887037220, u'status': 0, u'secs': 1418979266, u'severity': 0, u'val': 24.0}
#    print element.nanos
    return 

def setJsonRetUrl(url):
    return url + "/retrieval/data/getData.json"

def setRawRetUrl(url):
    return url + "/retrieval/data/getData.raw"



def getSelectedPVs(url, args):

    pv_list = []
    applianceMGMTUrl= url + "/mgmt/bpl/getAllPVs"
    #
    #   Want to use "list" because pv_list, which is "return values from AA" is list
    #    
    script_path = os.path.dirname(os.path.realpath(__file__))
#    input_filename = os.getcwd() +"/" + args.file
    input_filename = script_path + "/" + args.file

    lines = [line.strip() for line in open(input_filename)]

    #    print type(lines),  lines
    #    print "mgmturl  :", applianceMGMTUrl
    #    print patterns.split()

    for args.pattern in args.pattern.split():
        #      print urllib.urlencode({"pv" : args.pattern})

        resp = urllib2.urlopen(url= applianceMGMTUrl + "?" + urllib.urlencode({"pv" : args.pattern}))

        matchingPVs = json.load(resp)

        pv_list.extend(matchingPVs)


#    print type(lines), pv_list
#    print set(pv_list).intersection(set(lines))
#
#   https://docs.python.org/2/library/stdtypes.html#set.intersection
#   Compare two lists in python and return matches
#
    return set(pv_list).intersection(set(lines))




def main():

    #   https://docs.python.org/2/howto/argparse.html
    parser = argparse.ArgumentParser()

    parser.add_argument("-i", "--ip",      help="Archiver Appliance IP address", default="10.1.5.14")
    parser.add_argument("-p", "--pattern", help="PVs patterns, case sensitive",  default ="*")
    parser.add_argument("-v", "--verbose", help="output verbosity",              action="store_true")
    parser.add_argument("-d", "--days",    help="days to monitor from now", type=float, default=7.0)
    parser.add_argument("-f", "--file",    help="filename which has selected PV list",  default="munji_pv_list")

    args = parser.parse_args()

    # if not args.pattern :
    #     args.pattern = "*"

    url = "http://" + args.ip + ":17665"

    if args.verbose:
        print ""
        print ">>>" 
        print ">>> Default URL and Pattern are used as follows:"
        print ">>>  URL :" + url
        print ">>>  Pattern : " + args.pattern
        print ">>>"
        

    matchingPVs = []
    matchingPVs = getSelectedPVs(url, args)
    

    if matchingPVs:

        _now  = datetime.now()
        _from = _now - timedelta(days=args.days)

        _from_iso_string = _from.isoformat()
        _now_iso_string  = _now.isoformat()
        
        fromString       = urllib.urlencode( {'from' : _from_iso_string} ) 
        toString         = urllib.urlencode( {'to'   : _now_iso_string } )

        #   userString = urllib.urlencode( {'userreduced' : "true"} )
        
        #    _from        = datetime(2014,12,19,17,40,00,00)
        #    _from_string = _from.strftime("%Y-%m-%dT%H:%M:%S")
        #    _now_string  = _now.strftime ("%Y-%m-%dT%H:%M:%S")
        #    "From" and "To" have the iso time format at  http://epicsarchiverap.sourceforge.net/userguide.html
        #    Python datetime has the isoformat at https://docs.python.org/2/library/datetime.html
        #    Monday, December 22 13:42:51 KST 2014, jhlee
        

        
        #   Still don't understand what the following Strings means,
        #   get the structure form archiveViewer, and simply add only 
        #   magicString to queryString  
        #  
        #   Monday, December 22 10:40:19 KST 2014, jhlee
        #
        magicString = "%2B09%3A00"
        # http://en.wikipedia.org/wiki/Percent-encoding
        # %2B : "+"
        # %3A : ":"
        # userString  = "&usereduced=true"
        # cahowString = "&ca_how=0"
        # cacountString = "&ca_count=1907"
        
        suffixString = magicString
        # suffixString = magicString# + userString + cahowString + cacountString
        
        if args.verbose:
            print "fromString : ",  fromString
            print "toString   : ",  toString
        #        print "userString : ",  userString
            print ""
            
        hostname = socket.gethostname() 
        hostip   = socket.gethostbyname(hostname)
            
        report_filename = ""
        dest_directory = "/var/www/data/"
        
        for pv in sorted(matchingPVs):

            report_filename = "/tmp/" + pv.replace(":", "_").lower() + ".txt"
            #       locat test 
#            report_filename = "tmp/" + pv.replace(":", "_").lower() + ".txt"
            
            queryString  = '?pv=mean_300(' + pv + ')'
            queryString += '&'
            queryString += fromString 
            queryString += magicString
            queryString += '&'
            queryString += toString 
            queryString += suffixString
            
            dataresp = urllib2.urlopen(setJsonRetUrl(url) + queryString)
            data     = json.load(dataresp)

            # print data
            # print "Total Data Size " , len(data[0]['data'])
            # print ">>> ", data[0]['data'][0]
            #        print data[0]['data']
            # # typeInfo = json.load(urllib2.urlopen(setMGMTurl(url) + 'getPVTypeInfo?' + queryString))
            # # if typeInfo: 
            # #     dataStores = typeInfo['dataStores']
            # #     print dataStores
            
            try :
                file = open(report_filename, "w")
                file.write("# \n")
                file.write("# Filename    : " + report_filename  + "\n")
                file.write("# PV name     : " + pv               + "\n")
                file.write("# From        : " + _from_iso_string + "\n")
                file.write("# To          : " + _now_iso_string  + "\n")
                file.write("# queryString : " + queryString      + "\n")
                file.write("# hostname    : " + hostname         + "\n")
                file.write("# host IP     : " + hostip           + "\n")
                file.write("# \n")
                file.write("# time, val, nanos, status, severity    \n")
                
                dataList = []

                for el in data[0]['data']:
                    #               print type(el) ; returns <type 'dict'> 
                    #               print el; returns {u'nanos': 887056444, u'status': 0, u'secs': 1418979584, u'severity': 0, u'val': 25.0}
                    #               print "%s, %s  \n" % (el['secs'], datetime.fromtimestamp(el['secs']))
                    dataList.append("%s, %s, %s, %s, %s \n" % (convertDate(el['secs']), el['val'], el['nanos'], el['status'], el['severity']))
                    
                s = ''.join(dataList)

                file.write(s)
                file.close()

#           need to comment out, if one wants to test this script with user accounts
#           Monday, March 30 09:32:22 KST 2015, jhlee
# 
                shutil.copy (report_filename, dest_directory)
#

            except IOError, (errno, strerror):
                print "I/O error(%s): %s" % (errno, strerror)


    sys.exit()

if __name__ == '__main__': main()

