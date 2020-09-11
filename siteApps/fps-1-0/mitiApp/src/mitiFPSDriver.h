#ifndef   _MIT_FPS_DRIVER_H
#define   _MIT_FPS_DRIVER_H

#include "intech_global.h"

using namespace std;

namespace miti_fps
{
	class mitiFPSDriver 
	{
		public:
			mitiFPSDriver(const int devNum);
			~mitiFPSDriver();
			mitiFPSDriver* GetInstance();

			/* method function - public */ 
			unsigned int ReadUInt32Data(unsigned int addr, int bramindex = 1);
			void WriteUInt32Data(unsigned int addr, int bramindex, int data);
			
		/*Member variables */
		private:
			int fd;
			int page_size;

			// bram access pointer
			unsigned int *bram_ctrl_4000;   //4000_0000
			unsigned int *bram_ctrl_4200;   //4200_0000
			unsigned int *bram_ctrl_4600;   //4600_0000

		/* method function - private */ 
		private:
			void init_device64();
			void dev_ctrl64(unsigned int** ptr, unsigned int BASE_ADDR);
			void bram_unmap(unsigned int** ptr, unsigned int page_size);
			void initial_setup();
			void gth_reset();
			void device_release();

	};

};


#endif
