#include "time.h"


unsigned int Get_PL_CMD64(int cmd)
{
	return CTR_CMD_MEM[cmd];
}

unsigned int Send_PL_CMD64(int cmd, unsigned int val)
{
    CTR_CMD_MEM[cmd] = val; 
    return CTR_CMD_MEM[cmd];
}


void DEV_CTRL64(unsigned int** PTR,unsigned int BASE_ADDR, unsigned int page_size, int fd, int type)
{
    switch(type)
    {
        case 1: //dev/mem
            // *PTR = (unsigned int *)mmap(NULL, page_size, PROT_READ | PROT_WRITE, MAP_SHARED, fd, BASE_ADDR);
            *PTR = (unsigned int *)mmap(NULL, page_size, PROT_READ | PROT_WRITE, MAP_SHARED, fd, BASE_ADDR);
            break;
        case 2:
            *PTR = (unsigned int *)mmap(NULL, page_size, PROT_READ | PROT_WRITE, MAP_SHARED, fd, 0); 
            break;
    }             
        if(*PTR == (unsigned int*)-1)
        {
                perror("mmap error aaa");
                exit(-1);
        }
        return;
}

void INIT_DEVICE64(unsigned int page_size, int *fd, int type)
{
    switch(type)
    {
        case 1:
            *fd = open("/dev/mem", O_RDWR| O_SYNC);
            DEV_CTRL64(&CTR_CMD_MEM, XPAR_BRAM_0_BASEADDR, 4096*128*2*8, *fd, 1);  //dev/mem
            DEV_CTRL64(&CTR_CMD_MEM1, XPAR_BRAM_1_BASEADDR, 4096*128*2*8, *fd, 1);
            // DEV_CTRL64(&CTR_CMD_MEM2, XPAR_BRAM_0_BASEADDR, page_size, *fd, 1);
            // DEV_CTRL64(&CTR_CMD_MEM3, XPAR_BRAM_0_BASEADDR, page_size, *fd, 1);
            DEV_CTRL64(&CTR_CMD_DDR, XPAR_DDR_0_BASEADDR, 4096*128*2*8, *fd, 1);
            break;

        case 2:
            *fd = open("/dev/uio2", O_RDWR| O_SYNC);

            // DEV_CTRL(&CTR_CMD_GPIO, XPAR_BRAM_0_BASEADDR, page_size, *fd, 2);    //dev/uio0
            break;
    }

    if(*fd == -1)
    {
        perror("fd error");
        exit(-1);
    }
}

void CLOSE_DEVICE(unsigned int page_size)
{
    // DEV_RELEASE(&WR_CMD_MEM_DAC_PID, page_size);
}

void DEV_RELEASE(unsigned int** PTR, unsigned int page_size)
{
	if(munmap(*PTR, page_size) == -1) {
		printf("Can't unmap memory from user space.\n");
		exit(0);
	}
	printf("close DEV_RELEASE\n");
}