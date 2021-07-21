#!/usr/local/bin/python3.8
import sys
import os
import glob

filename = str(sys.argv[1])
seq    = open(filename, 'r')
seqnl = open(filename.rsplit('.',1)[0]+"NL.stt", 'w')

for line in seq:
    strline = line.rstrip('\n')
    strline = strline.replace('{', 'open')
    strline = strline.replace('}', 'close')
    strline = strline.replace('open', '{open}')
    strline = strline.replace('close', '{close}')
    strline = strline.replace('\\n', '\\\\n')
    strline = strline.replace('\'\\0\'', '{charnull}')
    strline = strline+'{nl}\\'
    seqnl.write(strline+'\n')

seq.close()
seqnl.close()

