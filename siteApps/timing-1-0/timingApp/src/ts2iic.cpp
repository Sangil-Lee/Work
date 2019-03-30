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
#include <linux/i2c-dev.h>

#include "register_map.h"
#include "ts2iic.h"
#include "ts2ip.h"

/************************** Constant Definitions *****************************/
#define IIC_SLAVE_FORCE 0x0706
#define IIC_SLAVE	      0x0703	/* Change slave address			*/
#define IIC_FUNCS	      0x0705	/* Get the adapter functionality */
#define IIC_RDWR	      0x0707	/* Combined R/W transfer (one stop only)*/

#define MAX_IIC_RW_BUF_SIZE (512)

#define IIC_maxDrv (4)
#define IIC_maxDrvNameL (15)

/* Slave IIC Device Number... exclude IIC MUX */
#define IIC_maxSlv_bp (1)
#define IIC_maxSlv_ob (4)
#define IIC_maxSlv_sl (4)
#define IIC_maxSlv_sh (4)

/* slv iic name by num*/
#define SLV_MAX7313_A (0)
#define SLV_SI5338A   (1)
#define SLV_SI570		  (2)
#define SLV_ADN4604   (3)
#define SLV_INA226A   (4)
#define SLV_MAX7313_B (5)
#define SLV_TMP112A   (6)

#define IIC_MUX_ADDR  (0x74) // PCA9546

/************************** si5338 *****************************/
#define IIC_SCLK_RATE	  400000
#define IIC_ADDR				0x70
#define IIC_BUS					0

#define LOS_MASK_IN1IN2IN3		0x04
#define LOS_MASK				LOS_MASK_IN1IN2IN3
#define PLL_LOL					0x10
#define LOS_FDBK				0x08
#define LOS_CLKIN				0x04
#define SYS_CAL					0x01
#define LOCK_MASK				(PLL_LOL | LOS_CLKIN | SYS_CAL)
#define FCAL_OVRD_EN			0x80
#define SOFT_RESET				0x02
#define EOB_ALL					0x10
#define DIS_LOL					0x80

//==============================================================================
//----===@ structure
//==============================================================================

typedef struct
{
	unsigned int  name;
	unsigned char addr;
	unsigned char isMux;
	unsigned char muxAddress;
	unsigned char muxNum;
}s_ts2iic;

typedef struct
{
	int       isInit;
	int       fd    [IIC_maxDrv];
	int       isOpen[IIC_maxDrv];
	int       slvNum[IIC_maxDrv];
	s_ts2iic* piic  [IIC_maxDrv];
}s_ts2iicSys;

//==============================================================================
//----===@ global variable
//==============================================================================

static char drvName[IIC_maxDrv][IIC_maxDrvNameL] =
{
	{DRV_NAME_iic_bp},
	{DRV_NAME_iic_ob},
	{DRV_NAME_iic_sl},
	{DRV_NAME_iic_sh}
};

/* Back plane IIC Slave Information */
static s_ts2iic iicSlvs_bp[IIC_maxSlv_bp] = 
{
	{SLV_MAX7313_A, 0x18, 0, 0, 0}				// MAX7313(0x18)
};

/* on board IIC Slave Information */
static s_ts2iic iicSlvs_ob[IIC_maxSlv_ob] = 
{
	{SLV_SI5338A, 0x70, 0,           0, 0},	
	{SLV_SI570  , 0x5D, 1, IIC_MUX_ADDR, 1},
	{SLV_ADN4604, 0x4B, 1, IIC_MUX_ADDR, 8},
	{SLV_INA226A, 0x40, 1, IIC_MUX_ADDR, 4}
};

/* slave board Low floor IIC Slave Information */
static s_ts2iic iicSlvs_sl[IIC_maxSlv_sl] = 
{
	{SLV_MAX7313_A, 0x18, 1, IIC_MUX_ADDR	,1},	
	{SLV_MAX7313_B, 0x19, 1, IIC_MUX_ADDR	,2}	,
	{SLV_TMP112A, 	0x48, 1, IIC_MUX_ADDR	,4}	,
	{SLV_ADN4604, 	0x4B, 1, IIC_MUX_ADDR	,8}
};

/* slave board High floor IIC Slave Information */
static s_ts2iic iicSlvs_sh[IIC_maxSlv_sh] = 
{
	{SLV_MAX7313_A, 0x18, 1, IIC_MUX_ADDR	,1},	
	{SLV_MAX7313_B, 0x19, 1, IIC_MUX_ADDR	,2}	,
	{SLV_TMP112A, 	0x48, 1, IIC_MUX_ADDR	,4}	,
	{SLV_ADN4604, 	0x4B, 1, IIC_MUX_ADDR	,8}
};

/* System information */
static s_ts2iic* piicBus[IIC_maxDrv] = 
{
	(s_ts2iic*) &iicSlvs_bp,
	(s_ts2iic*) &iicSlvs_ob,
	(s_ts2iic*) &iicSlvs_sl,
	(s_ts2iic*) &iicSlvs_sh
};

static int iicSlvNum[IIC_maxDrv] = 
{
	IIC_maxSlv_bp,
	IIC_maxSlv_ob,
	IIC_maxSlv_sl,
	IIC_maxSlv_sh
};

static s_ts2iicSys  gSysIns = {0,};
static s_ts2iicSys* gpSysIns=NULL;

//============================================================================== 
//----===@ iic_prnInfo     
// Parameters  :                    
// Description : print iic slave information... 
//==============================================================================
static STATUS iic_prnInfo(s_ts2iic *piic, int busId, int titlePrn)
{
	if(piic == NULL)
		return RET_ERR;

	if(titlePrn == 0)
	{
		prnM1("-----------------------------------------------\n");
		prnM1("   M.S \r\t iicAddr \r\t\t isMux \r\t\t\t MuxAddr \r\t\t\t\t MuxCh\n");
		prnM1("-----------------------------------------------\n");
	}

	prnM1("   %d.%d \r\t  0x%02x \r\t\t   %d \r\t\t\t   0x%02x \r\t\t\t\t   %d\n", 
		busId, piic->name, piic->addr, piic->isMux, piic->muxAddress, piic->muxNum );

	return RET_OK;
}

//============================================================================== 
//----===@ iic_getInfo    
// Parameters  :                
// Description :                
//==============================================================================
static s_ts2iic* iic_getInfo(int busId, int slvid)
{
	return gpSysIns->piic[busId]+slvid;
}


//============================================================================== 
//----===@ iic_setMux 
// Parameters  :               
// Description :                                                                
//==============================================================================
static STATUS iic_setMux(int fd, s_ts2iic* piic)
{
	int status;
	unsigned char WriteBuffer[2];
	unsigned char BytesWritten;
	
	unsigned char ReadBuffer = 0x0;	/* Buffer to hold data read.*/
	unsigned short int BytesToRead;
	
	if(piic == NULL)prnErrRet();

	status = ioctl(fd, IIC_SLAVE_FORCE, piic->muxAddress);
	if(status < 0 )prnErrRet();

	WriteBuffer[0] = piic->muxNum;
	BytesWritten = write(fd, WriteBuffer, 1);
	if(BytesWritten != 1)prnErrRet();

	BytesToRead = read(fd, &ReadBuffer, 1);
	if(BytesToRead != 1 )prnErrRet();

	return RET_OK;
}

//============================================================================== 
//----===@ iic_wr   
// Parameters  :            
// Description :
//==============================================================================
static int iic_wr(int busId, int slvId, unsigned int offset, unsigned int offset_size, char* pData, int size)
{
	int i;
	int fd;
	int found = 0;
	int Status = 0;
	unsigned char WriteBuffer[MAX_IIC_RW_BUF_SIZE + 2];
	unsigned char BytesWritten;
	s_ts2iic* piic;
	
	if(gpSysIns == NULL    ) prnErrRet();
	if(busId >= IIC_maxDrv ) prnErrRet();
	if(pData == NULL       ) prnErrRet();
	if(size == 0           ) prnErrRet();
		
	/* check is valid slave.. */
	for(i = 0; i < gpSysIns->slvNum[busId]; i++)
	{
		piic = iic_getInfo(busId, i);
		if(piic->name == (unsigned)slvId)
		{
			//iic_prnInfo(busId, piic, 0);
			fd = gpSysIns->fd[busId];
			found = 1;
			break;
		}		
	}
	
	/* if found wirte data to iic */
	if(!found)
		prnErrRet();
	
	if(piic->isMux)
	{
		/* do iic mux control */	
		if(!iic_setMux(fd, piic))
			prnErrRet();
	}
	
	if(size > MAX_IIC_RW_BUF_SIZE )prnErrRet();
	
	Status = ioctl(fd, IIC_SLAVE_FORCE, piic->addr);
	if(Status < 0)prnErrRet();

	if( offset_size == 1)
	{
		WriteBuffer[0] = (unsigned char)(offset);
	}
	else if( offset_size == 2)
	{
		WriteBuffer[0] = (unsigned char)(offset>>8);
		WriteBuffer[1] = (unsigned char)(offset);
	}
	else
		prnErrRet();

	memcpy(&WriteBuffer[offset_size], pData, size);	
	BytesWritten = write(fd, WriteBuffer, size + offset_size);

	return BytesWritten-offset_size;
}

//============================================================================== 
//----===@ iic_rd                                                           
// Parameters  :                                                                 
// Description : raon iic bus init....                                                      
//==============================================================================
static int iic_rd(int busId, int slvId, unsigned int offset, unsigned int offset_size, char* pData, int size)
{
	int i;
	int fd;
	int found = 0;
	int Status = 0;
	unsigned char WriteBuffer[2];
	unsigned char BytesWritten;
	unsigned char BytesRead=0;
	s_ts2iic* piic;
	
	if(gpSysIns == NULL    ) prnErrRet();
	if(busId >= IIC_maxDrv ) prnErrRet();
	if(pData == NULL       ) prnErrRet();
	if(size == 0           ) prnErrRet();
		
	/* check is valid slave.. */
	for(i = 0; i < gpSysIns->slvNum[busId]; i++)
	{
		piic = iic_getInfo(busId, i);
		if(piic->name == (unsigned)slvId)
		{
			//iic_prnInfo(busId, piic, 0);
			fd = gpSysIns->fd[busId];
			found = 1;
			break;
		}		
	}
	
	/* if found wirte data to iic */
	if(!found)prnErrRet();
	
	if(piic->isMux)
	{
		/* do iic mux control */	
		if(!iic_setMux(fd, piic ))prnErrRet();
	}
	
	if(size > MAX_IIC_RW_BUF_SIZE )prnErrRet();
	
	Status = ioctl(fd, IIC_SLAVE_FORCE, piic->addr);
	if(Status < 0)prnErrRet();

	if( offset_size == 1)
	{
		WriteBuffer[0] = (unsigned char)(offset);
	}
	else if( offset_size == 2)
	{
		WriteBuffer[0] = (unsigned char)(offset>>8);
		WriteBuffer[1] = (unsigned char)(offset);
	}
	else
		prnErrRet();
	
	BytesWritten = write(fd, WriteBuffer, offset_size);
	if(BytesWritten != offset_size)prnErrRet();

	BytesRead = read(fd, pData, size);
	return BytesRead;
}











//==============================================================================
//----===@ max7313_configIO
// Parameters  : P70 Port[7:0] / PF8 Port[15:8]
// Description : max7313 port I/O config, 1:input, 0:output 
//==============================================================================
static STATUS max7313_configIO(int busId, int slvId, char PF8, char P70 )
{
	unsigned char txBuf;
	unsigned int txReg;
	int bytesWrite;

	txReg = 0x06;	txBuf = P70;
	bytesWrite = iic_wr(busId, slvId, txReg, 1, (char*)&txBuf, 1 );
	if(bytesWrite != 1)prnErrRet();

	txReg = 0x07;	txBuf = PF8;
	bytesWrite = iic_wr(busId, slvId, txReg, 1, (char*)&txBuf, 1 );
	if(bytesWrite != 1)prnErrRet();

	return RET_OK;
}

//==============================================================================
//----===@ max7313_setOutput
// Parameters  : P70 Port[7:0] / PF8 Port[15:8]
// Description : max7313 port Output set, 1:High, 0:Low
//==============================================================================
static STATUS max7313_setOutput(int busId, int slvId, char PF8, char P70 )
{
	unsigned char txBuf;
	unsigned int txReg;
	int bytesWrite;

	txReg = 0x02;	txBuf = P70;
	bytesWrite = iic_wr(busId, slvId, txReg, 1, (char*)&txBuf, 1 );
	if(bytesWrite != 1)prnErrRet();

	txReg = 0x03;	txBuf = PF8;
	bytesWrite = iic_wr(busId, slvId, txReg, 1, (char*)&txBuf, 1 );
	if(bytesWrite != 1)prnErrRet();

	return RET_OK;
}


//============================================================================== 
//----===@ si5338_write                                                           
// Parameters  :                                                                 
// Description :
//==============================================================================
static unsigned int si5338_write(unsigned char w_addr, unsigned char w_data)
{
	int ret = iic_wr(IIC_BUS_ob, SLV_SI5338A, w_addr, 1, (char*)&w_data, 1 );
	return ret;
}


//============================================================================== 
//----===@ si5338_read                                                           
// Parameters  :                                                                 
// Description :
//==============================================================================
static unsigned char si5338_read(unsigned char r_addr)
{
	unsigned char rxData = 0;
	iic_rd(IIC_BUS_ob, SLV_SI5338A, r_addr, sizeof(r_addr), (char*)&rxData, 1 );
	return (unsigned char)rxData;
}

//============================================================================== 
//----===@ si5338_write_mask                                                           
// Parameters  :                                                                 
// Description :
//==============================================================================
static unsigned int si5338_write_mask(unsigned char Addr, unsigned char Data, unsigned char Mask)
{
	unsigned int ret;
	unsigned char reg_val;

	if(Mask == 0xFF)
	{	// All bits
		ret = si5338_write(Addr, Data);
		if(ret != 1)
			return ret;
	}
	else
	{						// Write by mask
		reg_val = si5338_read(Addr);
		reg_val &= ~Mask;	// Clear bits to write
		reg_val |= Data & Mask;	// Set bits by mask

		ret = si5338_write(Addr, reg_val);
		if(ret != 1)
			return ret;
	}
	return 1;
}




























//==============================================================================
//----===@ ts2iic_getSlv_ID
// Parameters  :
// Description : 
//==============================================================================
STATUS ts2iic_getSlv_ID(unsigned char* psl, unsigned char* psh)
{
	unsigned char rxData[2];
	int bytesRead;

	bytesRead = iic_rd(IIC_BUS_sl, SLV_MAX7313_A, 0, 1, (char*)rxData, sizeof(rxData));
	if(!bytesRead)
		prnErrRet();	
  
	*psl = rxData[0] & 0x0f;

	bytesRead = iic_rd(IIC_BUS_sh, SLV_MAX7313_A, 0, 1, (char*)rxData, sizeof(rxData));
	if(!bytesRead)
		prnErrRet();	

	*psh = rxData[0] & 0x0f;

	return RET_OK;
}


//==============================================================================
//----===@ ts2iic_getSlv_temp
// Parameters  :
// Description : get slave 1st floor[low] temperature
//==============================================================================
STATUS ts2iic_getSlv_temp(float* psl, float* psh)
{
	unsigned char rxData[2];
	int bytesRead;

  rxData[0]=0;
  rxData[1]=0;
	bytesRead = iic_rd(IIC_BUS_sl, SLV_TMP112A, 0, 1, (char*)rxData, sizeof(rxData));
	if(!bytesRead)
		prnErrRet();	
	*psl = (((float)rxData[0] *0.0625) *16) + ((float)(rxData[1] >> 4)*0.0625);

  rxData[0]=0;
  rxData[1]=0;
	bytesRead = iic_rd(IIC_BUS_sh, SLV_TMP112A, 0, 1, (char*)rxData, sizeof(rxData));
	if(!bytesRead)
		prnErrRet();	
	*psh = (((float)rxData[0] *0.0625) *16) + ((float)(rxData[1] >> 4)*0.0625);
	return RET_OK;
}





//==============================================================================
//----===@ ts2iic_setFLed
// Parameters  :
// Description : front lcd panel LED control function [if master]
//==============================================================================
STATUS ts2iic_setFLed(unsigned char ledV)
{
	unsigned char txBuf;
	unsigned int txReg;
	int bytesWrite;

	txReg = 0x02;	
	txBuf = ledV ;
	bytesWrite = iic_wr(IIC_BUS_ob, SLV_MAX7313_A, txReg, 1, (char*)&txBuf, 1 );
	if(bytesWrite != 1) 
    prnErrRet();	
	
	return RET_OK;
}

//==============================================================================
//----===@ ts2iic_setFanSpeed
// Parameters  : 
// Description : set fan speed [if master]
//==============================================================================
STATUS ts2iic_setFanSpeed(char pwmIntensity )
{
	unsigned char txBuf;
	unsigned int txReg;
	int bytesWrite;

	// Outputs intensity P9, P8 
	txReg = 0x14;	txBuf = pwmIntensity;
	bytesWrite = iic_wr(IIC_BUS_bp, SLV_MAX7313_A, txReg, 1, (char*)&txBuf, 1 );
	if(bytesWrite != 1)
    prnErrRet();

	return RET_OK;
}

//==============================================================================
//----===@ ts2iic_setCpsConfig
// Parameters  :
// Description : 
//==============================================================================
STATUS ts2iic_setCpsConfig(int busId, struct s_cpsConfig* pCfg)
{
	ifRet(pCfg == NULL);
	prnM0(" ts2iic_setCpsConfig busId : %d\r\n", busId);
	ifRet(busId > IIC_BUS_MAX);


	unsigned char txBuf;
	unsigned int  txReg;
	int bytesWrite;
	
	int i;
	
	for( i = 0; i < 8; i++)
	{
		txReg = 0x90+i;
		txBuf = (pCfg->outP[i*2] & 0x0f) | ((pCfg->outP[i*2+1] & 0x0f) << 4);
		bytesWrite = iic_wr(busId, SLV_ADN4604, txReg, 1, (char*)&txBuf, 1 );
		if(bytesWrite != 1) prnErrRet();
	}

	for( i = 0; i < 16; i++)
	{
		txReg = 0x20+i;
		bytesWrite = iic_wr(busId, SLV_ADN4604, txReg, 1, (char*)&pCfg->outP_En[i], 1 );
		if(bytesWrite != 1) prnErrRet();
	}

	txReg =0x80;	txBuf = 0x01;
	bytesWrite = iic_wr(busId, SLV_ADN4604, txReg, 1, (char*)&txBuf, 1 );
	if(bytesWrite != 1)
    prnErrRet();
	return RET_OK;
}

//==============================================================================
//----===@ ts2iic_getCpsStat
// Parameters  :
// Description : 
//==============================================================================
STATUS ts2iic_getCpsStat(int busId, struct s_cpsConfig* pStat)
{
	ifRet(pStat == NULL);
	prnM0(" ts2iic_getCpsStat busId : %d\r\n", busId);

	unsigned int i;
	unsigned char rxAddr;
	unsigned char rxData;
	int bytesRead;

	ifRet(busId > IIC_BUS_MAX);

	for( i = 0; i < 8; i++)
	{
		rxAddr = 0xB0+i;
		bytesRead = iic_rd(busId, SLV_ADN4604, rxAddr, 1, (char*)&rxData, 1 );
	  if(bytesRead != 1)
      prnErrRet();

		pStat->outP[i*2]   = rxData & 0x0f;
		pStat->outP[i*2+1] = (rxData & 0xf0) >> 4;
	}

	for( i = 0; i < 16; i++)
	{
		rxAddr = 0x20+i;
		bytesRead = iic_rd(busId, SLV_ADN4604, rxAddr, 1, (char*)&rxData, 1 );
	  if(bytesRead != 1)
      prnErrRet();      

		pStat->outP_En[i]   = rxData;
	}

	prnM0(" ts2iic_getCpsStat busId done!!..\r\n");
	return RET_OK;
}




//==============================================================================
//----===@ ts2iic_setSlv_IO
// Parameters  : P70 Port[7:0] / PF8 Port[15:8]
// Description : port I/O config, 1:input, 0:output 
//==============================================================================
STATUS ts2iic_setSlv_IO(int busId, unsigned int ioMode)
{
	unsigned char wrData[2];
	unsigned int  m_st;
  //---------------------------------------------------------------------------
	// port I/O Config
	// max7313 port I/O config, 1:input, 0:output 
	if(max7313_configIO(busId, SLV_MAX7313_B, 0x00, 0x00 )!= 1) 
		prnErrRet();

  //---------------------------------------------------------------------------
	// port Output set to high or low
	if(ioMode == TS2IO_ModeOutput)
	{
		wrData[0] = 0x0;
		wrData[1] = 0x0;
		m_st      = 0x0000;
	}
	else
	{
		wrData[0] = 0xff;
		wrData[1] = 0xff;
		m_st      = 0xffff;
	}

	if(max7313_setOutput(busId, SLV_MAX7313_B, wrData[1], wrData[0] )!= 1) 
		prnErrRet();

	return RET_OK;
}

//==============================================================================
//----===@ ts2iic_initBp
// Parameters  :
// Description : initialize backplane [if master]
//==============================================================================
static STATUS ts2iic_initBp()
{
	unsigned char txBuf;
	unsigned int txReg;
	int bytesWrite;

  //---------------------------------------------------------------------------
	// port I/O Config
	// max7313 port I/O config, 1:input, 0:output 
	if(max7313_configIO(IIC_BUS_bp, SLV_MAX7313_A, 0xf0, 0xf0 )!= 1) 
		prnErrRet();

  //---------------------------------------------------------------------------
	// PWM config 
	// Master, O16 Intensity Register -> Master intensity duty cycle is 15/15 (full), O16 intensity duty cycle is 1/16
	txReg = 0x0e;	txBuf = 0xf0;
	bytesWrite = iic_wr(IIC_BUS_bp, SLV_MAX7313_A, txReg, 1, (char*)&txBuf, 1 );
	if(bytesWrite != 1) 
    prnErrRet();
	
	// Configuration Register -> Enable blink
	txReg = 0x0f;	txBuf = 0x01;
	bytesWrite = iic_wr(IIC_BUS_bp, SLV_MAX7313_A, txReg, 1, (char*)&txBuf, 1 );
	if(bytesWrite != 1) 
    prnErrRet();

	// Outputs intensity P9, P8 
//	if(ts2iic_setFanSpeed( 0x94 )!= 1) 
	if(ts2iic_setFanSpeed( 0x94 )!= 1) 
		prnErrRet();

  //---------------------------------------------------------------------------
	// port Output set to high or low
	if(max7313_setOutput(IIC_BUS_bp, SLV_MAX7313_A, 0x03, 0x00 )!= 1) 
		prnErrRet();

	return RET_OK;
}

//============================================================================== 
//----===@ ts2iic_initClk                                                           
// Parameters  :                                                                 
// Description :
//==============================================================================
STATUS ts2iic_initClk(int option)
{
	unsigned int ret;

	int i;
	Reg_Data rd;
	unsigned char reg_val, test_data;
	Reg_Data* pRegData;

	// Check chip by reading Revision reg 0x00
	prnM2("ts2iic_initClk...\r\n");
	test_data = si5338_read(0);
	if(test_data != 1)
	{
		prnM1("[ERROR]si5338_init test_data read\r\n");
		return RET_ERR;
	}

	// I2C Programming Procedure
	prnM1("si5338_init I2C Programming Procedure...\r\n");
	si5338_write(246, 0x01);					//Hard reset
	// Disable Outputs
	prnM1("si5338_init Disable Outputs...\r\n");
	si5338_write_mask(230, EOB_ALL, EOB_ALL);	// EOB_ALL = 1
	// Pause LOL
	prnM1("si5338_init Pause LOL...\r\n");
	si5338_write_mask(241, DIS_LOL, DIS_LOL);	// DIS_LOL = 1
	// Write new configuration to device accounting for the write-allowed mask
	prnM1("si5338_init Write new configuration...\r\n");
	
		if(option == REF_CLK_Ext)
			pRegData = Reg_extClk;
		else	
			pRegData = Reg_ob25MHz;

	for(i=0; i<NUM_REGS_MAX; i++)
	{
		rd = pRegData[i];
		ret = si5338_write_mask(rd.Reg_Addr, rd.Reg_Val, rd.Reg_Mask);
		if(ret != 1)
			return RET_ERR;
	}
	// Validate clock input status
	i=0;
	prnM1("si5338_init Validate clock input status...\r\n");
	reg_val = si5338_read(218) & LOS_MASK;
	while(reg_val != 0){
		i++;
		if(i>10000)
		{
			prnM1("[ERROR]si5338_init Validate clock input status...\r\n");
			return RET_ERR;
		}
		reg_val = si5338_read(218) & LOS_MASK;
	}
	// Configure PLL for locking
	prnM1("si5338_init Configure PLL for locking...\r\n");
	si5338_write_mask(49, 0, FCAL_OVRD_EN);	//FCAL_OVRD_EN = 0
	// Initiate Locking of PLL
	prnM1("si5338_init Initiate Locking of PLL...\r\n");
	si5338_write(246, SOFT_RESET);			//SOFT_RESET = 1
	usleep(25000);								// Wait 25 ms
	// Restart LOL
	prnM1("si5338_init Restart LOL...\r\n");
	si5338_write_mask(241, 0, DIS_LOL);		// DIS_LOL = 0
	si5338_write(241, 0x65);					// Set reg 241 = 0x65

	// Confirm PLL lock status
	prnM1("si5338_init Confirm PLL lock status...\r\n");
	i=0;
	reg_val = si5338_read(218) & LOCK_MASK;
	while(reg_val != 0){
		i++;
		if(i>10000)
		{
			prnM1("[ERROR]si5338_init Confirm PLL lock status...\r\n");
			return RET_ERR;
		}
		reg_val = si5338_read(218) & LOCK_MASK;
	}
	//copy FCAL values to active registers
	prnM1("si5338_init copy FCAL values to active registers...\r\n");
	si5338_write_mask( 47, si5338_read( 237), 0x03);	// 237[1:0] to 47[1:0]
	si5338_write(46, si5338_read( 236));	// 236[7:0] to 46[7:0]
	si5338_write(45, si5338_read( 235));	// 235[7:0] to 45[7:0]
	si5338_write_mask( 47, 0x14, 0xFC);		// Set 47[7:2] = 000101b
	// Set PLL to use FCAL values
	prnM1("si5338_init Set PLL to use FCAL values...\r\n");
	si5338_write_mask( 49, FCAL_OVRD_EN, FCAL_OVRD_EN);	//FCAL_OVRD_EN = 1
	// Enable Outputs
	prnM1("si5338_init Enable Outputs...\r\n");
	si5338_write(230, 0x00);					//EOB_ALL = 0
	prnM2("ts2iic_initClk ok..\r\n");
	return RET_OK;
}

//==============================================================================
//----===@ ts2iic_initCps
// Parameters  :
// Description : 
//==============================================================================
STATUS ts2iic_initCps(int busId)
{
	unsigned char txBuf;
	unsigned int txReg;
	int bytesWrite;

	prnM2("ts2iic_initCps %d\r\n",busId);
	//adn4604 reset
	txReg = 0x00;	txBuf = 1;
	bytesWrite = iic_wr(busId, SLV_ADN4604, txReg, 1, (char*)&txBuf, 1 );
	if(bytesWrite != 1)
    prnErrRet(); 
	
	//adn4604 mapselect 0
	txReg = 0x81;	txBuf = 0x00;
	bytesWrite = iic_wr(busId, SLV_ADN4604, txReg, 1, (char*)&txBuf, 1 );
	if(bytesWrite != 1)
    prnErrRet(); 
	
	//adn4604 termination - All Termination Enable
	txReg = 0xF0;	txBuf = 0x00;
	bytesWrite = iic_wr(busId, SLV_ADN4604, txReg, 1, (char*)&txBuf, 1 );
	if(bytesWrite != 1)
    prnErrRet(); 

	//adn4604 update
	txReg =0x80;	txBuf = 0x01;
	bytesWrite = iic_wr(busId, SLV_ADN4604, txReg, 1, (char*)&txBuf, 1 );
	if(bytesWrite != 1)
    prnErrRet(); 

	prnM2("ts2iic_initCps ok..\r\n");
	return RET_OK;
}


//============================================================================== 
//----===@ ts2iic_init    
// Parameters  :                
// Description : iic dev open & parameter initialize
//==============================================================================
STATUS ts2iic_init()
{
	int i = 0;
	int j = 0;

	memset(&gSysIns,0,sizeof(gSysIns));
	gpSysIns = &gSysIns;
	
	prnM2("\n");
	prnM2("======================================================\r\n");
	prnM2("ts2iicInit\r\n");
	prnM2("======================================================\r\n");
	for(i = 0; i < IIC_maxDrv; i++)		
	{

#if 1		
		/* Open the device. */
		gpSysIns->fd[i] = open(drvName[i], O_RDWR);
		if(gpSysIns->fd[i] < 0)
		{
			prnM2("Cannot open the IIC device. i = %d\n", i);
			return RET_ERR;
		}
#endif
		
		/* set configuration */
		gpSysIns->piic[i]   = piicBus[i];
		gpSysIns->isOpen[i] = 1;
		gpSysIns->slvNum[i] = iicSlvNum[i];
				
		for(j = 0; j < gpSysIns->slvNum[i]; j++)
		{
			s_ts2iic* pSlvIns = iic_getInfo(i, j);
			iic_prnInfo(pSlvIns, i, i);
		}
		prnM1("\n");
	}

	ifRet(ts2iic_initBp() == 0);
	
	gSysIns.isInit = 1;

	prnM2("ts2iicInit ok..\n");
	return RET_OK;
}

