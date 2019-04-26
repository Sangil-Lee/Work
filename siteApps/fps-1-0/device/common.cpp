#include "time.h"

/* base addr */
unsigned int * CTR_CMD_MEM;     //bram0
unsigned int * CTR_CMD_MEM1;    //bram1
unsigned int * CTR_CMD_MEM2;    //bram2
unsigned int * CTR_CMD_MEM3;    //bram3
unsigned int * CTR_CMD_DDR;     //ddr

#ifdef ENABLE_GPIO_UIO
    unsigned int * CTR_CMD_GPIO_1ms;
    unsigned int * CTR_CMD_GPIO_1us;
#endif