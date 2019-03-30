/*
 * MIT License
 *
 * Copyright (c) 2106 Osprey DCS
 *
 * Permission is hereby granted, free of charge, to any person obtaining a copy
 * of this software and associated documentation files (the "Software"), to deal
 * in the Software without restriction, including without limitation the rights
 * to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 * copies of the Software, and to permit persons to whom the Software is
 * furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included in all
 * copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
 * SOFTWARE.
 */
#include <ctype.h>
#include <stdint.h>
#include <stdio.h>
#include <xil_io.h>
#include <xparameters.h>
#include "softwareBuildDate.h"
#include "sysmon.h"
#include "xadc.h"
#include "util.h"

#define SYSMON_WORD_CAPACITY    128

#define FIRMWARE_BUILD_DATE_WORD_BASE   0
#define SOFTWARE_BUILD_DATE_WORD_BASE   2
#define LINK_STATUS_WORD_BASE           4
#define XADC_WORD_BASE                  22
#define SFP_WORD_BASE                   64

/*
 * SYSMON RAM is 16 bits wide, but accessed as if it were 32 bits wide.
 */
#define In32(wordOffset)    Xil_In32(XPAR_BRAM_0_BASEADDR+((wordOffset)*4))
#define Out32(wordOffset,v) Xil_Out32(XPAR_BRAM_0_BASEADDR+((wordOffset)*4),(v))

/*
 * Stash build information.
 */
void
sysmonInit(void)
{
    uint32_t v;

    v = Xil_In32(XPAR_GPIO_DATE_BASEADDR);
    Out32(FIRMWARE_BUILD_DATE_WORD_BASE + 0, v >> 16);
    Out32(FIRMWARE_BUILD_DATE_WORD_BASE + 1, v);
    printf("Firmware build date: %u\n", (unsigned int)v);
    v = SOFTWARE_BUILD_DATE;
    Out32(SOFTWARE_BUILD_DATE_WORD_BASE + 0, v >> 16);
    Out32(SOFTWARE_BUILD_DATE_WORD_BASE + 1, v);
    printf("Software build date: %u\n", (unsigned int)v);
}

void
sysmonStashSFPword(int wordOffset, int word)
{
    Out32(SFP_WORD_BASE + wordOffset, word & 0xFFFF);
}
    
void
sysmonStashStatus(void)
{
    uint32_t v;
    int chan;

    for (chan = 0 ; chan < XADC_CHANNEL_COUNT ; chan++) {
        Out32(XADC_WORD_BASE + chan, xadcVal(chan));
    }
    v = Xil_In32(XPAR_GPIO_DATE_BASEADDR+8); // Fan Speed
    Out32(XADC_WORD_BASE + chan, v);

    v = Xil_In32(XPAR_LINK_GPIO_BASEADDR+0);
    Out32(LINK_STATUS_WORD_BASE + 0, v >> 16);
    Out32(LINK_STATUS_WORD_BASE + 1, v);
    v = Xil_In32(XPAR_LINK_GPIO_BASEADDR+8);
    Out32(LINK_STATUS_WORD_BASE + 2, v >> 16);
    Out32(LINK_STATUS_WORD_BASE + 3, v);
}


static void
showChar(int c)
{
    c &= 0xFF;
    if (!isprint(c))
        c = '.';
    printf("%c", c);
}

void
sysmonDump(void)
{
    int row, i;
    uint16_t v;

    printf("  W   B\n");
    for (row = 0 ; row < SYSMON_WORD_CAPACITY / 8 ; row++) {
        printf("%3d %3d ", row * 8, row * 8 * 2);
        for (i = 0 ; i < 8 ; i++) {
            v = In32(row * 8 + i);
            printf(" %04X", v);
        }
        printf("  ");
        for (i = 0 ; i < 8 ; i++) {
            v = In32(row * 8 + i);
            showChar(v >> 8);
            showChar(v);
        }
        printf("\n");
    }
}
