#!/bin/bash

MACHINE_TYPE=`uname -m`
DIRNAME=''
if [ ${MACHINE_TYPE} == 'x86_64' ]; then
	DIRNAME='O.linux-x86_64'
elif [ ${MACHINE_TYPE} == 'x86' ]; then
	DIRNAME='O.linux-x86'
elif [ ${MACHINE_TYPE} == 'armv7l' ]; then
	DIRNAME='O.linux-arm'
fi

if [ ! -d ${DIRNAME} ]; then
	mkdir ${DIRNAME}/
fi

g++ -c -fPIC Dnst_Enthalpy.f -lgfortran
mv Dnst_Enthalpy.o ${DIRNAME}/
