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
#include "fastProtectionProtocol.h"
#include "softwareBuildDate.h"
#include "sysmon.h"
#include "xadc.h"
#include "util.h"

#define MAX_LINKS               8
#define MAX_NODES_PER_LINK      5

#define FIRMWARE_BUILD_DATE_WORD_BASE   0
#define SOFTWARE_BUILD_DATE_WORD_BASE   2
#define LINK_STATUS_WORD_BASE           4
#define XADC_WORD_BASE                  22
#define SFP_WORD_BASE                   64
#define   SFP_WORD_OFFSET_VENDOR_NAME     0
#define   SFP_WORD_OFFSET_PART_NAME       8
#define   SFP_WORD_OFFSET_PART_NUMBER    16
#define   SFP_WORD_OFFSET_DATE_CODE      24
#define   SFP_WORD_OFFSET_RX_POWER       27
#define   SFP_WORD_OFFSET_TEMPERATURE    28
#define   SFP_WORD_COUNT            29

#define MITIGATE_SYSMON_CAPACITY    400
#define ACQ_NODE_SYSMON_CAPACITY    128
static uint16_t sysmonBuf[MITIGATE_SYSMON_CAPACITY];

/*
 * Stash a single value of local system monitoring
 */
static void
stashLocal(unsigned int index, int value)
{
    if (index < MITIGATE_SYSMON_CAPACITY) {
        sysmonBuf[index] = value;
    }
    else {
        static int warnCount;

        if (warnCount < 100) {
            warnCount++;
            warn("Attempt to stash to location %u of %u\n", index,
                                   MITIGATE_SYSMON_CAPACITY);
        }
    }
}

/*
 * Stash build information.
 */
void
sysmonInit(void)
{
    uint32_t v;

    v = Xil_In32(XPAR_GPIO_GENERIC0_BASEADDR);
    stashLocal(FIRMWARE_BUILD_DATE_WORD_BASE + 0, v >> 16);
    stashLocal(FIRMWARE_BUILD_DATE_WORD_BASE + 1, v);
    printf("Firmware build date: %u\n", (unsigned int)v);
    v = SOFTWARE_BUILD_DATE;
    stashLocal(SOFTWARE_BUILD_DATE_WORD_BASE + 0, v >> 16);
    stashLocal(SOFTWARE_BUILD_DATE_WORD_BASE + 1, v);
    printf("Software build date: %u\n", (unsigned int)v);
}

void
sysmonStashSFPword(int wordOffset, int word)
{
    stashLocal(SFP_WORD_BASE + wordOffset, word);
}

void
sysmonStashStatus(void)
{
    uint32_t v;
    int chan;

    for (chan = 0 ; chan < XADC_CHANNEL_COUNT ; chan++) {
        stashLocal(XADC_WORD_BASE + chan, xadcVal(chan));
    }
    v = Xil_In32(XPAR_GPIO_GENERIC0_BASEADDR+8); // Fan Speed
    stashLocal(XADC_WORD_BASE + chan, v);

    v = Xil_In32(XPAR_GPIO_AURORA_BASEADDR+0);
    stashLocal(LINK_STATUS_WORD_BASE + 0, v >> 16);
    stashLocal(LINK_STATUS_WORD_BASE + 1, v);
    v = Xil_In32(XPAR_GPIO_AURORA_BASEADDR+8);
    stashLocal(LINK_STATUS_WORD_BASE + 2, v >> 16);
    stashLocal(LINK_STATUS_WORD_BASE + 3, v);
}

static void
showChar(int c)
{
    c &= 0xFF;
    if (!isprint(c))
        c = '.';
    printf("%c", c);
}

static void
showMem(uint16_t *ip, int count)
{
    int row, i;
    uint16_t v;

    for (row = 0 ; row < (count / 8) ; row++) {
        printf("%3d ", row * 8);
        for (i = 0 ; i < 8 ; i++) {
            v = ip[row * 8 + i];
            printf(" %04X", v);
        }
        printf("%7d ", row * 8 * 2);
        for (i = 0 ; i < 8 ; i++) {
            v = ip[row * 8 + i];
            showChar(v >> 8);
            showChar(v);
        }
        printf("\n");
    }
}

void
sysmonDump(int link, unsigned int node)
{
    if (link < 0) {
        printf("Mitigation node system monitors:\n");
        showMem(sysmonBuf, MITIGATE_SYSMON_CAPACITY);
    }
    else {
        int i, a;
        static uint16_t buf[ACQ_NODE_SYSMON_CAPACITY];

        if ((link >= MAX_LINKS) || (node >= MAX_NODES_PER_LINK)) return;
        a = ((node * MAX_LINKS) + link) * ACQ_NODE_SYSMON_CAPACITY;
        for (i = 0 ; i < ACQ_NODE_SYSMON_CAPACITY ; i++, a++) {
            Xil_Out32(XPAR_GPIO_SYSMON_BASEADDR, a);
            buf[i] = Xil_In32(XPAR_GPIO_SYSMON_BASEADDR);
        }
        printf("Link %d, node %d system monitors:\n", link, node);
        showMem(buf, ACQ_NODE_SYSMON_CAPACITY);
    }
}

static int
remoteSysmonBase(int link, int node)
{
    return ((node * MAX_LINKS) + link) * ACQ_NODE_SYSMON_CAPACITY;
}
int
sysmonValue(int link, int node, int channel)
{
    int idx; 
    
    if (node < 0) {
        return sysmonBuf[XADC_WORD_BASE+channel];
    }
    else {
        idx = remoteSysmonBase(link, node) + XADC_WORD_BASE + channel;
        Xil_Out32(XPAR_GPIO_SYSMON_BASEADDR, idx);
        return Xil_In32(XPAR_GPIO_SYSMON_BASEADDR);
    }
}

static int
sfpVal(int link, int node, int towardsMitigation, int isTemperature)
{
    int idx; 
    
    if (node < 0) {
        if (link < 0) idx = 0;
        else          idx = link + 1;
        return sysmonBuf[SFP_WORD_BASE +
                         (SFP_WORD_COUNT * idx) +
                         (isTemperature ? SFP_WORD_OFFSET_TEMPERATURE
                                        : SFP_WORD_OFFSET_RX_POWER)];
    }
    else {
        idx = remoteSysmonBase(link, node) + SFP_WORD_BASE +
              (SFP_WORD_COUNT * (towardsMitigation ? 0 : 1)) +
              (isTemperature ? SFP_WORD_OFFSET_TEMPERATURE
                             : SFP_WORD_OFFSET_RX_POWER);
        Xil_Out32(XPAR_GPIO_SYSMON_BASEADDR, idx);
        return Xil_In32(XPAR_GPIO_SYSMON_BASEADDR);
    }
}

int
sysmonSFPrxPower(int link, int node, int towardsMitigation)
{
    return sfpVal(link, node, towardsMitigation, 0);
}

int
sysmonSFPtemperature(int link, int node, int towardsMitigation) {
    return sfpVal(link, node, towardsMitigation, 1);
}
