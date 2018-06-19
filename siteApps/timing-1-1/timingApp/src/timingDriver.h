#ifndef		_TIMING_REGISTER_MAP_H
#define		_TIMING_REGISTER_MAP_H

#include <cstdio>
#include <string>
#include <stdint.h>
#include <string.h>
#include <fcntl.h>
#include <unistd.h>
#include <sys/time.h>
#include <sys/ioctl.h>
#include "ts2api.h"
#include "ts2regmap.h"

#define IIC_maxDrv (4)
#define IIC_maxDrvNameL (15)
#define IIC_maxSlv_bp (1)

/************************** Constant Definitions *****************************/
#define IIC_SLAVE_FORCE 0x0706
#define IIC_SLAVE	      0x0703	/* Change slave address			*/
#define IIC_FUNCS	      0x0705	/* Get the adapter functionality */
#define IIC_RDWR	      0x0707	/* Combined R/W transfer (one stop only)*/

#define MAX_IIC_RW_BUF_SIZE (512)

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

using namespace std;

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

namespace timing
{
	class timingDriver 
	{
		public:
			timingDriver(const int mode);
			inline double GetFirmware(){return firmware;}
			inline double GetSoftware(){return software;}

			inline void SetFirmware(double fware){firmware = fware; printf("FWare:%f\n", firmware);}
			inline void SetSoftware(double sware){software = sware;}
			int			GetEVFileDescriptor();
			int ts2ip_rd(int fd, unsigned int offset, unsigned int *rdData);
			int ts2ip_wr(int fd, unsigned int offset, unsigned int wrData);


		private:
			double firmware;
			double software;

			s_ts2sys*	gpSys;
			s_ts2iicSys* gpSysIns;
			
			int init(int mode);
			int ts2iic_init();
			int ts2iic_getSlv_ID(unsigned char* psl, unsigned char* psh);
			s_ts2iic* iic_getInfo(int busId, int slvid);
			int iic_prnInfo(s_ts2iic *piic, int busId, int titlePrn);
			int ts2iic_initBp();
			int max7313_configIO(int busId, int slvId, char PF8, char P70 );
			int iic_wr(int busId, int slvId, unsigned int offset, unsigned int offset_size, char* pData, int size);
			int max7313_setOutput(int busId, int slvId, char PF8, char P70 );
			int ts2iic_setFanSpeed(char pwmIntensity );
			int iic_setMux(int fd, s_ts2iic* piic);
			int iic_rd(int busId, int slvId, unsigned int offset, unsigned int offset_size, char* pData, int size);
			int ts2iic_initCps(int busId);
			int ts2ip_init(struct s_ts2ip* pIP);
			int ts2iic_initClk(int option);
			unsigned int si5338_write(unsigned char w_addr, unsigned char w_data);
			unsigned int si5338_write_mask(unsigned char Addr, unsigned char Data, unsigned char Mask);
			unsigned char si5338_read(unsigned char r_addr);
			int ts2ip_resetGtp(struct s_ts2ip* pIP);
			int ts2api_setCpsByMode(struct s_ts2sys* pSys);
			int ts2api_setIoByMode(struct s_ts2sys* pSys);
			int ts2iic_setCpsConfig(int busId, struct s_cpsConfig* pCfg);
			int ts2ip_setSlv_IO(struct s_ts2ip* pIP);
			int ts2iic_setSlv_IO(int busId, unsigned int ioMode);
			int ts2ip_setTxDbusIRIGB(struct s_ts2ip* pIP);
			int ts2ip_setEvgTime(struct s_ts2ip* pIP );
			int ts2ip_setMevCodeA(struct s_ts2ip* pIP);
			int ts2ip_setMxcFreq(struct s_ts2ip* pIP, unsigned int mxcN, unsigned int freq);
			int ts2ip_setEvgTrgSrc(struct s_ts2ip* pIP);
			int ts2ip_setSeqTable(struct s_ts2ip* pIP);
			int ts2ip_setSeqTrgSrc(struct s_ts2ip* pIP);
			int ts2ip_setEvMappingRam(struct s_ts2ip* pIP);
			int ts2ip_setRxDbus(struct s_ts2ip* pIP);
			int ts2ip_setPdpPolarity(struct s_ts2ip* pIP);
			int ts2ip_setEvgEnable(struct s_ts2ip* pIP);
			int ts2ip_setEvrEnable(struct s_ts2ip* pIP);
			int ts2ip_setPdpShape(struct s_ts2ip* pIP);


			//Common

			//Link

			//Event Trigger

			//Soft Event

			//Sequence

			//Multiplexed Counter

			//Pulse Generation

			//Output

			//Input

			//GPIO

			//Upstream Port

			//Frequency 
	};
};
#endif
