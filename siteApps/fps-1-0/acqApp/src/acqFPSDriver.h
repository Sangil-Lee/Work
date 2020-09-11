#ifndef   _ACQ_FPS_REGISTER_MAP_H
#define   _ACQ_FPS_REGISTER_MAP_H

// #include <cstdio>
// #include <string>
// #include <stdint.h>
// #include <string.h>
// #include <fcntl.h>
// #include <unistd.h>
// #include <sys/time.h>
// #include <sys/ioctl.h>

//#include "acqFPSData.h"

#include "intech_global.h"
/* base addr */

using namespace std;

namespace acq_fps
{
  class acqFPSDriver 
  {
    public:
		acqFPSDriver(const int devNum);
		~acqFPSDriver();

		acqFPSDriver* GetInstance();

		unsigned int ReadUInt32Data(unsigned int addr, int bramindex = 1);
		void WriteUInt32Data(unsigned int addr, int data,int bramindex=1);

		void ReadUInt32Array(unsigned int addr, unsigned int * buff);

		unsigned int ReadUInt32GPIO();
		void WriteUInt32_GPIO(int data);
	//Member variables
	private:
		int fd;
		int page_size;

		//BRam
		unsigned int *bram_ctrl_4000;     //bram0	0x4000_0000
		unsigned int *bram_ctrl_4004;   //bram0	0x4004_0000
		unsigned int *bram_ctrl_4200;    //bram1	0x4200_0000
		//unsigned int *ctr_cmd_mem2;	   //bram2	0x4600_0000

		unsigned int *Bram_Ctrl_GPIO_0;	// 0x4100_0000
		unsigned int *Bram_Ctrl_GPIO_1;	// 0x4100_1000

	//Member Functions.
    private:
		void init_device64();
		void dev_ctrl64(unsigned int** ptr, unsigned int BASE_ADDR);
		void dev_ctrl64_gpio(unsigned int** ptr, unsigned int BASE_ADDR);		
		void bram_unmap(unsigned int** ptr, unsigned int page_size);
		void initial_setup();
		void gtx_reset();
		// void set_boardid(unsigned int board_num);
		// void clcd_init();
		// int LCDInstructionSet();
		// void WriteModeOn(int *pData);
		// void InStructionModeOn(int *pData);
		// void Send_PL_CMD (int cmd ,int val);
  };

};
#endif