//Wrapping class for system calls of acquisition FPS device driver.
// #include <cstdlib>
// #include <cstring>
// #include <cmath>
// #include <sys/mman.h>
// #include <fcntl.h>

#include "acqFPSDriver.h"

// #define NUM_REGS_MAX 350

namespace acq_fps
{
	//==============================================================================
	//----===@ acqFPSDriver
	// Parameters  :
	// Description :
	//==============================================================================
		acqFPSDriver::acqFPSDriver(const int devNum):page_size(4096*128*2*8)
		{
			init_device64();

			//set_boardid(AN11);
			initial_setup();
		}

		//==============================================================================
		//----===@ ~acqFPSDriver
		// Parameters  :
		// Description :
		//==============================================================================
		acqFPSDriver::~acqFPSDriver()
		{
			bram_unmap(&bram_ctrl_4000, page_size);	// 10ms BRAM
			bram_unmap(&bram_ctrl_4200, page_size);
			//bram_unmap(&ctr_cmd_mem2, page_size);
			bram_unmap(&bram_ctrl_4004, page_size);	// 20ms BRAM  			
			bram_unmap(&Bram_Ctrl_GPIO_0, 4096);
			bram_unmap(&Bram_Ctrl_GPIO_1, 4096);
		}

		acqFPSDriver* acqFPSDriver::GetInstance()
		{
			return this;
		}

		void acqFPSDriver::init_device64()
		{
			if( -1 == (fd =open("/dev/mem", O_RDWR| O_SYNC) ))
			{
				perror("fd error");
				exit(-1);
			};
			
			dev_ctrl64(&bram_ctrl_4000, XPAR_BRAM_0_BASEADDR);				// 10ms BRAM
			dev_ctrl64(&bram_ctrl_4004, XPAR_BRAM_20_BASEADDR);			// 20ms BRAM
			dev_ctrl64(&bram_ctrl_4200, XPAR_BRAM_1_BASEADDR);
			//dev_ctrl64(&ctr_cmd_mem2, XPAR_BRAM_2_BASEADDR);

			dev_ctrl64_gpio(&Bram_Ctrl_GPIO_0, XPAR_GPIO_0_BASEADDR);	// PL -> PS Trigger
			dev_ctrl64_gpio(&Bram_Ctrl_GPIO_1, XPAR_GPIO_1_BASEADDR);	// PS -> PL Trigger
		}

		void acqFPSDriver::dev_ctrl64(unsigned int** ptr, unsigned int BASE_ADDR)
		{
			*ptr = (unsigned int *)mmap(NULL, page_size, PROT_READ | PROT_WRITE, MAP_SHARED, fd, BASE_ADDR);
			if(*ptr == (unsigned int*)-1)
			{
				perror("mmap error");
				exit(-1);
			};
		}
		void acqFPSDriver::dev_ctrl64_gpio(unsigned int** ptr, unsigned int BASE_ADDR)
		{
			*ptr = (unsigned int *)mmap(NULL, 4096, PROT_READ | PROT_WRITE, MAP_SHARED, fd, BASE_ADDR);
			if(*ptr == (unsigned int*)-1)
			{
				perror("mmap error");
				exit(-1);
			};
		}

		void acqFPSDriver::bram_unmap(unsigned int** ptr, unsigned int page_size)
		{
			if(munmap(*ptr, page_size) == -1) 
			{
				printf("Can't unmap memory from user space.\n");
				exit(0);
			};

			printf("close DEV_RELEASE\n");
		};

		void acqFPSDriver::ReadUInt32Array(unsigned int addr, unsigned int * buff)
		{
			if( addr == 0){
				memcpy(&buff[0], &bram_ctrl_4004[0], 16 * 10000 );	// 0x4004_0000
			}else{
				memcpy(&buff[0], &bram_ctrl_4000[0], 16 * 10000 );		// 0x4000_0000
			}
		}
		
		unsigned int acqFPSDriver::ReadUInt32Data(unsigned int addr, int bramindex)
		{
			unsigned int result = 0;
			switch (bramindex)
			{
				case 0:
					// result = bram_ctrl_4000[addr];
					break;
				case 1:
					result = bram_ctrl_4200[addr];
					break;
				case 2:
					//result = ctr_cmd_mem2[addr];
					break;
				default:
					break;
			}
			return result;
		}

		void acqFPSDriver::WriteUInt32Data(unsigned int addr, int data,int bramindex)
		{
			
			switch (bramindex)
			{
				case 0:
					// bram_ctrl_4000[addr] = data;
					break;
				case 1:
					bram_ctrl_4200[addr] = data;
					break;
				case 2:
					//ctr_cmd_mem2[addr] = data;
					break;
				default:
					break;
			}
			
		}

		unsigned int acqFPSDriver::ReadUInt32GPIO()
		{
			return Bram_Ctrl_GPIO_0[0];
		}

		void acqFPSDriver::WriteUInt32_GPIO(int data)
		{
			Bram_Ctrl_GPIO_1[0] = data;
		}

		void acqFPSDriver::initial_setup()
		{
			gtx_reset();
			//clcd_init();
		}

		void acqFPSDriver::gtx_reset()
		{
			WriteUInt32Data(GTX_RESET, 1);
			WriteUInt32Data(GTX_RESET, 0);		
		}

} //name space end
