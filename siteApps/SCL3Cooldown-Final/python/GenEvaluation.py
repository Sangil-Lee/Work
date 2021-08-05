#!/usr/local/bin/python3

import re
from epics import PV


pv = PV('Cryo-CDL00:TBx01-CV7505:CDLogic.CALC')

calc = pv.get()
print(calc)

#calc = re.split('()|', calc)

print(calc)