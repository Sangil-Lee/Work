

## cross compiler set-up for arm64 cpu
1. sudo apt install build-essential
2. $ sudo apt install binutils-aarch64-linux-gnu

# gnu c compiler
3. $ sudo apt install gcc-aarch64-linux-gnu

# gnu c++ compiler
4. $ sudo apt install g++-aarch64-linux-gnu

# GDB
5. $ sudo apt install gdb-multiarch


####### Test Example ###################
vi hello.cpp
--------------------------
#include <iostream>
using namespace std;

int main() {
    cout << "Hello, World!" << endl;
    return 0;
}
--------------------------

$>arm-linux-gnueabihf-g++ -g -o hello hello.cpp
$>file hello
hello: ELF 32-bit LSB shared object, ARM, EABI5 version 1 (SYSV) ......

$>aarch64-linux-gnu-g++ -g -o hello hello.cpp
$>file hello
hello: ELF 64-bit LSB shared object, ARM aarch64, version 1 (SYSV) ......


############## Setup Compiler for make #####################
AR=${CROSS_COMPILE}ar
AS=${CROSS_COMPILE}as
LD=${CROSS_COMPILE}ld
CC=$(CROSS_COMPILE)gcc
CXX=$(CROSS_COMPILE)g++
NM=${CROSS_COMPILE}nm
RANLIB=${CROSS_COMPILE}ranlib

CFLAGS=""
CPPFLAGS=""
LDFLAGS=""
LIBS=""

hello : hello.cpp
        $(CXX) -g -o hello hello.cpp

clean:
        rm hello

#############################################
 32bit
export CROSS_COMPILE=arm-linux-gnueabi-
export ARCH=arm

# 32bit, GPU
export CROSS_COMPILE=arm-linux-gnueabihf-
export ARCH=arm

# 64bit
export CROSS_COMPILE=aarch64-linux-gnu-
export ARCH=arm64

make
##########################################
alias armmake='make -j8 ARCH=arm CROSS_COMPILE=arm-linux-gnueabi- '
alias arm64make='make -j8 ARCH=arm CROSS_COMPILE=aarch64-linux-gnu- '
