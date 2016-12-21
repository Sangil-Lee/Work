source ${HOME}/epics/R3.14.12.4/setEpicsEnv.sh

export GNU_DIR=${HOME}/rtems/gnu
export PATH=$GNU_DIR/autoconf-2.68/bin:$PATH
export PATH=$GNU_DIR/automake-1.11/bin:$PATH
export PATH=${GNU_DIR}/rtems-4.10/bin:/usr/local/bin:$PATH
export LD_LIBRARY_PATH=${GNU_DIR}/rtems-4.10/lib:$LD_LIBRARY_PATH
export RTEMS_ROOT=${HOME}/rtems/4.10.2
export TARGET_DIR=${RTEMS_ROOT}/tgt
export CC_FOR_HOST=gcc44
export CFLAGS_FOR_HOST="-g -O2 -std=gnu99"
export RTEMS_MAKEFILE_PATH=${TARGET_DIR}/powerpc-rtems4.10/mvme3100
