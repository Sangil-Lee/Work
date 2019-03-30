#!/usr/bin/python

from subprocess import Popen, PIPE

p1=Popen(["dmesg"], stdout=PIPE)

print p1.communicate()
