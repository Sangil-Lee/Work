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
#if !defined( __ts2iic_h__ )
#define __ts2iic_h__

#ifdef __cplusplus
extern "C" {
#endif

#include "inc/com_def.h"


#define DRV_NAME_iic_bp "/dev/i2c-1"
#define DRV_NAME_iic_ob "/dev/i2c-2"
#define DRV_NAME_iic_sl "/dev/i2c-4"
#define DRV_NAME_iic_sh "/dev/i2c-3"

#define IIC_BUS_bp (0)
#define IIC_BUS_ob (1)
#define IIC_BUS_sl (2)
#define IIC_BUS_sh (3)
#define IIC_BUS_MAX (3)

#define FLED_PWR_OK    0x01 
#define FLED_FAN_ERR   0x02 
#define FLED_EPICS     0x04
#define FLED_TIMNG_NET 0x08


#define REF_CLK_Ext     0
#define REF_CLK_ob25MHz 1


#define TS2IO_ModeInput  (0)
#define TS2IO_ModeOutput (1)


struct s_cpsConfig{
  unsigned char eqBoost0   ;
  unsigned char eqBoost1   ;
	unsigned char outP[16]    ;
	unsigned char outP_En[16] ;
} ;


STATUS ts2iic_init();
STATUS ts2iic_initClk(int option);
STATUS ts2iic_initCps(int busId);

STATUS ts2iic_setFanSpeed(char pwmIntensity );
STATUS ts2iic_setFLed(unsigned char ledV);
STATUS ts2iic_setSlv_IO(int busId, unsigned int ioMode);

STATUS ts2iic_setCpsConfig(int busId, struct s_cpsConfig* pCfg);
STATUS ts2iic_getCpsStat  (int busId, struct s_cpsConfig* pStat);

STATUS ts2iic_getSlv_ID(unsigned char* psl, unsigned char* psh);
STATUS ts2iic_getSlv_temp(float* psl, float* psh);




#ifdef __cplusplus
}
#endif
#endif
/*==============================================================================
                                  END OF FILE
==============================================================================*/
