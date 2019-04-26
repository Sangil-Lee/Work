#!/usr/bin/env python

import epics
import sys
import time

resetPV00 = epics.PV("FP:1:EnableOutput")

while (True):
    resetPV00.put(1)
    print "reset now"
    time.sleep(1.0)

