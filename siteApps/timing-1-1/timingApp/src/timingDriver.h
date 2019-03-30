#ifndef   _TIMING_REGISTER_MAP_H
#define   _TIMING_REGISTER_MAP_H

#include <cstdio>
#include <string>
#include <stdint.h>
#include <string.h>
#include <fcntl.h>
#include <unistd.h>
#include <sys/time.h>
#include <sys/ioctl.h>

#include "timingData.h"

using namespace std;

namespace timing
{
  class timingDriver 
  {
    public:

      timingDriver(const int mode, const int devNum);
      ~timingDriver();
      // double GetFirmware(){return firmware;}
      // double GetSoftware(){return software;}
      // void SetFirmware(double fware){firmware = fware; printf("FWare:%f\n", firmware);}
      // void SetSoftware(double sware){software = sware;}

      int test(int argc, char *argv[]);
      timingDriver* getInstance();
      int start_thread();
      int isInit  ; 

      int IP_RD(uint offset, uint *rdData) { return ip_rd(ev.fd, offset, rdData); }
      int IP_WR(uint offset, uint wrData)  { return ip_wr(ev.fd, offset, wrData); }

      int ip_rd(int fd, uint offset, uint *rdData);
      int ip_wr(int fd, uint offset, uint wrData);

      int bist(uint tMode);

//============================================================================================================================
    public:
      double firmware;
      double software;

      int                 opMode  ; 
      int                 evMode  ; 
      int                 evNum  ; 
      char                evName[100];

      struct s_ts2ev      ev; 
      struct s_ts2gtp     gtp;

      struct s_ts2slv     ob;
      struct s_ts2slv     sl;
      struct s_ts2slv     sh;

    private:
      int                 lcd_fd;
      int                 iic_fd    [IIC_maxDrv];
      int                 iic_isOpen[IIC_maxDrv];
      int                 iic_slvNum[IIC_maxDrv];
      struct s_ts2iic*    iic_piic  [IIC_maxDrv];

//============================================================================================================================
      int sys_init(const int mode, const int num);
      
      int sys_intrEn(int l_fd, uint wrData);
      int sys_intrClear(int l_fd);
      int sys_intrMask(int l_fd, uint wrData);
      int sys_getIntrVal(int l_fd, uint* rdData);
      
      int init_bp();
      int init_ip();
      int init_cps(int busId);
      int init_clk();
      
	public:
//============================================================================================================================
      int get_ipInfo(int l_fd, struct s_ipInfo *pInfo);
      int get_slaveBoardID();
      int get_slaveTemp();
      int get_sysStat();
      int get_gtpStat();
      int get_evgStat();
      int get_evrStat();
      int get_evTime();
      int get_fifoStat(int l_fd, uint addr, struct s_fifoStat *fStat);

//============================================================================================================================
      int set_sys_IO(uint lsbV, uint msbV);
      int set_sys_FMC_LED(uint link0, uint link1);
      int set_fp_led(unsigned char pwrOK, unsigned char fanERR, unsigned char epicsOK, unsigned char timingNetworkOK);
      int set_fanSpeed(char pwmFront, char pwmRear );
      
      int set_slaveIO_byMode();
      int set_slaveIO_sub(s_ts2slv* pSlv, uint setVal);
      int set_cpsByMode();
      int set_cpsConfig(struct s_ts2slv* pCfg);

      int set_gtp(uint setCommand, uint setDelay);

//============================================================================================================================
      int set_evg(uint setCommand, uint setDelay);

      int set_evg_igen(uint igen_enable, uint igen_mode_ext, struct s_evTime igenTime);
      int set_evg_sw_evgW(uint idec_mode, struct s_evTime igenTime);

      int set_evUser(uint evCode );
      int set_evUTrg(uint setDelay);
      int set_evSeq_Resume(uint seqN);


      //--------------------------------------------------------------------
      int set_evg_enable(uint dbus, uint evCodeA, uint evCodeB, uint mevCodeA, uint seqTrgA, uint seqTrgB, uint mxc);

      int set_evg_mxcFreq(uint mxcNum, float mxcFreq_Hz);

      int set_evg_tx_DBusSel(uint dBusNum, ev_src evSrc);
      
      int set_evg_evTrg(uint evTrgNum, ev_src evSrc);
      int set_evg_evTrgMask(uint evTrgMask);
      
      int set_evg_mevCodeA(uint trgNum, uint evCode);

      int cfg_evg_seqSrc(uint seqSrc_OR_Num , uint seqSrcList);
      int set_evg_seqTrg(uint seqA_TrgCmbSrc, uint seqA_repeat, uint seqB_TrgCmbSrc, uint seqB_repeat);
      int set_evg_seqTable(uint seqNum, uint offset, uint evCode, uint stopbit, uint timeStamp);
      
//============================================================================================================================
      int set_evr(uint setCommand, uint setDelay);
      int set_evr_enable           (uint dbus, uint evCodeA, uint evCodeB, uint streamD, uint pdp)  ;
      int set_evr_evLogFifo_Enable (uint enVal)  ;
      
      int cfg_evr_pdp(uint pdpNum, uint delay, uint width, uint polarity);
      int cfg_evr_outSrcSel(ev_out outPortNum, ev_out_src evOutSrc);
	  int set_evr_pdpAndOut(uint pdpTrgMode, uint extOutSwap );
      
      int set_evr_evRam(uint evCode, uint mapData);

//============================================================================================================================
      int prn_ipInfo(int l_fd, struct s_ipInfo *pInfo);
      
      int prn_ip (int pLevel);
      int prn_sys(int pLevel);
      int prn_gtp(int pLevel);
      int prn_evg(int pLevel);
      int prn_evr(int pLevel);
      int prn_all(int pLevel);

      int prn_time(struct s_evTime evTime);
      int prn_evLog();
      int prn_fifoStat(struct s_fifoStat fStat, int prnLabel);


//============================================================================================================================
      unsigned char lcdWrite(unsigned char value);
      void lcdWriteStr(const char *s);
      void lcdWrite2B(uint v);
      void lcdDrawStr(uint x, uint y, const char *s);
      void lcdClear(void);
      void lcdSetColor(unsigned char color);
      void lcdSetColRow(unsigned char col, unsigned char row);
      void lcdSetBgColor(unsigned char color);
      void lcdSetFont(unsigned char font);

      int iic_setMux(int fd, s_ts2iic* piic);
      int iic_wr(int busId, int slvId, uint offset, uint offset_size, char* pData, int size);
      int iic_rd(int busId, int slvId, uint offset, uint offset_size, char* pData, int size);

      s_ts2iic* iic_getInfo(int busId, int slvid);
      int iic_prnInfo();

      int max7313_configIO(int busId, int slvId, char PF8, char P70 );
      int max7313_setOutput(int busId, int slvId, char PF8, char P70 );

      uint si5338_write(unsigned char w_addr, unsigned char w_data);
      uint si5338_write_mask(unsigned char Addr, unsigned char Data, unsigned char Mask);
      unsigned char si5338_read(unsigned char r_addr);
      
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
