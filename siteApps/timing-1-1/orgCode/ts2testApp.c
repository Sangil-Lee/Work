//============================================================================//
//                                                                            //
//  Copyright (c) 2014 ~ by DURUTRONIX. All Rights Reserved.                  //
//                                                                            //
//============================================================================//

/*==============================================================================
                        EDIT HISTORY FOR MODULE

when                who            what, where, why
------------------- -------------  ---------------------------------------------
2018-05-07 14:53:53 laykim         Create
==============================================================================*/

//==============================================================================
//----===@ include header
//==============================================================================
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/ioctl.h>
#include <string.h>
#include <signal.h>
#include <pthread.h>

//#include "inc/com_def.h"
#include "ts2regmap.h"
#include "ts2ip.h"
#include "ts2iic.h"
#include "ts2api.h"


//------------------------------------------------------------------------------
//----===@ main
//------------------------------------------------------------------------------
int main(int argc, char *argv[])
{
	int evMode;

  prnM2("======================================================\r\n");
  prnM2("raon ts2 api test app.\n");
  prnM2("======================================================\r\n");

	if(argc < 2)
	{
		prnM2("Usage: raon {1 [evg] | 2 [evr] | 3 [evf] | 4 [evs] | 9 [ev system test mode]}\n");
		return RET_ERR;
	}

	evMode = atoi(argv[1]);
  prnM2("cmdline args: %s %d\r\n", argv[0], evMode);

  ts2api_init(evMode);

  ntshell_main();

  ts2api_fin();
  
  taskDelay(1500000);
  prnM2("Good-bye!\n");
  return RET_OK;

}













