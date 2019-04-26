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
            description='Show fast protection system monitors.',
            formatter_class=argparse.ArgumentDefaultsHelpFormatter)
parser.add_argument('-p', '--prefix', default='FP:1:', help='Record name prefix')
args = parser.parse_args()

def showValue(pvname=None, value=None, **kw):
    seconds = kw['posixseconds']
    microseconds = (kw['nanoseconds'] + 500) / 1000
    pvTime = datetime.datetime.fromtimestamp(seconds)
    timeString = pvTime.strftime('%Y-%m-%d %H:%M:%S')
    sevr = ('', ' MINOR', ' MAJOR', ' INVALID')[kw['severity']]
    print '%s.%06d %-30s %10g%s' % (timeString, microseconds, pvname, value, sevr)

def addPV(name):
        epics.PV(name,
                 form='time',
                 auto_monitor=epics.dbr.DBE_VALUE|epics.dbr.DBE_ALARM,
                 callback=showValue)

def addFPGA(name):
    for suffix in ('FPGA:Temperature', 'FPGA:VccINT', 'FPGA:VccAUX', \
                   'FPGA:VccBRAM', 'FPGA:VccPINT', 'FPGA:VccPAUX',   \
                   'FPGA:VccDDR', 'FanSpeed'):
        print args.prefix + name + suffix
        addPV(args.prefix + name + suffix)
addFPGA('M:')
for l in range(0,len(nodesPerLink)):
    for n in range(0,nodesPerLink[l]):
        addFPGA('L%XN%X:' % (l, n))

def addSFP(name):
    for suffix in ('RxPower', 'Temperature'):
        addPV(args.prefix + name + suffix)

addSFP('M:SFP:EVR:')
for l in range(0,len(nodesPerLink)):
    if (nodesPerLink[l] != 0):
        addSFP('M:SFP:L%X:' % (l))
for l in range(0,len(nodesPerLink)):
    for n in range(0,nodesPerLink[l]):
        addSFP('L%XN%X:SFP0:' % (l, n))
        if (n < (nodesPerLink[l]-1)): addSFP('L%XN%X:SFP1:' % (l, n))

for s in ('15_00', '31_16', '39_32'):
    addPV(args.prefix + 'StaleNodes' + s)

while True:
    time.sleep(10)
