#!/usr/bin/env python

import argparse
import datetime
import epics
import math
import sys
import time

###############################################################################
# Declare system configuration: number of outputs, number of nodes per link
outputCount = 16
nodesPerLink = [1, 2, 0, 0, 0, 0, 0, 0]

###############################################################################
# No user-servicable parts beyond this point!
parser = argparse.ArgumentParser(\
            description='Show fast protection I/O points.',
            formatter_class=argparse.ArgumentDefaultsHelpFormatter)
parser.add_argument('-p', '--prefix', default='FP:1:', help='Record name prefix')
args = parser.parse_args()

def showValue(pvname=None, value=None, **kw):
    seconds = kw['posixseconds']
    if seconds:
        nanoseconds = kw['nanoseconds']
        pvTime = datetime.datetime.fromtimestamp(seconds)
        timeString = pvTime.strftime('%Y-%m-%d %H:%M:%S')
    else:
        timeString = '                   '
    sevr = ('', ' MINOR', ' MAJOR', ' INVALID')[kw['severity']]
    print '%s.%09d %-30s %3g%s' % (timeString, nanoseconds, pvname, value, sevr)

# Inputs
for l in range(0,len(nodesPerLink)):
    for n in range(0,nodesPerLink[l]):
        for i in range(0,64):
            name = 'L%XN%XI%2.2d' % (l, n, i)
            epics.PV(args.prefix + name, form='time',
                           auto_monitor=epics.dbr.DBE_VALUE|epics.dbr.DBE_ALARM,
                           callback=showValue)
# Outputs
for o in range(0,outputCount):
    name = 'Mitigate%2.2d' % (o)
    epics.PV(args.prefix + name, form='time', callback=showValue)

while True:
    time.sleep(10)
