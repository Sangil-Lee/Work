/*
* Placeholder PetaLinux user application.
*
* Replace this with your application code

* Copyright (C) 2013 - 2016  Xilinx, Inc.  All rights reserved.
*
* Permission is hereby granted, free of charge, to any person
* obtaining a copy of this software and associated documentation
* files (the "Software"), to deal in the Software without restriction,
* including without limitation the rights to use, copy, modify, merge,
* publish, distribute, sublicense, and/or sell copies of the Software,
* and to permit persons to whom the Software is furnished to do so,
* subject to the following conditions:
*
* The above copyright notice and this permission notice shall be included
* in all copies or substantial portions of the Software.
*
* Use of the Software is limited solely to applications:
* (a) running on a Xilinx device, or (b) that interact
* with a Xilinx device through a bus or interconnect.
*
* THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
* IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
* FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT.
* IN NO EVENT SHALL XILINX  BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY,
* WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN
* CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.
*
* Except as contained in this notice, the name of the Xilinx shall not be used
* in advertising or otherwise to promote the sale, use or other dealings in this
* Software without prior written authorization from Xilinx.
*
*/

#include <iostream>
using namespace std;

#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include "ntshell_main.h"



#include "commonDefine.h"
#include "timingData.h"
#include "timingDriver.h"




using timing::timingDriver;

timing::timingDriver *ptiming;



//------------------------------------------------------------------------------
//----===@ ts2 thread
//------------------------------------------------------------------------------

pthread_t m_Tid;
void* threadFunc( void* pTr );

STATUS threadStart(void* pTr )
{
  int rc = pthread_create (&m_Tid, NULL, threadFunc, pTr);
  ifRet( rc != 0 );
  prnM2("threadStart\r\n");
}

void threadStop()
{
  prnM2("threadStop\r\n");
  pthread_cancel (m_Tid);
  prnM2("threadStop\r\n");
}


void* threadFunc( void* pTr )
{
  prnM2("threadFunc1\r\n");
  ((timing::timingDriver *)pTr)->start_thread();
  prnM2("threadFunc2\r\n");
  pthread_exit(0);
  prnM2("threadFunc3\r\n");
}

int timingAppTest(int argc, char *argv[])
{
  ifRet(argc < 2);

  ptiming->test(argc,argv);

  return RET_OK;
}

static unsigned int convert(char *st) {
  char *x;
  for (x = st ; *x ; x++) {
    if (!isdigit(*x))
      return 0L;
  }
  return (strtoul(st, 0L, 10));
}

//==============================================================================
//----===@ main
// Parameters  :
// Description :
//==============================================================================

int main(int argc, char *argv[])
{
  int evMode;
  int evNum;
  ifRet(argc<3);
  
  evMode = convert(argv[1]);
  evNum = convert(argv[2]);

  ifRet(evMode>RAON_MODE_MAX);

  ptiming = new timingDriver(evMode,evNum);

  ntshell_setCmd((char*)"do" ,(char*)"timingDriver.xxx()" , (void (*)(int, char**))timingAppTest);
  
  ifRet(ptiming->isInit != 1);
  threadStart(ptiming);
  
  ntshell_main();

  delete ptiming;

  taskDelay(3*1000000);
  threadStop();

  return 0;
}


