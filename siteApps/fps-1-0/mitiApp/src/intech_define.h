#ifndef __INTECH_DEFINE_H
#define __INTECH_DEFINE_H

/* base addr */
// #define XPAR_BRAM_0_BASEADDR 0x80030000
// #define XPAR_BRAM_1_BASEADDR 0x80032000
// #define XPAR_BRAM_2_BASEADDR 0x80034000
#define XPAR_BRAM_0_BASEADDR 0x40000000
#define XPAR_BRAM_1_BASEADDR 0x42000000
#define XPAR_BRAM_2_BASEADDR 0x46000000
/* bram type */
#define Bram0_Idx 0	// 4200
#define Bram1_Idx 1	// 4000
#define Bram2_Idx 2	// 4600

/* bram offset */
#define LINK_MASK_1 0
#define LINK_MASK_2 1
#define LINK_MASK_3 2
#define LINK_MASK_4 3
#define LINK_MASK_5 4
#define LINK_MASK_6 5
#define LINK_MASK_7 6
#define DEVICE_RELEASE 7
#define GTH_RESET 8

#define LINK_MASK_8     0x19
#define LINK_MASK_EVG   0x20

/* machine mode */
#define MM0 0
#define MM1 1
#define MM2 2
#define MM3 3

#define NUM_REGS_MAX 350
#define CMD_LCD_ENA 0
#define CMD_LCD_DATA 1


#endif
