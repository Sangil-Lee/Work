#include "mitiFPSDriver.h"

namespace miti_fps
{
	//==============================================================================
	//----===@ mitiFPSDriver
	// Parameters  :
	// Description :
	//==============================================================================
	mitiFPSDriver::mitiFPSDriver(const int devNum):page_size(4096*128*2*8)
	{
		init_device64();
		initial_setup();
	}

	//==============================================================================
	//----===@ ~mitiFPSDriver
	// Parameters  :
	// Description :
	//==============================================================================
	mitiFPSDriver::~mitiFPSDriver()
	{
		bram_unmap(&bram_ctrl_4000, page_size);
		bram_unmap(&bram_ctrl_4200, page_size);
		bram_unmap(&bram_ctrl_4600, page_size);
	}

	mitiFPSDriver* mitiFPSDriver::GetInstance()
	{
		return this;
	}

	void mitiFPSDriver::init_device64()
	{
		if( -1 == (fd =open("/dev/mem", O_RDWR| O_SYNC) )){
			perror("fd error");
			exit(-1);
		};
		
		dev_ctrl64(&bram_ctrl_4000, XPAR_BRAM_0_BASEADDR);
		dev_ctrl64(&bram_ctrl_4200, XPAR_BRAM_1_BASEADDR);
		dev_ctrl64(&bram_ctrl_4600, XPAR_BRAM_2_BASEADDR);
	}

	void mitiFPSDriver::dev_ctrl64(unsigned int** ptr, unsigned int BASE_ADDR)
	{
		*ptr = (unsigned int *)mmap(NULL, page_size, PROT_READ | PROT_WRITE, MAP_SHARED, fd, BASE_ADDR);
		if( *ptr == (unsigned int*)-1 ){
			perror("mmap error aaa");
			exit(-1);
		};
	}

	void mitiFPSDriver::bram_unmap(unsigned int** ptr, unsigned int page_size)
	{
		if(munmap(*ptr, page_size) == -1){
			printf("Can't unmap memory from user space.\n");
			exit(0);
		};
	};

	unsigned int mitiFPSDriver::ReadUInt32Data(unsigned int addr, int bramindex)
	{
		unsigned int temp = addr & 0x00FF;
		unsigned int result = 0;
		switch (bramindex)
		{
			case 0:
				result = bram_ctrl_4200[temp];
				break;
			case 1:
				result = bram_ctrl_4000[temp];
				break;
			case 2:
				result = bram_ctrl_4600[temp];
				break;
			default:
				result = 0;
				break;
		}
		return result;
	}

	void mitiFPSDriver::WriteUInt32Data(unsigned int addr, int bramindex, int data)
	{
		unsigned int temp = addr & 0x00FF;
		switch (bramindex)
		{
			case 0:
				bram_ctrl_4200[temp] = data;
				break;
			case 1:
				bram_ctrl_4000[temp] = data;
				break;
			case 2:
				bram_ctrl_4600[temp] = data;
				break;
			default:
				break;
		}
	}

	void mitiFPSDriver::initial_setup()
	{
		gth_reset();

		device_release();

		// masking value reset
		for(int i = 0 ; i<112; i++){	// 112 = 8(link) * 7(node) *2(dry & wet)
			bram_ctrl_4600[i] = 0;
		}		

	}

	void mitiFPSDriver::gth_reset()
	{
		WriteUInt32Data(GTH_RESET, Bram0_Idx, 0xf );
		WriteUInt32Data(GTH_RESET, Bram0_Idx, 0x0 );
	}
	
	void mitiFPSDriver::device_release()
	{
		WriteUInt32Data(DEVICE_RELEASE, Bram0_Idx, 1);
		WriteUInt32Data(DEVICE_RELEASE, Bram0_Idx, 0);
	}

} //name space end




