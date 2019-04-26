#ifndef __TIME_H
#define __TIME_H

#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <sys/mman.h>
#include <fcntl.h>
#include <string.h>
#include <math.h>
#include <string.h>

/* c++ */
#include<iostream>
#include<fstream>
#include<sstream>
#include <vector>
#include <set>
#include <iterator>
#include <string>
#include <algorithm>

/* fps header */
#include "define.h"

/* base addr */
#define XPAR_BRAM_0_BASEADDR 0x40000000

#define XPAR_BRAM_1_BASEADDR 0x42000000

#define XPAR_DDR_0_BASEADDR 0x30000000

// #define XPAR_BRAM_1_BASEADDR 0x42000000
// #define XPAR_BRAM_1_HIGHADDR 0x40001FFF //256k

/* device extern declare ++ device.cpp */
extern unsigned int * CTR_CMD_MEM;     //bram0
extern unsigned int * CTR_CMD_MEM1;    //bram1
extern unsigned int * CTR_CMD_MEM2;    //bram2
extern unsigned int * CTR_CMD_MEM3;    //bram3
extern unsigned int * CTR_CMD_DDR;     //ddr

typedef struct _ACQUISITION{     //size(bit)
    // DAY day;                    //16
    unsigned short interlock;
    unsigned short day;         //9
    unsigned short hour;        //5
    unsigned short min;         //6
    unsigned short sec;         //6
    unsigned short msec_100;    //4
    unsigned short msec_10;     //4
    unsigned short msec_1;      //4
    unsigned int count;         //18
    unsigned long long Node_Status;  //64
}ACQ;


/* function declare */
int FPS_START();
int Create_Log(int Position, unsigned int *data);
// void PMS_EVENT(unsigned int *data);
// void MAKE_CSV(int int_pos, unsigned int *data);
// int FPS_START();

/* device.cpp include */

unsigned int Get_PL_CMD64(int cmd);
unsigned int Send_PL_CMD64(int cmd, unsigned int val);
void DEV_CTRL64(unsigned int** PTR,unsigned int BASE_ADDR, unsigned int page_size, int fd, int type);
void INIT_DEVICE64(unsigned int page_size, int *fd, int type);
void CLOSE_DEVICE(unsigned int page_size);
void DEV_RELEASE(unsigned int** PTR, unsigned int page_size);

#endif