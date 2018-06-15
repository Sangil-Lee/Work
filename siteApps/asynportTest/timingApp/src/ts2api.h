/*******************************************************************************
 *                                                                             *
 *  Copyright (c) 2014 ~ by DURUTRONIX. All Rights Reserved.                   *
 *                                                                             *
 ******************************************************************************/

/*==============================================================================
                        EDIT HISTORY FOR MODULE

when                who            what, where, why
------------------- -------------  ---------------------------------------------
2018-05-07 14:53:53 laykim         Create
==============================================================================*/
#if !defined( __ts2api_h__ )
#define __ts2api_h__

#ifdef __cplusplus
extern "C" {
#endif

#include "inc/com_def.h"

#include "ts2regmap.h"
#include "ts2ip.h"
#include "ts2iic.h"

struct s_ts2slv{
	unsigned char      id  ;	
  int                busId;
	float              temp;
	unsigned int       io  ;	
  struct s_cpsConfig cps ;
  struct s_cpsConfig cpsStat ;
};


struct s_ts2sys{
	int                 isInit  ;	

	int                 evMode  ;	
  struct s_ts2ip      ip      ;
  int                 ob_busId;
  struct s_cpsConfig  ob_cps  ;
  struct s_cpsConfig  ob_cpsStat  ;

  struct s_ts2slv     sl;
  struct s_ts2slv     sh;
};

STATUS ts2api_shellCmd ( int argc, char **argv );



STATUS ts2api_init( int evMode );
STATUS ts2api_getInstance(struct s_ts2sys* pSys);
STATUS ts2ip_rd(int fd, unsigned int offset, unsigned int *rdData);
STATUS ts2api_fin ( );


#ifdef __cplusplus
}
#endif
#endif
/*==============================================================================
                                  END OF FILE
==============================================================================*/
