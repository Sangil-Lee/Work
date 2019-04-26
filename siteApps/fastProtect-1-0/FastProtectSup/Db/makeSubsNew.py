#!/usr/bin/env python

import sys
import argparse

print 'hello'

###############################################################################
# Declare system configuration: number of outputs, number of nodes per link
outputCount = 16
nodesPerLink = [1, 2, 0, 0, 0, 0, 0, 0]

###############################################################################
# No user-servicable parts beyond this point!
parser = argparse.ArgumentParser(\
            description='Create fast protection database substitution file.',
            formatter_class=argparse.ArgumentDefaultsHelpFormatter)
parser.add_argument('outputName', nargs=1)
args = parser.parse_args()
aHi = 0x50000

sys.stdout = open(args.outputName[0], 'w')

#
# Interlock inputs
#
linkCount = 0
print 'file "perInterlock.template" {'
print "  pattern { P, R, PORT, L, N, Id, Sa }"
for l in range(0,len(nodesPerLink)):
    if ((nodesPerLink[l] < 0) or (nodesPerLink[l] > 5)):
        print >>sys.stderr, "Bad number of nodes specified for link %d" % (l) 
    if (nodesPerLink[l] != 0): linkCount += 1
    for n in range(0,nodesPerLink[l]):
        for i in range(0,64):
            print '  {"\\$(P)", "\\$(R)", "\\$(PORT)", %x, %x, %02d %02X}' % (l, n, i, ((n<<3)|l)<<8|i)
print '}'

#
# Mitigation outputs
#
print 'file "perOutput.template" {'
print "  pattern { P, R, PORT, Od, Oh }"
for o in range(0,outputCount):
    print '  {"\\$(P)", "\\$(R)", "\\$(PORT)", %02d %02X}' % (o, o)
print '}'

#
# SFP modules
# See comments in FastProtectSup.c for description of ASYN address value
#
def makeSFP(name, idx):
    idx |= 0x50000
    print ' {"\\$(P)", "\\$(R)", "\\$(PORT)", "%s", 0x%X, 0x%X}' % \
                                                            (name, idx, idx + 1)
print 'file "perSFP.template" {'
b = 328
print "  pattern { P, R, PORT, NAME, RX_IDX, T_IDX }"
# SFPs on mitigation node
#  EVR
makeSFP ("M:SFP:EVR", b)
b += 2
#  Links
for l in range(0,len(nodesPerLink)):
    if (nodesPerLink[l] != 0):
        i = b + (l * 2)
        makeSFP("M:SFP:L%X" % (l), i)
# SFPs on acquisition node(s)
for l in range(0,len(nodesPerLink)):
    for n in range(0,nodesPerLink[l]):
        i = b + (l * 2) + (n * 32) + 16
        makeSFP("L%XN%X:SFP0" % (l, n), i)
        if (n < (nodesPerLink[l]-1)): makeSFP("L%XN%X:SFP1" % (l, n), i + 16)
print '}'

#
# Per-FPGA records
#
def makeFPGA(name, base):
    print ' {"\\$(P)", "\\$(R)", "\\$(PORT)", "%s"' % (name),
    for i in range(0,8):
        print ', 0x%x' % (aHi + base + i),
    print '}'
print 'file "perFPGA.template" {'
print '  pattern { P, R, PORT, NAME, A0, A1, A2, A3, A4, A5, A6, A7 }'
makeFPGA('M:', 0)
for l in range(0,len(nodesPerLink)):
    for n in range(0,nodesPerLink[l]):
        makeFPGA('L%XN%X:' % (l, n), ((n * 8 + l) * 8) + 8)
print '}'

#
# Per-IOC records
#
#print 'file "perIOC.template" {'
#print "  pattern { P, R, PORT }"
#print '  {"\\$(P)", "\\$(R)", "\\$(PORT)"}'
#print '}'
#
## Declare system configuration: number of outputs, number of nodes per link
#outputCount = 16
#links = 8
#nodes_per_link = 5
#
#node_ranges = {0:"15_00", 1:"31_16", 2:"39_32"}
#
#print 'file "opiOutput.template" {'
#print "  pattern { P, R, Od, El, Id }"
#for out in range(0, outputCount):
#    for l in range(0, 80):
#        print '  {"\\$(P)", "\\$(R)", "%02d", "%02d", "%d"}' % (out, l, l)
#print '}'
#
#
