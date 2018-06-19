//Wrapping class for system calls of timing device driver.
#include "register_map.h"
#include "timingDriver.h"

namespace timing{
	static s_ts2iicSys  gSysIns = {0,};
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

	timingDriver::timingDriver(const int mode):firmware(0.1),software(3.14125)
	{
		init(mode);	
	};

	int timingDriver::init(int mode)
	{
		gpSys = new s_ts2sys();
		ifRet(gpSys == NULL);

		gpSys->evMode = mode;
		int rt = ts2iic_init();
		ifRet(rt == 0);

		rt = ts2iic_getSlv_ID( &gpSys->sl.id, &gpSys->sh.id);
		ifRet(rt == 0);


		rt = ts2iic_initCps(IIC_BUS_ob);
		ifRet(rt == 0);

		//---------------------------------------------
		// ev, gtp ip default setting
		taskDelay(1000000);
		rt = ts2ip_init(&gpSys->ip);
		ifRet(rt == 0);

		//---------------------------------------------
		// system setting with evg, evr, evf, evs
		if(gpSys->evMode == RAON_EVG)
		{
			rt = ts2iic_initClk(REF_CLK_Ext);
			gpSys->ip.ev.cfg.igen_extOn  = 1;
			gpSys->ip.ev.cfg.igen_decOff = 0;
		}
		else
		{
			rt = ts2iic_initClk(REF_CLK_ob25MHz);
			gpSys->ip.ev.cfg.igen_extOn  = 0;
			gpSys->ip.ev.cfg.igen_decOff = 1;
		};

		ifRet(rt == 0); taskDelay(300000);

		rt = ts2ip_resetGtp(&gpSys->ip);

		ifRet(rt == 0); taskDelay(1500000);

		if(gpSys->sl.id == 0x02)
		{
			rt = ts2iic_initCps(IIC_BUS_sl);
			ifRet(rt == 0);
		}

		if(gpSys->sh.id == 0x02)
		{
			rt = ts2iic_initCps(IIC_BUS_sh);
			ifRet(rt == 0);
		}

		gpSys->isInit = 1;

		rt = ts2api_setCpsByMode(gpSys);
		ifRet(rt == 0);

		rt = ts2api_setIoByMode(gpSys);
		ifRet(rt == 0);

		//----------------------------------------------------
		// txDbus src select 1 : user, mxc[6:0]
		//                   0 : external input [7:0]
		gpSys->ip.ev.cfg.txDbusSrcSelect = 0xffffffff; // 0 : external input, 1: user
		ts2ip_setTxDbusIRIGB(&gpSys->ip);

		//----------------------------------------------------
		// set evg Time manually.
		gpSys->ip.ev.cfg.evgTime.day  = 128;
		gpSys->ip.ev.cfg.evgTime.hour = 23;
		gpSys->ip.ev.cfg.evgTime.min  = 53;
		gpSys->ip.ev.cfg.evgTime.sec  = 51;
		ts2ip_setEvgTime(&gpSys->ip);

		//----------------------------------------------------
		// evCodeA, mevCodeA(-> evCodeB) Trigger Source select
		// gpSys->ip.ev.cfg.evTrgSrcSel[0]  : always external input;
		gpSys->ip.ev.cfg.evTrgSrcSel[1]  = 1; // 0 : external , 1: user trigger
		gpSys->ip.ev.cfg.evTrgSrcSel[2]  = 1; // 0 : external , 1: mxc 0
		gpSys->ip.ev.cfg.evTrgSrcSel[3]  = 1; // 0 : external , 1: mxc 1
		gpSys->ip.ev.cfg.evTrgSrcSel[4]  = 1; // 0 : external , 1: mxc 2
		gpSys->ip.ev.cfg.evTrgSrcSel[5]  = 1; // 0 : external , 1: mxc 3
		gpSys->ip.ev.cfg.evTrgSrcSel[6]  = 1; // 0 : external , 1: mxc 4
		gpSys->ip.ev.cfg.evTrgSrcSel[7]  = 1; // 0 : external , 1: mxc 5
		gpSys->ip.ev.cfg.evTrgSrcSel[8]  = 1; // 0 : external , 1: mxc 6
		gpSys->ip.ev.cfg.evTrgSrcSel[9]  = 1; // 0 : external , 1: mxc 7
		gpSys->ip.ev.cfg.evTrgSrcSel[10] = 1; // 0 : external , 1: mxc 8
		gpSys->ip.ev.cfg.evTrgSrcSel[11] = 1; // 0 : external , 1: mxc 9
		gpSys->ip.ev.cfg.evTrgSrcSel[12] = 1; // 0 : external , 1: mxc 10
		gpSys->ip.ev.cfg.evTrgSrcSel[13] = 1; // 0 : external , 1: mxc 11
		gpSys->ip.ev.cfg.evTrgSrcSel[14] = 1; // 0 : external , 1: mxc 12
		gpSys->ip.ev.cfg.evTrgSrcSel[15] = 1; // 0 : external , 1: mxc 13
		ts2ip_setEvgTrgSrc   (&gpSys->ip );

		//----------------------------------------------------
		// mxc freq config 1 = 12.3ns, 0 - not allowed.
		gpSys->ip.ev.cfg.mxcFreq[0]  = 50000;
		gpSys->ip.ev.cfg.mxcFreq[1]  = 500;
		gpSys->ip.ev.cfg.mxcFreq[2]  = 500;
		gpSys->ip.ev.cfg.mxcFreq[3]  = 500;
		gpSys->ip.ev.cfg.mxcFreq[4]  = 500;
		gpSys->ip.ev.cfg.mxcFreq[5]  = 500;
		gpSys->ip.ev.cfg.mxcFreq[6]  = 500;
		gpSys->ip.ev.cfg.mxcFreq[7]  = 500;
		gpSys->ip.ev.cfg.mxcFreq[8]  = 500;
		gpSys->ip.ev.cfg.mxcFreq[9]  = 500;
		gpSys->ip.ev.cfg.mxcFreq[10] = 500;
		gpSys->ip.ev.cfg.mxcFreq[11] = 500;
		gpSys->ip.ev.cfg.mxcFreq[12] = 500;
		gpSys->ip.ev.cfg.mxcFreq[13] = 500;

		for(int i=0;i<MAX_mxc_NUM;i++)
			ts2ip_setMxcFreq(&gpSys->ip, i, gpSys->ip.ev.cfg.mxcFreq[i]);

		//----------------------------------------------------
		// evCodeA[5bit] -> evCodeB[11 bit] convert / 0 (no action) ~ 0x7ff
		gpSys->ip.ev.cfg.mevCodeA[0]  = 0;
		gpSys->ip.ev.cfg.mevCodeA[1]  = 0;
		gpSys->ip.ev.cfg.mevCodeA[2]  = 0;
		gpSys->ip.ev.cfg.mevCodeA[3]  = 0;
		gpSys->ip.ev.cfg.mevCodeA[4]  = 0;
		gpSys->ip.ev.cfg.mevCodeA[5]  = 0;
		gpSys->ip.ev.cfg.mevCodeA[6]  = 0;
		gpSys->ip.ev.cfg.mevCodeA[7]  = 0;
		gpSys->ip.ev.cfg.mevCodeA[8]  = 0;
		gpSys->ip.ev.cfg.mevCodeA[9]  = 0;
		gpSys->ip.ev.cfg.mevCodeA[10] = 0;
		gpSys->ip.ev.cfg.mevCodeA[11] = 0;
		gpSys->ip.ev.cfg.mevCodeA[12] = 0;
		gpSys->ip.ev.cfg.mevCodeA[13] = 0;
		gpSys->ip.ev.cfg.mevCodeA[14] = 0;
		gpSys->ip.ev.cfg.mevCodeA[15] = 0;
		ts2ip_setMevCodeA    (&gpSys->ip)                                      ;

		//----------------------------------------------------
		// evOption is only set to first seq packet, seq repeat count 0 ~ 65535
		// stopbit is only set to last seq packet, 0 or 1
		// evCodeB 0x0 ~ 0x7ff
		// timeStamp 0x0 ~ 0xffffffff
		int tmp_timeStamp;
		tmp_timeStamp = 100;

		// seq A
		gpSys->ip.ev.cfg.seqTableA[0].stopbit         = 0;                    
		gpSys->ip.ev.cfg.seqTableA[0].evOption        = 100; // seq repeat count 0 ~ 65535
		gpSys->ip.ev.cfg.seqTableA[0].evCodeB         = 400; 
		gpSys->ip.ev.cfg.seqTableA[0].timeStamp       = tmp_timeStamp;                    
		tmp_timeStamp += 200;
		gpSys->ip.ev.cfg.seqTableA[1].stopbit         = 0;
		gpSys->ip.ev.cfg.seqTableA[1].evOption        = 0;
		gpSys->ip.ev.cfg.seqTableA[1].evCodeB         = 401;
		gpSys->ip.ev.cfg.seqTableA[1].timeStamp       = tmp_timeStamp;
		tmp_timeStamp += 200;
		gpSys->ip.ev.cfg.seqTableA[2].stopbit         = 0;
		gpSys->ip.ev.cfg.seqTableA[2].evOption        = 0;
		gpSys->ip.ev.cfg.seqTableA[2].evCodeB         = 402;
		gpSys->ip.ev.cfg.seqTableA[2].timeStamp       = tmp_timeStamp;
		tmp_timeStamp += 200;
		gpSys->ip.ev.cfg.seqTableA[3].stopbit         = 1;
		gpSys->ip.ev.cfg.seqTableA[3].evOption        = 0;
		gpSys->ip.ev.cfg.seqTableA[3].evCodeB         = 403;
		gpSys->ip.ev.cfg.seqTableA[3].timeStamp       = tmp_timeStamp;
		tmp_timeStamp += 200;

		// seq B
		tmp_timeStamp = 100;
		gpSys->ip.ev.cfg.seqTableB[0].stopbit         = 0;                    
		gpSys->ip.ev.cfg.seqTableB[0].evOption        = 500; // seq repeat count 0 ~ 65535
		gpSys->ip.ev.cfg.seqTableB[0].evCodeB         = 500; 
		gpSys->ip.ev.cfg.seqTableB[0].timeStamp       = tmp_timeStamp;                    
		tmp_timeStamp += 200;
		gpSys->ip.ev.cfg.seqTableB[1].stopbit         = 0;
		gpSys->ip.ev.cfg.seqTableB[1].evOption        = 0;
		gpSys->ip.ev.cfg.seqTableB[1].evCodeB         = 501;
		gpSys->ip.ev.cfg.seqTableB[1].timeStamp       = tmp_timeStamp;
		tmp_timeStamp += 200;
		gpSys->ip.ev.cfg.seqTableB[2].stopbit         = 0;
		gpSys->ip.ev.cfg.seqTableB[2].evOption        = 0;
		gpSys->ip.ev.cfg.seqTableB[2].evCodeB         = 502;
		gpSys->ip.ev.cfg.seqTableB[2].timeStamp       = tmp_timeStamp;
		tmp_timeStamp += 200;
		gpSys->ip.ev.cfg.seqTableB[3].stopbit         = 1;
		gpSys->ip.ev.cfg.seqTableB[3].evOption        = 0;
		gpSys->ip.ev.cfg.seqTableB[3].evCodeB         = 503;
		gpSys->ip.ev.cfg.seqTableB[3].timeStamp       = tmp_timeStamp;
		tmp_timeStamp += 200;

		ts2ip_setSeqTable    (&gpSys->ip)                                      ;

		gpSys->ip.ev.cfg.seqMask.A_OR0  = 0 \
										  | D_seqSrc_usr    \
										  ;        

		gpSys->ip.ev.cfg.seqMask.A_OR1  = 0
										  ;        

		gpSys->ip.ev.cfg.seqMask.A_sel  = 1; 
		gpSys->ip.ev.cfg.seqMask.B_OR0  = 0 \
										  ;        
		gpSys->ip.ev.cfg.seqMask.B_OR1  = 0 \
										  ;        
		gpSys->ip.ev.cfg.seqMask.B_sel  = 0; 
		ts2ip_setSeqTrgSrc   (&gpSys->ip)                                      ;

		// evr mapping ram config
		gpSys->ip.ev.cfg.evMappingRam[ 10] = 0xffffffff;  
		gpSys->ip.ev.cfg.evMappingRam[400] = 0x00010000;  
		gpSys->ip.ev.cfg.evMappingRam[401] = 0x00020000;  
		gpSys->ip.ev.cfg.evMappingRam[402] = 0x00040000;  
		gpSys->ip.ev.cfg.evMappingRam[403] = 0x00080000;  
		gpSys->ip.ev.cfg.evMappingRam[500] = 0x00100000;  
		gpSys->ip.ev.cfg.evMappingRam[501] = 0x00200000;  
		gpSys->ip.ev.cfg.evMappingRam[502] = 0x00400000;  
		gpSys->ip.ev.cfg.evMappingRam[503] = 0x00800000;  

		ts2ip_setEvMappingRam(&gpSys->ip);

		//----------------------------------------------------
		// gpSys->ip.ev.cfg.swapExOut
		// for test
		gpSys->ip.ev.cfg.swapExOut       = 0;

		// pdpTrgSrcSel 1 : pdpTrigger[31:16] from evCodeA
		//                  pdpTrigger[31:16] from EvMappingRam[15:0]
		//              0 : pdpTrigger[31: 0] from EvMappingRam[31:0]
		gpSys->ip.ev.cfg.pdpTrgSrcSel    = 0;      

		// rxDbusSrcSelect 1 : exOut[7:0] from pdpOutput[7:0]
		//                 0 : exOut[7:0] from rxDBus[7:0]
		gpSys->ip.ev.cfg.rxDbusSrcSelect = 0; // exOut[0], 0 : rxDBus[0] , 1: pdpOut[0] 
		ts2ip_setRxDbus      (&gpSys->ip)                                      ;

		//----------------------------------------------------
		// set pdp out polarity
		gpSys->ip.ev.cfg.pdpPolarity  = 0xffffffff;
		ts2ip_setPdpPolarity (&gpSys->ip);

		//----------------------------------------------------
		// set pdp out delay
		gpSys->ip.ev.cfg.pdp[0].delay        = 0; // max 0xffffffff
		gpSys->ip.ev.cfg.pdp[1].delay        = 0;
		gpSys->ip.ev.cfg.pdp[2].delay        = 0;
		gpSys->ip.ev.cfg.pdp[3].delay        = 0;
		gpSys->ip.ev.cfg.pdp[4].delay        = 0;
		gpSys->ip.ev.cfg.pdp[5].delay        = 0;
		gpSys->ip.ev.cfg.pdp[6].delay        = 0;
		gpSys->ip.ev.cfg.pdp[7].delay        = 0;
		gpSys->ip.ev.cfg.pdp[8].delay        = 0;
		gpSys->ip.ev.cfg.pdp[9].delay        = 0;
		gpSys->ip.ev.cfg.pdp[10].delay       = 0;
		gpSys->ip.ev.cfg.pdp[11].delay       = 0;
		gpSys->ip.ev.cfg.pdp[12].delay       = 0;
		gpSys->ip.ev.cfg.pdp[13].delay       = 0;
		gpSys->ip.ev.cfg.pdp[14].delay       = 0;
		gpSys->ip.ev.cfg.pdp[15].delay       = 0;
		gpSys->ip.ev.cfg.pdp[16].delay       = 0;
		gpSys->ip.ev.cfg.pdp[17].delay       = 0;
		gpSys->ip.ev.cfg.pdp[18].delay       = 0;
		gpSys->ip.ev.cfg.pdp[19].delay       = 0;
		gpSys->ip.ev.cfg.pdp[20].delay       = 0;
		gpSys->ip.ev.cfg.pdp[21].delay       = 0;
		gpSys->ip.ev.cfg.pdp[22].delay       = 0;
		gpSys->ip.ev.cfg.pdp[23].delay       = 0;
		gpSys->ip.ev.cfg.pdp[24].delay       = 0;
		gpSys->ip.ev.cfg.pdp[25].delay       = 0;
		gpSys->ip.ev.cfg.pdp[26].delay       = 0;
		gpSys->ip.ev.cfg.pdp[27].delay       = 0;
		gpSys->ip.ev.cfg.pdp[28].delay       = 0;
		gpSys->ip.ev.cfg.pdp[29].delay       = 0;
		gpSys->ip.ev.cfg.pdp[30].delay       = 0;
		gpSys->ip.ev.cfg.pdp[31].delay       = 0;

		// set pdp out width
		gpSys->ip.ev.cfg.pdp[0].width        = 20; // 0: 1 pulse [12.3ns], max 0xffffffff
		gpSys->ip.ev.cfg.pdp[1].width        = 20;
		gpSys->ip.ev.cfg.pdp[2].width        = 20;
		gpSys->ip.ev.cfg.pdp[3].width        = 20;
		gpSys->ip.ev.cfg.pdp[4].width        = 20;
		gpSys->ip.ev.cfg.pdp[5].width        = 20;
		gpSys->ip.ev.cfg.pdp[6].width        = 20;
		gpSys->ip.ev.cfg.pdp[7].width        = 20;
		gpSys->ip.ev.cfg.pdp[8].width        = 20;
		gpSys->ip.ev.cfg.pdp[9].width        = 20;
		gpSys->ip.ev.cfg.pdp[10].width       = 20;
		gpSys->ip.ev.cfg.pdp[11].width       = 20;
		gpSys->ip.ev.cfg.pdp[12].width       = 20;
		gpSys->ip.ev.cfg.pdp[13].width       = 20;
		gpSys->ip.ev.cfg.pdp[14].width       = 20;
		gpSys->ip.ev.cfg.pdp[15].width       = 20;
		gpSys->ip.ev.cfg.pdp[16].width       = 20;
		gpSys->ip.ev.cfg.pdp[17].width       = 20;
		gpSys->ip.ev.cfg.pdp[18].width       = 20;
		gpSys->ip.ev.cfg.pdp[19].width       = 20;
		gpSys->ip.ev.cfg.pdp[20].width       = 20;
		gpSys->ip.ev.cfg.pdp[21].width       = 20;
		gpSys->ip.ev.cfg.pdp[22].width       = 20;
		gpSys->ip.ev.cfg.pdp[23].width       = 20;
		gpSys->ip.ev.cfg.pdp[24].width       = 20;
		gpSys->ip.ev.cfg.pdp[25].width       = 20;
		gpSys->ip.ev.cfg.pdp[26].width       = 20;
		gpSys->ip.ev.cfg.pdp[27].width       = 20;
		gpSys->ip.ev.cfg.pdp[28].width       = 20;
		gpSys->ip.ev.cfg.pdp[29].width       = 20;
		gpSys->ip.ev.cfg.pdp[30].width       = 20;
		gpSys->ip.ev.cfg.pdp[31].width       = 20;
		ts2ip_setPdpShape    (&gpSys->ip);

		//----------------------------------------------------
		// enable evr control register
		gpSys->ip.ev.enEvr.streamData        = 1;       
		gpSys->ip.ev.enEvr.DBus              = 1; 
		gpSys->ip.ev.enEvr.evCodeA           = 1;    
		gpSys->ip.ev.enEvr.evCodeB           = 1;    
		gpSys->ip.ev.enEvr.pdpEnable         = 1;      
		ts2ip_setEvrEnable(&gpSys->ip);

		taskDelay(3000000);
		//----------------------------------------------------
		// enable evg control register
		gpSys->ip.ev.enEvg.mxc               = 1;   
		gpSys->ip.ev.enEvg.evTrgMsk16        = 0;   // evTrigger Enable Mask 16bit (evCodeA mask)
		gpSys->ip.ev.enEvg.DBus              = 1;  
		gpSys->ip.ev.enEvg.evCodeA           = 0;     
		gpSys->ip.ev.enEvg.evCodeB           = 1;     
		gpSys->ip.ev.enEvg.evCodeB_mevCodeA  = 0;              
		gpSys->ip.ev.enEvg.evCodeB_seqTrgA   = 1;             
		gpSys->ip.ev.enEvg.evCodeB_seqTrgB   = 0;             
		ts2ip_setEvgEnable(&gpSys->ip);

		taskDelay(1000000);

		return RET_OK;
	};

	int timingDriver::GetEVFileDescriptor()
	{
		ifRet(gpSys == NULL);
		return (gpSys->ip.ev.fd);
	}

	int timingDriver::ts2iic_init()
	{
		ifRet(gpSys == NULL);

		memset(&gSysIns,0,sizeof(gSysIns));
		gpSysIns = &gSysIns;

		prnM2("\n");
		prnM2("======================================================\r\n");
		prnM2("ts2iicInit\r\n");
		prnM2("======================================================\r\n");
		for(int i = 0; i < IIC_maxDrv; i++)		
		{
			/* Open the device. */
			gpSysIns->fd[i] = open(drvName[i], O_RDWR);
			if(gpSysIns->fd[i] < 0)
			{
				prnM2("Cannot open the IIC device. i = %d\n", i);
				return RET_ERR;
			}

			/* set configuration */
			gpSysIns->piic[i]   = piicBus[i];
			gpSysIns->isOpen[i] = 1;
			gpSysIns->slvNum[i] = iicSlvNum[i];

			for(int j = 0; j < gpSysIns->slvNum[i]; j++)
			{
				s_ts2iic* pSlvIns = iic_getInfo(i, j);
				iic_prnInfo(pSlvIns, i, i);
			};

			prnM1("\n");
		};

		ifRet(ts2iic_initBp() == 0);
		gpSysIns->isInit = 1;

		prnM2("ts2iicInit ok..\n");
		return RET_OK;
	};

	s_ts2iic* timingDriver::iic_getInfo(int busId, int slvid)
	{
		return gpSysIns->piic[busId]+slvid;
	};

	int timingDriver::iic_prnInfo(s_ts2iic *piic, int busId, int titlePrn)
	{
		if(piic == NULL) return RET_ERR;

		if(titlePrn == 0)
		{
			prnM1("-----------------------------------------------\n");
			prnM1("   M.S \r\t iicAddr \r\t\t isMux \r\t\t\t MuxAddr \r\t\t\t\t MuxCh\n");
			prnM1("-----------------------------------------------\n");
		};

		prnM1("   %d.%d \r\t  0x%02x \r\t\t   %d \r\t\t\t   0x%02x \r\t\t\t\t   %d\n", 
				busId, piic->name, piic->addr, piic->isMux, piic->muxAddress, piic->muxNum );

		return RET_OK;
	};

	int timingDriver::ts2iic_initBp()
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
	};

	int timingDriver::max7313_setOutput(int busId, int slvId, char PF8, char P70 )
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

	int timingDriver::max7313_configIO(int busId, int slvId, char PF8, char P70 )
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
	};

	int timingDriver::iic_wr(int busId, int slvId, unsigned int offset, unsigned int offset_size, char* pData, int size)
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
	};

	int timingDriver::ts2iic_setFanSpeed(char pwmIntensity )
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
	};

	int timingDriver::iic_setMux(int fd, s_ts2iic* piic)
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
	};

	int timingDriver::ts2ip_wr(int fd, unsigned int offset, unsigned int wrData)
	{
		if (fd == -1) 
			prnErrRet();

		struct s_regRW reg;
		reg.offset = offset;
		reg.val    = wrData;

		if (ioctl(fd, IOCTL_W, &reg) < 0) 
			prnErrRet();

		return RET_OK;
	}

	int timingDriver::ts2ip_rd(int fd, unsigned int offset, unsigned int *rdData)
	{
		if(fd == -1) prnErrRet();

		struct s_regRW reg;
		reg.offset = offset;
		reg.val    = 0;

		if (ioctl(fd, IOCTL_R, &reg) < 0) 
			prnErrRet();

		*rdData = reg.val;
		return RET_OK;
	};

	int timingDriver::ts2iic_getSlv_ID(unsigned char* psl, unsigned char* psh)
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
	};

	int timingDriver::iic_rd(int busId, int slvId, unsigned int offset, unsigned int offset_size, char* pData, int size)
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
	};

	int timingDriver::ts2iic_initCps(int busId)
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
	};

	int timingDriver::ts2ip_init(struct s_ts2ip* pIP)
	{
		int i;
		prnM2("\n");
		prnM2("======================================================\r\n");
		prnM2("ts2ip_init\r\n");
		prnM2("======================================================\r\n");

		for(i=0;i<5;i++)
		{
			pIP->ev.fd  = open(DRV_NAME_ip_ev,O_RDWR);
			if(pIP->ev.fd == -1)
				taskDelay(200000);
			else
				break;
		}

		for(i=0;i<5;i++)
		{
			pIP->gtp.fd = open(DRV_NAME_ip_gtp,O_RDWR);
			if(pIP->gtp.fd == -1)
				taskDelay(200000);
			else
				break;
		}

		if (pIP->ev.fd  == -1)prnErrRet();
		if (pIP->gtp.fd == -1)prnErrRet();

		//output reg  [31 : 0] m_st, // 3-state enable input, high=input, low=output
		ts2ip_wr(pIP->ev.fd, A_m_st_sl, C_m_st(0xffff));
		ts2ip_wr(pIP->ev.fd, A_m_st_sh, C_m_st(0xffff));

		prnM2("ts2ip_init ok..\n");
		pIP->isInit = 1;
		return RET_OK;
	};

	int timingDriver::ts2iic_initClk(int option)
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
		};

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
	};

	unsigned char timingDriver::si5338_read(unsigned char r_addr)
	{
		unsigned char rxData = 0;
		iic_rd(IIC_BUS_ob, SLV_SI5338A, r_addr, sizeof(r_addr), (char*)&rxData, 1 );
		return (unsigned char)rxData;
	};

	unsigned int timingDriver::si5338_write(unsigned char w_addr, unsigned char w_data)
	{
		int ret = iic_wr(IIC_BUS_ob, SLV_SI5338A, w_addr, 1, (char*)&w_data, 1 );
		return ret;
	};

	unsigned int timingDriver::si5338_write_mask(unsigned char Addr, unsigned char Data, unsigned char Mask)
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
	};

	int timingDriver::ts2ip_resetGtp(struct s_ts2ip* pIP)
	{
		int l_fd = pIP->gtp.fd;
		ifRet(pIP->gtp.fd < 0);

		ts2ip_wr(l_fd, A_gt_reset, C_soft_reset(1));
		taskDelay(1000);
		ts2ip_wr(l_fd, A_gt_reset, C_soft_reset(0));

		return RET_OK;
	};

	int timingDriver::ts2api_setCpsByMode(struct s_ts2sys* pSys)
	{
		ifRet(pSys == NULL)

		pSys->ob_busId = IIC_BUS_ob;
		pSys->sl.busId = IIC_BUS_sl;
		pSys->sh.busId = IIC_BUS_sh;

		if((pSys->evMode == RAON_EVG) || (pSys->evMode == RAON_EVS))
		{
			pSys->ob_cps.outP[CPS_GT0 ] = CPS_GT0;
			pSys->ob_cps.outP[CPS_GT1 ] = CPS_GT0;
			pSys->ob_cps.outP[CPS_FMC1] = CPS_GT0;
			pSys->ob_cps.outP[CPS_FMC2] = CPS_GT0;
			pSys->ob_cps.outP[CPS_SL  ] = CPS_GT0;
			pSys->ob_cps.outP[CPS_SH  ] = CPS_GT0;
			prnM2_fmtDec("evMode", pSys->evMode, "G, S");
		}
		else
		{
			pSys->ob_cps.outP[CPS_GT0 ] = CPS_FMC1;
			pSys->ob_cps.outP[CPS_GT1 ] = CPS_FMC1;
			pSys->ob_cps.outP[CPS_FMC1] = CPS_FMC1;
			pSys->ob_cps.outP[CPS_FMC2] = CPS_FMC1;
			pSys->ob_cps.outP[CPS_SL  ] = CPS_FMC1;
			pSys->ob_cps.outP[CPS_SH  ] = CPS_FMC1;
			prnM2_fmtDec("evMode", pSys->evMode, "F,R");
		}

		pSys->ob_cps.outP_En[CPS_GT0 ] = 0x30;
		pSys->ob_cps.outP_En[CPS_GT1 ] = 0x30;
		pSys->ob_cps.outP_En[CPS_FMC1] = 0x30;
		pSys->ob_cps.outP_En[CPS_FMC2] = 0x30;
		pSys->ob_cps.outP_En[CPS_SL  ] = 0x30;
		pSys->ob_cps.outP_En[CPS_SH  ] = 0x30;

		pSys->sl.cps.outP[CPS_SW_CTRL] = CPS_SW_00;
		pSys->sl.cps.outP_En[CPS_SW_CTRL] = 0x30;

		for(int i = 0; i < 12; i++)
		{
			pSys->sl.cps.outP[CPS_SW_00 + i] = CPS_SW_CTRL;
			pSys->sl.cps.outP_En[CPS_SW_00 + i] = 0x30;
		};

		pSys->sh.cps.outP[CPS_SW_CTRL] = CPS_SW_00;
		pSys->sh.cps.outP_En[CPS_SW_CTRL] = 0x30;

		for(int i = 0; i < 12; i++)
		{
			pSys->sh.cps.outP[CPS_SW_00 + i] = CPS_SW_CTRL;
			pSys->sh.cps.outP_En[CPS_SW_00 + i] = 0x30;
		};

		ts2iic_setCpsConfig(pSys->ob_busId, &pSys->ob_cps);

		if(pSys->sl.id==0x02)
			ts2iic_setCpsConfig(pSys->sl.busId, &pSys->sl.cps);

		if(pSys->sh.id==0x02)
			ts2iic_setCpsConfig(pSys->sh.busId, &pSys->sh.cps);

		return RET_OK;
	};

	int timingDriver::ts2api_setIoByMode(struct s_ts2sys* pSys)
	{
		ifRet(pSys == NULL)

		pSys->ip.ev.cfg.slio = 0xffff; // default input
		pSys->ip.ev.cfg.shio = 0xffff;

		if(pSys->sl.id==0x01)
		{
			if(pSys->evMode == RAON_EVG)
			{
				ts2iic_setSlv_IO(pSys->sl.busId, TS2IO_ModeInput);
				pSys->ip.ev.cfg.slio = 0xffff;
			}
			else
			{
				ts2iic_setSlv_IO(pSys->sl.busId, TS2IO_ModeOutput);
				pSys->ip.ev.cfg.slio = 0x0000;
			}
		}

		if(pSys->sh.id==0x01)
		{
			ts2iic_setSlv_IO(pSys->sh.busId, TS2IO_ModeOutput);
			pSys->ip.ev.cfg.shio = 0x0000;
		}

		ts2ip_setSlv_IO(&pSys->ip);	
		return RET_OK;
	};

	int timingDriver::ts2iic_setCpsConfig(int busId, struct s_cpsConfig* pCfg)
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

		if(bytesWrite != 1) prnErrRet();

		return RET_OK;
	};

	int timingDriver::ts2ip_setSlv_IO(struct s_ts2ip* pIP)
	{
		ifRet (pIP->ev.fd < 0) 

			ts2ip_wr(pIP->ev.fd, A_m_st_sl, C_m_st(pIP->ev.cfg.slio));
		ts2ip_wr(pIP->ev.fd, A_m_st_sh, C_m_st(pIP->ev.cfg.shio));

		return RET_OK;
	}

	int timingDriver::ts2iic_setSlv_IO(int busId, unsigned int ioMode)
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
	};

	int timingDriver::ts2ip_setTxDbusIRIGB(struct s_ts2ip* pIP)
	{
		int l_fd = pIP->ev.fd;
		ifRet(l_fd < 0);

		struct s_evConfig* pCfg = &pIP->ev.cfg;

		ts2ip_wr(l_fd, A_txDBusSel_igenMode , C_txDBusSel(pCfg->txDbusSrcSelect) \
				| C_igen_decOff(pCfg->igen_decOff)   \
				| C_igen_extOn(pCfg->igen_extOn)     );
		return RET_OK;
	};

	int timingDriver::ts2ip_setEvgTime(struct s_ts2ip* pIP )
	{
		int l_fd = pIP->ev.fd;
		ifRet(l_fd < 0);

		struct s_evTime* pTime = &pIP->ev.cfg.evgTime;

		ts2ip_wr(l_fd, A_evgW_set_Time    , C_evgW_set_h8(pTime->hour) | C_evgW_set_m8(pTime->min) | C_evgW_set_s8(pTime->sec) );
		ts2ip_wr(l_fd, A_evgW_set_Time + 4, C_evgW_set_d16(pTime->day));

		ts2ip_wr(l_fd, A_resetCtrl, C_evgW_set_time(1));
		taskDelay(10);
		ts2ip_wr(l_fd, A_resetCtrl, C_evgW_set_time(0));

		return RET_OK;
	};

	int timingDriver::ts2ip_setMxcFreq(struct s_ts2ip* pIP, unsigned int mxcN, unsigned int freq)
	{
		int l_fd = pIP->ev.fd;
		ifRet(l_fd < 0);

		ts2ip_wr(l_fd, A_mxc14_prescalerReg_00 + mxcN*4, freq );
		return RET_OK;
	};

	int timingDriver::ts2ip_setMevCodeA(struct s_ts2ip* pIP)
	{
		int l_fd = pIP->ev.fd;
		ifRet(l_fd < 0);

		int i;
		unsigned int* pmevA = pIP->ev.cfg.mevCodeA;

		for(i=0;i<MAX_mevCodeA;i++)
		{
			ts2ip_wr(l_fd, Mem_evCodeA + (((i & 0x1f) + 1) * 4) , (pmevA[i] & 0x7ff));
		}
		return RET_OK;
	};

	int timingDriver::ts2ip_setEvgTrgSrc(struct s_ts2ip* pIP)
	{
		int l_fd = pIP->ev.fd;
		ifRet(l_fd < 0);

		ts2ip_wr(l_fd, A_evTrgCtrl, 0 \
				| ((pIP->ev.cfg.evTrgSrcSel[1]  & 0x01) <<  1 ) \
				| ((pIP->ev.cfg.evTrgSrcSel[2]  & 0x01) <<  2 ) \
				| ((pIP->ev.cfg.evTrgSrcSel[3]  & 0x01) <<  3 ) \
				| ((pIP->ev.cfg.evTrgSrcSel[4]  & 0x01) <<  4 ) \
				| ((pIP->ev.cfg.evTrgSrcSel[5]  & 0x01) <<  5 ) \
				| ((pIP->ev.cfg.evTrgSrcSel[6]  & 0x01) <<  6 ) \
				| ((pIP->ev.cfg.evTrgSrcSel[7]  & 0x01) <<  7 ) \
				| ((pIP->ev.cfg.evTrgSrcSel[8]  & 0x01) <<  8 ) \
				| ((pIP->ev.cfg.evTrgSrcSel[9]  & 0x01) <<  9 ) \
				| ((pIP->ev.cfg.evTrgSrcSel[10] & 0x01) << 10 ) \
				| ((pIP->ev.cfg.evTrgSrcSel[11] & 0x01) << 11 ) \
				| ((pIP->ev.cfg.evTrgSrcSel[12] & 0x01) << 12 ) \
				| ((pIP->ev.cfg.evTrgSrcSel[13] & 0x01) << 13 ) \
				| ((pIP->ev.cfg.evTrgSrcSel[14] & 0x01) << 14 ) \
				| ((pIP->ev.cfg.evTrgSrcSel[15] & 0x01) << 15 ) \
				);
		return RET_OK;
	};

	int timingDriver::ts2ip_setSeqTable(struct s_ts2ip* pIP)
	{
		int l_fd = pIP->ev.fd;
		ifRet(l_fd < 0);

		int i;
		unsigned int seqMemOffset;

		struct s_seqTable* p_seqTableA = pIP->ev.cfg.seqTableA;
		struct s_seqTable* p_seqTableB = pIP->ev.cfg.seqTableB;

		seqMemOffset = 0;
		for(i=0;i<MAX_SEQ_NUM;i++)
		{
			ts2ip_wr(l_fd, Mem_seqA_Config + seqMemOffset + (i*4),\
					C_seq_option (p_seqTableA[i].evOption)  |\
					C_seq_stop   (p_seqTableA[i].stopbit)   |\
					C_seq_evCodeB(p_seqTableA[i].evCodeB));

			ts2ip_wr(l_fd, Mem_seqA_Config + seqMemOffset + 0x2000 + (i*4), 	p_seqTableA[i].timeStamp);
		}

		seqMemOffset = 0x4000;
		for(i=0;i<MAX_SEQ_NUM;i++)
		{
			ts2ip_wr(l_fd, Mem_seqA_Config + seqMemOffset + (i*4),\
					C_seq_option (p_seqTableB[i].evOption)  |\
					C_seq_stop   (p_seqTableB[i].stopbit)   |\
					C_seq_evCodeB(p_seqTableB[i].evCodeB));

			ts2ip_wr(l_fd, Mem_seqA_Config + seqMemOffset + 0x2000 + (i*4), 	p_seqTableB[i].timeStamp);
		}

		return RET_OK;
	}

	int timingDriver::ts2ip_setSeqTrgSrc(struct s_ts2ip* pIP)
	{
		int l_fd = pIP->ev.fd;
		ifRet(l_fd < 0);

		struct s_seqMask* psMask = &pIP->ev.cfg.seqMask;

		// evg seqTrgMskA0, A1 
		// seqTrgSrc 30    - usrTrg
		// seqTrgSrc 29:16 - mxc  13:0
		// seqTrgSrc 15:0  - exIn 15:0
		// seqTrg = A_seqTrgMskA0 | A_seqTrgMskA1 
		ts2ip_wr(l_fd, A_seqTrgMskA0, psMask->A_OR0); // select : mxc0
		ts2ip_wr(l_fd, A_seqTrgMskA1, psMask->A_OR1); // select : none
		ts2ip_wr(l_fd, A_seqTrgMskB0, psMask->B_OR0); // select : mxc0
		ts2ip_wr(l_fd, A_seqTrgMskB1, psMask->B_OR1); // select : none

		ts2ip_wr(l_fd, A_seqTrgCtrl, C_seqTrgSelA(psMask->A_sel) | C_seqTrgSelB(psMask->B_sel));

		return RET_OK;
	}

	int timingDriver::ts2ip_setEvMappingRam(struct s_ts2ip* pIP)
	{
		int l_fd = pIP->ev.fd;
		ifRet(l_fd < 0);

		unsigned int* pdpCtrl = pIP->ev.cfg.evMappingRam;
		for(int i=1;i<MAX_EVR_RAM_SIZE;i++)
		{
			ts2ip_wr(l_fd, MemA_EVR_0 + 4*(i & 0x7ff), pdpCtrl[i]);
		};
		return RET_OK;
	};

	int timingDriver::ts2ip_setRxDbus(struct s_ts2ip* pIP)
	{
		int l_fd = pIP->ev.fd;
		ifRet(l_fd < 0);

		ts2ip_wr(l_fd, A_rxDBusSel_swapExOut, C_rxDBusSel       (pIP->ev.cfg.rxDbusSrcSelect) \
				| C_evr_pdpTrgSrcSel(pIP->ev.cfg.pdpTrgSrcSel   ) \
				| C_swapExOut       (pIP->ev.cfg.swapExOut      ));
		return RET_OK;
	};

	int timingDriver::ts2ip_setPdpPolarity(struct s_ts2ip* pIP)
	{
		int l_fd = pIP->ev.fd;
		ifRet(l_fd < 0);

		ts2ip_wr(l_fd, A_pdpPolarity, pIP->ev.cfg.pdpPolarity);
		return RET_OK;
	};

	int timingDriver::ts2ip_setEvgEnable(struct s_ts2ip* pIP)
	{
		int l_fd = pIP->ev.fd;
		ifRet(l_fd < 0);

		struct s_evgEnable* pEn = &pIP->ev.enEvg; 
		ts2ip_wr(l_fd, A_evgEnableMask, 0 \
				| C_mxc14Enable      (pEn->mxc             ) \
				| C_evTrgMsk         (pEn->evTrgMsk16      ) \
				| C_txEnable_DBus    (pEn->DBus            ) \
				| C_txEnable_evCodeA (pEn->evCodeA         ) \
				| C_txEnable_evCodeB (pEn->evCodeB         ) \
				| C_txEnable_mevCodeA(pEn->evCodeB_mevCodeA) \
				| C_seqTrgA_en       (pEn->evCodeB_seqTrgA ) \
				| C_seqTrgB_en       (pEn->evCodeB_seqTrgB ) \
				);
		return RET_OK;
	};

	int timingDriver:: ts2ip_setEvrEnable(struct s_ts2ip* pIP)
	{
		int l_fd = pIP->ev.fd;
		ifRet(l_fd < 0);

		struct s_evrEnable* pEn = &pIP->ev.enEvr; 
		ts2ip_wr(l_fd, A_evrEnableMask, 0 \
				| C_rxEnable_streamData(pEn->streamData) \
				| C_rxEnable_DBus      (pEn->DBus      ) \
				| C_rxEnable_evCodeA   (pEn->evCodeA   ) \
				| C_rxEnable_evCodeB   (pEn->evCodeB   ) \
				| C_pdpEnable          (pEn->pdpEnable ) \
				); 
		return RET_OK;
	};

	int timingDriver::ts2ip_setPdpShape(struct s_ts2ip* pIP)
	{
		int i;
		int l_fd = pIP->ev.fd;
		ifRet(l_fd < 0);

		struct s_pdpConfig* pPdp = pIP->ev.cfg.pdp;

		for(i=0;i<MAX_PDP_NUM;i++)
		{
			ts2ip_wr(l_fd, MemC_EVR_pdpDelay + 4 * i, pPdp[i].delay);
			ts2ip_wr(l_fd, MemC_EVR_pdpWidth + 4 * i, pPdp[i].width);
		}

		ts2ip_wr(l_fd, A_evr_RegUpdate, C_evr_setCtrlReg(1)); 
		taskDelay(10);
		ts2ip_wr(l_fd, A_evr_RegUpdate, C_evr_setCtrlReg(0)); 
		taskDelay(1000);

		return RET_OK;
	};

};
