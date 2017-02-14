#!/usr/bin/env python

#
# Simple-minded script to dump log file contents
#

import argparse
import datetime
import io
import struct
import sys
import time

#
# Parse arguments
#
epilog = 'Columns:\n'                                                       \
         '   1 - ISO-format date YYYY-MM-DD\n'                              \
         '   2 - ISO-format time HH-MM-SS.xxx\n'                            \
         '   3 - Mitigation outputs\n'                                      \
         '   4 - Input number offset (N in following expressions, 0 to 2496)\n'\
         '   5 - Input 63+N:48+N states\n'                                  \
         '   6 - Input 47+N:32+N states\n'                                  \
         '   7 - Input 31+N:16+N states\n'                                  \
         '   8 - Input 15+N:N  states\n'                                    \
         '   9 - Transition(s) present on inputs 63+N:48+N\n'               \
         '  10 - Transition(s) present on inputs 47+N:32+N\n'               \
         '  11 - Transition(s) present on inputs 31+N:16+N\n'               \
         '  12 - Transition(s) present on inputs 15+N:N\n'                  \
         'Column 3 contains "****" to indicate that communication between\n'\
         'mitigation node and the IOC was lost at this time.\n'             \
         'Columns 4 through 12 are omitted if the log entry is present only\n' \
         'due to a change of state of one or more mitigation outputs.\n'    \
         'Inputs, outputs and transitions are shown in hexadecimal\n'

parser = argparse.ArgumentParser(description='Show log file as ASCII table.',
                        formatter_class=argparse.RawDescriptionHelpFormatter,
                        epilog=epilog)
parser.add_argument('-e', '--evr', type=float, default=125e6, help='EVR symbol rate (Hz), default 125e6')
parser.add_argument('-t', '--tail', action='store_true', help='Remain active upon reaching end of last file')
parser.add_argument('-U', '--UTC', action='store_true', help='Show time stamps in UTC rather than local time zone')
parser.add_argument('filenames', nargs='*')
args = parser.parse_args()
nsPerTick = 1e9 / args.evr

def fetchLog(f, buf, waitForMore):
    loc = f.tell()
    while True:
        n = f.readinto(buf)
        if (n == 7*4): return True
        if not waitForMore: return False
        time.sleep(1)
        f.seek(loc)

fileCount = len(args.filenames)
fileIndex = 0
for fname in args.filenames:
    buf = bytearray(7*4)
    f = io.open(fname, mode='rb')
    fileIndex += 1
    while fetchLog(f, buf, args.tail if fileIndex == fileCount else False):
        (h, seconds, ticks, iHi, iLo, tHi, tLo) = struct.unpack_from('<7I', buf)
        outputs = h & 0xFFFF
        ns = int(ticks * nsPerTick)
        if (ns > 1000000000):
            seconds += ns / 1000000000
            ns %= 1000000000
        if (args.UTC):
            sampleTime = datetime.datetime.utcfromtimestamp(seconds)
        else:
            sampleTime = datetime.datetime.fromtimestamp(seconds)
        dt = sampleTime.strftime('%Y-%m-%d %H:%M:%S')
        print '%s.%09d' % (dt, ns),
        if h & (1 << 31):
            print '****'
        elif (h & 0x400000) == 0:
            print '%04X' % (outputs)
        else:
            acqNodeIndex = (h >> 16) & 0x3F
            inputOffset = acqNodeIndex * 64
            print '%04X %4d %04X %04X %04X %04X %04X %04X %04X %04X' % (
                            outputs, inputOffset,
                            iHi >> 16, iHi & 0xFFFF, iLo >> 16, iLo & 0xFFFF,
                            tHi >> 16, tHi & 0xFFFF, tLo >> 16, tLo & 0xFFFF)
    f.close()
