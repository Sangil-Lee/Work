#!/usr/bin/env python

import argparse
import epics
import numpy as np
import sys

parser = argparse.ArgumentParser(\
            description='Enable inputs on one or more acquistion nodes.',
            formatter_class=argparse.ArgumentDefaultsHelpFormatter)
parser.add_argument('-l', '--link', type=int, default=0, choices=range(0,8), help='Link number')
parser.add_argument('-n', '--node', type=int, default=0, choices=range(0,5), help='Node number')
parser.add_argument('-o', '--output', type=int, default=[0], choices=range(0,16), nargs='*', help='Output number')
parser.add_argument('-g', '--goodState', type=int, default=1, choices=[0,1], help='"Good" state')
parser.add_argument('-p', '--prefix', default='FP:1:', help='Record name prefix')
parser.add_argument('-v', '--verbose', action='store_true', help='Show bitmaps')
args = parser.parse_args()

# Useful constants
maxNodesPerLink = 5
maxLinks = 8
inputsPerNode = 64
bitmapSize = (maxNodesPerLink * maxLinks * inputsPerNode) / 32

# Initialize the various bitmaps
zeroBitmap = np.zeros(bitmapSize, dtype=np.uint32)
goodStateBitmap = np.zeros(bitmapSize, dtype=np.uint32)
importantBitmap = np.zeros(bitmapSize, dtype=np.uint32)

# Make all inputs on the selected node important
idx32 = ((args.node * 8) + args.link) * (inputsPerNode / 32)
for i in range(0, inputsPerNode/32):
    importantBitmap[idx32+i] = ~0;
    if (args.goodState):
        goodStateBitmap[idx32+i] = ~0;

if args.verbose:
    for n in range(0, maxNodesPerLink):
        for l in range(0, maxLinks):
            idx = ((n * maxLinks) + l) * 2
            goodLo = goodStateBitmap[idx]
            goodHi = goodStateBitmap[idx+1]
            importantLo = importantBitmap[idx]
            importantHi = importantBitmap[idx+1]
            print "%d %d %08X:%08X %08X:%08X" % (n, l, goodHi, goodLo, importantHi, importantLo)

for o in args.output:
    pref = args.prefix + 'O%02d' %(o)
    gName = pref + 'GoodState'
    iName = pref + 'Important'
    if (args.verbose): print "Set %s and %s" % (gName, iName)
    goodState = epics.PV(gName)
    important = epics.PV(iName)
    important.put(zeroBitmap)
    goodState.put(goodStateBitmap)
    important.put(importantBitmap)
