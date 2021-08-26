#!/bin/bash

#echo "Type a hex number"
#read hexNum
#echo $((16#$hexNum))

deciVal=$((2#$1))
echo $deciVal
caput SRF01-Bunker1:HWRB01-Logic:IntLock $deciVal
