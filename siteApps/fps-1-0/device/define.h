#ifndef __DEFINE_H
#define __DEFINE_H


#define INTERLOCK(x) { ((CTR_CMD_MEM[x] & 0x80000000) >> 31) == 1; }

#define INTERRUPT  ( CTR_CMD_MEM[0] )
#define CLEAR_INTERRUPT { CTR_CMD_MEM[0] = 0; }

#define TRIGGER_HIGH(x,y)    { CTR_CMD_MEM[x] = y; }
#define TRIGGER_LOW(x,y)    { CTR_CMD_MEM[x] = y; }

#define ILA_HIGH(x)     { CTR_CMD_MEM1[x] = 1; }
#define ILA_LOW(x)     { CTR_CMD_MEM1[x] = 0; }


#define ODD 0x11111111   //BRAM data 0 / 2
#define EVEN 0x22222222  //BRAM data 1 / 3 


/* time */
#define MAX_10MS 40000  // 10ms data size(index)
#define MAX_200MS 800000 // 200ms data size(index)
#define MAX_400MS 1600000 // 400ms data size(index)

#define OFFSET_200MS 800000
/* file */
#define FILEMODE (S_IRUSR | S_IWUSR | S_IRGRP | S_IROTH)

#define DDR_MAX_INDEX_SIZE_1MS             800000       // ddr size MAX

#define DDR_MAX_INDEX_SIZE_10MS            8000000 //after use....

#define INTERLOCK_AFTER_OFFSET                  800000      //ddr
#define INTERLOCK_AFTER_DDR_MAX_INDEX_SIZE_1MS  1600000

#endif