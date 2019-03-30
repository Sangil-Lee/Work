#!/bin/bash
# Shell  : epics_chanarch.sh
# Author : Jeong Han Lee
# email  : jhlee@ibs.re.kr
# Date   : Tuesday, February  4 23:29:02 KST 2014
# version : 0.0.1
#
#    * This script may give a little room to install again-and-again
#      Classic EPICS Channel Archiver located at
#      http://sourceforge.net/projects/epicschanarch/
# 
#
#      I hope it would reduce our time to concentrate real works
#      instead of painful modification of unmaintained source codes.
# 
#   * Must install the following packages via root or sudo
#     before running this script
#
#     For Debian Wheezy,
# 
#      aptitude install libxmlrpc-c++4-dev  libxerces-c-dev  libcurl3-dev 
#
#     Of course, EPICS base, and its extension must be installed
#
#
#
#  - 0.0.1  Wednesday, February  5 00:47:14 KST 2014, jhlee
#           * created
#




# cq   : quiet 
# c    : continue getting a partially-downloaded file. So it allows us not to re-download an existent file
# 
wget_options="wget -c"
# xzf  : quiet
# xzfv : verbose
tar_command="tar xzf"

# aptitude install libxmlrpc-c++4-dev  libxerces-c-dev  libcurl3-dev 

make_epicschanarch()
{

    chanarch_dir=$1
    cd ${chanarch_dir}
    echo $PWD
    echo ""
    make_cfg="${chanarch_dir}/make.cfg"
    sed -i~ -e 's/LOCAL=\/usr\/local/LOCAL=\/usr\//g' ${make_cfg}


    # modify Tools in order to compile them correctly
    # Tuesday, February  4 18:42:16 KST 2014, jhlee

    tools_dir=${chanarch_dir}/Tools/
    sed -i~ '$ a\#include <cstdio>\n#include <cstdlib>\n#include <cstdarg>' ${tools_dir}/ToolsConfig.h
    sed -i~ '18istatic int sort_compare(const int &a, const int &b)\n{\n  return b-a;\n}\n\nstatic const char *toString(const int &i)\n{\n  static char txt[10];\n  sprintf(txt, "%d", i);\n return txt;\n }\n' ${tools_dir}/AVLTree.h
    sed -i~ '18,23d'  ${tools_dir}/AVLTreeTest.cpp
    sed -i '5,6d'     ${tools_dir}/AVLTreeTest.cpp

    # modify LibIO
    # error: ‘LONG_MAX’ was not declared in this scope >> BinValue.cpp
    #./DataFile.h:131:5: error: ‘DataHeaderIterator’ does not name a type  >> DataFile.cpp
    # 
    libIO_dir=${chanarch_dir}/LibIO/
    sed -i~ '7i#include <climits>\n#include "BinChannelIterator.h"' ${libIO_dir}/BinValue.h


    # modify Storage
    # ../NameHash.cpp:4:23: warning: extra tokens at end of #include directive [enabled by default]

    storage_dir=${chanarch_dir}/Storage/
    sed -i~ -e 's/.h>./.h>/g' ${storage_dir}/NameHash.cpp 

    # modify XMLRPCServer
    # Cannot run or decode 'xmlrpc-c-config' 
    # ./main_standalone.cpp:81:101: warning: deprecated conversion from string constant to ‘char*’ [-Wwrite-strings]

    xmlrpcserver_dir=${chanarch_dir}/XMLRPCServer/

    sed -i~ -e 's/1.0/1./g'                       ${xmlrpcserver_dir}/xmlrpc-config-wrapper
    sed -i~ -e 's/"archiver./(char*)"archiver./g' ${xmlrpcserver_dir}/main_standalone.cpp
    sed -i  -e 's/"S:/(char*)"S:/g'               ${xmlrpcserver_dir}/main_standalone.cpp
    sed -i  -e 's/"A:/(char*)"A:/g'               ${xmlrpcserver_dir}/main_standalone.cpp
    sed -i  -e 's/"Get /(char*)"Get /g'           ${xmlrpcserver_dir}/main_standalone.cpp
 
    # Engine
    # ../hammer.cpp:28:17: error: ‘stderr’ was not declared in this scope
    # ../hammer.cpp:29:58: error: ‘fprintf’ was not declared in this scope
    engine_dir=${chanarch_dir}/Engine/
    sed -i~ -e 's/#include <string>/#include <cstdio>\n#include <cstdlib>\n#include <cstring>\n#include <iostream>/g' ${engine_dir}/hammer.cpp

    
    cd ${chanarch_dir}
    make 
}

chanarch_name="ChannelArchiver"
EXT_SRC=${EPICS_EXTENSIONS}/src


chanarch_filename=epicschanarch.tar.gz
cd ${EXT_SRC}

$wget_options -O ${chanarch_filename} http://epicschanarch.cvs.sourceforge.net/viewvc/epicschanarch/?view=tar
$tar_command ${chanarch_filename} --strip-components=1

#cd ${EXT_SRC}/${chanarch_name}

make_epicschanarch "${EXT_SRC}/${chanarch_name}"