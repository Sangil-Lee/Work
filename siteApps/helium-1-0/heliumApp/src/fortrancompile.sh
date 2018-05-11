#!/bin/bash

MACHINE_TYPE=`uname -m`
DIRNAME=''
if [ ${MACHINE_TYPE} == 'x86_64' ]; then
	DIRNAME='O.linux-x86_64'
else
	DIRNAME='O.linux-x86'
fi

if [ ! -d ${DIRNAME} ]; then
	mkdir ${DIRNAME}/
fi

g++ -c -fPIC Dnst6501.f Dnst6505.f  TempK5301.f DnstL5301.f Dnst5301.f -lgfortran
mv Dnst6501.o Dnst6505.o TempK5301.o DnstL5301.o Dnst5301.o ${DIRNAME}/
